#pragma once
#include "Vec3.h"
#include "Color.h"
namespace Rasterizer
{
	class Light
	{
	public:
		Light(float x, float y, float z, float ambient, float diffuse, float specular);
		Light(Vec3 position, float ambient, float diffuse, float specular);
		~Light() = default;

		float IntensityLight(Vec3 normal, Vec3 pixelPos, Vec3 const& lightBuffer) const;
		void SetPosition(float x, float y, float z);
		void SetPosition(Vec3 newPos);
		Vec3 GetPosition() const;

	private:
		Vec3 m_dirLook{ 0.f,0.f,0.5f };
		Vec3 m_position;
		float m_intensityAmbientComponent = 0.f;
		float m_intensityDiffuseComponent = 0.f;
		float m_intensitySpecularComponent = 0.f;

		bool m_haveAAmbient = true;
		bool m_havaAdiffuse = true;
		bool m_haveASpecular = true;
	};
}
