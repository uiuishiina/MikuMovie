#pragma once
//---  Texture.h  ---
#include"Device.h"
#include"DescriptorHeap.h"
#include<vector>

namespace {
	struct TexUV {
		unsigned char R, G, B, A;
	};
}

class Texture final
{
public:
	Texture() = default;
	~Texture() = default;
	//@brief	テクスチャ作成関数
	[[nodiscard]] bool Create(const DescriptorHeap& Heap)noexcept;
	//@brief	テクスチャ取得関数
	[[nodiscard]] ID3D12Resource* Get()const noexcept;
	//@brief	ディスクリプタハンドル取得関数
	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetHandle()const noexcept;
private:
	std::vector<TexUV> TextureUV_{ 256 * 256 };
	Microsoft::WRL::ComPtr<ID3D12Resource> TexData_{};
	D3D12_CPU_DESCRIPTOR_HANDLE Handle_{};
};