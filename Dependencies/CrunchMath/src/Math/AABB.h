#pragma once
#include "Vec3.h"
#include "Quaternion.h"

namespace CrunchMath{ 

	struct AABB
	{
		float Min[3];
		float Max[3];

		AABB();

		AABB(const Vec3& Min, const Vec3& Max);

		AABB(const AABB& Box);

		AABB& operator=(const AABB& Box);

		Vec3 ClosestPointAABBPt(const Vec3& Point) const;

		bool BroadPhaseCollisionTest(const AABB& Object2);

		//Narrow Phase Collision shouldn't be ever used for an AABB
		bool NarrowPhaseCollisionTest(const AABB& Object2);

		void Set(const Vec3& Min, const Vec3& Max);

		//Note: TO DO >>>
		//void TransformBox(const Quaternion& Rotation, const Vec3& Position, Vec3 Size);
	};
}