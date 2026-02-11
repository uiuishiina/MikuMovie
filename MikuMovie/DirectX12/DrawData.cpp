
#include"DrawData.h"
#include<cassert>
#include <DirectXMath.h>

namespace {
    struct Vertex {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 color;
    };
}

//@brief	データ作成関数
[[nodiscard]] bool DrawData::Create()noexcept {
    // 頂点バッファの生成
    if (!CreateVex()) {
        return false;
    }
    // インデックスバッファの生成
    if (!CreateInd()) {
        return false;
    }
    return true;
}

//@brief	描画コマンド
void DrawData::Draw(const CommandList& List)const noexcept {
    // 頂点バッファの設定
    List.Get()->IASetVertexBuffers(0, 1, &VertexBufferView_);
    // インデックスバッファの設定
    List.Get()->IASetIndexBuffer(&IndexBufferView_);
    // プリミティブ形状の設定（三角形）
    List.Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    // 描画コマンド
    List.Get()->DrawIndexedInstanced(3, 1, 0, 0, 0);
}

//@brief	頂点作成関数
[[nodiscard]] bool DrawData::CreateVex()noexcept {
    // 今回利用する三角形の頂点データ
    Vertex triangleVertices[] = {
        {  {0.0f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}, // 上頂点（赤色）
        { {0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, // 右下頂点（緑色）
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}  // 左下頂点（青色）
    };
    // 頂点データのサイズ
    const auto vertexBufferSize = sizeof(triangleVertices);
    // ヒープの設定を指定
    // CPU からアクセス可能なメモリを利用する為の設定
    D3D12_HEAP_PROPERTIES heapProperty{};
    heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
    heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProperty.CreationNodeMask = 1;
    heapProperty.VisibleNodeMask = 1;
    // どんなリソースを作成するかの設定
    D3D12_RESOURCE_DESC resourceDesc{};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Alignment = 0;
    resourceDesc.Width = vertexBufferSize;
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.SampleDesc.Quality = 0;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    // 頂点バッファの生成
    auto res = Device::Instance().GetDevice()->CreateCommittedResource(
        &heapProperty,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&VertexBuffer_));
    if (FAILED(res)) {
        assert(false && "頂点バッファの作成に失敗");
        return false;
    }

    Vertex* data{};
    res = VertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
    if (FAILED(res)) {
        assert(false && "頂点バッファのマップに失敗");
        return false;
    }
    memcpy_s(data, vertexBufferSize, triangleVertices, vertexBufferSize);
    VertexBuffer_->Unmap(0, nullptr);

    // 頂点バッファビューの設定
    VertexBufferView_.BufferLocation = VertexBuffer_->GetGPUVirtualAddress();  // 頂点バッファのアドレス
    VertexBufferView_.SizeInBytes = vertexBufferSize;                       // 頂点バッファのサイズ
    VertexBufferView_.StrideInBytes = sizeof(Vertex);                         // 1頂点あたりのサイズ
    return true;
}

//@brief	インデクス作成関数
[[nodiscard]] bool DrawData::CreateInd()noexcept {
    uint16_t triangleIndices[] = {
       0, 1, 2  // 三角形を構成する頂点のインデックス
    };
    // インデックスデータのサイズ
    const auto indexBufferSize = sizeof(triangleIndices);
    // ヒープの設定を指定
    D3D12_HEAP_PROPERTIES heapProperty{};
    heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
    heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProperty.CreationNodeMask = 1;
    heapProperty.VisibleNodeMask = 1;
    // リソースの設定を行う
    D3D12_RESOURCE_DESC resourceDesc{};
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDesc.Alignment = 0;
    resourceDesc.Width = indexBufferSize;
    resourceDesc.Height = 1;
    resourceDesc.DepthOrArraySize = 1;
    resourceDesc.MipLevels = 1;
    resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.SampleDesc.Quality = 0;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    // インデックスバッファの生成
    auto res = Device::Instance().GetDevice()->CreateCommittedResource(
        &heapProperty,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&IndexBuffer_));
    if (FAILED(res)) {
        assert(false && "インデックスバッファの作成に失敗");
        return false;
    }

    // インデックスバッファにデータを転送する
    uint16_t* data{};
    res = IndexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
    if (FAILED(res)) {
        assert(false && "インデックスバッファのマップに失敗");
        return false;
    }
    memcpy_s(data, indexBufferSize, triangleIndices, indexBufferSize);
    IndexBuffer_->Unmap(0, nullptr);
    // インデックスバッファビュー作成
    IndexBufferView_.BufferLocation = IndexBuffer_->GetGPUVirtualAddress();
    IndexBufferView_.SizeInBytes = indexBufferSize;
    IndexBufferView_.Format = DXGI_FORMAT_R16_UINT;  // triangleIndices の型が 16bit 符号なし整数なので R16_UINT
    return true;
}