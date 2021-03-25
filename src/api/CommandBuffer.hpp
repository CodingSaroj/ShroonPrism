#pragma once

#include "API.hpp"
#include "APIManager.hpp"

#include "api/Buffer.hpp"
#include "api/Mesh.hpp"
#include "api/Pipeline.hpp"
#include "api/Pass.hpp"
#include "api/Framebuffer.hpp"

namespace Shroon
{
    namespace Prism
    {
        class CommandBuffer
        {
        public:
            inline void Create()
            {
                CreateFn(*this);
            }

            inline void Destroy()
            {
                DestroyFn(*this);
            }

            inline void BeginRecording()
            {
                BeginRecordingFn(*this);
            }

            inline void EndRecording()
            {
                EndRecordingFn(*this);
            }

            inline void CmdBeginPass(Pass pass, Framebuffer fb, float clearR, float clearG, float clearB, float clearA)
            {
                CmdBeginPassFn(*this, pass, fb, clearR, clearG, clearB, clearA);
            }

            inline void CmdEndPass()
            {
                CmdEndPassFn(*this);
            }

            inline void CmdBindPipeline(Pipeline pl)
            {
                CmdBindPipelineFn(*this, pl);
            }

            inline void CmdBindMesh(Mesh mesh)
            {
                CmdBindMeshFn(*this, mesh);
            }

            inline void CmdCopyBuffer(Buffer src, uint32_t srcOffset, Buffer dst, uint32_t dstOffset, uint32_t size)
            {
                CmdCopyBufferFn(*this, src, srcOffset, dst, dstOffset, size);
            }

        private:
            Handle m_Handle;

            static void( * CreateFn)(CommandBuffer &);
            static void( * DestroyFn)(CommandBuffer &);

            static void( * BeginRecordingFn)(CommandBuffer &);
            static void( * EndRecordingFn)(CommandBuffer &);

            static void( * CmdBeginPassFn)(CommandBuffer &, Pass, Framebuffer, float, float, float, float);
            static void( * CmdEndPassFn)(CommandBuffer &);

            static void( * CmdBindPipelineFn)(CommandBuffer &, Pipeline);
            static void( * CmdBindMeshFn)(CommandBuffer &, Mesh);

            static void( * CmdCopyBufferFn)(CommandBuffer &, Buffer, uint32_t, Buffer, uint32_t, uint32_t);

            static void( * CmdDrawFn)(CommandBuffer &, uint32_t, uint32_t, uint32_t, uint32_t);
            static void( * CmdDrawIndexedFn)(CommandBuffer &, uint32_t, uint32_t, uint32_t, uint32_t);

            friend class APIManager;

            friend class GL33CommandBuffer;
            friend class GL45CommandBuffer;
            friend class VK12CommandBuffer;
        };
    }
}
