#include "CommandBuffer.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            void ES30CommandBuffer::Create(CommandBuffer & cb)
            {
                cb.m_Recording = false;
                cb.m_Commands.clear();
                cb.m_Passes.clear();
            }

            void ES30CommandBuffer::Destroy(CommandBuffer & cb)
            {
            }

            void ES30CommandBuffer::BeginRecording(CommandBuffer & cb)
            {
                cb.m_Recording = true;
                cb.m_Commands.clear();
                cb.m_Passes.clear();
            }

            void ES30CommandBuffer::EndRecording(CommandBuffer & cb)
            {
                cb.m_Recording = false;
            }

            void ES30CommandBuffer::Submit(CommandBuffer & cb)
            {
                if (cb.m_Recording)
                {
                    ErrorReporter(SHRN_PRISM_LEVEL_ERROR, "OpenGLES3.0::CommandBuffer", "EndRecording() not called before calling Submit().");
                    return;
                }

                for (auto & cmd : cb.m_Commands)
                {
                    switch(cmd.Type)
                    {
                        case CommandBuffer::CommandType::BindPass:
                        {
                            cb.m_CurrentSubpass = cb.m_Passes[cmd.BindData.Target.As<GLuint>()].m_Spec.SubpassSpecs[cmd.BindData.TargetIndex];
                            cb.m_CurrentSubpass.TargetPipeline->SetState(cb.m_CurrentSubpass.TargetPipelineState);

                            glUseProgram(cb.m_CurrentSubpass.TargetPipeline->GetRaw().As<GLuint>());

                            break;
                        }

                        case CommandBuffer::CommandType::BindFramebuffer:
                        {
                            glBindFramebuffer(GL_FRAMEBUFFER, cmd.BindData.Target.As<GLuint>());

                            break;
                        }

                        case CommandBuffer::CommandType::BindMesh:
                        {
                            glBindVertexArray(cmd.BindData.Target.As<GLuint>());

                            break;
                        }

                        case CommandBuffer::CommandType::BindUBO:
                        {
                            glUniformBlockBinding(cb.m_CurrentSubpass.TargetPipeline->GetRaw().As<GLuint>(), cmd.BindData.Target.As<GLuint>(), cmd.BindData.TargetIndex);

                            break;
                        }

                        case CommandBuffer::CommandType::BindTexture:
                        {
                            glBindTexture(GL_TEXTURE_2D, cmd.BindData.Target.As<GLuint>());
                            glActiveTexture(GL_TEXTURE0 + cmd.BindData.TargetIndex);

                            break;
                        }

                        case CommandBuffer::CommandType::Clear:
                        {
                            glClearColor(cmd.ClearData.r, cmd.ClearData.g, cmd.ClearData.b, cmd.ClearData.a);
                            glClearDepthf(cmd.ClearData.depth);
                            glClearStencil(cmd.ClearData.stencil);

                            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

                            break;
                        }

                        case CommandBuffer::CommandType::Draw:
                        {
                            if (cb.m_CurrentSubpass.TargetPipelineState.Fill == FillType::Wireframe)
                            {
                                PrimitiveType prim = cb.m_CurrentSubpass.TargetPipelineState.Primitive;

                                if (prim == PrimitiveType::Points)
                                {
                                    glDrawArraysInstanced(GL_POINTS, 0, cmd.DrawData.VertexCount, cmd.DrawData.InstanceCount);
                                }
                                else if (prim == PrimitiveType::Lines)
                                {
                                    glDrawArraysInstanced(GL_LINES, 0, cmd.DrawData.VertexCount, cmd.DrawData.InstanceCount);
                                }
                                else if (prim == PrimitiveType::LineStrip
                                        || prim == PrimitiveType::Triangles
                                        || prim == PrimitiveType::TriangleStrip)
                                {
                                    glDrawArraysInstanced(GL_LINE_STRIP, 0, cmd.DrawData.VertexCount, cmd.DrawData.InstanceCount);
                                }
                            }
                            else
                            {
                                glDrawArraysInstanced(cb.m_CurrentSubpass.TargetPipelineState.Primitive, 0, cmd.DrawData.VertexCount, cmd.DrawData.InstanceCount);
                            }

                            break;
                        }

                        case CommandBuffer::CommandType::DrawIndexed:
                        {
                            if (cb.m_CurrentSubpass.TargetPipelineState.Fill == FillType::Wireframe)
                            {
                                PrimitiveType prim = cb.m_CurrentSubpass.TargetPipelineState.Primitive;

                                if (prim == PrimitiveType::Points)
                                {
                                    glDrawElementsInstanced(GL_POINTS, cmd.DrawData.VertexCount, GL_UNSIGNED_INT, nullptr, cmd.DrawData.InstanceCount);
                                }
                                else if (prim == PrimitiveType::Lines)
                                {
                                    glDrawElementsInstanced(GL_LINES, cmd.DrawData.VertexCount, GL_UNSIGNED_INT, nullptr, cmd.DrawData.InstanceCount);
                                }
                                else if (prim == PrimitiveType::LineStrip
                                        || prim == PrimitiveType::Triangles
                                        || prim == PrimitiveType::TriangleStrip)
                                {
                                    glDrawElementsInstanced(GL_LINE_STRIP, cmd.DrawData.VertexCount, GL_UNSIGNED_INT, nullptr, cmd.DrawData.InstanceCount);
                                }
                            }
                            else
                            {
                                glDrawElementsInstanced(cb.m_CurrentSubpass.TargetPipelineState.Primitive, cmd.DrawData.VertexCount, GL_UNSIGNED_INT, nullptr, cmd.DrawData.InstanceCount);
                            }

                            break;
                        }
                    }
                }
            }

            void ES30CommandBuffer::CmdBindPass(CommandBuffer & cb, Pass & pass, uint16_t subpassIndex)
            {
                if (!cb.m_Recording)
                {
                    ErrorReporter(SHRN_PRISM_LEVEL_ERROR, "OpenGLES3.0::CommandBuffer", "BeginRecording() not called before recording command.");
                    return;
                }

                if (cb.m_Passes.count(pass.m_Handle.As<uint32_t>()) == 0)
                {
                    cb.m_Passes[pass.m_Handle.As<uint32_t>()] = pass;
                }

                cb.m_CurrentSubpass = pass.m_Spec.SubpassSpecs[subpassIndex];

                CommandBuffer::Command & cmd = cb.m_Commands.emplace_back();
                cmd.Type = CommandBuffer::CommandType::BindPass;
                cmd.BindData.Target = pass.m_Handle;
                cmd.BindData.TargetIndex = subpassIndex;
            }

            void ES30CommandBuffer::CmdBindFramebuffer(CommandBuffer & cb, Framebuffer & fb)
            {
                if (!cb.m_Recording)
                {
                    ErrorReporter(SHRN_PRISM_LEVEL_ERROR, "OpenGLES3.0::CommandBuffer", "BeginRecording() not called before recording command.");
                    return;
                }

                CommandBuffer::Command & cmd = cb.m_Commands.emplace_back();
                cmd.Type = CommandBuffer::CommandType::BindFramebuffer;
                cmd.BindData.Target = fb.GetRaw();
            }

            void ES30CommandBuffer::CmdBindMesh(CommandBuffer & cb, Mesh & mesh)
            {
                if (!cb.m_Recording)
                {
                    ErrorReporter(SHRN_PRISM_LEVEL_ERROR, "OpenGLES3.0::CommandBuffer", "BeginRecording() not called before recording command.");
                    return;
                }

                CommandBuffer::Command & cmd = cb.m_Commands.emplace_back();
                cmd.Type = CommandBuffer::CommandType::BindMesh;
                cmd.BindData.Target = mesh.GetRaw();
            }

            void ES30CommandBuffer::CmdBindUBO(CommandBuffer & cb, std::string name, uint32_t bindingPoint)
            {
                if (!cb.m_Recording)
                {
                    ErrorReporter(SHRN_PRISM_LEVEL_ERROR, "OpenGLES3.0::CommandBuffer", "BeginRecording() not called before recording command.");
                    return;
                }

                GLuint blockIndex = glGetUniformBlockIndex(cb.m_CurrentSubpass.TargetPipeline->GetRaw().As<GLuint>(), name.c_str());

                CommandBuffer::Command & cmd = cb.m_Commands.emplace_back();
                cmd.Type = CommandBuffer::CommandType::BindUBO;
                cmd.BindData.Target = blockIndex;
                cmd.BindData.TargetIndex = bindingPoint;
            }

            void ES30CommandBuffer::CmdBindTexture(CommandBuffer & cb, Texture & tex, uint16_t slot)
            {
                if (!cb.m_Recording)
                {
                    ErrorReporter(SHRN_PRISM_LEVEL_ERROR, "OpenGLES3.0::CommandBuffer", "BeginRecording() not called before recording command.");
                    return;
                }

                CommandBuffer::Command & cmd = cb.m_Commands.emplace_back();
                cmd.Type = CommandBuffer::CommandType::BindTexture;
                cmd.BindData.Target = tex.GetRaw();
                cmd.BindData.TargetIndex = slot;
            }

            void ES30CommandBuffer::CmdClear(CommandBuffer & cb, float r, float g, float b, float a, float depth, uint32_t stencil)
            {
                if (!cb.m_Recording)
                {
                    ErrorReporter(SHRN_PRISM_LEVEL_ERROR, "OpenGLES3.0::CommandBuffer", "BeginRecording() not called before recording command.");
                    return;
                }

                CommandBuffer::Command & cmd = cb.m_Commands.emplace_back();
                cmd.Type = CommandBuffer::CommandType::Clear;

                cmd.ClearData.r = r;
                cmd.ClearData.g = g;
                cmd.ClearData.b = b;
                cmd.ClearData.a = a;

                cmd.ClearData.depth = depth;
                cmd.ClearData.stencil = stencil;
            }

            void ES30CommandBuffer::CmdDraw(CommandBuffer & cb, uint32_t vtxCount, uint32_t instanceCount)
            {
                if (!cb.m_Recording)
                {
                    ErrorReporter(SHRN_PRISM_LEVEL_ERROR, "OpenGLES3.0::CommandBuffer", "BeginRecording() not called before recording command.");
                    return;
                }

                CommandBuffer::Command & cmd = cb.m_Commands.emplace_back();
                cmd.Type = CommandBuffer::CommandType::Draw;
                cmd.DrawData.VertexCount = vtxCount;
                cmd.DrawData.InstanceCount = instanceCount;
            }

            void ES30CommandBuffer::CmdDrawIndexed(CommandBuffer & cb, uint32_t vtxCount, uint32_t instanceCount)
            {
                if (!cb.m_Recording)
                {
                    ErrorReporter(SHRN_PRISM_LEVEL_ERROR, "OpenGLES3.0::CommandBuffer", "BeginRecording() not called before recording command.");
                    return;
                }

                CommandBuffer::Command & cmd = cb.m_Commands.emplace_back();
                cmd.Type = CommandBuffer::CommandType::DrawIndexed;
                cmd.DrawData.VertexCount = vtxCount;
                cmd.DrawData.InstanceCount = instanceCount;
            }
        }
    }
}
