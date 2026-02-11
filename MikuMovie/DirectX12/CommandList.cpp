#include"CommandList.h"
#include<cassert>

//@brief	コマンドリスト作成関数
[[nodiscard]] bool CommandList::Create(const CommandAllocator& commandAllocator)noexcept {
    // コマンドリストの作成
    const auto hr = Device::Instance().GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&List_));
    if (FAILED(hr)) {
        assert(false && "コマンドリストの作成に失敗しました");
        return false;
    }
    // コマンドリストを初期化状態に設定
    List_->Close();
    return true;
}

//@brief	リセット関数
void CommandList::Reset(const CommandAllocator& commandAllocator) noexcept {
    if (!List_) {
        assert(false && "コマンドリストが未作成です");
    }
    // コマンドリストをリセット
    List_->Reset(commandAllocator.Get(), nullptr);
}

//@brief	コマンドリスト取得関数
[[nodiscard]] ID3D12GraphicsCommandList* CommandList::Get() const noexcept {
    if (!List_) {
        assert(false && "コマンドリストが未作成です");
    }
    return List_.Get();
}