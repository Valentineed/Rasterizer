#include "pch.h"
#include "Mesh.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cassert>

#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>

namespace Rasterizer
{
	Mesh* Mesh::CreateCube()
	{
		Mesh* cube = new Mesh();
		Texture* texture = new Texture("Asset\\lol.png");

		// vertices
		cube->ReserveCoordinate(8);
		const float v = sqrtf(1.f / 3.f);
		int right_up_front = cube->AddCoordinate(v, v, v);
		int left_up_front = cube->AddCoordinate(-v, v, v);
		int right_down_front = cube->AddCoordinate(v, -v, v);
		int left_down_front = cube->AddCoordinate(-v, -v, v);
		int right_up_back = cube->AddCoordinate(v, v, -v);
		int left_up_back = cube->AddCoordinate(-v, v, -v);
		int right_down_back = cube->AddCoordinate(v, -v, -v);
		int left_down_back = cube->AddCoordinate(-v, -v, -v);

		// texel
		cube->ReserveTexel(4);
		int left_down = cube->AddTexel(-1.f, -1.f);
		int right_down = cube->AddTexel(2.f, -1.f);
		int left_up = cube->AddTexel(-1.f, 2.f);
		int right_up = cube->AddTexel(2.f, 2.f);

		// normal
		cube->ReserveNormal(6);
		int right = cube->AddNormal(1.f, 0.f, 0.f);
		int up = cube->AddNormal(0.f, 1.f, 0.f);
		int front = cube->AddNormal(0.f, 0.f, 1.f);
		int left = cube->AddNormal(-1.f, 0.f, 0.f);
		int down = cube->AddNormal(0.f, -1.f, 0.f);
		int back = cube->AddNormal(0.f, 0.f, -1.f);

		cube->ReserveTriangle(12);
		// front face
		cube->AddTriangle({ left_up_front, front, left_up }, { left_down_front, front, left_down }, { right_up_front, front, right_up }, texture);
		cube->AddTriangle({ right_up_front, front, right_up }, { left_down_front, front, left_down }, { right_down_front, front, right_down }, texture);
		// up face
		cube->AddTriangle({ right_up_front, up, right_down }, { right_up_back, up, right_up }, { left_up_back, up, left_up }, texture);
		cube->AddTriangle({ right_up_front, up, right_down }, { left_up_back, up, left_up }, { left_up_front, up, left_down }, texture);
		// right face
		cube->AddTriangle({ right_down_back, right, right_down }, { right_up_back, right, right_up }, { right_up_front, right, left_up }, texture);
		cube->AddTriangle({ right_down_back, right, right_down }, { right_up_front, right, left_up }, { right_down_front, right, left_down }, texture);
		// back face
		cube->AddTriangle({ left_down_back, back, left_down }, { left_up_back, back, left_up }, { right_up_back, back, right_up }, texture);
		cube->AddTriangle({ left_down_back, back, left_down }, { right_up_back, back, right_up }, { right_down_back, back, right_down },  texture);
		// down face
		cube->AddTriangle({ left_down_back, down, left_down }, { right_down_back, down, right_down }, { right_down_front, down, right_up }, texture);
		cube->AddTriangle({ left_down_back, down, left_down }, { right_down_front, down, right_up }, { left_down_front, down, left_up }, texture);
		// left face
		cube->AddTriangle({ left_down_front, left, right_down }, { left_up_back, left, left_up }, { left_down_back, left, left_down }, texture);
		cube->AddTriangle({ left_down_front, left, right_down }, { left_up_front, left, right_up }, { left_up_back, left, left_up },  texture);

		return cube;
	}

	Mesh* Mesh::CreateSphere(int sectorCount, int stackCount)
	{
		Mesh* sphere = new Mesh();
		sphere->AddTexel(0.f, 0.f); // TODO: redesign: so that m_texelbuffer[0] don't blow up

		sphere->ReserveCoordinate(((size_t)sectorCount - 1) * (size_t)stackCount + 2);
		sphere->ReserveNormal(((size_t)sectorCount - 1) * (size_t)stackCount + 2);
		sphere->ReserveTriangle(((size_t)sectorCount - 1) * (size_t)stackCount * 2);

		float sectorStep = (float)M_PI / sectorCount;
		float stackStep = 2 * (float)M_PI / stackCount;

		for (int sector = 1; sector < sectorCount; sector++)
		{
			float sectorAngle = (float)M_PI / 2 - sector * sectorStep;
			float parallelY = sinf(sectorAngle); // * sphereRadius (1)
			float parallelRadius = cosf(sectorAngle); // * sphereRadius (1)

			for (int stack = 0; stack < stackCount; stack++)
			{
				float stackAngle = stack * stackStep;
				float meridianX = cosf(stackAngle) * parallelRadius;
				float meridianZ = sinf(stackAngle) * parallelRadius;

				sphere->AddCoordinate(meridianX, parallelY, meridianZ);
				sphere->AddNormal(meridianX, parallelY, meridianZ);
			}
		}

		sphere->AddCoordinate(0.f, 1.f, 0.f); // north pole
		sphere->AddCoordinate(0.f, -1.f, 0.f); // south pole
		sphere->AddNormal(0.f, 1.f, 0.f); // north pole
		sphere->AddNormal(0.f, -1.f, 0.f); // south pole

		int northPoleIndex = (int)sphere->m_coordinateBuffer.size() - 2;
		int southPoleIndex = (int)sphere->m_coordinateBuffer.size() - 1;
		int lastParallelIndex = (int)sphere->m_coordinateBuffer.size() - 3;

		for (int sector = 0; sector < sectorCount - 1; sector++)
		{
			for (int stack = 0; stack < stackCount; stack++)
			{
				int flatSide1 = sector * stackCount + stack;
				int top = flatSide1 - stackCount;
				int flatSide2 = sector * stackCount + (stack + 1 + stackCount) % stackCount;
				int bottom = flatSide2 + stackCount;

				if (top < 0)
				{
					top = northPoleIndex;
				}
				else if (bottom > lastParallelIndex)
				{
					bottom = southPoleIndex;
				}

				sphere->AddTriangle({ flatSide1, flatSide1 }, { top, top }, { flatSide2, flatSide2 });
				sphere->AddTriangle({ bottom, bottom }, { flatSide1, flatSide1 }, { flatSide2, flatSide2 });
			}
		}
		return sphere;
	}

	Mesh* Mesh::CreateFromFile(std::string filePath)
	{
		//https://en.wikipedia.org/wiki/Wavefront_.obj_file
		Mesh* obj = new Mesh();
		std::string currentLine;
		std::ifstream objFile(filePath);

		if (!objFile.is_open())
		{
			std::cout << "Error opening file : " << filePath << std::endl;
			return nullptr;
		}

		while (getline(objFile, currentLine))
		{
			currentLine.erase(currentLine.find_last_not_of(" ") + 1);
			std::istringstream stream(currentLine);

			std::string lineType;
			stream >> lineType;

			if (lineType == "v")
			{
				float x, y, z, w;
				stream >> x;
				stream >> y;
				stream >> z;

				if (!stream.eof())
				{
					stream >> w;
					if (w != 1.0f)
					{
						w = 1 / w;
						x *= w;
						y *= w;
						z *= w;
					}
				}

				obj->AddCoordinate(x, y, z);
				continue;
			}

			if (lineType == "vn")
			{
				float x, y, z;
				stream >> x;
				stream >> y;
				stream >> z;

				obj->AddNormal(x, y, z);
				//obj->m_normalBuffer[obj->m_normalBuffer.size() - 1].Normalize(); // TODO: check if need
				continue;
			}

			if (lineType == "vt")
			{
				float u, v, w;
				stream >> u;

				if (!stream.eof())
				{
					stream >> v;
				}
				else
				{
					v = 0;
				}

				if (!stream.eof())
				{
					stream >> w;
				}
				else
				{
					w = 0;
				}

				obj->AddTexel(u, v);
				continue;
			}

			if (lineType == "f")
			{
				std::string s0, s1, s2;
				Vertex v0, v1, v2;

				stream >> s0;
				v0 = obj->Parse(s0);

				stream >> s1;
				v1 = obj->Parse(s1);

				stream >> s2;
				v2 = obj->Parse(s2);

				obj->AddTriangle(v0, v1, v2);

				while (!stream.eof())
				{
					v1 = v2;
					stream >> s2;

					v2 = obj->Parse(s2);

					obj->AddTriangle(v0, v1, v2);
				}

				continue;
			}
		}

		obj->AddTexel(0.f, 0.f); // TODO: redesign: so that m_texelbuffer[0] don't blow up if no texel where define
		obj->AddNormal(1.f, 0.f, 0.f); // TODO: redesign: so that m_texelbuffer[0] don't blow up if no texel where define
		return obj;
	}

	Vertex Mesh::Parse(std::string const& string)
	{
		Vertex vertex;
		std::istringstream stream(string);
		std::string index;

		if (getline(stream, index, '/'))
		{
			vertex.m_coordinateIndex = std::stoi(index) - 1;
			if (vertex.m_coordinateIndex < 0)
			{
				vertex.m_coordinateIndex += m_coordinateBuffer.size() + 1;
			}

			if (getline(stream, index, '/'))
			{
				if (!index.empty())
				{
					vertex.m_texelIndex = std::stoi(index) - 1;
					if (vertex.m_texelIndex < 0)
					{
						vertex.m_texelIndex += m_texelBuffer.size() + 1;
					}
				}

				if (getline(stream, index, '/'))
				{
					vertex.m_normalIndex = std::stoi(index) - 1;
					if (vertex.m_normalIndex < 0)
					{
						vertex.m_normalIndex += m_normalBuffer.size() + 1;
					}
				}
			}
		}
		return vertex;
	}

	void Mesh::ReserveCoordinate(int quantity)
	{
		m_coordinateBuffer.reserve(quantity);
	}

	int Mesh::AddCoordinate(float x, float y, float z)
	{
		// TODO: lookup if coordinate exist
		m_coordinateBuffer.emplace_back(x, y, z);
		return (int)m_coordinateBuffer.size() - 1;
	}

	std::vector<Vec3> const& Mesh::GetCoordinateBuffer() const
	{
		return m_coordinateBuffer;
	}

	void Mesh::ReserveNormal(int quantity)
	{
		m_normalBuffer.reserve(quantity);
	}

	int Mesh::AddNormal(float x, float y, float z)
	{
		// TODO: lookup if normal exist
		m_normalBuffer.emplace_back(x, y, z);
		return (int)m_normalBuffer.size() - 1;
	}

	std::vector<Vec3> const& Mesh::GetNormalBuffer() const
	{
		return m_normalBuffer;
	}

	void Mesh::ReserveTexel(int quantity)
	{
		m_texelBuffer.reserve(quantity);
	}

	int Mesh::AddTexel(float u, float v)
	{
		// TODO: lookup if texel exist
		m_texelBuffer.emplace_back(u, v);
		return (int)m_texelBuffer.size() - 1;
	}

	std::vector<Vec2> const& Mesh::GetTexelBuffer() const
	{
		return m_texelBuffer;
	}

	void Mesh::ReserveTriangle(int quantity)
	{
		m_triangleBuffer.reserve(quantity);
	}

	void Mesh::AddTriangle(Vertex const& WindowVertexA, Vertex const& WindowVertexB, Vertex const& WindowVertexC, Texture* texture)
	{
#ifdef _DEBUG
		assert(WindowVertexA.m_coordinateIndex < m_coordinateBuffer.size());
		assert(WindowVertexB.m_coordinateIndex < m_coordinateBuffer.size());
		assert(WindowVertexC.m_coordinateIndex < m_coordinateBuffer.size());

		assert(WindowVertexA.m_normalIndex < m_coordinateBuffer.size());
		assert(WindowVertexB.m_normalIndex < m_coordinateBuffer.size());
		assert(WindowVertexC.m_normalIndex < m_coordinateBuffer.size());
#endif // _DEBUG

		m_triangleBuffer.emplace_back(WindowVertexA, WindowVertexB, WindowVertexC, texture);
	}

	std::vector<Triangle> const& Mesh::GetTriangleBuffer() const
	{
		return m_triangleBuffer;
	}
}