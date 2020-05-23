#include "pch.h"
#include "Vertex.h"

#include <sstream>

namespace Rasterizer
{
	Vertex::Vertex(int coordinateIndex) :
		m_coordinateIndex(coordinateIndex)
	{
	}

	Vertex::Vertex(int coordinateIndex, int normalIndex) :
		m_coordinateIndex(coordinateIndex),
		m_normalIndex(normalIndex)
	{
	}

	Vertex::Vertex(int coordinateIndex, Color color) :
		m_coordinateIndex(coordinateIndex),
		m_color(color)
	{
	}

	Vertex::Vertex(int coordinateIndex, int normalIndex, Color color) :
		m_coordinateIndex(coordinateIndex),
		m_normalIndex(normalIndex),
		m_color(color)
	{
	}

	Vertex::Vertex(int coordinateIndex, int normalIndex, int texelIndex) :
		m_coordinateIndex(coordinateIndex),
		m_normalIndex(normalIndex),
		m_texelIndex(texelIndex)
	{
	}

	Vertex::Vertex(int coordinateIndex, Color color, int normalIndex) :
		m_coordinateIndex(coordinateIndex),
		m_color(color),
		m_normalIndex(normalIndex)
	{
	}

	Vertex::Vertex(int coordinateIndex, int normalIndex, Color color, int texelIndex) :
		m_coordinateIndex(coordinateIndex),
		m_normalIndex(normalIndex),
		m_color(color),
		m_texelIndex(texelIndex)
	{
	}
}
