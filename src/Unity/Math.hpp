#pragma once
#include "../AvirA.hpp"
#include <cmath>

namespace AvirA
{
	struct Vector2
	{
		float x = 0;
		float y = 0;
	};

	struct Vector3
	{
		float x = 0;
		float y = 0;
		float z = 0;

		float Length() const
		{
			return sqrtf(x * x + y * y + z * z);
		}

		float Distance(const Vector3& other) const
		{
			float dx = x - other.x;
			float dy = y - other.y;
			float dz = z - other.z;
			return sqrtf(dx * dx + dy * dy + dz * dz);
		}

		float Dot(const Vector3& other) const
		{
			return x * other.x + y * other.y + z * other.z;
		}

		Vector3 operator-(const Vector3& other) const
		{
			Vector3 out;
			out.x = x - other.x;
			out.y = y - other.y;
			out.z = z - other.z;
			return out;
		}

		Vector3 operator+(const Vector3& other) const
		{
			Vector3 out;
			out.x = x + other.x;
			out.y = y + other.y;
			out.z = z + other.z;
			return out;
		}
	};

	struct Vector4
	{
		float x = 0;
		float y = 0;
		float z = 0;
		float w = 0;
	};

	struct Quaternion
	{
		float x = 0;
		float y = 0;
		float z = 0;
		float w = 1;
	};

	struct Color
	{
		float r = 0;
		float g = 0;
		float b = 0;
		float a = 1;
	};

	struct Rect
	{
		float x = 0;
		float y = 0;
		float width = 0;
		float height = 0;
	};

	struct Bounds
	{
		Vector3 center;
		Vector3 extents;
	};

	struct Ray
	{
		Vector3 origin;
		Vector3 direction;
	};

	struct Matrix4x4
	{
		float m[4][4] = { 0 };
	};
}
