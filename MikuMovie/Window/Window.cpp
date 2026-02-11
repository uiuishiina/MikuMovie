
#include"Window.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

//@brief    ウィンドウ作成関数
[[nodiscard]] bool Window::Create(HINSTANCE instance, int width, int height, std::string_view name) noexcept {

	//ウィンドウ作成
    WNDCLASS wc{};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = instance;
    wc.lpszClassName = name.data();
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    RegisterClass(&wc);

    Handle_ = CreateWindow(wc.lpszClassName, wc.lpszClassName,
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        nullptr, nullptr, instance, nullptr);
    if (!Handle_) {
        return false;
    }
    ShowWindow(Handle_, SW_SHOW);
    // ウィンドウを更新
    UpdateWindow(Handle_);

    Size_ = { width ,height };

    return true;
}

//@brief    ループ関数
[[nodiscard]] bool Window::MessageLoop() const noexcept {
    MSG msg{};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false;
        }
        // メッセージ処理
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

//@brief    サイズ取得関数
[[nodiscard]] std::pair<int, int> Window::GetSize() const noexcept {
    return Size_;
}

//@brief    ハンドル取得関数
[[nodiscard]] HWND Window::GetHandle()const noexcept {
    return Handle_;
}