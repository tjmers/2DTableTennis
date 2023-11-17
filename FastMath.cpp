#include "FastMath.h"


std::unordered_map<int, float> FastMath::sines;
std::unordered_map<int, float> FastMath::cosines;

bool FastMath::has_init = false;
void FastMath::Init()
{
	if (has_init) return;
	for (int i = 0; i <= 90; i++)
	{
		sines[i] = std::sin(i * PI / 180);
		cosines[i] = std::cos(i * PI / 180);
	}

	has_init = true;
}

float FastMath::sin(int n)
{
	bool is_neg = false;
	n = n % 360;
	if (n < 0)
		n += 360;
	if (n > 180)
	{
		n = 360 - n;
		is_neg = true;
	}
	if (n > 90)
		n = 180 - n;


	return is_neg ? -sines[n] : sines[n];
}

float FastMath::cos(int n)
{
	n %= 360;
	if (n < 0) {
		n += 360;
	}

	if (n > 180) {
		n = 360 - n;
	}

	bool is_neg = false;
	if (n > 90) {
		is_neg = true;
		n = 180 - n;
	}

	return is_neg ? -cosines[n] : cosines[n];
}



void FastMath::Normalize(Vector2DF& vector)
{
	float i_mag = i_sqrt(vector.x * vector.x + vector.y * vector.y);
	vector.x *= i_mag;
	vector.y *= i_mag;
}

float FastMath::i_sqrt(float n)
{
	const float threehalfs = 1.5F;
	float y = n;

	long i = *(long*)&y;

	i = 0x5f3759df - (i >> 1);
	y = *(float*)&i;

	y = y * (threehalfs - ((n * 0.5F) * y * y));
	y = y * (threehalfs - ((n * 0.5F) * y * y));

	return y;
}

int FastMath::GetAngle(const Vector2DF& vector)
{
	return (int)(atan2(vector.y, vector.x) * 180 / PI);
}