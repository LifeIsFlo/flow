#include "flow_sprite_loader.h"
#include "flow_vector2.h"
#include "flow_color.h"
#include "flow_io.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace flow
{
    Sprite SpriteLoader::LoadSprite(const std::string &assetLocation)
    {
        int width = 0;
        int height = 0;
        int channels = 0;

        // Force 4 channels (RGBA) even if the source PNG is RGB (3 channels)
        // DirectX 11 textures natively prefer 4-component formats like DXGI_FORMAT_R8G8B8A8_UNORM
        unsigned char *pixelData = stbi_load(assetLocation.c_str(), &width, &height, &channels, STBI_rgb_alpha);
        std::vector<Color> colorData{};

        if (!pixelData)
        {
            io::Console::WriteLine("Failed to load image: " + assetLocation);
            GetArtificialColorData(colorData, 500, 500);
        }
        else
        {
            Color *colorDataPtr = reinterpret_cast<Color *>(pixelData);

            size_t pixelCount = static_cast<size_t>(width * height);

            colorData.assign(colorDataPtr, colorDataPtr + pixelCount);

            stbi_image_free(pixelData);
        }

        return Sprite(colorData, width, height);
    }

    void SpriteLoader::GetArtificialColorData(std::vector<Color> &colors, const unsigned int &height, const unsigned int &width)
    {
        colors.resize(height * width);

        for (int i = 0; i < colors.size(); i++)
        {
            int y = floor(i / width);
            int x = i - y * width;
            Vector2 pos = {static_cast<float>(x), static_cast<float>(y)};
            Vector2 center = {width / 2.0f, height / 2.0f};
            float distance = Vector2::Distance(center, pos);

            if (distance < center.x / 2 && distance > center.x / 3)
            {
                colors[i] = Color(255, 0, 0, 255);
            }
            else
            {
                colors[i] = Color(255, 255, 255, 255);
            }
        }
    }
}