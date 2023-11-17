#pragma once

#include "Graphics.h"
#include <cmath>
class Match;
#include "Match.h"
#include "Log.h"

class Ball
{
	constexpr static float PI = 3.14159265358979323846f;
	float x, y, x_velocity, y_velocity, angular_spin;
	const float radius;
	bool in_play;
	float current_rotation;

	void UpdateAngularSpeed(float c);
	void UpdateAirResistanceAndGravity();
	int UpdateFloorBounce();
	int GetWinner() const;

	Match* match;
public:
	Ball(Match* match, float start_x, float start_y, float radius);
	~Ball();

	void Update();
	void HandleCollision(const Player* player, int side_number);
	void Draw(const Graphics* g) const;

	float GetX() const;
	float GetY() const;
	float GetRadius() const;
	float GetXVelocity() const;
	float GetYVelocity() const;
};