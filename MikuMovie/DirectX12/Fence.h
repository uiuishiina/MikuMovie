#pragma once
#include"Device.h"

class Fence final
{
public:
	Fence() = default;
	~Fence() = default;
	//@brief	フェンス作成関数
	[[nodiscard]] bool Create()noexcept;
	//[brief	フェンス取得関数
	[[nodiscard]] ID3D12Fence* Get()const noexcept;
	//@brief	フェンス待機関数
	void Wait(UINT64 value)const noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D12Fence> Fence_{};
	HANDLE Wait_{};
};