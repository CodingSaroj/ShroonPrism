#pragma once

#include "hl/API.hpp"
#include "hl/APIManager.hpp"

#include "hl/api/Buffer.hpp"
#include "hl/api/Mesh.hpp"
#include "hl/api/Pipeline.hpp"
#include "hl/api/Pass.hpp"
#include "hl/api/Framebuffer.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            class CommandBuffer
            {
            public:
                inline ~CommandBuffer()
                {
                    DestroyFn(*this);
                }

                inline void Create()
                {
                    CreateFn(*this);
                }

                inline void BeginRecording()
                {
                    BeginRecordingFn(*this);
                }

                inline void EndRecording()
                {
                    EndRecordingFn(*this);
                }

                inline void Submit()
                {
                    SubmitFn(*this);
                }

                inline void CmdBindPass(Pass & pass, uint16_t subpassIndex = 0)
                {
                    CmdBindPassFn(*this, pass, subpassIndex);
                }

                inline void CmdBindFramebuffer(Framebuffer & fb)
                {
                    CmdBindFramebufferFn(*this, fb);
                }

                inline void CmdBindMesh(Mesh & mesh)
                {
                    CmdBindMeshFn(*this, mesh);
                }

                inline void CmdBindUBO(std::string name, uint32_t bindingPoint)
                {
                    CmdBindUBOFn(*this, name, bindingPoint);
                }

                inline void CmdBindTexture(Texture & tex, uint16_t slot = 0)
                {
                    CmdBindTextureFn(*this, tex, slot);
                }

                inline void CmdClear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f, float depth = 0.0f, uint32_t stencil = 0)
                {
                    CmdClearFn(*this, r, g, b, a, depth, stencil);
                }

                inline void CmdDraw(uint32_t vertexCount, uint32_t instanceCount)
                {
                    CmdDrawFn(*this, vertexCount, instanceCount);
                }

                inline void CmdDrawIndexed(uint32_t vertexCount, uint32_t instanceCount)
                {
                    CmdDrawIndexedFn(*this, vertexCount, instanceCount);
                }

            private:
                enum class CommandType : uint8_t
                {
                    BindPass,
                    BindFramebuffer,
                    BindMesh,
                    BindUBO,
                    BindTexture,

                    Clear,

                    Draw,
                    DrawIndexed
                };

                struct Command
                {
                    CommandType Type;

                    union
                    {
                        struct
                        {
                            Handle Target;
                            uint16_t TargetIndex;
                        } BindData;

                        struct
                        {
                            float r, g, b, a;
                            float depth;
                            uint32_t stencil;
                        } ClearData;

                        struct
                        {
                            uint32_t VertexCount;
                            uint32_t InstanceCount;
                        } DrawData;
                    };
                };

                Handle m_Handle;

                bool m_Recording = false;
                std::vector<Command> m_Commands;

                std::unordered_map<uint32_t, Pass> m_Passes;
                PassSpecification::SubpassSpecification m_CurrentSubpass;

                static void( * CreateFn)(CommandBuffer &);
                static void( * DestroyFn)(CommandBuffer &);

                static void( * BeginRecordingFn)(CommandBuffer &);
                static void( * EndRecordingFn)(CommandBuffer &);

                static void( * SubmitFn)(CommandBuffer &);

                static void( * CmdBindPassFn)(CommandBuffer &, Pass &, uint16_t);
                static void( * CmdBindFramebufferFn)(CommandBuffer &, Framebuffer &);
                static void( * CmdBindMeshFn)(CommandBuffer &, Mesh &);
                static void( * CmdBindUBOFn)(CommandBuffer &, std::string, uint32_t);
                static void( * CmdBindTextureFn)(CommandBuffer &, Texture &, uint16_t);

                static void( * CmdClearFn)(CommandBuffer &, float, float, float, float, float, uint32_t);

                static void( * CmdDrawFn)(CommandBuffer &, uint32_t, uint32_t);
                static void( * CmdDrawIndexedFn)(CommandBuffer &, uint32_t, uint32_t);

                friend class APIManager;

                friend class GL33CommandBuffer;
                friend class GL45CommandBuffer;
                friend class ES20CommandBuffer;
                friend class ES30CommandBuffer;
            };
        }
    }
}
