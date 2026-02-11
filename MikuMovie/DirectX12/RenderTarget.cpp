#include"RenderTarget.h"
#include<cassert>

//@brief	レンダーターゲット作成関数
[[nodiscard]] bool RenderTarget::Create(const SwapChain& swapChain, const DescriptorHeap& heap)noexcept {
    // スワップチェインの設定を取得
    const auto& desc = swapChain.GetDesc();

    // レンダーターゲットリソースのサイズを設定
    Target_.resize(desc.BufferCount);

    // ディスクリプターヒープのハンドルを取得
    auto handle = heap.Get()->GetCPUDescriptorHandleForHeapStart();

    // ディスクリプターヒープのタイプを取得
    auto heapType = heap.GetType();

    // バックバッファの生成
    for (uint8_t i = 0; i < desc.BufferCount; ++i) {
        const auto hr = swapChain.Get()->GetBuffer(i, IID_PPV_ARGS(&Target_[i]));
        if (FAILED(hr)) {
            assert(false && "バックバッファの取得に失敗しました");
            return false;
        }

        // レンダーターゲットビューを作成してディスクリプタヒープのハンドルと関連付ける
        Device::Instance().GetDevice()->CreateRenderTargetView(Target_[i].Get(), nullptr, handle);

        // 次のハンドルへ移動
        handle.ptr += Device::Instance().GetDevice()->GetDescriptorHandleIncrementSize(heapType);
    }

    return true;
}

//@brief	レンダーターゲットハンドル取得関数
[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE RenderTarget::GetHandle(const DescriptorHeap& heap, UINT index) const noexcept {
    if (index >= Target_.size() || !Target_[index]) {
        assert(false && "不正なレンダーターゲットです");
    }
    // ディスクリプタヒープのハンドルを取得
    auto handle = heap.Get()->GetCPUDescriptorHandleForHeapStart();

    // ディスクリプタヒープのタイプを取得
    auto heapType = heap.GetType();
    assert(heapType == D3D12_DESCRIPTOR_HEAP_TYPE_RTV && "ディスクリプタヒープのタイプが RTV ではありません");

    // インデックスに応じてハンドルを移動
    handle.ptr += index * Device::Instance().GetDevice()->GetDescriptorHandleIncrementSize(heapType);
    return handle;
}

//@brief	レンダーターゲット取得関数
[[nodiscard]] ID3D12Resource* RenderTarget::Get(uint32_t index) const noexcept {
    if (index >= Target_.size() || !Target_[index]) {
        assert(false && "不正なレンダーターゲットです");
    }
    return Target_[index].Get();
}