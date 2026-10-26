#pragma once
#include "flow_vertex.h"
#include <vector>
#include <span>

namespace flow
{
    struct Material
	{
		std::string shaderLocation;
		std::string textureLocation;
		Color baseColor;
	};
}