#pragma once
#include "flow_vector3.h"
#include "flow_model.h"
#include "flow_mesh.h"
#include <string>
#include <cstdint>
#include <fstream>
#include <thread>
#include <vector>
#include <string>

namespace flow
{
    class ObjLoader
	{
	public:
		enum ObjFileCommand : uint8_t
		{
			COMMAND_OBJECT_NAME,
			COMMAND_FACE,
			COMMAND_VERTEX,
			COMMAND_TEXTURECOORD,
			COMMAND_NORMAL,
			COMMAND_MATERIAL,
			COMMAND_SMOOTHING,
			COMMENT
		};

		static Model LoadModel(const std::string& assetLocation);
		static bool CompileLine(const std::string& line, ObjFileCommand& obj_file_command, Vector3& value, std::vector<std::string>& paramsOut);

		static const std::string POS;
		static const std::string TEX;
		static const std::string NOR;
		static const std::string MTL;
		static const std::string OBJ;
		static const std::string FACE;
		static const std::string SMOOTHING;
	};


}