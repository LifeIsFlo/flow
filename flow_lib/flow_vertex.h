#pragma once
#include "flow_vector2.h"
#include "flow_vector3.h"
#include "flow_color.h"

namespace flow
{
    struct Vertex
	{
		Vector3 position;
		Vector3 normal;
		Vector2 uv;
		Color color;
	};
} 
