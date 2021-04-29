#include "hl/api/Pass.hpp"
#include "hl/gl33/Pass.hpp"
#include "hl/gl45/Pass.hpp"
#include "hl/es30/Pass.hpp"

namespace Shroon
{
    namespace Prism
    {
        namespace HL
        {
            void( * Pass::CreateFn)(Pass &, PassSpecification &) = 0;
            void( * Pass::DestroyFn)(Pass &) = 0;

            void APIManager::InitPassAPI()
            {
                switch (s_GraphicsAPI)
                {
                    case API::GL33:
                    {
                        Pass::CreateFn  = GL33Pass::Create;
                        Pass::DestroyFn = GL33Pass::Destroy;

                        break;
                    }

                    case API::GL45:
                    {
                        Pass::CreateFn  = GL45Pass::Create;
                        Pass::DestroyFn = GL45Pass::Destroy;

                        break;
                    }

                    case API::ES30:
                    {
                        Pass::CreateFn  = ES30Pass::Create;
                        Pass::DestroyFn = ES30Pass::Destroy;

                        break;
                    }
                }
            }
        }
    }
}
