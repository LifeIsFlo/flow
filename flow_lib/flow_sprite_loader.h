#pragma once
#include <vector>
#include <string>
#include <map>
#include "flow_sprite.h"
#include "flow_color.h"

namespace flow
{
    class SpriteLoader
    {
    public:
        static Sprite LoadSprite(const std::string &assetLocation);
        static void GetArtificialColorData(std::vector<Color> &colors, const unsigned int &width, const unsigned int &height);
    };
}