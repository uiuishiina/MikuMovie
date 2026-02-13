
#include"CompileShader.h"
#include<cassert>

#include <D3Dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

//@brief	
[[nodiscard]] bool CompileShader::Create(const std::string path, const std::string entry,const std::string Version)noexcept {
   
   // シェーダファイルのパス
    const std::string  filePath = path;
    const std::wstring temp = std::wstring(filePath.begin(), filePath.end());
    // シェーダのコンパイルエラーなどが分かる様にする
    ID3DBlob* error{};

    auto res = D3DCompileFromFile(temp.data(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry.c_str(), Version.c_str(), D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &Shader_, &error);
    if (FAILED(res)) {
        char* p = static_cast<char*>(error->GetBufferPointer());
        assert(false && "シェーダのコンパイルに失敗しました");
    }
    if (error) {
        error->Release();
    }
    return true;
}

//@brief	シェーダー取得関数
[[nodiscard]] ID3DBlob* CompileShader::GetShader()const noexcept {
    if (!Shader_) {
        assert(false && "シェーダが未作成です");
    }
    return Shader_.Get();
}