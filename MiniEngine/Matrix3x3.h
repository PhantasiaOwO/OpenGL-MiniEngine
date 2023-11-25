#pragma once
#include <GL/glut.h>

#include "Vector3.h"

class Matrix3x3 {
	float _data[3][3];
	float _glMatrix[4][4];

	inline void InterSetIdentity() {
		_data[0][0] = 1.;
		_data[0][1] = 0;
		_data[0][2] = 0;
		_data[1][0] = 0;
		_data[1][1] = 1.;
		_data[1][2] = 0;
		_data[2][0] = 0;
		_data[2][1] = 0;
		_data[2][2] = 1.;
	}

public:
	Matrix3x3() { InterSetIdentity(); }

	inline float* operator[](size_t i) {
		return (float*)_data[i];
	}

	inline const float* operator[](size_t i) const {
		return (float*)_data[i];
	}

	inline float Determinant() const {
		float coFactor00 = _data[1][1] * _data[2][2] - _data[1][2] * _data[2][1];
		float coFactor10 = _data[1][2] * _data[2][0] - _data[1][0] * _data[2][2];
		float coFactor20 = _data[1][0] * _data[2][1] - _data[1][1] * _data[2][0];
		float det = _data[0][0] * coFactor00 + _data[0][1] * coFactor10 + _data[0][2] * coFactor20;
		return det;
	}

	inline Matrix3x3 Inverse() const {
		float det = Determinant();
		Matrix3x3 ret;
		if (std::fabs(det) > 1e-6) {
			ret[0][0] = _data[1][1] * _data[2][2] - _data[1][2] * _data[2][1];
			ret[0][1] = _data[0][2] * _data[2][1] - _data[0][1] * _data[2][2];
			ret[0][2] = _data[0][1] * _data[1][2] - _data[0][2] * _data[1][1];
			ret[1][0] = _data[1][2] * _data[2][0] - _data[1][0] * _data[2][2];
			ret[1][1] = _data[0][0] * _data[2][2] - _data[0][2] * _data[2][0];
			ret[1][2] = _data[0][2] * _data[1][0] - _data[0][0] * _data[1][2];
			ret[2][0] = _data[1][0] * _data[2][1] - _data[1][1] * _data[2][0];
			ret[2][1] = _data[0][1] * _data[2][0] - _data[0][0] * _data[2][1];
			ret[2][2] = _data[0][0] * _data[1][1] - _data[0][1] * _data[1][0];

			float inv_det = 1.0 / det;
			for (size_t i = 0; i < 3; ++i)
			for (size_t j = 0; j < 3; ++j) {
				ret[i][j] *= inv_det;
			}
		}
		return ret;
	}

	inline Matrix3x3 Transpose() const {
		Matrix3x3 m;
		for (size_t i = 0; i < 3; ++i)
		for (size_t j = 0; j < 3; ++j) {
			m[i][j] = _data[j][i];
		}
		return m;
	}

	// 矩阵运算
public:
	inline friend bool operator==(const Matrix3x3& lhs, const Matrix3x3& rhs) {
		for (size_t i = 0; i < 3; i++)
		for (size_t j = 0; j < 3; j++) {
			if (lhs[i][j] != rhs[i][j]) return false;
		}
		return true;
	}

	inline friend bool operator!=(const Matrix3x3& lhs, const Matrix3x3& rhs) {
		return !(lhs == rhs);
	}

	inline friend Matrix3x3 operator+(const Matrix3x3& lhs, const Matrix3x3& rhs) {
		Matrix3x3 ret;
		for (size_t i = 0; i < 3; i++)
		for (size_t j = 0; j < 3; j++) {
			ret[i][j] = lhs[i][j] + rhs[i][j];
		}
		return ret;
	}

	inline friend Matrix3x3 operator-(const Matrix3x3& lhs, const Matrix3x3& rhs) {
		Matrix3x3 ret;
		for (size_t i = 0; i < 3; i++) 
		for (size_t j = 0; j < 3; j++) {
			ret[i][j] = lhs[i][j] - rhs[i][j];
		}
		return ret;
	}

	inline friend Matrix3x3 operator*(const Matrix3x3& lhs, const Matrix3x3& rhs) {
		Matrix3x3 ret;
		for (size_t i = 0; i < 3; i++)
		for (size_t j = 0; j < 3; j++) {
			ret[i][j] = lhs[i][0] * rhs[0][j]
				+ lhs[i][1] * rhs[1][j]
				+ lhs[i][2] * rhs[2][j];
		}
		return ret;
	}

	inline friend Vector3 operator*(const Matrix3x3& lhs, const Vector3& v3) {
		Vector3 ret;
		for (size_t i = 0; i < 3; i++) {
			ret[i] = lhs[i][0] * v3[0]
				+ lhs[i][1] * v3[1]
				+ lhs[i][2] * v3[2];
		}
		return ret;
	}

	inline friend Matrix3x3 operator*(const Matrix3x3& lhs, const float& factor) {
		Matrix3x3 ret;
		for (size_t i = 0; i < 3; i++)
		for (size_t j = 0; j < 3; j++) {
			ret[i][j] = lhs[i][j] * factor;
		}
		return ret;
	}

	inline friend Matrix3x3 operator-(const Matrix3x3& lhs) {
		Matrix3x3 ret;
		for (size_t i = 0; i < 3; i++)
		for (size_t j = 0; j < 3; j++) {
			ret[i][j] = -lhs[i][j];
		}
		return ret;
	}
};

// GL矩阵是竖向递增
class Matrix4x4ForGL {
	GLfloat _data[16];

public:
	explicit Matrix4x4ForGL(Matrix3x3 m) {
		_data[0] = m[0][0];
		_data[4] = m[0][1];
		_data[8] = m[0][2];
		_data[12] = 0;
		
		_data[1] = m[1][0];
		_data[5] = m[1][1];
		_data[9] = m[1][2];
		_data[13] = 0;
		
		_data[2] = m[2][0];
		_data[6] = m[2][1];
		_data[10] = m[2][2];
		_data[14] = 0;
		
		_data[3] = 0;
		_data[7] = 0;
		_data[11] = 0;
		_data[15] = 1;
	}

	operator GLfloat*() {
		return _data;
	}
};

