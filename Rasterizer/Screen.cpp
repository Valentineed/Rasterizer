#include "pch.h"
#include "Screen.h"
#include <iostream>      
#include <math.h> 

namespace Rasterizer
{
	Screen::Screen(size_t width, size_t height) :
		m_width(width),
		m_height(height),
		m_pixelBuffer(new Color[width * height]),
		m_depthBuffer(new float[width * height])
	{
	}

	Screen::~Screen()
	{
		if (m_pixelBuffer != nullptr)
		{
			delete[] m_pixelBuffer;
		}
		if (m_depthBuffer != nullptr)
		{
			delete[] m_depthBuffer;
		}
	}

	void Screen::Resize(unsigned int width, unsigned int height)
	{
		unsigned int newCapacity = width * height;
		if (m_capacity < newCapacity)
		{
			if (m_pixelBuffer != nullptr)
			{
				delete[] m_pixelBuffer;
				delete[] m_depthBuffer;
			}

			m_pixelBuffer = new Color[newCapacity];
			m_depthBuffer = new float[newCapacity];
			m_capacity = newCapacity;
		}

		m_width = width;
		m_height = height;
	}

	int Screen::GetWidth() const
	{
		return m_width;
	}

	int Screen::GetHeight() const
	{
		return m_height;
	}

	Color* Screen::GetPixelBuffer() const
	{
		return m_pixelBuffer;
	}

	float* Screen::GetDepthBuffer() const
	{
		return m_depthBuffer;
	}

	void Screen::SetPixelColor(unsigned int x, unsigned int y, const Color& color) const
	{
		m_pixelBuffer[m_width * y + x] = color;
	}

	void Screen::Clear() const
	{
		int size = m_width * m_height;
		for (int i = 0; i < size; i++)
		{
			m_pixelBuffer[i] = m_clearColor;
			m_depthBuffer[i] = 2.f;
		}
	}

	void Screen::DrawTriangle(WindowTriangle const& triangle, std::vector<Light> const& lights, std::vector<Vec3> const& lightBuffer, bool useLight) const
	{
		for (int x = triangle.m_minX; x <= triangle.m_maxX; x++) // TODO: split in inline function
		for (int y = triangle.m_minY; y <= triangle.m_maxY; y++)
		{
			const int pixelIndex = m_width * y + x;

			// calculate weight of each vertex
			const float denom = (triangle.m_vertices[1].m_windowPosition.m_y - triangle.m_vertices[2].m_windowPosition.m_y) * (triangle.m_vertices[0].m_windowPosition.m_x - triangle.m_vertices[2].m_windowPosition.m_x) + (triangle.m_vertices[2].m_windowPosition.m_x - triangle.m_vertices[1].m_windowPosition.m_x) * (triangle.m_vertices[0].m_windowPosition.m_y - triangle.m_vertices[2].m_windowPosition.m_y);
			const float w0 = ((triangle.m_vertices[1].m_windowPosition.m_y - triangle.m_vertices[2].m_windowPosition.m_y) * (x - triangle.m_vertices[2].m_windowPosition.m_x) + (triangle.m_vertices[2].m_windowPosition.m_x - triangle.m_vertices[1].m_windowPosition.m_x) * (y - triangle.m_vertices[2].m_windowPosition.m_y)) / denom;
			const float w1 = ((triangle.m_vertices[2].m_windowPosition.m_y - triangle.m_vertices[0].m_windowPosition.m_y) * (x - triangle.m_vertices[2].m_windowPosition.m_x) + (triangle.m_vertices[0].m_windowPosition.m_x - triangle.m_vertices[2].m_windowPosition.m_x) * (y - triangle.m_vertices[2].m_windowPosition.m_y)) / denom;
			const float w2 = 1.f - w0 - w1;
			if (w0 < 0.f || w1 < 0.f || w2 < 0.f)
			{
				continue;
			}

			// calculate depth
			const float zCurrent = triangle.m_vertices[0].m_position.m_z * w0
								+ triangle.m_vertices[1].m_position.m_z * w1
								+ triangle.m_vertices[2].m_position.m_z * w2;

			if (zCurrent > m_depthBuffer[pixelIndex])
			{
				continue;
			}
			m_depthBuffer[pixelIndex] = zCurrent;

			// calculate color from texture or from triangle vertex
			Color newColor;
			if (triangle.m_texture != nullptr)
			{
				const float u = triangle.m_vertices[0].m_texel.m_u * w0
								+ triangle.m_vertices[1].m_texel.m_u * w1
								+ triangle.m_vertices[2].m_texel.m_u * w2;
				const float v = triangle.m_vertices[0].m_texel.m_v * w0
								+ triangle.m_vertices[1].m_texel.m_v * w1
								+ triangle.m_vertices[2].m_texel.m_v * w2;

				newColor = triangle.m_texture->GetColor(u, v);
			}
			else
			{
				newColor.m_alpha = triangle.m_vertices[0].m_color.m_alpha * w0
									+ triangle.m_vertices[1].m_color.m_alpha * w1
									+ triangle.m_vertices[2].m_color.m_alpha * w2;

				newColor.m_red = triangle.m_vertices[0].m_color.m_red * w0
								+ triangle.m_vertices[1].m_color.m_red * w1
								+ triangle.m_vertices[2].m_color.m_red * w2;
				newColor.m_green = triangle.m_vertices[0].m_color.m_green * w0
								+ triangle.m_vertices[1].m_color.m_green * w1
								+ triangle.m_vertices[2].m_color.m_green * w2;
				newColor.m_blue = triangle.m_vertices[0].m_color.m_blue * w0
								+ triangle.m_vertices[1].m_color.m_blue * w1
								+ triangle.m_vertices[2].m_color.m_blue * w2;
			}
			if (newColor.m_alpha == 0.f)
			{
				continue;
			}

			// calculate alpha
			if (newColor.m_alpha == 1.f)
			{
				m_pixelBuffer[pixelIndex] = newColor;
			}
			else
			{
				Color& oldColor = m_pixelBuffer[pixelIndex];
				float oldA = 1.f - newColor.m_alpha;

				m_pixelBuffer[pixelIndex].m_red = oldColor.m_red * oldA + newColor.m_red * newColor.m_alpha;
				m_pixelBuffer[pixelIndex].m_green = oldColor.m_green * oldA + newColor.m_green * newColor.m_alpha;
				m_pixelBuffer[pixelIndex].m_blue = oldColor.m_blue * oldA + newColor.m_blue * newColor.m_alpha;
			}

			// calculate light
			if (false == useLight)
			{
				continue;
			}

			Vec3 normal{
				(triangle.m_vertices[0].m_normal.m_x * w0) + (triangle.m_vertices[1].m_normal.m_x * w1) + (triangle.m_vertices[2].m_normal.m_x * w2),
				(triangle.m_vertices[0].m_normal.m_y * w0) + (triangle.m_vertices[1].m_normal.m_y * w1) + (triangle.m_vertices[2].m_normal.m_y * w2),
				(triangle.m_vertices[0].m_normal.m_z * w0) + (triangle.m_vertices[1].m_normal.m_z * w1) + (triangle.m_vertices[2].m_normal.m_z * w2)
			};
			Vec3 actualPos{ 
				(triangle.m_vertices[0].m_position.m_x * w0) + (triangle.m_vertices[1].m_position.m_x * w1) + (triangle.m_vertices[2].m_position.m_x * w2),
				(triangle.m_vertices[0].m_position.m_y * w0) + (triangle.m_vertices[1].m_position.m_y * w1) + (triangle.m_vertices[2].m_position.m_y * w2),
				zCurrent};

			float intens = 0.f;
			int i = 0.f;
			for (auto& light : lights) 
			{
				intens += light.IntensityLight(normal,actualPos, lightBuffer[i]);
			}

			m_pixelBuffer[pixelIndex].m_red *= intens;
			m_pixelBuffer[pixelIndex].m_green *= intens;
			m_pixelBuffer[pixelIndex].m_blue *= intens;
		}
	}

	void Screen::DrawFrame(WindowTriangle const& triangle, std::vector<Light> const& lights, std::vector<Vec3> const& lightBuffer,bool useLight) const
	{
		DrawLine(triangle, triangle.m_vertices[0], triangle.m_vertices[1], lights, lightBuffer, useLight);
		DrawLine(triangle, triangle.m_vertices[1], triangle.m_vertices[2], lights, lightBuffer, useLight);
		DrawLine(triangle, triangle.m_vertices[2], triangle.m_vertices[0], lights, lightBuffer, useLight);
	}

	void Screen::DrawLine(WindowTriangle const& triangle, WindowVertex const& first, WindowVertex const& second, std::vector<Light> const& lights, std::vector<Vec3> const& lightBuffer, bool useLight) const
	{
		int x = first.m_windowPosition.m_x;
		int deltaX = second.m_windowPosition.m_x - x;
		int stepX = (deltaX > 0 ? 1 : -1);
		deltaX *= stepX;

		int y = first.m_windowPosition.m_y;
		int deltaY = second.m_windowPosition.m_y - y;
		int stepY = (deltaY > 0 ? 1 : -1);
		deltaY *= stepY;

		const bool isSteepSlope = deltaY > deltaX;
		const int halfBigCorrection = (isSteepSlope ? deltaY : deltaX);
		const int bigCorrection = halfBigCorrection + halfBigCorrection;

		const int smallCorrection = (isSteepSlope ? deltaX + deltaX : deltaY + deltaY);

		int error = smallCorrection - halfBigCorrection;

		// Bresenham algorithm is a "draw / move"
		// the next 13 line are use to inverse the code "move / draw" state
		{
			if (error < 0)
			{
				if (isSteepSlope)
					x -= stepX;
				else
					y -= stepY;
				error += bigCorrection;
			}
			if (isSteepSlope)
				y -= stepY;
			else
				x -= stepX;
			error -= smallCorrection;
		}

		// "move / draw" algorithm so the 'continue;' don't skip the move part
		for (int i = 0; i <= halfBigCorrection + 1; i++)
		{
			if (error >= 0)
			{
				if (isSteepSlope)
					x += stepX;
				else
					y += stepY;
				error -= bigCorrection;
			}
			if (isSteepSlope)
				y += stepY;
			else
				x += stepX;
			error += smallCorrection;

			if (triangle.m_minX <= x && x <= triangle.m_maxX &&
				triangle.m_minY <= y && y <= triangle.m_maxY)
			{
				const int pixelIndex = m_width * y + x;

				// calculate weight of each vertex
				float w2 = i / (float)halfBigCorrection;
				float w1 = 1 - w2;

				// calculate depth
				const float zCurrent = first.m_position.m_z * w1
					+ second.m_position.m_z * w2;

				if (zCurrent > m_depthBuffer[pixelIndex])
				{
					continue;
				}
				m_depthBuffer[pixelIndex] = zCurrent;

				// calculate color from texture or from triangle vertex
				Color newColor;
				if (triangle.m_texture != nullptr)
				{
					const float u = first.m_texel.m_u * w1
						+ second.m_texel.m_u * w2;
					const float v = first.m_texel.m_v * w1
						+ second.m_texel.m_v * w2;

					newColor = triangle.m_texture->GetColor(u, v);
				}
				else
				{
					newColor.m_alpha = first.m_color.m_alpha * w1
						+ second.m_color.m_alpha * w2;

					newColor.m_red = first.m_color.m_red * w1
						+ second.m_color.m_red * w2;
					newColor.m_green = first.m_color.m_green * w1
						+ second.m_color.m_green * w2;
					newColor.m_blue = first.m_color.m_blue * w1
						+ second.m_color.m_blue * w2;
				}
				if (newColor.m_alpha == 0.f)
				{
					continue;
				}

				// calculate alpha
				if (newColor.m_alpha == 1.f)
				{
					m_pixelBuffer[pixelIndex] = newColor;
				}
				else
				{
					Color& oldColor = m_pixelBuffer[pixelIndex];
					float oldA = 1.f - newColor.m_alpha;

					m_pixelBuffer[pixelIndex].m_red = oldColor.m_red * oldA + newColor.m_red * newColor.m_alpha;
					m_pixelBuffer[pixelIndex].m_green = oldColor.m_green * oldA + newColor.m_green * newColor.m_alpha;
					m_pixelBuffer[pixelIndex].m_blue = oldColor.m_blue * oldA + newColor.m_blue * newColor.m_alpha;
				}

				// calculate light
				if (false == useLight)
				{
					continue;
				}

				Vec3 normal{
					(first.m_normal.m_x * w1) + (second.m_normal.m_x * w2),
					(first.m_normal.m_y * w1) + (second.m_normal.m_y * w2),
					(first.m_normal.m_z * w1) + (second.m_normal.m_z * w2)
				};

				Vec3 actualPos{
					(first.m_position.m_x * w1) + (second.m_position.m_x * w2),
					(first.m_position.m_y * w1) + (second.m_position.m_y * w2),
					zCurrent };

				float intens = 0.f;
				int i = 0.f;
				for (auto& light : lights)
				{
					intens += light.IntensityLight(normal, actualPos, lightBuffer[i]);
				}

				m_pixelBuffer[pixelIndex].m_red *= intens;
				m_pixelBuffer[pixelIndex].m_green *= intens;
				m_pixelBuffer[pixelIndex].m_blue *= intens;
			}
		}
	}
}