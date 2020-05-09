#pragma once

#include "Math/IntVector.h"

struct DVector {
	double x = 0;
	double y = 0;
	double z = 0;

	DVector(double x = 0, double y = 0, double z = 0) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	DVector(FVector floatVector) {
		this->x = floatVector.X;
		this->y = floatVector.Y;
		this->z = floatVector.Z;
	}


	static FORCEINLINE double Distance(const DVector& v1, const DVector& v2) {
		return sqrt((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y) + (v2.z - v1.z) * (v2.z - v1.z));
	}
	static FORCEINLINE double Distance(const FVector& v1, const DVector& v2) {
		return sqrt((v2.x - v1.X) * (v2.x - v1.X) + (v2.y - v1.Y) * (v2.y - v1.Y) + (v2.z - v1.Z) * (v2.z - v1.Z));
	}

	FVector floatVector() {
		return FVector(x, y, z);
	}
	bool isZero() {
		return x == 0 && y == 0 && z == 0;
	}
	static FORCEINLINE double DotProduct(const DVector& A, const DVector& B)
	{
		return A | B;
	}
	FORCEINLINE double operator|(const DVector& V) const
	{
		return x * V.x + y * V.y + z * V.z;
	}


	static FORCEINLINE DVector CrossProduct(const DVector& A, const DVector& B)
	{
		return A ^ B;
	}
	FORCEINLINE DVector operator^(const DVector& V) const
	{
		return DVector
		(
			y * V.z - z * V.y,
			z * V.x - x * V.z,
			x * V.y - y * V.x
		);
	}




	FORCEINLINE DVector operator+(const DVector& V) const {
		return DVector(x + V.x, y + V.y, z + V.z);
	}
	FORCEINLINE DVector operator+=(FVector* V) {
		x += V->X; y += V->Y; z += V->Z;
		return *this;
	}
	FORCEINLINE DVector operator+=(const DVector& V) {
		x += V.x; y += V.y; z += V.z;
		return *this;
	}



	FORCEINLINE DVector operator-(const FVector& V) const {
		return DVector(x - V.X, y - V.Y, z - V.Z);
	}
	FORCEINLINE DVector operator-(const DVector& V) const {
		return DVector(x - V.x, y - V.y, z - V.z);
	}
	FORCEINLINE DVector operator-=(FVector* V) {
		x -= V->X; y -= V->Y; z -= V->Z;
		return *this;
	}
	FORCEINLINE DVector operator-=(DVector* V) {
		x -= V->x; y -= V->y; z -= V->z;
		return *this;
	}
	FORCEINLINE DVector operator-=(double scale) {
		x -= scale; y -= scale; z -= scale;
		return *this;
	}



	FORCEINLINE DVector operator*(double scale) const {
		return DVector(x * scale, y * scale, z * scale);
	}
	FORCEINLINE DVector operator*(const DVector& v) const {
		return DVector(x * v.x, y * v.y, z * v.z);
	}
	FORCEINLINE DVector operator*=(const DVector& V) {
		x *= V.x; y *= V.y; z *= V.z;
		return *this;
	}
	FORCEINLINE DVector operator*=(double Scale) {
		x *= Scale; y *= Scale; z *= Scale;
		return *this;
	}


	FORCEINLINE DVector operator/(double scale) const {
		return DVector(x / scale, y / scale, z / scale);
	}



	FORCEINLINE bool Normalize(float Tolerance = SMALL_NUMBER)
	{
		const float SquareSum = x * x + y * y + z * z;
		if (SquareSum > Tolerance)
		{
			double s = SquareSum;
			double x2 = s * 0.5;
			int64_t i = *(int64_t*)&s;
			// The magic number is for doubles is from https://cs.uwaterloo.ca/~m32rober/rsqrt.pdf
			i = 0x5fe6eb50c7b537a9 - (i >> 1);
			s = *(double*)&i;
			s = s * (1.5 - (x2 * s * s));   // 1st iteration
			s = s * (1.5 - (x2 * s * s));   // 2nd iteration, this can be removed
			x *= s; y *= s; z *= s;
			return true;
		}
		return false;
	}




	inline DVector RotateAngleAxis(const double AngleDeg, const DVector& Axis) const {
		double radians = AngleDeg * (PI / 180.0f);
		double S = sin(radians);
		double C = cos(radians);

		const float XX = Axis.x * Axis.x;
		const float YY = Axis.y * Axis.y;
		const float ZZ = Axis.z * Axis.z;

		const float XY = Axis.x * Axis.y;
		const float YZ = Axis.y * Axis.z;
		const float ZX = Axis.z * Axis.x;

		const float XS = Axis.x * S;
		const float YS = Axis.y * S;
		const float ZS = Axis.z * S;

		const float OMC = 1.f - C;

		return DVector(
			(OMC * XX + C) * x + (OMC * XY - ZS) * y + (OMC * ZX + YS) * z,
			(OMC * XY + ZS) * x + (OMC * YY + C) * y + (OMC * YZ - XS) * z,
			(OMC * ZX - YS) * x + (OMC * YZ + XS) * y + (OMC * ZZ + C) * z
		);
	}







};