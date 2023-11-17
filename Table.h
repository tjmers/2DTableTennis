#pragma once

#include "Graphics.h"

class Table
{
	const D2D1_RECT_F table;
	const D2D1_RECT_F net;
public:
	Table(int table_height, int net_height);
	~Table();

	void Draw(const Graphics* g) const;

	D2D1_RECT_F GetTable() const;
	D2D1_RECT_F GetNet() const;
};