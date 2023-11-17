#pragma once

#include <cmath>
#include <unordered_map>

struct Vector2DF
{
	float x, y;
	Vector2DF(float x, float y) : x(x), y(y) {}
};

class FastMath
{
	static std::unordered_map<int, float> sines;
	static std::unordered_map<int, float> cosines;
	// can add tangents
	static bool has_init;
public:
	static void Init();
	constexpr static float PI = 3.14159265358979323846f;
	static float sin(int n);
	static float cos(int n);

	static void Normalize(Vector2DF& vector);
	static float i_sqrt(float n);
	static int GetAngle(const Vector2DF& vector);
};