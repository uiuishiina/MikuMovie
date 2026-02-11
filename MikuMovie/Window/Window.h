#pragma once
//---  Window.h  ---

#include <Windows.h>
#include <string>

 //@brief	ウィンドウ制御クラス
class Window final {
public:
    //@brief    インスタンス取得関数
    static Window& Instance() {
        static Window instance_;
        return instance_;
    }

    //@brief    ウィンドウ作成関数
    [[nodiscard]] bool Create(HINSTANCE instance, int width, int height, std::string_view name = "MikuMovie") noexcept;
    //@brief    ループ関数
    [[nodiscard]] bool MessageLoop() const noexcept;
    //@brief    サイズ取得関数
    [[nodiscard]] std::pair<int, int> GetSize() const noexcept;
    //@brief    ハンドル取得関数
    [[nodiscard]] HWND GetHandle()const noexcept;
private:
    Window() = default;
    ~Window() = default;
    HWND Handle_{};  /// ウィンドウハンドル
    std::pair<int, int> Size_{};
};
