
#include"Texture.h"
#include<cassert>
#include <DirectXTex.h>
#pragma comment(lib, "DirectXTex.lib")


//@brief	テクスチャ作成関数
[[nodiscard]] bool Texture::Create(const DescriptorHeap& Heap)noexcept {

	//テクスチャデータ作成
	DirectX::TexMetadata metadata_{};
	DirectX::ScratchImage image_{};
	{
		const auto hr = DirectX::LoadFromWICFile(L"Data/test.jpg", DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &metadata_, image_);
		if (FAILED(hr)) {
			assert(false && "テクスチャデータ作成失敗");
			return false;
		}
	}
	
	D3D12_HEAP_PROPERTIES textureHeapprop = {};
	textureHeapprop.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	textureHeapprop.CreationNodeMask = 0;
	textureHeapprop.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Format = metadata_.format;
	resDesc.Width = metadata_.width;
	resDesc.Height = metadata_.height;
	resDesc.DepthOrArraySize = metadata_.arraySize;
	resDesc.SampleDesc.Count = 1;
	resDesc.SampleDesc.Quality = 0;
	resDesc.MipLevels = metadata_.mipLevels;
	resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata_.dimension);
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	{
		const auto hr= Device::Instance().GetDevice()->CreateCommittedResource(
			&textureHeapprop,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			nullptr,
			IID_PPV_ARGS(&TexData_)
		);
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

	srvDesc.Format = metadata_.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	Handle_ = Heap.Get()->GetCPUDescriptorHandleForHeapStart();
	Device::Instance().GetDevice()->CreateShaderResourceView(TexData_.Get(), &srvDesc, Handle_);

	return true;
}

//@brief	テクスチャ取得関数
[[nodiscard]] ID3D12Resource* Texture::Get()const noexcept {
	if (!TexData_) {
		assert(false && "テクスチャが作成されていません");
	}
	return TexData_.Get();
}

//@brief	ディスクリプタハンドル取得関数
[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE Texture::GetHandle()const noexcept {
	if (!TexData_) {
		assert(false && "テクスチャが作成されていません");
	}
	return Handle_;
}