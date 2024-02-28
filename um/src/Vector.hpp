#pragma once
#include <numbers>
#include <cmath>
#include "Global.hpp"

struct view_matrix_t
{
	float* operator[](int index)
	{
		return matrix[index];
	}

	float matrix[4][4];
};
struct Vector2
{
	Vector2(
		const float x = 0.f,
		const float y = 0.f) noexcept :
		x(x), y(y) { }
	const Vector2 operator+(const Vector2& other) const noexcept
	{
		return Vector2{ x + other.x, y + other.y };
	}
	const Vector2 operator-(const Vector2& other) const noexcept
	{
		return Vector2{ x - other.x, y - other.y };
	}
	const Vector2 operator*(const Vector2& other) const noexcept
	{
		return { x * other.x,y * other.y };
	}
	const Vector2 operator/(const Vector2& other) const noexcept
	{
		return { x / other.x,y / other.y };
	}
	const Vector2 operator*(const float factor) const noexcept
	{
		return Vector2{ x * factor, y * factor};
	}
	const Vector2 operator/(const float factor) const noexcept
	{
		return Vector2{ x / factor, y / factor};
	}
	bool operator==(const Vector2& other) const noexcept
	{
		return x == other.x && y == other.y;
	}
	bool operator!=(const Vector2& other) const noexcept
	{
		return x != other.x || y != other.y;
	}
	ImVec2 ToImVec2()
	{
		return ImVec2(x, y);
	}
	float Length()
	{
		return sqrtf(powf(x, 2) + powf(y, 2));
	}
	float DistanceTo(const Vector2& Pos)
	{
		return sqrtf(powf(Pos.x - x, 2) + powf(Pos.y - y, 2));
	}
	float x, y;
};
struct Vector3
{
	// constructor
	Vector3(
		const float x = 0.f,
		const float y = 0.f,
		const float z = 0.f) noexcept :
		x(x), y(y), z(z) { }

	// operator overloads
	const Vector3 operator-(const Vector3& other) const noexcept
	{
		return Vector3{ x - other.x, y - other.y, z - other.z };
	}

	const Vector3 operator+(const Vector3& other) const noexcept
	{
		return Vector3{ x + other.x, y + other.y, z + other.z };
	}

	const Vector3 operator/(const float factor) const noexcept
	{
		return Vector3{ x / factor, y / factor, z / factor };
	}

	const Vector3 operator*(const float factor) const noexcept
	{
		return Vector3{ x * factor, y * factor, z * factor };
	}
	// utils
	float dot(Vector3 a, Vector3 b)  //calculates dot product of a and b
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	float mag(Vector3 a)  //calculates magnitude of a
	{
		return std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
	}
	float Length()
	{
		return sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
	}
	float DistanceTo(const Vector3& Pos)
	{
		return sqrtf(powf(Pos.x - x, 2) + powf(Pos.y - y, 2) + powf(Pos.z - z, 2));
	}
	const Vector3 ToAngle() const noexcept
	{
		return Vector3{
			std::atan2(-z, std::hypot(x, y)) * (180.0f / std::numbers::pi_v<float>),
			std::atan2(y, x) * (180.0f / std::numbers::pi_v<float>),
			0.0f };
	}

	const bool IsZero() const noexcept
	{
		return x == 0.f && y == 0.f && z == 0.f;
	}

	Vector3 WorldtoScreen(view_matrix_t matrix) const
	{
		float _x = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z + matrix[0][3];
		float _y = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z + matrix[1][3];
		float w = matrix[3][0] * x + matrix[3][1] * y + matrix[3][2] * z + matrix[3][3];

		if (w < 0.001f)
			return 0;

		float inv_w = 1.f / w;
		_x *= inv_w;
		_y *= inv_w;

		float screen_x = screenWidth * 0.5f;
		float screen_y = screenHeight * 0.5f;

		screen_x += 0.5f * _x * screenWidth + 0.5f;
		screen_y -= 0.5f * _y * screenHeight + 0.5f;

		return { screen_x,screen_y,w };
	}
	// struct data
	float x, y, z;
};