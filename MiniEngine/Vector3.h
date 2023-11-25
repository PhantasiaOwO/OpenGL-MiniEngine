#pragma once

#include <corecrt_math_defines.h>
#include <cmath>

class Vector3 {
private:
	// 底层存储结构
	float _data[3];

	inline void SetData_Internal(const float& x, const float& y, const float& z) {
		_data[0] = x;
		_data[1] = y;
		_data[2] = z;
	}

	// 构造函数；拷贝构造 拷贝赋值自动生成就可以
public:
	inline Vector3() {
		SetData_Internal(float(), float(), float());
	}

	inline Vector3(float x, float y, float z) {
		SetData_Internal(x, y, z);
	}

	// 数据获取 引用 & 值
public:
	inline float& X() { return _data[0]; }
	inline float& Y() { return _data[1]; }
	inline float& Z() { return _data[2]; }

	inline float X() const { return _data[0]; }
	inline float Y() const { return _data[1]; }
	inline float Z() const { return _data[2]; }

	inline const float* DataPtr() { return _data; }

	inline float& operator[](size_t index) {
		return _data[index];
	}

	inline const float& operator[](size_t index) const {
		return _data[index];
	}

	// 数据变形
public:
	inline float SquareMagnitude() const {
		return _data[0] * _data[0] + _data[1] * _data[1] + _data[2] * _data[2];
	}

	inline float Magnitude() const {
		return sqrt(SquareMagnitude());
	}

	inline void Normalize() {
		operator/=(Magnitude());
	}

	inline Vector3 Normalized() const {
		Vector3 tmp(*this);
		return tmp.operator/=(Magnitude());
	}

#pragma region 运算

	inline friend Vector3 operator+(const Vector3& lhs, const Vector3& rhs) {
		return Vector3(lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2]);
	}

	inline friend Vector3 operator-(const Vector3& lhs, const Vector3& rhs) {
		return Vector3(lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2]);
	}

	// 取反
	inline friend Vector3 operator-(const Vector3& lhs) {
		return Vector3(-lhs[0], -lhs[1], -lhs[2]);
	}

	// 数乘
	inline friend Vector3 operator*(const Vector3& lhs, float factor) {
		Vector3 v(lhs);
		return v *= factor;
	}

	inline friend Vector3 operator*(float factor, const Vector3& rhs) {
		Vector3 v(rhs);
		return v *= factor;
	}

	inline friend Vector3 operator/(const Vector3& lhs, float factor) {
		Vector3 lhs1(lhs);
		return lhs1 /= factor;
	}

	inline friend float Dot(const Vector3& lhs, const Vector3& rhs) {
		return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
	}

	inline friend Vector3 Cross(const Vector3& lhs, const Vector3& rhs) {
		return Vector3(
			lhs[1] * rhs[2] - lhs[2] * rhs[1],
			lhs[0] * rhs[2] - lhs[2] * rhs[0],
			lhs[0] * rhs[1] - lhs[1] * rhs[0]
		);
	}

	inline friend float Angle(const Vector3& lhs, const Vector3& rhs) {
		float dot = Dot(lhs, rhs);
		float val = dot / lhs.Magnitude() / rhs.Magnitude();
		float angle = acos(val) * 180 / M_PI;
		return angle;
	}

	inline friend bool operator==(const Vector3& lhs, const Vector3& rhs) {
		return lhs[0] == rhs[0] && lhs[1] == rhs[1] && lhs[2] == rhs[2];
	}

	inline friend bool operator!=(const Vector3& lhs, const Vector3& rhs) {
		return !(lhs == rhs);
	}

#pragma endregion

	// 运算赋值运算符
public:
	inline Vector3& operator+=(const Vector3& other) {
		_data[0] += other[0];
		_data[1] += other[1];
		_data[2] += other[2];
		return *this;
	}

	inline Vector3& operator-=(const Vector3& other) {
		_data[0] -= other[0];
		_data[1] -= other[1];
		_data[2] -= other[2];
		return *this;
	}

	inline Vector3& operator*=(const float& value) {
		_data[0] *= value;
		_data[1] *= value;
		_data[2] *= value;
		return *this;
	}

	inline Vector3& operator/=(const float& value) {
		_data[0] /= value;
		_data[1] /= value;
		_data[2] /= value;
		return *this;
	}
};



