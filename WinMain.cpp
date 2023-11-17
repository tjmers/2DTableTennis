#include "Match.h"

constexpr int GAME_LOOP_TIMER = 1;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

Graphics* g;
Match* match;

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{

	WNDCLASSEX window{ 0 };
	window.hInstance = hInstance;
	window.lpszClassName = L"mainwin";
	window.lpfnWndProc = WndProc;
	window.cbSize = sizeof(window);
	window.style = CS_OWNDC;
	window.cbClsExtra = 0;
	window.cbWndExtra = 0;
	window.hIcon = nullptr;
	window.hCursor = LoadCursor(NULL, IDC_ARROW);
	window.lpszMenuName = nullptr;
	window.hIconSm = nullptr;
	RegisterClassEx(&window);

	RECT client_region = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRectEx(&client_region, WS_CAPTION | WS_SYSMENU | WS_BORDER, FALSE, 0);

	HWND hWnd = CreateWindowExW(
		0,
		L"mainwin",
		L"2D Ping Pong",
		WS_CAPTION | WS_SYSMENU | WS_BORDER,
		75, 25, client_region.right - client_region.left, client_region.bottom - client_region.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	g = new Graphics();
	if (!g->Init(hWnd))
	{
		delete g;
		return -1;
	}

	match = new Match(g);

	SetTimer(hWnd, GAME_LOOP_TIMER, 16, (TIMERPROC)NULL);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	MSG msg;
	BOOL gResult;


	while (gResult = GetMessage(&msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	KillTimer(hWnd, GAME_LOOP_TIMER);
	delete g;
	delete match;


	if (gResult == -1) {
		return -1;
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		
	case WM_TIMER:
		if (wParam == GAME_LOOP_TIMER)
		{
			match->Update();

			InvalidateRect(hWnd, NULL, TRUE);
		}
		return 0;

	case WM_PAINT:
		g->BeginDraw();
		g->ClearScreen(0.0f, 0.0f, 0.0f);
		match->Draw();

		g->EndDraw();
		return DefWindowProcW(hWnd, uMsg, wParam, lParam);

	case WM_KEYDOWN:
		match->HandleKeyDown(wParam);
		return 0;

	case WM_KEYUP:
		match->HandleKeyUp(wParam);
		return 0;

	default:
		return DefWindowProcW(hWnd, uMsg, wParam, lParam);
	}
}