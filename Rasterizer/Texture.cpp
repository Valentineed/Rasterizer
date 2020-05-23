#include "pch.h"
#include "Texture.h"

#include <limits>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Rasterizer
{
	Texture::Texture(const char* img)
	{
		constexpr float transformToFloat = 1.f / 255.f;

		stbi_set_flip_vertically_on_load(true);
		int x, y, comp;
		stbi_uc* imageBuffer = stbi_load(img, &x, &y, &comp, 0);
		m_width = x;
		m_height = y;

		m_texelBuffer = new Color[m_width * m_height];
		int count = 0;
		if (comp == 4)
		{
			for (unsigned int i = 0; i < (m_width * m_height * comp); i++)
			{
				if (count == 0)
				{
					m_texelBuffer[i / comp].m_red = imageBuffer[i] * transformToFloat;
					count++;
				}
				else if (count == 1)
				{
					m_texelBuffer[i / comp].m_green = imageBuffer[i] * transformToFloat;
					count++;
				}
				else if (count == 2)
				{
					m_texelBuffer[i / comp].m_blue = imageBuffer[i] * transformToFloat;
					count++;
				}
				else if (count == 3)
				{
					m_texelBuffer[i / comp].m_alpha = imageBuffer[i] * transformToFloat;
					count = 0;
				}
			}
		}
		else if (comp == 3)
		{
			for (unsigned int i = 0; i < m_width * m_height * comp; i++)
			{
				if (count == 0)
				{
					m_texelBuffer[i / comp].m_red = imageBuffer[i] * transformToFloat;
					count++;
				}
				else if (count == 1)
				{
					m_texelBuffer[i / comp].m_green = imageBuffer[i] * transformToFloat;
					count++;
				}
				else if (count == 2)
				{
					m_texelBuffer[i / comp].m_blue = imageBuffer[i] * transformToFloat;
					count = 0;
				}
			}
		}
		stbi_image_free(imageBuffer);
	}

	Texture::~Texture()
	{
		if (m_texelBuffer != nullptr)
		{
			delete[] m_texelBuffer;
		}
	}

	Color Texture::GetColor(float uParam, float vParam) const
	{
		/* nearest neighbour
		int u2 = uParam * (float)m_width + 0.5f;
		int v2 = vParam * (float)m_height + 0.5f;
		int pixelIndex = u2 + (int)v2 * m_width;
		return m_texelBuffer[pixelIndex];
		*/

		// find true coordinate in texture so the image is mirror-repeat
		float u = uParam - floorf(uParam);
		if (fmodf(ceilf(uParam), 2.f) == 0)
		{
			u = 1.f - u;
		}
		u = u * (m_width - 1);

		float v = vParam - floorf(vParam);
		if (fmodf(ceilf(vParam), 2.f) == 0)
		{
			v = 1.f - v;
		}
		v = v * (m_height - 1);

		// find neighbouring texel index
		int index00 = (int)u + (int)v * m_width;
		Color const& texel00 = m_texelBuffer[index00];
		Color const& texel10 = m_texelBuffer[index00 + 1];
		Color const& texel01 = m_texelBuffer[index00 + m_width];
		Color const& texel11 = m_texelBuffer[index00 + m_width + 1];

		// find relative weight of each texel
		float wu = u - floor(u);
		float wv = v - floor(v);
		float w00 = texel00.m_alpha * (1.f - wu) * (1.f - wv);
		float w10 = texel10.m_alpha * wu * (1.f - wv);
		float w01 = texel01.m_alpha * (1.f - wu) * wv;
		float w11 = texel11.m_alpha * wu * wv;

		//interpolate color from texel weight
		return Color{
			texel00.m_red * w00 + texel10.m_red * w10 + texel01.m_red * w01 + texel11.m_red * w11,
			texel00.m_green * w00 + texel10.m_green * w10 + texel01.m_green * w01 + texel11.m_green * w11,
			texel00.m_blue * w00 + texel10.m_blue * w10 + texel01.m_blue * w01 + texel11.m_blue * w11,
			w00 + w10 + w01 + w11
		};
	}
}