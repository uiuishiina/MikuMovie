
#include"RootSignature.h"
#include<cassert>

//@brief	ルートシグネチャー作成関数
[[nodiscard]] bool RootSignature::Create()noexcept {
	
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
    rootSignatureDesc.NumParameters = 0;
    rootSignatureDesc.pParameters = nullptr;
    rootSignatureDesc.NumStaticSamplers = 0;
    rootSignatureDesc.pStaticSamplers = nullptr;
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    // ルートシグネチャのシリアライズ
    ID3DBlob* signature{};
    auto      res = D3D12SerializeRootSignature(
        &rootSignatureDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        &signature,
        nullptr);
    bool success = SUCCEEDED(res);
    if (!success) {
        assert(false && "ルートシグネチャのシリアライズに失敗");
    }
    else {
        // ルートシグネチャの生成
        res = Device::Instance().GetDevice()->CreateRootSignature(
            0,
            signature->GetBufferPointer(),
            signature->GetBufferSize(),
            IID_PPV_ARGS(&Root_));
        success &= SUCCEEDED(res);
        if (!success) {
            assert(false && "ルートシグネチャの生成に失敗");
        }
    }
    if (signature) {
        signature->Release();
    }
    return success;
}

//@brief	ルートシグネチャー取得関数
[[nodiscard]] ID3D12RootSignature* RootSignature::Get()const noexcept {
    if (!Root_) {
        assert(false && "ルートシグネチャが生成されていません");
    }
    return Root_.Get();
}