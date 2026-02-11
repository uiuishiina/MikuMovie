
#include"DXGI.h"
#include<cassert>
#pragma comment(lib, "dxgi.lib")

//@brief	DXGi作成関数
[[nodiscard]] bool DXGI::Create()noexcept {
#if _DEBUG
    // デバッグレイヤーをオンに
    // これを行う事で、DirectXのエラー内容をより詳細に知ることができる
    ID3D12Debug* debug;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)))) {
        debug->EnableDebugLayer();
    }
#endif
    // DXGIファクトリーの作成
    {
        UINT createFactoryFlags = 0;
#if _DEBUG
        createFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
        const auto hr = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&Factory_));
        if (FAILED(hr)) {
            assert(false && "DXGIファクトリーの作成に失敗");
            return false;
        }
    }
    // アダプタの取得
    {
        // アダプタを列挙
        auto                                  select = 0;
        Microsoft::WRL::ComPtr<IDXGIAdapter1> dxgiAdapter{};

        while (Factory_->EnumAdapters1(select, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND) {
            DXGI_ADAPTER_DESC1 desc{};
            dxgiAdapter->GetDesc1(&desc);

            select++;
            // ソフトウェアアダプタは除外
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
                dxgiAdapter->Release();
                continue;
            }
            // Direct3D12 が動かない場合も除外
            if (FAILED(D3D12CreateDevice(dxgiAdapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) {
                dxgiAdapter->Release();
                continue;
            }
            Adapter_ = dxgiAdapter;
            break;
        }
        if (!Adapter_) {
            assert(false && "アダプタの取得に失敗");
            return false;
        }
    }
    return true;
}

//@brief	ファクトリー取得関数
[[nodiscard]] IDXGIFactory4* DXGI::GetFactory() const noexcept {
    if (!Factory_) {
        assert(false && "DXGIファクトリーが未作成です");
    }
    return Factory_.Get();
}

//@brief	アダプター取得関数
[[nodiscard]] IDXGIAdapter1* DXGI::GetAdapter() const noexcept {
    if (!Adapter_) {
        assert(false && "ディスプレイアダプターが未作成です");
    }
    return Adapter_.Get();
}