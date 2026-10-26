#include <map>
#include "flow_model.h"
#include "flow_obj_loader.h"
#include "flow_mesh.h"
#include "flow_util.h"

namespace flow
{
    const std::string ObjLoader::POS = "v";
    const std::string ObjLoader::TEX = "vt";
    const std::string ObjLoader::NOR = "vn";
    const std::string ObjLoader::MTL = "usemtl";
    const std::string ObjLoader::OBJ = "o";
    const std::string ObjLoader::FACE = "f";
    const std::string ObjLoader::SMOOTHING = "s";

    Model ObjLoader::LoadModel(const std::string &assetLocation)
    {
        std::ifstream fileStream(assetLocation);

        bool canStillRun = true;

        unsigned int addToVertexIndex = 0;
        unsigned int addToTextureIndex = 0;
        unsigned int addToNormalIndex = 0;

        Model model;
        std::string lastName = "";
        std::string meshName = "";

        while (canStillRun)
        {
            bool meshFinished = false;
            // obj data
            std::vector<Vector3> positions;
            std::vector<Vector3> normals;
            std::vector<Vector2> textureCoords;

            // mesh data
            std::vector<unsigned int> indices;
            std::vector<Vertex> vertices;

            // changed every file line
            std::string currentLine;
            Vector3 value;
            std::vector<std::string> params;
            ObjFileCommand command;

            int indexCount = 0;

            while (!meshFinished && std::getline(fileStream, currentLine))
            {
                bool hr = CompileLine(currentLine, command, value, params);
                if (!hr)
                    continue;

                bool shouldChange = true;

                switch (command)
                {
                case COMMAND_OBJECT_NAME:
                    if (!lastName.empty())
                    {
                        meshFinished = true;
                    }
                    else
                    {
                        meshName = params[0];
                        shouldChange = false;
                    }
                    if (shouldChange)
                        meshName = lastName;
                    lastName = params[0];
                    break;
                case COMMAND_NORMAL:
                    normals.push_back({value.x, value.y, value.z});
                    break;
                case COMMAND_VERTEX:
                    positions.push_back({value.x, value.y, value.z});
                    break;
                case COMMAND_TEXTURECOORD:
                    textureCoords.push_back({value.x, value.y});
                    break;
                case COMMAND_FACE:
                    // per index in face [0] = vertexIndex, [1] = textureCoordIndex, [2] = normalIndex
                    for (int i = 0; i < params.size(); i++)
                    {
                        std::vector<std::string> sep;
                        util::string_split(params[i], &sep, '/');

                        int positionIndex = std::stoi(sep[0]) - addToVertexIndex - 1;
                        int uvIndex = std::stoi(sep[1]) - addToTextureIndex - 1;
                        int normalIndex = std::stoi(sep[2]) - addToNormalIndex - 1;

                        vertices.push_back({positions[positionIndex], normals[normalIndex], textureCoords[uvIndex], Color()});

                        indices.push_back(indexCount);
                        indexCount++;
                    }
                    break;
                }
            }
            if (!meshFinished)
            {
                meshName = lastName;
            }
            model.AddMesh(meshName, indices, vertices);
            canStillRun = meshFinished;
            addToVertexIndex += positions.size();
            addToTextureIndex += textureCoords.size();
            addToNormalIndex += normals.size();
        }
        return model;
    }

    bool ObjLoader::CompileLine(const std::string &line, ObjFileCommand &obj_file_command, Vector3 &value, std::vector<std::string> &paramsOut)
    {
        obj_file_command = COMMENT;
        value = Vector3::ZERO;
        paramsOut.clear();

        std::string command = line.substr(0, line.find_first_of(' '));

        obj_file_command = [command]()
        {
            if (command == POS)
                return COMMAND_VERTEX;
            if (command == TEX)
                return COMMAND_TEXTURECOORD;
            if (command == NOR)
                return COMMAND_NORMAL;
            if (command == FACE)
                return COMMAND_FACE;
            if (command == OBJ)
                return COMMAND_OBJECT_NAME;
            if (command == MTL)
                return COMMAND_MATERIAL;
            if (command == SMOOTHING)
                return COMMAND_SMOOTHING;

            return COMMENT;
        }();

        int commandSize = command.size() + 1;

        if (commandSize != 0)
        {

            std::vector<std::string> params;

            auto substrParams = line.substr(commandSize, line.size() - commandSize);

            util::string_split(substrParams, &params, ' ');

            switch (obj_file_command)
            {
            case COMMAND_NORMAL:
            case COMMAND_TEXTURECOORD:
            case COMMAND_VERTEX:
                value.x = std::stof(params[0]);
                if (params.size() >= 2)
                    value.y = std::stof(params[1]);
                if (params.size() >= 3)
                    value.z = std::stof(params[2]);
                break;
            default:
                break;
            }

            paramsOut = params;

            return true;
        }
        return false;
    }
}