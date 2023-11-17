#include "Player.h"
#include "Log.h"

void RotatedRectangle::Transform(float x, float y) {
	top_left.x += x;
	top_right.x += x;
	bottom_left.x += x;
	bottom_right.x += x;

	top_left.y += y;
	top_right.y += y;
	bottom_left.y += y;
	bottom_right.y += y;
}

Player::Player(float x, float y)
	: unrotated_hitbox(D2D1::RectF(x, y, x + WIDTH, y + HEIGHT)), left(false), right(false), up(false), down(false),
	  rotation_angle(0), should_rotate_left(false), should_rotate_right(false), actual_hitbox( { 0 } ), fast(false)
{
	UpdateRotatedVerticies();
}

Player::~Player()
{}



void Player::Update(bool is_left)
{

	if (should_rotate_left && !should_rotate_right)
	{
		RotateLeft();
		UpdateRotatedVerticies();
	}

	if (should_rotate_right && !should_rotate_left)
	{
		RotateRight();
		UpdateRotatedVerticies();
	}

	MoveHitboxes(GetVelocityVector());

	FixPosition(is_left);
}

void Player::FixPosition(bool is_left)
{
	if (actual_hitbox.top_left.y < 0 || actual_hitbox.top_right.y < 0)
	{
		float diff = -min(actual_hitbox.top_left.y, actual_hitbox.top_right.y);
		unrotated_hitbox.top += diff;
		unrotated_hitbox.bottom += diff;
		actual_hitbox.Transform(0.0f, diff);
	}

	else if (actual_hitbox.bottom_left.y >= SCREEN_HEIGHT || actual_hitbox.bottom_right.y >= SCREEN_HEIGHT)
	{
		float diff = SCREEN_HEIGHT - max(actual_hitbox.bottom_left.y, actual_hitbox.bottom_right.y);
		unrotated_hitbox.top += diff;
		unrotated_hitbox.bottom += diff;
		actual_hitbox.Transform(0.0f, diff);
	}

	if (is_left)
	{
		if (actual_hitbox.bottom_left.x < 0 || actual_hitbox.top_left.x < 0)
		{
			float diff = -min(actual_hitbox.bottom_left.x, actual_hitbox.top_left.x);
			unrotated_hitbox.left += diff;
			unrotated_hitbox.right += diff;
			actual_hitbox.Transform(diff, 0.0f);
		}
		else if ((int)actual_hitbox.bottom_right.x >= SCREEN_WIDTH / 2 || (int)actual_hitbox.top_right.x >= SCREEN_WIDTH / 2)
		{
			float diff = (float)(SCREEN_WIDTH / 2) - max(actual_hitbox.top_right.x, actual_hitbox.bottom_right.x) - 1;
			unrotated_hitbox.right += diff;
			unrotated_hitbox.left += diff;
			actual_hitbox.Transform(diff, 0.0f);
		}


	}
	else
	{
		if (actual_hitbox.bottom_left.x <= (float)(SCREEN_WIDTH / 2) || actual_hitbox.top_left.x <= (float)(SCREEN_WIDTH / 2))
		{
			float diff = (float)(SCREEN_WIDTH / 2) - min(actual_hitbox.bottom_left.x, actual_hitbox.top_left.x);
			unrotated_hitbox.right += diff;
			unrotated_hitbox.left += diff;
			actual_hitbox.Transform(diff, 0.0f);
		}
		else if (actual_hitbox.top_right.x >= SCREEN_WIDTH || actual_hitbox.bottom_right.x >= SCREEN_WIDTH)
		{
			float diff = SCREEN_WIDTH - max(actual_hitbox.top_right.x, actual_hitbox.bottom_right.x);
			unrotated_hitbox.right += diff;
			unrotated_hitbox.left += diff;
			actual_hitbox.Transform(diff, 0.0f);
		}
	}
}

// returns 0 for no collision, 1 for collision on top, 2 for collision on right, 3 for collision on bottom, 4 for collision on left
int Player::CheckCollision(const Ball& ball) const
{
	
	// Number of steps for CCD
	constexpr int numSteps = 50;

	// for each line segment of the hitbox, we find the squared distance between it and the radius
	for (int i = 0; i < 4; i++) {
		D2D1_POINT_2F p1 = *((D2D1_POINT_2F*)&actual_hitbox + i);
		D2D1_POINT_2F p2 = *((D2D1_POINT_2F*)&actual_hitbox + ((i + 1) % 4));

		// Calculate the velocity vector of the ball
		float velocityX = ball.GetXVelocity();
		float velocityY = ball.GetYVelocity();

		// Perform CCD by checking multiple points along the movement path
		for (int step = 0; step < numSteps; ++step) {
			// Calculate the current position of the ball
			float currentX = ball.GetX() + velocityX / numSteps;
			float currentY = ball.GetY() + velocityY / numSteps;

			// Calculate the vector from p1 to the circle's center
			float cx = currentX - p1.x;
			float cy = currentY - p1.y;

			// Calculate the dot product
			float dotProduct = (p2.x - p1.x) * cx + (p2.y - p1.y) * cy;

			// Calculate the squared magnitude of the line segment
			float lengthSquared = (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y);

			// Calculate the distance along the line where the closest point is
			float t = min(max(dotProduct / lengthSquared, 0.0f), 1.0f);

			// Calculate the closest point on the line segment to the circle's center
			float closestX = p1.x + t * (p2.x - p1.x);
			float closestY = p1.y + t * (p2.y - p1.y);

			// Calculate the squared distance between the closest point and the circle's center
			float distanceSquared = (closestX - currentX) * (closestX - currentX) +
				(closestY - currentY) * (closestY - currentY);

			// Check if the squared distance is less than or equal to the squared circle's radius
			if (distanceSquared <= (ball.GetRadius() * ball.GetRadius())) {
				return i; // Collision detected
			}
		}
	}

	return 0; // No collision detected
	
	/*
	
	// get an equation for the lines
	Vector2DF ball_velocity = { ball.GetXVelocity(), ball.GetXVelocity()};
	D2D1_POINT_2F player_velocity = GetVelocityVector();
	ball_velocity.x -= player_velocity.x;
	ball_velocity.y -= player_velocity.y;


	float ball_line_slope = ball_velocity.y - ball_velocity.x;
	float ball_lower_x = min(ball.GetX(), ball.GetX() - ball_velocity.x);
	float ball_upper_x = max(ball.GetX(), ball.GetX() - ball_velocity.x);
	float ball_lower_y = min(ball.GetY(), ball.GetY() - ball_velocity.y);
	float ball_upper_y = max(ball.GetY(), ball.GetY() - ball_velocity.y);

	// we only ever need to check for collision on two sides of the player
	// we can determine which two sides to check by taking the velocity vector projection on to the normal vector of the paddle and checking if > 0

	// what if we take the angle the ball velocity vector is at and add the angle that the player is rotated at

	if (ball_velocity.x == 0 && ball_velocity.y == 0)
		return 0;
	int velocity_angle = FastMath::GetAngle(ball_velocity);


	velocity_angle -= rotation_angle;



	if (velocity_angle < 360)
		velocity_angle += 360;
	else if (velocity_angle >= 360)
		velocity_angle -= 360;

	if (velocity_angle > 90 && velocity_angle < 270)
	{
		// check for collision on left side
		if (rotation_angle == 0)
		{
			if (ball.GetX() + ball.GetRadius() >= actual_hitbox.top_left.x && ball.GetX() + ball.GetRadius() - ball_velocity.x < actual_hitbox.top_left.x && ball.GetY() + ball.GetRadius() >= actual_hitbox.top_left.y && ball.GetY() - ball.GetRadius() <= actual_hitbox.bottom_left.y)
				return 4;
		}
		else
		{
			float slope = (actual_hitbox.top_left.y - actual_hitbox.bottom_left.y) / (actual_hitbox.top_left.x - actual_hitbox.bottom_left.x);
			if (slope != ball_line_slope)
			{
				float x_of_collision = (actual_hitbox.top_left.y - slope * actual_hitbox.top_left.x + ball_line_slope * ball.GetX() - ball.GetY()) / (ball_line_slope - slope);
				if (x_of_collision > ball_lower_x && x_of_collision < ball_upper_x && x_of_collision > min(actual_hitbox.top_left.x, actual_hitbox.bottom_left.x) && x_of_collision < max(actual_hitbox.top_left.x, actual_hitbox.bottom_left.x))
					return 4;
			}
		}
	}
	else
	{
		// check for collision on right side
		debugLog.append("Checking for collision on right");
		if (rotation_angle == 0)
		{
			if (ball.GetX() - ball.GetRadius() <= actual_hitbox.top_right.x && ball.GetX() - ball.GetRadius() - ball_velocity.x > actual_hitbox.top_right.x && ball.GetY() + ball.GetRadius() >= actual_hitbox.top_right.y && ball.GetY() - ball.GetRadius() <= actual_hitbox.bottom_right.y)
				return 2;
		}
		else
		{
			float slope = (actual_hitbox.top_left.y - actual_hitbox.bottom_left.y) / (actual_hitbox.top_left.x - actual_hitbox.bottom_left.x);
			if (slope != ball_line_slope)
			{
				float x_of_collision = (actual_hitbox.top_right.y - slope * actual_hitbox.top_right.x + ball_line_slope * ball.GetX() - ball.GetY()) / (ball_line_slope - slope);
				if (x_of_collision > ball_lower_x && x_of_collision < ball_upper_x && x_of_collision > min(actual_hitbox.top_right.x, actual_hitbox.bottom_right.x) && x_of_collision < max(actual_hitbox.top_right.x, actual_hitbox.bottom_right.x))
					return 2;
			}
		}
	}
	

	return 0;

	*/
}


void Player::Draw(const Graphics* g) const
{	
	D2D1_POINT_2F midpoint_top = { (actual_hitbox.top_left.x + actual_hitbox.top_right.x) / 2, (actual_hitbox.top_left.y + actual_hitbox.top_right.y) / 2 };

	D2D1_POINT_2F midpoint_bottom = { (actual_hitbox.bottom_left.x + actual_hitbox.bottom_right.x) / 2, (actual_hitbox.bottom_left.y + actual_hitbox.bottom_right.y) / 2 };
	g->DrawLine(midpoint_top, midpoint_bottom, (float)(WIDTH));
	//g->DrawLine(actual_hitbox.top_left, actual_hitbox.top_right, 3.0f);
	//g->DrawLine(actual_hitbox.top_left, actual_hitbox.bottom_left, 3.0f);
	//g->DrawLine(actual_hitbox.bottom_right, actual_hitbox.top_right, 3.0f);
	//g->DrawLine(actual_hitbox.bottom_right, actual_hitbox.bottom_left, 3.0f);
}


void Player::SetUp(bool b) { up = b; }

void Player::SetDown(bool b) { down = b; }

void Player::SetLeft(bool b) { left = b; }

void Player::SetRight(bool b) { right = b; }

void Player::SetShouldRotateLeft(bool b) { should_rotate_left = b; }
void Player::SetShouldRotateRight(bool b) { should_rotate_right = b; }

void Player::SetFast(bool b) { fast = b; }

void Player::RotateRight()
{
	rotation_angle += fast ? 6 : 2;
	if (rotation_angle > 90)
		rotation_angle = 90;
}

void Player::RotateLeft()
{
	rotation_angle -= fast ? 6 : 2;
	if (rotation_angle < -90)
		rotation_angle = -90;
}

void Player::UpdateRotatedVerticies()
{
	//static bool first = true;
	const static int HALF_WIDTH = WIDTH / 2, HALF_HEIGHT = HEIGHT / 2;
	D2D1_POINT_2F center = { unrotated_hitbox.left + HALF_WIDTH, unrotated_hitbox.top + HALF_HEIGHT };
	const static int STARTING_ANGLE = (int)(std::atan((float)HEIGHT / WIDTH) * 180 / FastMath::PI);
	const static float DIAG_MAGNITUDE = (float)std::sqrt(HALF_HEIGHT * HALF_HEIGHT + HALF_WIDTH * HALF_WIDTH);

	//if (first)
	//{
	//	//std::vector<wchar_t> diag_mag_to_print;
	//	//add_float_to_vector(diag_mag_to_print, (float)STARTING_ANGLE);
	//	//diag_mag_to_print.push_back(0);
	//	//MessageBox(NULL, &diag_mag_to_print[0], L"STARTING_ANGLE", MB_OK);
	//	std::vector<wchar_t> v;
	//	add_float_to_vector(v, FastMath::sin(STARTING_ANGLE) * 1000);
	//	v.push_back(',');
	//	add_float_to_vector(v, FastMath::sin(180 - STARTING_ANGLE) * 1000);
	//	v.push_back(0);
	//	MessageBox(NULL, &v[0], L"SINES", MB_OK);
	//	first = false;
	//}
	
	

	actual_hitbox = {
		{ center.x + DIAG_MAGNITUDE * FastMath::cos(STARTING_ANGLE + 180 + rotation_angle), center.y + DIAG_MAGNITUDE * FastMath::sin(STARTING_ANGLE + 180 + rotation_angle) },
		{ center.x + DIAG_MAGNITUDE * FastMath::cos(-STARTING_ANGLE + rotation_angle), center.y + DIAG_MAGNITUDE * FastMath::sin(-STARTING_ANGLE + rotation_angle) },
		{ center.x + DIAG_MAGNITUDE * FastMath::cos(STARTING_ANGLE + rotation_angle), center.y + DIAG_MAGNITUDE * FastMath::sin(STARTING_ANGLE + rotation_angle) },
		{ center.x + DIAG_MAGNITUDE * FastMath::cos(180 - STARTING_ANGLE + rotation_angle), center.y + DIAG_MAGNITUDE * FastMath::sin(180 - STARTING_ANGLE + rotation_angle) }
	};
}


D2D1_POINT_2F Player::GetVelocityVector() const
{
	if ((left ^ right) && (up ^ down))
		return { (float)((right * 2 - 1) * DIAG_SPEED * (fast + 1)), (float)((down * 2 - 1) * DIAG_SPEED * (fast + 1)) };


	if (left != right)
		return { (float)((right * 2 - 1) * SPEED * (fast + 1)), 0.0f };

	if (up != down)
		return { 0.0f, (float)((down * 2 - 1) * SPEED * (fast + 1)) };

	return { 0.0f, 0.0f };
}

const RotatedRectangle& Player::GetActualHitbox() const
{
	return actual_hitbox;
}


void Player::MoveHitboxes(const D2D1_POINT_2F& velocity_vector)
{
	unrotated_hitbox.left += velocity_vector.x;
	unrotated_hitbox.right += velocity_vector.x;
	unrotated_hitbox.top += velocity_vector.y;
	unrotated_hitbox.bottom += velocity_vector.y;

	actual_hitbox.Transform(velocity_vector.x, velocity_vector.y);

}



void Player::LogActualHitbox() const
{
	debugLog.append("Player Left Actual Hitbox: (" + 
		std::to_string(actual_hitbox.top_left.x) + ", " + std::to_string(actual_hitbox.top_left.y) + ") (" + 
		std::to_string(actual_hitbox.top_right.x) + ", " + std::to_string(actual_hitbox.top_right.y) + ") (" + 
		std::to_string(actual_hitbox.bottom_right.x) + ", " + std::to_string(actual_hitbox.bottom_right.y) + ") (" + 
		std::to_string(actual_hitbox.bottom_left.x) + ", " + std::to_string(actual_hitbox.bottom_left.y) + ')');

	debugLog.append("Player Left Unrotated Hitbox: Left(" +
		std::to_string(unrotated_hitbox.left) + ") Right(" +
		std::to_string(unrotated_hitbox.right) + ") Top(" +
		std::to_string(unrotated_hitbox.top) + ") Bottom(" +
		std::to_string(unrotated_hitbox.bottom) + ")");
//	std::vector<wchar_t> str;
//	str.push_back(L'(');
//	add_float_to_vector(str, actual_hitbox.top_left.x);
//	str.push_back(L',');
//	str.push_back(L' ');
//	add_float_to_vector(str, actual_hitbox.top_left.y);
//	str.push_back(L')');
//	str.push_back(L',');
//	str.push_back(L' ');
//
//	str.push_back(L'(');
//	add_float_to_vector(str, actual_hitbox.top_right.x);
//	str.push_back(L',');
//	str.push_back(L' ');
//	add_float_to_vector(str, actual_hitbox.top_right.y);
//	str.push_back(L')');
//	str.push_back(L',');
//	str.push_back(L' ');
//
//	str.push_back(L'(');
//	add_float_to_vector(str, actual_hitbox.bottom_right.x);
//	str.push_back(L',');
//	str.push_back(L' ');
//	add_float_to_vector(str, actual_hitbox.bottom_right.y);
//	str.push_back(L')');
//	str.push_back(L',');
//	str.push_back(L' ');
//
//
//	str.push_back(L'(');
//	add_float_to_vector(str, actual_hitbox.bottom_left.x);
//	str.push_back(L',');
//	str.push_back(L' ');
//	add_float_to_vector(str, actual_hitbox.bottom_left.y);
//	str.push_back(L')');
//	str.push_back(0);
//
//
//	MessageBox(NULL, &str[0], L"A", MB_OK);
}
//
//int get_int_length(int n) {
//	int length = 0;
//	while (n > 0) {
//		n /= 10;
//		length++;
//	}
//	return length;
//}
//
//void Player::add_float_to_vector(std::vector<wchar_t>& vector, float num)
//{
//	// for numInt = 73
//	int numInt = (int)num;
//	int length = get_int_length(numInt); // 2
//
//	int multiplier = (int)std::pow(10, length - 1); // 10
//	while (length > 0)
//	{
//		vector.push_back((numInt / multiplier) + '0');
//		numInt %= multiplier;
//		multiplier /= 10;
//		length--;
//	}
//}