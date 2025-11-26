#pragma once

#include <glm/glm.hpp>

#include <memory>
#include <vector>

#define VD_RECTANGLE 0
#define VD_RECTANGLE_Y 1
#define VD_CUBE 2
#define VD_SUZANNE 3

namespace TetraEngine {
	struct Vertex
	{
		float x, y, z;
		float u, v;
		float nx, ny, nz;
		Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz) : x(x), y(y), z(z), nx(nx), ny(ny), nz(nz), u(u), v(v)
		{
		}
	};

	class VertexData
	{
	    std::vector<Vertex> verts;
	    std::vector<unsigned int> faces;
	public:
		static int lastId;
		static std::vector<std::shared_ptr<VertexData>> collection;
		static void InitialisePrefabs();
		static std::shared_ptr<VertexData> GetPrefab(int id);
		static std::shared_ptr<VertexData> CreateVertexData();
		static std::shared_ptr<VertexData> LoadFromFile(const char* path);

		int id;
		std::vector<int> materialMap;
		float* vert;
		unsigned int VBO;
		unsigned int VAO;
		unsigned int EBO;
		VertexData(int id);
		void Setup();
		void Update();
		void LoadVerts(Vertex* ptr, int size);
		void LoadFaces(unsigned int* ptr, int size);
		void AddVert(Vertex vert);
		void Draw();
		void Transform(glm::mat4 transform);

	    std::vector<Vertex>& GetVertices();
        std::vector<uint>& GetFaces();

	};
}

