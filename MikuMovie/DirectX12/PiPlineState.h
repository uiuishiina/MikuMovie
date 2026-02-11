#pragma once
//---  PiPlineState.h  ---
#include"Device.h"
#include"CompileShader.h"
#include"RootSignature.h"
#include"DrawData.h"

class PiPlineState final
{
public:
	PiPlineState() = default;
	~PiPlineState() = default;
	//@brief	パイプラインステート作成関数
	[[nodiscard]] bool Create(const RootSignature& rootSignature)noexcept;
	//@brief	パイプラインステート取得関数
	[[nodiscard]] ID3D12PipelineState* Get()const noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> PiPline_{};
	CompileShader Vs_;
	CompileShader Ps_;
};