
#include"RootSignature.h"
#include<cassert>

//@brief	ルートシグネチャー作成関数
[[nodiscard]] bool RootSignature::Create()noexcept {
    D3D12_DESCRIPTOR_RANGE textureDescriptorRange = {};
    textureDescriptorRange.NumDescriptors = 1;
    textureDescriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    textureDescriptorRange.BaseShaderRegister = 0;
    textureDescriptorRange.RegisterSpace = 0;
    textureDescriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    D3D12_DESCRIPTOR_RANGE constantBufferDescriptorRange = {};
    constantBufferDescriptorRange.NumDescriptors = 1;
    constantBufferDescriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    constantBufferDescriptorRange.BaseShaderRegister = 0;
    constantBufferDescriptorRange.RegisterSpace = 0;
    constantBufferDescriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    D3D12_ROOT_PARAMETER rootparam[2] = {};
    rootparam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootparam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
    rootparam[0].DescriptorTable.pDescriptorRanges = &textureDescriptorRange;
    rootparam[0].DescriptorTable.NumDescriptorRanges = 1;

    rootparam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootparam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
    rootparam[1].DescriptorTable.pDescriptorRanges = &constantBufferDescriptorRange;
    rootparam[1].DescriptorTable.NumDescriptorRanges = 1;

    D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
    samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    samplerDesc.Filter = D3D12_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR;
    samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
    samplerDesc.MinLOD = 0.0f;
    samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    samplerDesc.RegisterSpace = 0;
	
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
    rootSignatureDesc.NumParameters = 2;
    rootSignatureDesc.pParameters = rootparam;
    rootSignatureDesc.NumStaticSamplers = 1;
    rootSignatureDesc.pStaticSamplers = &samplerDesc;
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