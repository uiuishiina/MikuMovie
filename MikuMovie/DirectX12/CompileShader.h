#pragma once
//---  CompileShader.h  ---
#include "Device.h"
#include <string>

class CompileShader final
{
public:
	CompileShader() = default;
	~CompileShader() = default;
	//@brief	シェーダー作成関数
	[[nodiscard]] bool Create(const std::string path,const std::string entry, const std::string Version)noexcept;
	//@brief	シェーダー取得関数
	[[nodiscard]] ID3DBlob* GetShader()const noexcept;
private:
	Microsoft::WRL::ComPtr<ID3DBlob> Shader_{};
};