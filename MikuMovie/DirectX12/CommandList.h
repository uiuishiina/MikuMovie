#pragma once
//---  CommandList.h  ---
#include"CommandAllocator.h"
#include"Device.h"

class CommandList final
{
public:
	CommandList() = default;
	~CommandList() = default;
	//@brief	コマンドリスト作成関数
	[[nodiscard]] bool Create(const CommandAllocator& commandAllocator)noexcept;
	//@brief	リセット関数
	void Reset(const CommandAllocator& commandAllocator) noexcept;
	//@brief	コマンドリスト取得関数
	[[nodiscard]] ID3D12GraphicsCommandList* Get() const noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> List_{};
};