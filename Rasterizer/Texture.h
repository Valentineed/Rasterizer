#pragma once

#include "Color.h"

namespace Rasterizer
{
	class Texture
	{
	public:
		Texture() = delete;
		Texture(const char* img);
		~Texture();

		Color GetColor(float u, float v) const;

	private:
		unsigned int m_width = 0;
		unsigned int m_height = 0;
		Color* m_texelBuffer = nullptr;

	};
}