
#include"CommandQueue.h"
#include<cassert>

//@brief	コマンドキュー作成関数
[[nodiscard]] bool CommandQueue::Create()noexcept {
    D3D12_COMMAND_QUEUE_DESC desc{};
    desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;       // ダイレクトコマンドキュー
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;  // 通常優先度
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;        // 特別フラグなし
    desc.NodeMask = 0;                                    // GPU はひとつのみ使用する

    // コマンドキューの作成
    const auto hr = Device::Instance().GetDevice()->CreateCommandQueue(&desc, IID_PPV_ARGS(&Queue_));
    if (FAILED(hr)) {
        assert(false && "コマンドキューの作成に失敗");
        return false;
    }
    return true;
}

//@brief	コマンドキュー取得関数
[[nodiscard]] ID3D12CommandQueue* CommandQueue::Get()const noexcept {
    if (!Queue_) {
        assert(false && "コマンドキューが未作成です");
        return nullptr;
    }
    return Queue_.Get();
}