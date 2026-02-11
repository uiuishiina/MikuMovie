#pragma once
//---  DrawData.h  ---
#include"Device.h"
#include"CommandList.h"

class DrawData final
{
public:
	DrawData() = default;
	~DrawData() = default;
	//@brief	データ作成関数
	[[nodiscard]] bool Create()noexcept;
	//@brief	描画コマンド
	void Draw(const CommandList& List)const noexcept;
private:
	//@brief	頂点作成関数
	[[nodiscard]] bool CreateVex()noexcept;
	//@brief	インデクス作成関数
	[[nodiscard]] bool CreateInd()noexcept;

	Microsoft::WRL::ComPtr<ID3D12Resource> VertexBuffer_{};  /// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexBuffer_{};   /// インデックスバッファ

	D3D12_VERTEX_BUFFER_VIEW VertexBufferView_ = {};
	D3D12_INDEX_BUFFER_VIEW  IndexBufferView_ = {};
};