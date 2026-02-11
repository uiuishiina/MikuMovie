#pragma once
//---  CommandAllocator.h  ---
#include"Device.h"

class CommandAllocator final
{
public:
	CommandAllocator() = default;
	~CommandAllocator() = default;
	//@brief	アロケーター作成関数
	[[nodiscard]] bool Create()noexcept;
	//@brief	アロケーター取得関数
	[[nodiscard]] ID3D12CommandAllocator* Get()const noexcept;
	//@brief	リセット関数
	void Reset()const noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> Allocator_{};
};