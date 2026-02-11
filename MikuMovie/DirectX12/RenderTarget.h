#pragma once
//---  RenderTarget.h  ---
#include"Device.h"
#include"SwapChain.h"
#include"DescriptorHeap.h"
#include<vector>

class RenderTarget final
{
public:
	RenderTarget() = default;
	~RenderTarget() = default;
	//@brief	レンダーターゲット作成関数
	[[nodiscard]] bool Create(const SwapChain& swapChain, const DescriptorHeap& heap)noexcept;
	//@brief	レンダーターゲットハンドル取得関数
	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetHandle(const DescriptorHeap& heap, UINT index) const noexcept;
	//@brief	レンダーターゲット取得関数
	[[nodiscard]] ID3D12Resource* Get(uint32_t index) const noexcept;
private:
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> Target_{};
};