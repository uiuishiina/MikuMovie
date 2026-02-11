
#include"Device.h"
#include<cassert>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")


//@brief	デバイス作成関数
[[nodiscard]] bool Device::Create()noexcept {
    // DXGIの生成
        if (!DXGIInstance_.Create()) {
            assert(false && "DXGIのアダプタ設定に失敗しました");
            return false;
        }

    // デバイス作成
    const auto hr = D3D12CreateDevice(DXGIInstance_.GetAdapter(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&Device_));
    if (FAILED(hr)) {
        assert(false && "デバイス作成に失敗");
        return false;
    }
    return true;
}

//@brief	デバイス取得関数
[[nodiscard]] ID3D12Device* Device::GetDevice()const noexcept {
    if (!Device_) {
        assert(false && "Device未作成");
    }
    return Device_.Get();
}

//@brief	DXGI取得関数
[[nodiscard]] const DXGI& Device::GetDXGI()const noexcept {
    if (!Device_) {
        assert(false && "DXGI未作成");
    }
    return DXGIInstance_;
}