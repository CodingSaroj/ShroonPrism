#pragma once

#include <iostream>
#include <functional>
#include <string>

#define SHRN_PRISM_LEVEL_INFO 0
#define SHRN_PRISM_LEVEL_WARNING 1
#define SHRN_PRISM_LEVEL_ERROR 2
#define SHRN_PRISM_LEVEL_FATAL 3

namespace Shroon
{
    namespace Prism
    {
        extern std::function<void(int, std::string, std::string)> ErrorReporter;
    }
}

#ifdef SHRN_PRISM_DEFAULT_ERROR_REPORTER
    std::function<void(int, std::string, std::string)> Shroon::Prism::ErrorReporter = [](int level, std::string location, std::string log)
    {
        switch (level)
        {
            case SHRN_PRISM_LEVEL_INFO: { std::cout<<"Info::"; break; }
            case SHRN_PRISM_LEVEL_WARNING: { std::cerr<<"Warning::"; break; }
            case SHRN_PRISM_LEVEL_ERROR: { std::cerr<<"Error::"; break; }
            case SHRN_PRISM_LEVEL_FATAL: { std::cerr<<"FatalError::"; break; }
        }

        if (level == SHRN_PRISM_LEVEL_INFO)
        {
            std::cout<<location<<": "<<log<<"\n";
        }
        else
        {
            std::cerr<<location<<": "<<log<<"\n";
        }
    };
#endif
