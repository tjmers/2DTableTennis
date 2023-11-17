#include "Match.h"

void Match::update_inputs() {
	player_left->SetUp(Input::GetKeyDown(W));
	player_left->SetLeft(Input::GetKeyDown(A));
	player_left->SetDown(Input::GetKeyDown(S));
	player_left->SetRight(Input::GetKeyDown(D));
	player_left->SetShouldRotateLeft(Input::GetKeyDown(Q));
	player_left->SetShouldRotateRight(Input::GetKeyDown(E));
	player_left->SetFast(Input::GetKeyDown(LEFT_SHIFT));

	player_right->SetUp(Input::GetKeyDown(UP));
	player_right->SetLeft(Input::GetKeyDown(LEFT));
	player_right->SetDown(Input::GetKeyDown(DOWN));
	player_right->SetRight(Input::GetKeyDown(RIGHT));
	player_right->SetShouldRotateLeft(Input::GetKeyDown(COMMA));
	player_right->SetShouldRotateRight(Input::GetKeyDown(PERIOD));
	player_right->SetFast(Input::GetKeyDown(SLASH));
}

Match::Match(Graphics* g)
	: player_left(nullptr), player_right(nullptr),
	  ball(new Ball(this, 128.0f, SCREEN_HEIGHT - 100.0f, 5.0f)),
	  table(new Table(10, 64)), g(g)
{
	FastMath::Init();
	player_left = new Player(100, 100);
	player_right = new Player(SCREEN_WIDTH - 100, 100);
}

Match::~Match()
{
	delete player_left;
	delete player_right;
	delete ball;
	delete table;
}

void Match::Update()
{
	update_inputs();
	ball->Update();
	player_left->Update(true);
	player_right->Update(false);

	ball->HandleCollision(player_left, player_left->CheckCollision(*ball));
	ball->HandleCollision(player_right, player_right->CheckCollision(*ball));


}

void Match::Draw() const
{
	g->SetColor(D2D1::ColorF::White);
	player_left->Draw(g);
	player_right->Draw(g);
	ball->Draw(g);
	g->SetColor(D2D1::ColorF::DimGray);
	table->Draw(g);
}



Player* Match::GetPlayerLeft() const
{
	return player_left;
}


Player* Match::GetPlayerRight() const
{
	return player_right;
}

Table* Match::GetTable() const
{
	return table;
}





void Match::HandleKeyDown(const WPARAM vk_code)
{
	switch (vk_code)
	{
	case 'W':
		player_left->SetUp(true);
		break;
	case 'A':
		player_left->SetLeft(true);
		break;
	case 'S':
		player_left->SetDown(true);
		break;
	case 'D':
		player_left->SetRight(true);
		break;
	case 'Q':
		player_left->SetShouldRotateLeft(true);
		break;
	case 'E':
		player_left->SetShouldRotateRight(true);
		break;
	case VK_SHIFT:
		player_left->SetFast(true);
		break;

	case VK_LEFT:
		player_right->SetLeft(true);
		break;
	case VK_RIGHT:
		player_right->SetRight(true);
		break;
	case VK_UP:
		player_right->SetUp(true);
		break;
	case VK_DOWN:
		player_right->SetDown(true);
		break;
	case VK_OEM_COMMA:
		player_right->SetShouldRotateLeft(true);
		break;
	case VK_OEM_PERIOD:
		player_right->SetShouldRotateRight(true);
		break;
	case VK_OEM_2:
		player_right->SetFast(true);
		break;

	case VK_SPACE:
		player_left->LogActualHitbox();
		break;
	}
}



void Match::HandleKeyUp(const WPARAM vk_code)
{
	switch (vk_code)
	{
	case 'W':
		player_left->SetUp(false);
		break;
	case 'A':
		player_left->SetLeft(false);
		break;
	case 'S':
		player_left->SetDown(false);
		break;
	case 'D':
		player_left->SetRight(false);
		break;
	case 'Q':
		player_left->SetShouldRotateLeft(false);
		break;
	case 'E':
		player_left->SetShouldRotateRight(false);
		break;
	case VK_SHIFT:
		player_left->SetFast(false);
		break;

	case VK_LEFT:
		player_right->SetLeft(false);
		break;
	case VK_RIGHT:
		player_right->SetRight(false);
		break;
	case VK_UP:
		player_right->SetUp(false);
		break;
	case VK_DOWN:
		player_right->SetDown(false);
		break;
	case VK_OEM_COMMA:
		player_right->SetShouldRotateLeft(false);
		break;
	case VK_OEM_PERIOD:
		player_right->SetShouldRotateRight(false);
		break;
	case VK_OEM_2:
		player_right->SetFast(false);
		break;
	}
}