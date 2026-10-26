#pragma once
#include "flow_model.h"
#include "flow_material.h"
#include "flow_sprite.h"
#include "flow_asset_registry.h"

namespace flow
{
    class AssetDatabase
    {
        static AssetRegistry<Model> _models;
        static AssetRegistry<Sprite> _sprites;
        static AssetRegistry<Material> _materials;
    public:
        static Model* LoadModel(const std::string &assetLocation);
        static Material* LoadMaterial(const std::string &assetLocation);
        static Sprite* LoadSprite(const std::string &assetLocation);

        static void CleanUp();

        static bool AssetPathExists(const std::string &assetLocation);

        template <typename T>
        static bool AssetLoaded(const std::string &assetLocation)
        {
		    return AssetRegistry<T>::registry.contains(assetLocation);
        }
    };
}
