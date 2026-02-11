#include"CommandAllocator.h"
#include<cassert>

//@brief	アロケーター作成関数
[[nodiscard]] bool CommandAllocator::Create()noexcept {
    // コマンドアロケータの生成
    const auto hr = Device::Instance().GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&Allocator_));
    if (FAILED(hr)) {
        assert(false && "コマンドアロケータの作成に失敗しました");
        return false;
    }
    return true;
}

//@brief	アロケーター取得関数
[[nodiscard]] ID3D12CommandAllocator* CommandAllocator::Get()const noexcept {
    if (!Allocator_) {
        assert(false && "コマンドアロケータ未作成");
        return nullptr;
    }
    return Allocator_.Get();
}

//@brief	リセット関数
void CommandAllocator::Reset()const noexcept {
    if (!Allocator_) {
        assert(false && "コマンドアロケータが未作成です");
    }
    Allocator_->Reset();
}