#pragma once
//---  DescriptorHeap.h  ---
#include"Device.h"

class DescriptorHeap final
{
public:
	DescriptorHeap() = default;
	~DescriptorHeap() = default;
	//@brief	ディスクリプタ作成関数
	[[nodiscard]] bool Create(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible = false)noexcept;
	//@brief	ディスクリプタ取得関数
	[[nodiscard]] ID3D12DescriptorHeap* Get()const noexcept;
	//@brief	ディスクリプタタイプ取得
	[[nodiscard]] D3D12_DESCRIPTOR_HEAP_TYPE GetType()const noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> Heap_{};
	D3D12_DESCRIPTOR_HEAP_TYPE Type_{};
};