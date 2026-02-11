
#include"SwapChain.h"
#include<cassert>

//@brief	スワップチェーン作成関数
[[nodiscard]] bool SwapChain::Create(const CommandQueue& commandQueue)noexcept {
    const auto [w, h] = Window::Instance().GetSize();

    Desc_ = {};
    Desc_.BufferCount = 2;                                // バックバッファの数（ダブルバッファ）
    Desc_.Width = w;                                // バックバッファの横幅
    Desc_.Height = h;                                // バックバッファの縦幅
    Desc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;       // バックバッファのフォーマット
    Desc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // レンダーターゲットとして使用
    Desc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;    // 毎フレーム画面更新するので描画が終わったらバッファを破棄
    Desc_.SampleDesc.Count = 1;                                // マルチサンプリングなし

    // 一時的なスワップチェインの作成
    // スワップチェインのアップグレードが必要になる
    Microsoft::WRL::ComPtr<IDXGISwapChain1> tempSwapChain{};
    {
        const auto hr = Device::Instance().GetDXGI().GetFactory()->CreateSwapChainForHwnd(commandQueue.Get(), Window::Instance().GetHandle(),
            &Desc_, nullptr, nullptr, &tempSwapChain);
        if (FAILED(hr)) {
            assert(false && "スワップチェインの作成に失敗");
            return false;
        }
    }

    // スワップチェインのバージョンをアップグレード
    {
        // 一時的なスワップチェインを IDXGISwapChain3 に変換
        const auto hr = tempSwapChain->QueryInterface(IID_PPV_ARGS(&SwapChain_));
        if (FAILED(hr)) {
            assert(false && "スワップチェインのアップグレードに失敗");
            return false;
        }
    }

    return true;
}

//@brief	スワップチェーン取得関数
[[nodiscard]] IDXGISwapChain3* SwapChain::Get()const noexcept {
    if (!SwapChain_) {
        assert(false && "スワップチェインが未作成です");
        return nullptr;
    }
    return SwapChain_.Get();
}

//@brief	設定取得関数
[[nodiscard]] const DXGI_SWAP_CHAIN_DESC1& SwapChain::GetDesc()const noexcept {
    if (!SwapChain_) {
        assert(false && "スワップチェインが未作成です");
    }
    return Desc_;
}