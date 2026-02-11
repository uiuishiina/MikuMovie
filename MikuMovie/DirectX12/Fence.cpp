
#include"Fence.h"
#include<cassert>

//@brief	フェンス作成関数
[[nodiscard]] bool Fence::Create()noexcept {

	// フェンスの生成
	HRESULT hr = Device::Instance().GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence_));
	if (FAILED(hr)) {
		assert(false && "フェンスの作成に失敗しました");
		return false;
	}
	// GPU 同期用のイベントハンドルを作成
	Wait_ = CreateEvent(nullptr, false, false, "WAIT_GPU");
	if (!Wait_) {
		assert(false && "GPU 同期用のイベントハンドルの作成に失敗しました");
		return false;
	}
	return true;
}

//[brief	フェンス取得関数
[[nodiscard]] ID3D12Fence* Fence::Get()const noexcept {
	if (!Fence_) {
		assert(false && "フェンスが未作成です");
		return nullptr;
	}
	return Fence_.Get();
}

//@brief	フェンス待機関数
void Fence::Wait(UINT64 value)const noexcept {
	if (!Fence_) {
		assert(false && "フェンスが未作成です");
		return;
	}

	// フェンスの値が指定された値に達するまで待機
	if (Fence_->GetCompletedValue() < value) {
		// GPU がフェンス値に到達するまで待つ
		Fence_->SetEventOnCompletion(value, Wait_);
		WaitForSingleObject(Wait_, INFINITE);
	}
}