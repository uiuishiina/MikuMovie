#pragma once
//---  CommandQueue.h  ---
#include "device.h"

class CommandQueue final
{
public:
	CommandQueue() = default;
	~CommandQueue() = default;
	//@brief	コマンドキュー作成関数
	[[nodiscard]] bool Create()noexcept;
	//@brief	コマンドキュー取得関数
	[[nodiscard]] ID3D12CommandQueue* Get()const noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> Queue_{};
};