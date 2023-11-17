#pragma once
#include "Graphics.h"
#include "Ball.h"
#include "FastMath.h"
#include <vector>
#include "Table.h"

struct RotatedRectangle
{
	D2D1_POINT_2F top_left, top_right, bottom_right, bottom_left;
	void Transform(float x, float y);
};


class Player
{

	constexpr static int WIDTH = 12, HEIGHT = 56;
	constexpr static int SPEED = 10;
	constexpr static int DIAG_SPEED = 7;
	D2D1_RECT_F unrotated_hitbox;
	bool left, right, up, down;
	int rotation_angle; // from [-90, 90] , even
	bool should_rotate_left, should_rotate_right;
	bool fast;
	RotatedRectangle actual_hitbox;

	void FixPosition(bool is_left);
	void RotateLeft();
	void RotateRight();
	void UpdateRotatedVerticies();
	void MoveHitboxes(const D2D1_POINT_2F& velocity_vector);
	//static void add_float_to_vector(std::vector<wchar_t>& vector, float num);
public:
	Player(float x, float y);
	~Player();


	void Update(bool is_left);
	int CheckCollision(const Ball& ball) const;
	void Draw(const Graphics* g) const;
	void SetUp(bool b);
	void SetDown(bool b);
	void SetLeft(bool b);
	void SetRight(bool b);
	void SetShouldRotateLeft(bool b);
	void SetShouldRotateRight(bool b);
	void SetFast(bool b);

	D2D1_POINT_2F GetVelocityVector() const;

	const RotatedRectangle& GetActualHitbox() const;
	void LogActualHitbox() const;
};