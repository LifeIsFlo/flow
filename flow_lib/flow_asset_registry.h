#pragma once
#include "flow_fwd.h"
#include <string>
#include <map>

namespace flow
{
    template <typename T>
    struct AssetRegistry
    {
        std::map<std::string, std::unique_ptr<T>> registry;

        std::unique_ptr<T>& operator [] (const std::string& assetLocation)
        {
            return registry[assetLocation];
        }

        bool contains(const std::string& assetLocation){
            return registry.contains(assetLocation);
        }

        void clear()
        {
            registry.clear();
        }
    };
}
