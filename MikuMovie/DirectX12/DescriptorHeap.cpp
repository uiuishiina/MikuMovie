
#include"DescriptorHeap.h"
#include<cassert>

//@brief	
[[nodiscard]] bool DescriptorHeap::Create(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible)noexcept {
    // ヒープの設定
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    heapDesc.Type = type;
    heapDesc.NumDescriptors = numDescriptors;
    heapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    
    Type_ = type;

    // ディスクリプタヒープの生成
    HRESULT hr = Device::Instance().GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&Heap_));
    if (FAILED(hr)) {
        assert(false && "ディスクリプタヒープの生成に失敗しました");
        return false;
    }

    return true;
}

//@brief	
[[nodiscard]] ID3D12DescriptorHeap* DescriptorHeap::Get()const noexcept {
    if(!Heap_) {
        assert(false && "ディスクリプタヒープが未生成です");
        return nullptr;
    }
    return Heap_.Get();
}

//@brief	ディスクリプタタイプ取得
[[nodiscard]] D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeap::GetType()const noexcept {
    if (!Heap_) {
        assert(false && "ディスクリプタヒープが未生成です");
    }
    return Type_;
}