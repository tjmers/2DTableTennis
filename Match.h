#pragma once

class Player;
class Ball;
class Table;

#include "Player.h"


class Match
{
	Player* player_left;
	Player* player_right;
	Ball* ball;
	Table* table;
	Graphics* g;

public:
	Match(Graphics* g);
	~Match();

	void Update();
	void Draw() const;

	void HandleKeyDown(WPARAM vk_code);
	void HandleKeyUp(WPARAM vk_code);

	Player* GetPlayerLeft() const;
	Player* GetPlayerRight() const;
	Table* GetTable() const;
};