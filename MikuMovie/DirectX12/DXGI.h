#pragma once
//---  DXGI.h  ---
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>

class DXGI final
{
public:
	DXGI() = default;
	~DXGI() = default;
	//@brief	DXGi作成関数
	[[nodiscard]] bool Create()noexcept;
	//@brief	ファクトリー取得関数
	[[nodiscard]] IDXGIFactory4* GetFactory() const noexcept;
	//@brief	アダプター取得関数
	[[nodiscard]] IDXGIAdapter1* GetAdapter() const noexcept;
private:
	Microsoft::WRL::ComPtr<IDXGIFactory4> Factory_{};  /// DXGIを作成するファクトリー
	Microsoft::WRL::ComPtr<IDXGIAdapter1> Adapter_{};  /// ディスプレイモード取得用アダプタ
};