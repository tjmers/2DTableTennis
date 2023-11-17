#include "Table.h"

Table::Table(int table_height, int net_height)
	: table(D2D1::RectF(0.0f, (float)(SCREEN_HEIGHT - table_height), (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT)),
	  net(D2D1::RectF(SCREEN_WIDTH / 2 - 4, SCREEN_HEIGHT - net_height, SCREEN_WIDTH / 2 + 4, SCREEN_HEIGHT))
{}

Table::~Table() {}

void Table::Draw(const Graphics* g) const
{
	g->FillRect(table);
	g->FillRect(net);
}


D2D1_RECT_F Table::GetTable() const
{
	return table;
}

D2D1_RECT_F Table::GetNet() const
{
	return net;
}