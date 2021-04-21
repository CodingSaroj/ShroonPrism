#pragma once

#include "ErrorReporter.hpp"

#include "hl/api/CommandBuffer.hpp"

#include "hl/gl45/Core.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            class GL45CommandBuffer
            {
            public:
                static void Create(CommandBuffer & cb);
                static void Destroy(CommandBuffer & cb);

                static void BeginRecording(CommandBuffer & cb);
                static void EndRecording(CommandBuffer & cb);

                static void Submit(CommandBuffer & cb);

                static void CmdBindPass(CommandBuffer & cb, Pass & pass, uint16_t subpassIndex);
                static void CmdBindFramebuffer(CommandBuffer & cb, Framebuffer & fb);
                static void CmdBindMesh(CommandBuffer & cb, Mesh & mesh);
                static void CmdBindUBO(CommandBuffer & cb, std::string name, uint32_t bindingPoint);
                static void CmdBindTexture(CommandBuffer & cb, Texture & tex, uint16_t slot);

                static void CmdClear(CommandBuffer & cb, float r, float g, float b, float a, float depth, uint32_t stencil);

                static void CmdDraw(CommandBuffer & cb, uint32_t vtxCount, uint32_t instanceCount);
                static void CmdDrawIndexed(CommandBuffer & cb, uint32_t vtxCount, uint32_t instanceCount);
            };
        }
    }
}
