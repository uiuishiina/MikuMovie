#pragma once
//---  Device.h  ---
#include"DXGI.h"

class Device final
{
public:
	//@brief	インスタンス取得関数
	static Device& Instance() noexcept{
		static Device instance;
		return instance;
	}
	//@brief	デバイス作成関数
	[[nodiscard]] bool Create()noexcept;
	//@brief	デバイス取得関数
	[[nodiscard]] ID3D12Device* GetDevice()const noexcept;
	//@brief	DXGI取得関数
	[[nodiscard]] const DXGI& GetDXGI()const noexcept;
private:
	Device() = default;
	~Device() = default;
	DXGI DXGIInstance_{};
	Microsoft::WRL::ComPtr<ID3D12Device> Device_{};
};