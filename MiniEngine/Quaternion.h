#pragma once

#define USE_MATH_DEFINES

#include <cmath>
#include <corecrt_math_defines.h>
#include <valarray>

#include "Matrix3x3.h"
#include "Vector3.h"

class Quaternion {
private:
	// 底层存储结构
	float _data[4];

	inline void SetData_Internal(const float& w, const float& x, const float& y, const float& z) {
		_data[0] = w;
		_data[1] = x;
		_data[2] = y;
		_data[3] = z;
	}

	inline float& operator[](size_t index) {
		return _data[index];
	}

	inline const float& operator[](size_t index) const {
		return _data[index];
	}

public:
	inline Quaternion() {
		SetData_Internal(float(1), float(0), float(0), float(0));
	}

	inline Quaternion(float w, float x, float y, float z) {
		SetData_Internal(w, x, y, z);
	}

	// 数据获取
public:
	inline float& W() { return _data[0]; }
	inline float& X() { return _data[1]; }
	inline float& Y() { return _data[2]; }
	inline float& Z() { return _data[3]; }

	inline float W() const { return _data[0]; }
	inline float X() const { return _data[1]; }
	inline float Y() const { return _data[2]; }
	inline float Z() const { return _data[3]; }

	// 数据变形
public:
	inline float SquareMagnitude() const {
		return _data[0] * _data[0] + _data[1] * _data[1] + _data[2] * _data[2] + _data[3] * _data[3];
	}

	inline float Magnitude() const {
		return sqrt(SquareMagnitude());
	}

	inline void Normalize() {
		operator/=(Magnitude());
	}

	inline Quaternion Normalized() const {
		Quaternion tmp(*this);
		return tmp.operator/=(Magnitude());
	}

	Vector3 XAxis() const {
		return (RotateMatrix() * Vector3{1, 0, 0}).Normalized();
	}

	Vector3 YAxis() const {
		return (RotateMatrix() * Vector3{0, 1, 0}).Normalized();
	}

	Vector3 ZAxis() const {
		return (RotateMatrix() * Vector3{0, 0, 1}).Normalized();
	}

	inline void SetFromEulerAngleZYX(const Vector3& angles) {
		Matrix3x3 rotX, rotY, rotZ;
		{
			float rad = angles[0] * M_PI / 180;
			rotX[0][0] = 1;
			rotX[0][1] = 0;
			rotX[0][2] = 0;
			rotX[1][0] = 0;
			rotX[1][1] = std::cos(rad);
			rotX[1][2] = -std::sin(rad);
			rotX[2][0] = 0;
			rotX[2][1] = std::sin(rad);
			rotX[2][2] = std::cos(rad);
		}
		{
			float rad = angles[1] * M_PI / 180;
			rotY[0][0] = std::cos(rad);
			rotY[0][1] = 0;
			rotY[0][2] = std::sin(rad);
			rotY[1][0] = 0;
			rotY[1][1] = 1;
			rotY[1][2] = 0;
			rotY[2][0] = -std::sin(rad);
			rotY[2][1] = 0;
			rotY[2][2] = std::cos(rad);
		}
		{
			float rad = angles[2] * M_PI / 180;
			rotZ[0][0] = std::cos(rad);
			rotZ[0][1] = -std::sin(rad);
			rotZ[0][2] = 0;
			rotZ[1][0] = std::sin(rad);
			rotZ[1][1] = std::cos(rad);
			rotZ[1][2] = 0;
			rotZ[2][0] = 0;
			rotZ[2][1] = 0;
			rotZ[2][2] = 1;
		}
		Matrix3x3 rot = rotZ * rotY * rotX; // 欧拉角到矩阵为外旋XYZ（内旋ZYX）
		SetFromRotateMatrix(rot);
		Normalize();
	}

	inline Vector3 EulerAngleZYX() const {
		Vector3 angles;

		// copy from piccolo 外旋XYZ（内旋ZYX）的转换
		// roll (x-axis rotation)
		float rollNum = 2 * (_data[0] * _data[1] + _data[2] * _data[3]);
		float rollDen = 1 - 2 * (_data[1] * _data[1] + _data[2] * _data[2]);
		angles.X() = (std::atan2(rollNum, rollDen)) * 180 / M_PI;

		// pitch (y-axis rotation)
		float pitchNum = std::sqrt(1 + 2 * (_data[0] * _data[2] - _data[1] * _data[3]));
		float pitchDen = std::sqrt(1 - 2 * (_data[0] * _data[2] - _data[1] * _data[3]));
		angles.Y() = (2 * std::atan2(pitchNum, pitchDen) - M_PI / 2) * 180 / M_PI;

		// yaw (z-axis rotation)
		float yawNum = 2 * (_data[0] * _data[3] + _data[1] * _data[2]);
		float yawDen = 1 - 2 * (_data[2] * _data[2] + _data[3] * _data[3]);
		angles.Z() = (std::atan2(yawNum, yawDen)) * 180 / M_PI;

		return angles;
	}

	inline void SetFromRotateMatrix(const Matrix3x3& rotation) {
		float trace = rotation[0][0] + rotation[1][1] + rotation[2][2];
		float root;

		if (trace > 0.0) {
			// |w| > 1/2, may as well choose w > 1/2
			root = std::sqrt(trace + 1.0f); // 2w
			_data[0] = 0.5f * root;
			root = 0.5f / root; // 1/(4w)
			_data[1] = (rotation[2][1] - rotation[1][2]) * root;
			_data[2] = (rotation[0][2] - rotation[2][0]) * root;
			_data[3] = (rotation[1][0] - rotation[0][1]) * root;
		} else {
			// |w| <= 1/2
			size_t s_iNext[3] = {1, 2, 0};
			size_t i = 0;
			if (rotation[1][1] > rotation[0][0]) i = 1;
			if (rotation[2][2] > rotation[i][i]) i = 2;
			size_t j = s_iNext[i];
			size_t k = s_iNext[j];

			root = std::sqrt(rotation[i][i] - rotation[j][j] - rotation[k][k] + 1.0f);
			float* apkQuat[3] = {&_data[1], &_data[2], &_data[3]};
			*apkQuat[i] = 0.5f * root;
			root = 0.5f / root;
			_data[0] = (rotation[k][j] - rotation[j][k]) * root;
			*apkQuat[j] = (rotation[j][i] + rotation[i][j]) * root;
			*apkQuat[k] = (rotation[k][i] + rotation[i][k]) * root;
		}
		Normalize();
	}

	inline Matrix3x3 RotateMatrix() const {
		float xxMul2 = _data[1] * _data[1] * 2;
		float yyMul2 = _data[2] * _data[2] * 2;
		float zzMul2 = _data[3] * _data[3] * 2;
		float wxMul2 = _data[0] * _data[1] * 2;
		float wyMul2 = _data[0] * _data[2] * 2;
		float wzMul2 = _data[0] * _data[3] * 2;
		float xyMul2 = _data[1] * _data[2] * 2;
		float xzMul2 = _data[1] * _data[3] * 2;
		float yzMul2 = _data[2] * _data[3] * 2;

		Matrix3x3 m;
		m[0][0] = 1 - yyMul2 - zzMul2;
		m[0][1] = xyMul2 - wzMul2;
		m[0][2] = wyMul2 + xzMul2;
		m[1][0] = xyMul2 + wzMul2;
		m[1][1] = 1 - xxMul2 - zzMul2;
		m[1][2] = yzMul2 - wxMul2;
		m[2][0] = xzMul2 - wyMul2;
		m[2][1] = wxMul2 + yzMul2;
		m[2][2] = 1 - xxMul2 - yyMul2;
		return m;
	}

	inline void SetFromAngleAxis(const float& angle, const Vector3 & axis) {
		Vector3 normal(axis.Normalized());
		float rad = (fmod(angle + 180, 360) - 180) / 180 * M_PI;
		float cos = std::cos(rad / 2);
		float sin = std::sin(rad / 2);
		_data[0] = cos;
		_data[1] = normal.X() * sin;
		_data[2] = normal.Y() * sin;
		_data[3] = normal.Z() * sin;
		Normalize();
	}

	static Quaternion GetFromRotateMatrix(const Matrix3x3 & rotateMatrix);
	static Quaternion GetFromEulerAngleZyx(const Vector3 & euler);
	static Quaternion GetFromAngleAxis(const float& angle, const Vector3 & axis);

	// 运算
public:
	inline friend Quaternion operator+(const Quaternion& lhs, const Quaternion& rhs) {
		return Quaternion(
			lhs[0] + rhs[0],
			lhs[1] + rhs[1],
			lhs[2] + rhs[2],
			lhs[3] + rhs[3]);
	}

	inline friend Quaternion operator-(const Quaternion& lhs, const Quaternion& rhs) {
		return Quaternion(
			lhs[0] - rhs[0],
			lhs[1] - rhs[1],
			lhs[2] - rhs[2],
			lhs[3] - rhs[3]);
	}

	inline friend Quaternion operator-(const Quaternion& lhs) {
		return Quaternion(-lhs[0], -lhs[1], -lhs[2], -lhs[3]);
	}

	inline friend Quaternion operator*(const Quaternion& lhs, const float& factor) {
		Quaternion q(lhs);
		return q *= factor;
	}

	inline friend Quaternion operator*(const float& factor, const Quaternion& rhs) {
		Quaternion q(rhs);
		return q *= factor;
	}

	inline friend Quaternion operator/(const Quaternion& lhs, const float& factor) {
		Quaternion q(lhs);
		return q /= factor;
	}

	inline friend Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs) {
		return Quaternion(
			lhs[0] * rhs[0] - lhs[1] * rhs[1] - lhs[2] * rhs[2] - lhs[3] * rhs[3],
			lhs[0] * rhs[1] + rhs[0] * lhs[1] + lhs[2] * rhs[3] - lhs[3] * rhs[2],
			lhs[0] * rhs[2] + rhs[0] * lhs[2] + lhs[3] * rhs[1] - lhs[1] * rhs[3],
			lhs[0] * rhs[3] + rhs[0] * lhs[3] + lhs[1] * rhs[2] - lhs[2] * rhs[1]
		);
	}

	inline friend bool operator==(const Quaternion& lhs, const Quaternion& rhs) {
		return lhs[0] == rhs[0] && lhs[1] == rhs[1] && lhs[2] == rhs[2] && lhs[3] == rhs[3];
	}

	inline friend bool operator!=(const Quaternion& lhs, const Quaternion& rhs) {
		return !(lhs == rhs);
	}

	// 运算赋值
public
:
	inline Quaternion& operator+=(const Quaternion& other) {
		_data[0] += other[0];
		_data[1] += other[1];
		_data[2] += other[2];
		_data[3] += other[3];
		return *this;
	}

	inline Quaternion& operator-=(const Quaternion& other) {
		_data[0] -= other[0];
		_data[1] -= other[1];
		_data[2] -= other[2];
		_data[3] -= other[3];
		return *this;
	}

	inline Quaternion& operator*=(const Quaternion& rhs) {
		_data[0] = _data[0] * rhs[0] - _data[1] * rhs[1] - _data[2] * rhs[2] - _data[3] * rhs[3];
		_data[1] = _data[0] * rhs[1] + rhs[0] * _data[1] + _data[2] * rhs[3] - _data[3] * rhs[2];
		_data[2] = _data[0] * rhs[2] + rhs[0] * _data[2] + _data[3] * rhs[1] - _data[1] * rhs[3];
		_data[3] = _data[0] * rhs[3] + rhs[0] * _data[3] + _data[1] * rhs[2] - _data[2] * rhs[1];
		return *this;
	}

	inline Quaternion& operator*=(const float& factor) {
		_data[0] *= factor;
		_data[1] *= factor;
		_data[2] *= factor;
		_data[3] *= factor;
		return *this;
	}

	inline Quaternion& operator/=(const float& factor) {
		_data[0] /= factor;
		_data[1] /= factor;
		_data[2] /= factor;
		_data[3] /= factor;
		return *this;
	}
};



