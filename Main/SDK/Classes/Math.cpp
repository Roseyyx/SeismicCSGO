#include "Math.h"


Vector Math::VectorTransform(const Vector& vecTransform, const matrix3x4_t& matrix)
{
	return Vector(vecTransform.DotProduct(matrix[0]) + matrix[0][3],
		vecTransform.DotProduct(matrix[1]) + matrix[1][3],
		vecTransform.DotProduct(matrix[2]) + matrix[2][3]);
}

void Math::VectorAngles(const Vector& vecForward, QAngle& angView)
{
	float flPitch, flYaw;

	if (vecForward.x == 0.f && vecForward.y == 0.f)
	{
		flPitch = (vecForward.z > 0.f) ? 270.f : 90.f;
		flYaw = 0.f;
	}
	else
	{
		flPitch = std::atan2f(-vecForward.z, vecForward.Length2D()) * 180.f / M_PI;

		if (flPitch < 0.f)
			flPitch += 360.f;

		flYaw = std::atan2f(vecForward.y, vecForward.x) * 180.f / M_PI;

		if (flYaw < 0.f)
			flYaw += 360.f;
	}

	angView.x = flPitch;
	angView.y = flYaw;
	angView.z = 0.f;
}

void Math::AngleVectors(const QAngle& angView, Vector* pForward, Vector* pRight, Vector* pUp)
{
	float sp, sy, sr, cp, cy, cr;

	DirectX::XMScalarSinCos(&sp, &cp, M_DEG2RAD(angView.x));
	DirectX::XMScalarSinCos(&sy, &cy, M_DEG2RAD(angView.y));
	DirectX::XMScalarSinCos(&sr, &cr, M_DEG2RAD(angView.z));

	if (pForward != nullptr)
	{
		pForward->x = cp * cy;
		pForward->y = cp * sy;
		pForward->z = -sp;
	}

	if (pRight != nullptr)
	{
		pRight->x = -1 * sr * sp * cy + -1 * cr * -sy;
		pRight->y = -1 * sr * sp * sy + -1 * cr * cy;
		pRight->z = -1 * sr * cp;
	}

	if (pUp != nullptr)
	{
		pUp->x = cr * sp * cy + -sr * -sy;
		pUp->y = cr * sp * sy + -sr * cy;
		pUp->z = cr * cp;
	}
}

QAngle Math::CalcAngle(const Vector& vecStart, const Vector& vecEnd) {
	QAngle angView;
	const Vector vecDelta = vecEnd - vecStart;
	VectorAngles(vecDelta, angView);
	angView.Normalize();

	return angView;
}