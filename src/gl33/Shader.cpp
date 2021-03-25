#include "Shader.hpp"

namespace Shroon
{
    namespace Prism
    {
        void GL33Shader::Create(Shader & shader, ShaderSpecification spec)
        {
            GLuint shaderHandles[3];
            GLint shaderStatuses[3] = {0, 0, 0};
            bool shaderAvailabilities[3] = {false, false, false};

            struct CompileData
            {
                std::string ShaderType;
                GLenum ShaderEnum;
                std::vector<uint32_t> * ShaderBinary;
            };

            CompileData shaderCompileData[] =
            {
                {"Vertex", GL_VERTEX_SHADER, spec.VertexShader},
                {"Geometry", GL_GEOMETRY_SHADER, spec.GeometryShader},
                {"Fragment", GL_FRAGMENT_SHADER, spec.FragmentShader},
            };

            char log[1024];

            for (uint8_t i = 0; i < 3; i++)
            {
                if (shaderCompileData[i].ShaderBinary)
                {
                    spirv_cross::CompilerGLSL compiler(std::move(*shaderCompileData[i].ShaderBinary));

                    auto resources = compiler.get_shader_resources();

                    for (auto & uniform : resources.uniform_buffers)
                    {
                        auto iter = std::find_if(shader.m_UniformStructures.begin(), shader.m_UniformStructures.end(), [&uniform](const UniformStructureNode & fmt)
                        {
                            if (fmt.Name == uniform.name)
                            {
                                return true;
                            }
                            else
                            {
                                return false;
                            }
                        });

                        if (iter != shader.m_UniformStructures.end())
                        {
                            continue;
                        }

                        UniformStructureNode node;
                        node.Type = compiler.get_type(uniform.type_id);
                        node.Name = uniform.name;
                        node.Size = compiler.get_declared_struct_size(node.Type);
                        auto ranges = compiler.get_active_buffer_ranges(uniform.id);

                        if (node.Type.basetype == spirv_cross::SPIRType::BaseType::Struct)
                        {
                            for (uint32_t i = 0; ; i++)
                            {
                                if (compiler.get_member_name(uniform.base_type_id, i) == "")
                                {
                                    break;
                                }

                                uint32_t memberSize = 0;

                                if (i < ranges.size())
                                {
                                    memberSize = ranges[i].range;
                                }
                                else
                                {
                                    memberSize = compiler.get_declared_struct_member_size(compiler.get_type(uniform.base_type_id), i);
                                }

                                node.Members.emplace_back(UniformStructureNode
                                {
                                    compiler.get_type(compiler.get_type(uniform.base_type_id).member_types[i]),
                                    compiler.get_member_name(uniform.base_type_id, i),
                                    memberSize
                                });
                            }
                        }

                        shader.m_RequiredUniformBlocks.emplace_back(node.Name);
                        shader.m_UniformStructures.emplace_back(std::move(node));
                    }

                    for (auto & sampledImage : resources.sampled_images)
                    {
                        UniformStructureNode node;
                        node.Type = compiler.get_type(sampledImage.type_id);
                        node.Name = sampledImage.name;
                        node.Size = 0;
                    }

                    spirv_cross::CompilerGLSL::Options options = compiler.get_common_options();
                    options.version = 330;
                    options.vulkan_semantics = false;
                    options.separate_shader_objects = false;

                    compiler.set_common_options(options);

                    std::string shaderSrc = std::move(compiler.compile());

                    shaderHandles[i] = glCreateShader(shaderCompileData[i].ShaderEnum);

                    char * shaderRaw = shaderSrc.data();

                    glShaderSource(shaderHandles[i], 1, &shaderRaw, 0);
                    glCompileShader(shaderHandles[i]);

                    glGetShaderiv(shaderHandles[i], GL_COMPILE_STATUS, &shaderStatuses[i]);
                    
                    if (!shaderStatuses[i])
                    {
                        glGetShaderInfoLog(shaderHandles[i], 1024, nullptr, log);

                        std::cout<<"GLSL::Error: "<<log<<"\n";

                        glDeleteShader(shaderHandles[i]);
                    }
                    else
                    {
                        shaderAvailabilities[i] = true;
                    }
                }
            }

            *reinterpret_cast<GLuint *>(&shader.m_Handle) = glCreateProgram();

            for (uint8_t i = 0; i < 3; i++)
            {
                if (shaderAvailabilities[i])
                {
                    glAttachShader(shader.m_Handle, shaderHandles[i]);
                }
            }

            glLinkProgram(shader.m_Handle);

            GLint status;

            glGetProgramiv(shader.m_Handle, GL_LINK_STATUS, &status);

            if (!status)
            {
                glGetProgramInfoLog(shader.m_Handle, 1024, nullptr, log);

                std::cout<<"GLSL::Error: "<<log<<"\n";
            }

            for (uint8_t i = 0; i < 3; i++)
            {
                if (shaderAvailabilities[i])
                {
                    glDetachShader(shader.m_Handle, shaderHandles[i]);
                    glDeleteShader(shaderHandles[i]);
                }
            }
        }

        void GL33Shader::Destroy(Shader & shader)
        {
            glDeleteProgram(shader.m_Handle);
        }

        void GL33Shader::BindUniformBlock(Shader & shader, std::string name, uint32_t bindingPoint)
        {
            GLuint blockIndex = glGetUniformBlockIndex(shader.m_Handle, name.c_str());

            glUniformBlockBinding(shader.m_Handle, blockIndex, bindingPoint);
        }
    }
}
