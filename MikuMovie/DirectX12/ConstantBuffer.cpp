#include"ConstantBuffer.h"
#include<cassert>
using namespace DirectX;

//@brief	コンスタントバッファ作成関数
[[nodiscard]] bool ConstantBuffer::Create(const DescriptorHeap& Heap)noexcept {
    XMMATRIX matrix = XMMatrixIdentity();
    XMMATRIX worldMatrix = XMMatrixIdentity();

    XMFLOAT3 eye(0, 0, -5);
    XMFLOAT3 target(0, 0, 0);
    XMFLOAT3 up(0, 1, 0);

    XMMATRIX lookMatrix = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
    const auto [w,h] = Window::Instance().GetSize();
    XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV2, static_cast<float>(w) / static_cast<float>(h), 1.0f, 10.0f);

    matrix *= worldMatrix;
    matrix *= lookMatrix;
    matrix *= projectionMatrix;
    // アライメント済みサイズの計算
    const auto size = (sizeof(matrix) + 255) & ~255;

    // バッファリソースの作成
    D3D12_HEAP_PROPERTIES heapProps{};
    heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    D3D12_RESOURCE_DESC resourceDesc{};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Width = size;
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    const auto res = Device::Instance().GetDevice()->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&Buffer_));
    if (FAILED(res)) {
        assert(false && "コンスタントバッファの作成に失敗しました");
        return false;
    }

    XMMATRIX* mapMatrix;
    const auto hr = Buffer_->Map(0, nullptr, (void**)&mapMatrix);
    *mapMatrix = matrix;

    // コンスタントバッファビューの設定
    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
    cbvDesc.BufferLocation = Buffer_->GetGPUVirtualAddress();
    cbvDesc.SizeInBytes = size;

    // ディスクリプタのサイズを取得
    UINT cbvDescriptorSize = Device::Instance().GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    // ディスクリプタヒープの開始ハンドルを取得
    D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = Heap.Get()->GetCPUDescriptorHandleForHeapStart();
    cpuHandle.ptr += cbvDescriptorSize;
    // コンスタントバッファビューとハンドルを関連付ける
    Device::Instance().GetDevice()->CreateConstantBufferView(&cbvDesc, cpuHandle);

    // GPU 用ディスクリプタハンドルを保存
    Handle_ = Heap.Get()->GetGPUDescriptorHandleForHeapStart();
    // 指定されたインデックス分ハンドルを進める
    Handle_.ptr += cbvDescriptorSize;

    return true;
}

//@brief	コンスタントバッファ取得関数
[[nodiscard]] ID3D12Resource* ConstantBuffer::Get()const noexcept {
    return Buffer_.Get();
}

//@brief	ディスクリプタハンドル取得関数
[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE ConstantBuffer::GetHandle()const noexcept {
    return Handle_;
}