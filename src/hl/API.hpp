#pragma once

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            enum class API
            {
                GL33,
                GL45,
                ES20,
                ES30
            };

            extern API s_GraphicsAPI;
        }
    }
}
