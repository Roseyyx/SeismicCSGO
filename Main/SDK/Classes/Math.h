#pragma once

#include <directxmath.h>

#include "../../Includes/MainIncludes.h"

#include "../DataTypes/Vector.h"

#include "../DataTypes/QAngle.h"

#include "../DataTypes/Matrix.h"


#define M_PI			DirectX::XM_PI
#define M_RAD2DEG( x )		DirectX::XMConvertToDegrees( x )
#define M_DEG2RAD( x )		DirectX::XMConvertToRadians( x )


namespace Math {
	Vector	VectorTransform(const Vector& vecTransform, const matrix3x4_t& matrix);
	void	VectorAngles(const Vector& vecForward, QAngle& angView);
	void	AngleVectors(const QAngle& angView, Vector* pForward, Vector* pRight = nullptr, Vector* pUp = nullptr);
	QAngle	CalcAngle(const Vector& vecStart, const Vector& vecEnd);

	template<class T, class U>
	static T Clamp(const T& in, const U& low, const U& high)
	{
		if (in <= low)
			return low;

		if (in >= high)
			return high;

		return in;
	}
}