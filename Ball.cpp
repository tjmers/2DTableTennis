#include "Ball.h"


Ball::Ball(Match* match, float start_x, float start_y, float radius)
	: x(start_x), y(start_y), radius(radius), x_velocity(0), y_velocity(0), angular_spin(0), in_play(false), current_rotation(PI), match(match)
{}

Ball::~Ball()
{}

int Ball::GetWinner() const
{
	if (x - radius < 0)
		return 1;

	if (x + radius >= SCREEN_WIDTH)
		return 2;

	return 0;
}


void Ball::Update()
{
	if (!in_play)
		return;

	UpdateAirResistanceAndGravity();
	UpdateFloorBounce();


	x += x_velocity;
	y += y_velocity;

}

void Ball::UpdateAngularSpeed(float c)
{

}

void Ball::UpdateAirResistanceAndGravity()
{
	constexpr static float G = -1.5f;
	constexpr static float HALFG = G / 2;

	y_velocity -= HALFG;

	constexpr static float air_resistance_coef = 0.001f;

	//float mag_velocity = std::sqrt(x_velocity * x_velocity + y_velocity * y_velocity);
	//mag_velocity -= mag_velocity * air_resistance_coef;
	//Vector2DF vel{ x_velocity, y_velocity };
	//FastMath::Normalize(vel);
	//vel.x *= mag_velocity;
	//vel.y *= mag_velocity;
	//x_velocity = vel.x;
	//y_velocity = vel.y;
}

// elastic collision
// returns 0 for no bounce, 1 for bounce on left, 2 for bounce on right
int Ball::UpdateFloorBounce()
{
	if (y - radius > match->GetTable()->GetTable().top)
	{
		y = match->GetTable()->GetTable().top + radius;
		y_velocity *= -.9f;

		return (x > (match->GetTable()->GetNet().left + match->GetTable()->GetNet().right) / 2) + 1;
	}

	return 0;
}


// fix this function
void Ball::HandleCollision(const Player* player, int side_number)
{
	if (side_number == 0) return;
	if (!in_play)
		in_play = true;


	const RotatedRectangle actual_hitbox = player->GetActualHitbox();
	const D2D1_POINT_2F p1 = *((D2D1_POINT_2F*)&actual_hitbox + side_number - 1);
	const D2D1_POINT_2F p2 = *((D2D1_POINT_2F*)&actual_hitbox + (side_number % 4));
	Vector2DF vector_to_reflect_on = { p1.y - p2.y, p2.x - p1.x };

	//if ((vector_to_reflect_on.x * y_velocity + vector_to_reflect_on.y * x_velocity) *
	//	FastMath::i_sqrt(vector_to_reflect_on.x * vector_to_reflect_on.x + vector_to_reflect_on.y * vector_to_reflect_on.y) *
	//	FastMath::i_sqrt(x_velocity * x_velocity + y_velocity * y_velocity) < 0)
	//{
	//	vector_to_reflect_on.x *= -1;
	//	vector_to_reflect_on.y *= -1;
	//}

	float m = vector_to_reflect_on.y / vector_to_reflect_on.x;
	float m_squared = m * m;
	float i_m_squared_plus_one = 1 / (1 + m_squared);
	
	float old_x_velocity = x_velocity;
	x_velocity = (1 - m_squared) * x_velocity * i_m_squared_plus_one + 2 * m * y_velocity * i_m_squared_plus_one;
	y_velocity = 2 * m * old_x_velocity * i_m_squared_plus_one + (m_squared - 1) * y_velocity * i_m_squared_plus_one;

	// now add the velocity vector of the paddle
	D2D1_POINT_2F player_velocity = player->GetVelocityVector();
	x_velocity += player_velocity.x;
	y_velocity += player_velocity.y;

	// do a thing
	x += x_velocity;
	y += y_velocity;

	// lose some energy
	x_velocity *= .7f;
	y_velocity *= .7f;

}

void Ball::Draw(const Graphics* g) const
{
	g->FillCircle(x, y, radius);
}


float Ball::GetX() const { return x; }
float Ball::GetY() const { return y; }
float Ball::GetRadius() const { return radius; }
float Ball::GetXVelocity() const { return x_velocity; }
float Ball::GetYVelocity() const { return y_velocity; }