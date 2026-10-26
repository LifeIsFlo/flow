#include "flow_asset_database.h"
#include "flow_obj_loader.h"
#include "flow_sprite_loader.h"
#include "flow_io.h"
#include "flow_asset_registry.h"
#include <memory>


namespace flow
{
    AssetRegistry<Model> AssetDatabase::_models;
	AssetRegistry<Sprite> AssetDatabase::_sprites;
	AssetRegistry<Material> AssetDatabase::_materials;

	Model* AssetDatabase::LoadModel(const std::string& assetLocation)
	{
		io::Console::WriteLine("Loading model: " + assetLocation + "...");

		if (!_models.contains(assetLocation)) {
			Model* ret = nullptr;

			if(AssetPathExists(assetLocation))
			if (assetLocation.ends_with(".obj")) {
				_models[assetLocation] = std::make_unique<Model>(ObjLoader::LoadModel(assetLocation));
				ret = _models[assetLocation].get();
			}
			if (ret) io::Console::WriteLine("Model loaded!");
			return ret;
		}

		io::Console::WriteLine("Model found!");
		return _models[assetLocation].get();
	}

	Sprite* AssetDatabase::LoadSprite(const std::string& assetLocation)
	{
	    io::Console::WriteLine("Loading sprite: " + assetLocation + "...");
		if (!_sprites.contains(assetLocation))
		{
			if (AssetPathExists(assetLocation)) {

				_sprites[assetLocation] = std::make_unique<Sprite>(SpriteLoader::LoadSprite(assetLocation));
                
				Sprite* ret = _sprites[assetLocation].get();

				if (ret) io::Console::WriteLine("Sprite loaded!");
				return ret;
			}
			return nullptr;
		}

		io::Console::WriteLine("Sprite found!");
		return _sprites[assetLocation].get();
	}

	Material* AssetDatabase::LoadMaterial(const std::string& assetLocation)
	{
		return nullptr;
	}

	bool AssetDatabase::AssetPathExists(const std::string& assetLocation)
	{
		bool ret = std::filesystem::exists(assetLocation);
		if (!ret)
		{
			io::Console::WriteLine("Asset file does not exist!: " + assetLocation);
		}
		return ret;
	}

	void AssetDatabase::CleanUp()
	{
		_models.clear();
		_sprites.clear();
		_materials.clear();
	}
}