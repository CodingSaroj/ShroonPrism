#pragma once

namespace Shroon
{
    namespace Prism
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
                InitUniformManagerAPI();
                InitShaderAPI();
                InitFramebufferAPI();
                InitPipelineAPI();
            }

        private:
            static void InitWindowAPI();

            static void InitBufferAPI();
            static void InitMeshAPI();
            static void InitTextureAPI();
            static void InitUniformManagerAPI();
            static void InitShaderAPI();
            static void InitFramebufferAPI();
            static void InitPipelineAPI();
        };
    }
}
