#pragma once

#include "Triangle.h"
#include "Vec3.h"
#include <vector>
#include <string>
#include "Texture.h"

namespace Rasterizer
{
	class Mesh
	{
	public:
		Mesh() = default;
		~Mesh() = default;

		static Mesh* CreateCube();
		static Mesh* CreateSphere(int sectorCount, int stackCount);
		static Mesh* CreateFromFile(std::string filePath);
		Vertex Parse(std::string const& string);

		void ReserveCoordinate(int quantity);
		int AddCoordinate(float x, float y, float z = 0.f);
		std::vector<Vec3> const& GetCoordinateBuffer() const;

		void ReserveNormal(int quantity);
		int AddNormal(float x, float y, float z = 0.f);
		std::vector<Vec3> const& GetNormalBuffer() const;

		void ReserveTexel(int quantity);
		int AddTexel(float u, float v);
		std::vector<Vec2> const& GetTexelBuffer() const;

		void ReserveTriangle(int quantity);
		void AddTriangle(Vertex const& WindowVertexA, Vertex const& WindowVertexB, Vertex const& WindowVertexC, Texture* texture = nullptr);
		std::vector<Triangle> const& GetTriangleBuffer() const;


	private:
		std::vector<Triangle> m_triangleBuffer;
		std::vector<Vec3> m_coordinateBuffer;
		std::vector<Vec3> m_normalBuffer;
		std::vector<Vec2> m_texelBuffer;
	};
}