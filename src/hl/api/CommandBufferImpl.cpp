#include "hl/api/CommandBuffer.hpp"
#include "hl/gl33/CommandBuffer.hpp"
#include "hl/gl45/CommandBuffer.hpp"
#include "hl/es30/CommandBuffer.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            void( * CommandBuffer::CreateFn)(CommandBuffer &) = 0;
            void( * CommandBuffer::DestroyFn)(CommandBuffer &) = 0;

            void( * CommandBuffer::BeginRecordingFn)(CommandBuffer &) = 0;
            void( * CommandBuffer::EndRecordingFn)(CommandBuffer &) = 0;

            void( * CommandBuffer::SubmitFn)(CommandBuffer &) = 0;

            void( * CommandBuffer::CmdBindPassFn)(CommandBuffer &, Pass &, uint16_t) = 0;
            void( * CommandBuffer::CmdBindFramebufferFn)(CommandBuffer &, Framebuffer &) = 0;
            void( * CommandBuffer::CmdBindMeshFn)(CommandBuffer &, Mesh &) = 0;
            void( * CommandBuffer::CmdBindUBOFn)(CommandBuffer &, std::string, uint32_t) = 0;
            void( * CommandBuffer::CmdBindTextureFn)(CommandBuffer &, Texture &, uint16_t) = 0;

            void( * CommandBuffer::CmdClearFn)(CommandBuffer &, float, float, float, float, float, uint32_t) = 0;

            void( * CommandBuffer::CmdDrawFn)(CommandBuffer &, uint32_t, uint32_t) = 0;
            void( * CommandBuffer::CmdDrawIndexedFn)(CommandBuffer &, uint32_t, uint32_t) = 0;

            void APIManager::InitCommandBufferAPI()
            {
                switch (s_GraphicsAPI)
                {
                    case API::GL33:
                    {
                        CommandBuffer::CreateFn             = GL33CommandBuffer::Create;
                        CommandBuffer::DestroyFn            = GL33CommandBuffer::Destroy;

                        CommandBuffer::BeginRecordingFn     = GL33CommandBuffer::BeginRecording;
                        CommandBuffer::EndRecordingFn       = GL33CommandBuffer::EndRecording;

                        CommandBuffer::SubmitFn             = GL33CommandBuffer::Submit;

                        CommandBuffer::CmdBindPassFn        = GL33CommandBuffer::CmdBindPass;
                        CommandBuffer::CmdBindFramebufferFn = GL33CommandBuffer::CmdBindFramebuffer;
                        CommandBuffer::CmdBindMeshFn        = GL33CommandBuffer::CmdBindMesh;
                        CommandBuffer::CmdBindUBOFn         = GL33CommandBuffer::CmdBindUBO;
                        CommandBuffer::CmdBindTextureFn     = GL33CommandBuffer::CmdBindTexture;

                        CommandBuffer::CmdClearFn           = GL33CommandBuffer::CmdClear;

                        CommandBuffer::CmdDrawFn            = GL33CommandBuffer::CmdDraw;
                        CommandBuffer::CmdDrawIndexedFn     = GL33CommandBuffer::CmdDrawIndexed;

                        break;
                    }

                    case API::GL45:
                    {
                        CommandBuffer::CreateFn             = GL45CommandBuffer::Create;
                        CommandBuffer::DestroyFn            = GL45CommandBuffer::Destroy;

                        CommandBuffer::BeginRecordingFn     = GL45CommandBuffer::BeginRecording;
                        CommandBuffer::EndRecordingFn       = GL45CommandBuffer::EndRecording;

                        CommandBuffer::SubmitFn             = GL45CommandBuffer::Submit;

                        CommandBuffer::CmdBindPassFn        = GL45CommandBuffer::CmdBindPass;
                        CommandBuffer::CmdBindFramebufferFn = GL45CommandBuffer::CmdBindFramebuffer;
                        CommandBuffer::CmdBindMeshFn        = GL45CommandBuffer::CmdBindMesh;
                        CommandBuffer::CmdBindUBOFn         = GL45CommandBuffer::CmdBindUBO;
                        CommandBuffer::CmdBindTextureFn     = GL45CommandBuffer::CmdBindTexture;

                        CommandBuffer::CmdClearFn           = GL45CommandBuffer::CmdClear;

                        CommandBuffer::CmdDrawFn            = GL45CommandBuffer::CmdDraw;
                        CommandBuffer::CmdDrawIndexedFn     = GL45CommandBuffer::CmdDrawIndexed;

                        break;
                    }

                    case API::ES30:
                    {
                        CommandBuffer::CreateFn             = ES30CommandBuffer::Create;
                        CommandBuffer::DestroyFn            = ES30CommandBuffer::Destroy;

                        CommandBuffer::BeginRecordingFn     = ES30CommandBuffer::BeginRecording;
                        CommandBuffer::EndRecordingFn       = ES30CommandBuffer::EndRecording;

                        CommandBuffer::SubmitFn             = ES30CommandBuffer::Submit;

                        CommandBuffer::CmdBindPassFn        = ES30CommandBuffer::CmdBindPass;
                        CommandBuffer::CmdBindFramebufferFn = ES30CommandBuffer::CmdBindFramebuffer;
                        CommandBuffer::CmdBindMeshFn        = ES30CommandBuffer::CmdBindMesh;
                        CommandBuffer::CmdBindUBOFn         = ES30CommandBuffer::CmdBindUBO;
                        CommandBuffer::CmdBindTextureFn     = ES30CommandBuffer::CmdBindTexture;

                        CommandBuffer::CmdClearFn           = ES30CommandBuffer::CmdClear;

                        CommandBuffer::CmdDrawFn            = ES30CommandBuffer::CmdDraw;
                        CommandBuffer::CmdDrawIndexedFn     = ES30CommandBuffer::CmdDrawIndexed;

                        break;
                    }
                }
            }
        }
    }
}
