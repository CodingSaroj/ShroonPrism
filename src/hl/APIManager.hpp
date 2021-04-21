#pragma once

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            class APIManager
            {
            public:
                inline static void InitAPI()
                {
                    InitWindowAPI();

                    InitBufferAPI();
                    InitMeshAPI();
                    InitTextureAPI();
                    InitFramebufferAPI();
                    InitPipelineAPI();
                    InitCommandBufferAPI();
                    InitPassAPI();
                }

            private:
                static void InitWindowAPI();

                static void InitBufferAPI();
                static void InitMeshAPI();
                static void InitTextureAPI();
                static void InitFramebufferAPI();
                static void InitPipelineAPI();
                static void InitCommandBufferAPI();
                static void InitPassAPI();
            };
        }
    }
}
