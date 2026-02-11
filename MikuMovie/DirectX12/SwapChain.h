#pragma once
//---  SwapChain.h  ---
#include"../Window/Window.h"
#include"Device.h"
#include"CommandQueue.h"

class SwapChain final
{
public:
	SwapChain() = default;
	~SwapChain() = default;
	//@brief	スワップチェーン作成関数
	[[nodiscard]] bool Create(const CommandQueue& commandQueue)noexcept;
	//@brief	スワップチェーン取得関数
	[[nodiscard]] IDXGISwapChain3* Get()const noexcept;
	//@brief	設定取得関数
	[[nodiscard]] const DXGI_SWAP_CHAIN_DESC1& GetDesc()const noexcept;
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain3> SwapChain_{};
	DXGI_SWAP_CHAIN_DESC1 Desc_{};
};