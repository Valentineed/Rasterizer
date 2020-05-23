#include "pch.h"
#include "Light.h"
#include <algorithm>
#include <math.h>
namespace Rasterizer
{
	Light::Light(float x, float y, float z, float ambient, float diffuse, float specular) :
		m_position(x, y, z),
		m_intensityAmbientComponent(ambient),
		m_intensityDiffuseComponent(diffuse),
		m_intensitySpecularComponent(specular)

	{
	}

	Light::Light(Vec3 position, float ambient, float diffuse, float specular) :
		m_position(position),
		m_intensityAmbientComponent(ambient),
		m_intensityDiffuseComponent(diffuse),
		m_intensitySpecularComponent(specular)

	{
	}

	float Light::IntensityLight(Vec3 normal, Vec3 pixelPos, Vec3 const& lightBuffer) const
	{
		float intensityDiffuseR = 0.f;
		float intensitySpecularR = 0.f;
		float intensityAmbientR = m_intensityAmbientComponent * m_haveAAmbient;
		normal.Normalize();

		if (m_intensityDiffuseComponent > 0.f)
		{
			/*Vec3 lightDir{ lightBuffer,pixelPos };
			lightDir.Normalize();*/

			intensityDiffuseR = m_intensityDiffuseComponent * m_havaAdiffuse * (normal.Dot(m_position));
		}
		
		if (m_intensitySpecularComponent > 0.f)
		{
			Vec3 Reflex = normal + m_dirLook;
			Reflex.Normalize();
			intensitySpecularR = m_intensitySpecularComponent * m_haveASpecular * powf((Reflex.Dot(m_dirLook)), 1.f);
		}
		float totalIntensity = intensityAmbientR + intensityDiffuseR + intensitySpecularR;

		if (totalIntensity < 0)
		{
			totalIntensity = 0;
		}

		if (totalIntensity > 1.f)
		{
			totalIntensity = 1.f;
		}
		return totalIntensity;
	}

	void Light::SetPosition(float x, float y, float z)
	{
		m_position.m_x = x;
		m_position.m_y = y;
		m_position.m_z = z;
	}

	void Light::SetPosition(Vec3 newPos)
	{
		m_position = newPos;
	}

	Vec3 Light::GetPosition() const
	{
		return m_position;
	}
}