
//
#include"Window/Window.h"
#include"DirectX12/Device.h"
#include"DirectX12/CommandQueue.h"
#include"DirectX12/CommandAllocator.h"
#include"DirectX12/CommandList.h"
#include"DirectX12/SwapChain.h"
#include"DirectX12/DescriptorHeap.h"
#include"DirectX12/RenderTarget.h"
#include"DirectX12/Fence.h"
#include"DirectX12/DrawData.h"
#include"DirectX12/RootSignature.h"
#include"DirectX12/PiPlineState.h"
#include"DirectX12/Texture.h"


class MikuMovie {
public:
	MikuMovie() = default;
	~MikuMovie() = default;

	[[nodiscard]] bool CreateMovie(HINSTANCE hInstance)noexcept {
		//ウィンドウ作成
		if (!Window::Instance().Create(hInstance, 1280, 720)) {
			return false;
		}
		//デバイス作成
		if (!Device::Instance().Create()) {
			return false;
		}
		//キュー作成
		if (!Queue_.Create()) {
			return false;
		}
		//アロケーター作成
		if (!Allocator_[0].Create()) {
			return false;
		}
		if (!Allocator_[1].Create()) {
			return false;
		}
		//リスト作成
		if (!List_.Create(Allocator_[0])) {
			return false;
		}
		//スワップチェーン作成
		if (!Swap_.Create(Queue_)) {
			return false;
		}
		//ディスクリプタヒープ作成
		if (!TargetHeap_.Create(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, Swap_.GetDesc().BufferCount)) {
			return false;
		}
		//テクスチャ用
		if (!TexHeap_.Create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1,true)) {
			return false;
		}
		//レンダーターゲット作成
		if (!Target_.Create(Swap_, TargetHeap_)) {
			return false;
		}
		//フェンス作成
		if (!Fence_.Create()) {
			return false;
		}
		//データ作成
		if (!Data_.Create()) {
			return false;
		}
		//ルートシグネチャー作成
		if (!Root_.Create()) {
			return false;
		}
		//パイプライン作成
		if (!PiPline_.Create(Root_)) {
			return false;
		}
		if (!Texture_.Create(TexHeap_)) {
			return false;
		}

		return true;
	}

	void Loop() {
		while (Window::Instance().MessageLoop()) {
			// 現在のバックバッファインデックスを取得
			const auto backBufferIndex = Swap_.Get()->GetCurrentBackBufferIndex();

			// 以前のフレームの GPU の処理が完了しているか確認して待機する
			if (FrameValue_[backBufferIndex] != 0) {
				Fence_.Wait(FrameValue_[backBufferIndex]);
			}

			// コマンドアロケータリセット
			Allocator_[backBufferIndex].Reset();
			// コマンドリストリセット
			List_.Reset(Allocator_[backBufferIndex]);

			// リソースバリアでレンダーターゲットを Present から RenderTarget へ変更
			auto pToRT = Barrier(Target_.Get(backBufferIndex), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
			List_.Get()->ResourceBarrier(1, &pToRT);

			// レンダーターゲットの設定
			D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { Target_.GetHandle(TargetHeap_, backBufferIndex) };
			List_.Get()->OMSetRenderTargets(1, handles, false, nullptr);

			// レンダーターゲットのクリア
			const float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };  // 赤色でクリア
			List_.Get()->ClearRenderTargetView(handles[0], clearColor, 0, nullptr);
			{
				// パイプラインステートの設定
				List_.Get()->SetPipelineState(PiPline_.Get());
				// ルートシグネチャの設定
				List_.Get()->SetGraphicsRootSignature(Root_.Get());

				// ビューポートの設定
				const auto [w, h] = Window::Instance().GetSize();
				D3D12_VIEWPORT viewport{};
				viewport.TopLeftX = 0.0f;
				viewport.TopLeftY = 0.0f;
				viewport.Width = static_cast<float>(w);
				viewport.Height = static_cast<float>(h);
				viewport.MinDepth = 0.0f;
				viewport.MaxDepth = 1.0f;
				List_.Get()->RSSetViewports(1, &viewport);

				// シザー矩形の設定
				D3D12_RECT scissorRect{};
				scissorRect.left = 0;
				scissorRect.top = 0;
				scissorRect.right = w;
				scissorRect.bottom = h;
				List_.Get()->RSSetScissorRects(1, &scissorRect);

				ID3D12DescriptorHeap* p[] = { TexHeap_.Get() };
				List_.Get()->SetDescriptorHeaps(1, p);
				List_.Get()->SetGraphicsRootDescriptorTable(0, TexHeap_.Get()->GetGPUDescriptorHandleForHeapStart());

				Data_.Draw(List_);
			}

			// リソースバリアでレンダーターゲットを RenderTarget から Present へ変更
			auto rtToP = Barrier(Target_.Get(backBufferIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
			List_.Get()->ResourceBarrier(1, &rtToP);

			// コマンドリストをクローズ
			List_.Get()->Close();

			// コマンドキューにコマンドリストを送信
			ID3D12CommandList* ppCommandLists[] = { List_.Get() };
			Queue_.Get()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

			// プレゼント
			Swap_.Get()->Present(1, 0);

			// フェンスにフェンス値を設定
			Queue_.Get()->Signal(Fence_.Get(), NextValue_);
			FrameValue_[backBufferIndex] = NextValue_;
			NextValue_++;

		}
	}

	D3D12_RESOURCE_BARRIER Barrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to) noexcept {
		D3D12_RESOURCE_BARRIER barrier{};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = resource;
		barrier.Transition.StateBefore = from;
		barrier.Transition.StateAfter = to;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		return barrier;
	}

private:
	CommandQueue		Queue_{};
	CommandAllocator	Allocator_[2]{};
	CommandList			List_{};
	SwapChain			Swap_{};
	DescriptorHeap		TargetHeap_{};//レンダーターゲット用
	DescriptorHeap		TexHeap_{};//テクスチャ用
	RenderTarget		Target_{};
	DrawData			Data_{};
	RootSignature		Root_{};
	PiPlineState		PiPline_{};
	Texture				Texture_{};

	Fence				Fence_{};
	UINT64				FrameValue_[2]{};
	UINT64				NextValue_ = 1;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	MikuMovie Miku;

	if (!Miku.CreateMovie(hInstance)) {
		return 1;
	}
	Miku.Loop();
	return 0;
}