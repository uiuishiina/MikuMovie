#pragma once
//---  ConstantBuffer.h  ---
#include"../Window/Window.h"
#include"Device.h"
#include"DescriptorHeap.h"
#include<DirectXMath.h>

class ConstantBuffer final
{
public:
	ConstantBuffer() = default;
	~ConstantBuffer() = default;

	//@brief	コンスタントバッファ作成関数
	[[nodiscard]] bool Create(const DescriptorHeap& Heap)noexcept;
	//@brief	コンスタントバッファ取得関数
	[[nodiscard]] ID3D12Resource* Get()const noexcept;
	//@brief	ディスクリプタハンドル取得関数
	[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE GetHandle()const noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> Buffer_{};   /// コンスタントバッファ
	D3D12_GPU_DESCRIPTOR_HANDLE            Handle_{};
};