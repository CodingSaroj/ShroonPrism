#include "Pipeline.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            void GL33Pipeline::Create(Pipeline & pl, PipelineSpecification & spec)
            {
                pl.m_Spec = spec;

                GLuint shaderHandles[3];
                GLint shaderStatuses[3] = {1, 1, 1};
                bool shaderAvailabilities[3];

                memset(shaderAvailabilities, 0, 3);

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
                            auto iter = pl.m_UniformStructures.find(uniform.name);

                            if (iter != pl.m_UniformStructures.end())
                            {
                                continue;
                            }

                            UniformStructureNode node;
                            node.Type = compiler.get_type(uniform.type_id);
                            node.Name = uniform.name;
                            node.Size = compiler.get_declared_struct_size(node.Type);
                            auto ranges = compiler.get_active_buffer_ranges(uniform.id);

                            if (node.Size % 16)
                            {
                                node.Size += 16 - (node.Size % 16);
                            }

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

                            pl.m_UniformStructures[node.Name] = std::move(node);
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

                            ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "OpenGL3.3::ShaderCompiler", "Failed to compile " + shaderCompileData[i].ShaderType + " shader. Log:");
                            ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "OpenGL3.3::ShaderCompiler", std::string(log));

                            glDeleteShader(shaderHandles[i]);
                        }
                        else
                        {
                            shaderAvailabilities[i] = true;
                        }
                    }
                }

                pl.m_Handle.AsRefOf<GLuint>() = glCreateProgram();

                for (uint8_t i = 0; i < 3; i++)
                {
                    if (shaderAvailabilities[i])
                    {
                        glAttachShader(pl.m_Handle.As<GLuint>(), shaderHandles[i]);
                    }
                }

                glLinkProgram(pl.m_Handle.As<GLuint>());

                GLint status;

                glGetProgramiv(pl.m_Handle.As<GLuint>(), GL_LINK_STATUS, &status);

                if (!status)
                {
                    glGetProgramInfoLog(pl.m_Handle.As<GLuint>(), 1024, nullptr, log);

                    ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "OpenGL3.3::ShaderLinker", "Failed to link shader. Log:");
                    ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "OpenGL3.3::ShaderLinker", std::string(log));
                }

                for (uint8_t i = 0; i < 3; i++)
                {
                    if (shaderAvailabilities[i])
                    {
                        glDetachShader(pl.m_Handle.As<GLuint>(), shaderHandles[i]);
                        glDeleteShader(shaderHandles[i]);
                    }
                }
            }

            void GL33Pipeline::Destroy(Pipeline & pl)
            {
                glDeleteProgram(pl.m_Handle.As<GLuint>());
            }

            void GL33Pipeline::ApplyStateChanges(Pipeline & pl)
            {
                GLenum enableMask = 0;

                if (pl.m_State.Flags & PipelineState::DepthTestBit)
                {
                    enableMask |= GL_DEPTH_TEST;
                }

                if (pl.m_State.Flags & PipelineState::StencilTestBit)
                {
                    enableMask |= GL_STENCIL_TEST;
                }

                if (pl.m_State.Flags & PipelineState::BlendingBit)
                {
                    enableMask |= GL_BLEND;
                }

                if (pl.m_State.Flags & PipelineState::MultisampleBit)
                {
                    enableMask |= GL_MULTISAMPLE;
                }

                if (pl.m_State.Flags & PipelineState::FaceCullingBit)
                {
                    enableMask |= GL_CULL_FACE;
                }

                if (enableMask)
                {
                    glEnable(enableMask);
                }

                glPolygonMode(GL_FRONT_AND_BACK, pl.m_State.Fill);
                glCullFace(pl.m_State.Cull);
            }
        }
    }
}
