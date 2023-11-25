#include "Quaternion.h"

Quaternion Quaternion::GetFromRotateMatrix(const Matrix3x3& rotateMatrix) {
	Quaternion q;
	q.SetFromRotateMatrix(rotateMatrix);
	return q;
}

Quaternion Quaternion::GetFromEulerAngleZyx(const Vector3& euler) {
	Quaternion q;
	q.SetFromEulerAngleZYX(euler);
	return q;
}

Quaternion Quaternion::GetFromAngleAxis(const float& angle, const Vector3& axis) {
	Quaternion q;
	q.SetFromAngleAxis(angle, axis);
	return q;
}
