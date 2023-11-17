#include "Graphics.h"

Graphics::Graphics()
	: factory(nullptr), render_target(nullptr), brush(nullptr)
{

}

Graphics::~Graphics()
{
	if (factory) factory->Release();
	if (render_target) render_target->Release();
	if (brush) brush->Release();
}

bool Graphics::Init(HWND hWnd)
{
    HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
    if (res != S_OK) return false;

    RECT client_rect;
    GetClientRect(hWnd, &client_rect);

    res = factory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(
            hWnd,
            D2D1::SizeU(client_rect.right, client_rect.bottom)
        ),
        &render_target
    );

    if (res != S_OK) return false;

    res = render_target->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 0), &brush);
    if (res != S_OK) return false;


    return true;

}


void Graphics::BeginDraw() { render_target->BeginDraw(); }
void Graphics::EndDraw() { render_target->EndDraw(); }


void Graphics::SetColor(const D2D1::ColorF& color) const { brush->SetColor(color); }

void Graphics::ClearScreen(float r, float g, float b)
{
    render_target->Clear(D2D1::ColorF(r, g, b));
}


void Graphics::FillRect(const D2D1_RECT_F& rect) const
{
    render_target->FillRectangle(rect, brush);
}

void Graphics::DrawLine(const D2D1_POINT_2F& point1, const D2D1_POINT_2F& point2, const float width) const
{
    render_target->DrawLine(point1, point2, brush, width);
}


void Graphics::FillCircle(float x, float y, float r) const
{
    render_target->FillEllipse({ { x, y }, r, r}, brush);
}