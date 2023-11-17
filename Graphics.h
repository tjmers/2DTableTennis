#pragma once

#include <Windows.h>

#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")

#include <iostream>


constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

class Graphics
{
	ID2D1Factory* factory;
	ID2D1HwndRenderTarget* render_target;
	ID2D1SolidColorBrush* brush;
public:
	Graphics();
	~Graphics();


	bool Init(HWND hWnd);

	void SetColor(const D2D1::ColorF& color) const;
	void ClearScreen(float r, float g, float b);
	void FillRect(const D2D1_RECT_F& rect) const;
	void FillCircle(float x, float y, float r) const;
	void DrawLine(const D2D1_POINT_2F& point1, const D2D1_POINT_2F& point2, const float width) const;

	void BeginDraw();
	void EndDraw();
};
