#include "pch.h"
#include "ClipSpace.h"

namespace Rasterizer
{
	Vec3 const ClipSpace::Edge::s_normalRight{ -1.f, 0.f };
	Vec3 const ClipSpace::Edge::s_normalLeft{ 1.f, 0.f };
	Vec3 const ClipSpace::Edge::s_normalTop{ 0.f, -1.f };
	Vec3 const ClipSpace::Edge::s_normalBottom{ 0.f, 1.f };
	Vec3 const ClipSpace::Edge::s_normalNear{ 0.f, 0.f, -1.f };
	Vec3 const ClipSpace::Edge::s_normalFar{ 0.f, 0.f, 1.f };

	Vec3 const ClipSpace::Edge::s_minimum{ -1.f, -1.f, -1.f };
	Vec3 const ClipSpace::Edge::s_maximum{ 1.f, 1.f, 1.f };
}
