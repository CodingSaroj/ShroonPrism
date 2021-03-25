#pragma once

#include "api/CommandBuffer.hpp"

#include "vk12/Core.hpp"
#include "vk12/State.hpp"

namespace Shroon
{
    namespace Prism
    {
        class VK12CommandBuffer
        {
        public:
            static void Create(CommandBuffer & cb);
            static void Destroy(CommandBuffer & cb);

            static void BeginRecording(CommandBuffer & cb);
            static void EndRecording(CommandBuffer & cb);

            static void CmdBeginPass(CommandBuffer & cb, Pass pass, Framebuffer fb, float clearR, float clearG, float clearB, float clearA);
            static void CmdEndPass(CommandBuffer & cb);

            static void CmdBindPipeline(CommandBuffer & cb, Pipeline pl);
            static void CmdBindMesh(CommandBuffer & cb, Mesh mesh);

            static void CmdCopyBuffer(CommandBuffer & cb, Buffer src, uint32_t srcOffset, Buffer dst, uint32_t dstOffset, uint32_t size);

            static void CmdDrawFn(CommandBuffer & cb, uint32_t vtxCount, uint32_t vtxStart, uint32_t instanceCount, uint32_t instanceStart);
            static void CmdDrawIndexedFn(CommandBuffer & cb, uint32_t vtxCount, uint32_t vtxStart, uint32_t instanceCount, uint32_t instanceStart);
        };
    }
}
