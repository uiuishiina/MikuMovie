#pragma once
//---  RootSignature.h  ---
#include"Device.h"

class RootSignature final
{
public:
	RootSignature() = default;
	~RootSignature() = default;
	//@brief	ルートシグネチャー作成関数
	[[nodiscard]] bool Create()noexcept;
	//@brief	ルートシグネチャー取得関数
	[[nodiscard]] ID3D12RootSignature* Get()const noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D12RootSignature> Root_{};
};