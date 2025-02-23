#include "tetrapc.h"
#include "OBJParser.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include "Utils.h"
#include "../rendering/VertexData.h"

using namespace TetraEngine;

std::ifstream OBJParser::stream;

int OBJParser::OBJRead(const std::string& path) {
    
    stream = std::ifstream(path);
    if (!stream.good())
        std::cout << "file couldnt open";

    int vertIDUV = 0;

    std::string line;

    std::vector<glm::vec3> coords;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    std::vector<Vertex> verts;
    std::vector<unsigned int> faces;

    std::shared_ptr<VertexData> mesh = nullptr;

    while (!stream.eof())
    {
        std::getline(stream, line);
        std::vector<std::string> words;
        Utils::Words(line, words, ' ');
        if (words.size() > 0) {

            if (words[0] == "o") {
                if (mesh != nullptr) {
                    //new object in file
                    //load generated one
                    mesh->LoadVerts(&verts[0], verts.size());
                    mesh->LoadFaces(&faces[0], faces.size());
                    mesh->Setup();

                    std::cout << "mesh has been loaded\n";
                }
                mesh = VertexData::CreateVertexData();
                verts.clear();
                faces.clear();

            }
            if (words[0] == "v") {
                glm::vec3 pos;
                pos.x = std::stof(words[1]);
                pos.y = std::stof(words[2]);
                pos.z = std::stof(words[3]);
                coords.push_back(pos);

            }
            if (words[0] == "vn") {
                glm::vec3 n;
                n.x = std::stof(words[1]);
                n.y = std::stof(words[2]);
                n.z = std::stof(words[3]);
                normals.push_back(n);
            }
            if (words[0] == "vt") {
                glm::vec2 uv;
                uv.x = std::stof(words[1]);
                uv.y = std::stof(words[2]);
                uvs.push_back(uv);
            }


            if (words[0] == "f") {
                // for TRIS
                if (words.size() == 4)
                {
                    std::vector<int> vertIDs;
                    for (int i = 1; i < 4; i++)
                    {
                        std::vector<std::string> values;
                        Utils::Words(words[i], values, '/');
                        unsigned int posID = std::stoi(values[0]) - 1;
                        unsigned int uvID = std::stoi(values[1]) - 1;
                        unsigned int normalID = std::stoi(values[2]) - 1;

                        glm::vec3 pos = coords[posID];
                        glm::vec2 uv = uvs[uvID];
                        glm::vec3 n = normals[normalID];

                        vertIDs.push_back(posID);
                        verts.push_back(Vertex(pos.x, pos.y, pos.z, uv.x, uv.y, n.x, n.y, n.z));
                    }
                    faces.push_back(verts.size() - 1);
                    faces.push_back(verts.size() - 2);
                    faces.push_back(verts.size() - 3);
                }
                //for QUADS
                if (words.size() == 5)
                {
                    std::vector<int> vertIDs;
                    for (int i = 1; i < 5; i++)
                    {
                        std::vector<std::string> values;
                        Utils::Words(words[i], values, '/');
                        unsigned int posID = std::stoi(values[0]) - 1;
                        unsigned int uvID = std::stoi(values[1]) - 1;
                        unsigned int normalID = std::stoi(values[2]) - 1;

                        glm::vec3 pos = coords[posID];
                        glm::vec2 uv = uvs[uvID];
                        glm::vec3 n = normals[normalID];

                        vertIDs.push_back(posID);
                        verts.push_back(Vertex(pos.x, pos.y, pos.z, uv.x, uv.y, n.x, n.y, n.z));
                    }
                    faces.push_back(verts.size() - 4);
                    faces.push_back(verts.size() - 3);
                    faces.push_back(verts.size() - 2);
                    faces.push_back(verts.size() - 4);
                    faces.push_back(verts.size() - 2);
                    faces.push_back(verts.size() - 1);
                }
            }
        }
    }
    if (mesh != nullptr) {
        //load last object
        mesh->LoadVerts(&verts[0], verts.size());
        mesh->LoadFaces(&faces[0], faces.size());
        mesh->Setup();

        std::cout << "mesh has been loaded\n";
        return mesh->id;
    }
}