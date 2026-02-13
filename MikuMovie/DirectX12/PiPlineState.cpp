
#include"PiPlineState.h"
#include<cassert>


//@brief	パイプラインステート作成関数
[[nodiscard]] bool PiPlineState::Create(const RootSignature& rootSignature)noexcept {
    if (!Vs_.Create("Shader/VertexShader.hlsl", "VS","vs_5_0")) {
        return false;
    }
    if (!Ps_.Create("Shader/PixelShader.hlsl", "PS", "ps_5_0")) {
        return false;
    }

    // 頂点レイアウト
    // 頂点バッファのフォーマットに合わせて設定する
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {
        {   "POSITION", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        {   "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    };
    // ラスタライザステート
    // ポリゴンの塗りつぶし方法や裏面カリングの設定を行う
    D3D12_RASTERIZER_DESC rasterizerDesc{};
    rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
    rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
    rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    rasterizerDesc.DepthClipEnable = true;
    rasterizerDesc.MultisampleEnable = false;
    rasterizerDesc.AntialiasedLineEnable = false;
    rasterizerDesc.ForcedSampleCount = 0;
    rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    // ブレンドステート
    // 描画結果の合成方法を設定する
    const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc = {
        FALSE,
        FALSE,
        D3D12_BLEND_ONE,
        D3D12_BLEND_ZERO,
        D3D12_BLEND_OP_ADD,
        D3D12_BLEND_ONE,
        D3D12_BLEND_ZERO,
        D3D12_BLEND_OP_ADD,
        D3D12_LOGIC_OP_NOOP,
        D3D12_COLOR_WRITE_ENABLE_ALL,
    };
    D3D12_BLEND_DESC blendDesc{};
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.IndependentBlendEnable = false;
    for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
        blendDesc.RenderTarget[i] = defaultRenderTargetBlendDesc;
    }

    // パイプラインステート
    // 各種設定を構造体にまとめる
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
    psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
    psoDesc.pRootSignature = rootSignature.Get();
    psoDesc.VS = { Vs_.GetShader()->GetBufferPointer(), Vs_.GetShader()->GetBufferSize()};
    psoDesc.PS = { Ps_.GetShader()->GetBufferPointer(), Ps_.GetShader()->GetBufferSize() };
    psoDesc.RasterizerState = rasterizerDesc;
    psoDesc.BlendState = blendDesc;
    psoDesc.DepthStencilState.DepthEnable = false;
    psoDesc.DepthStencilState.StencilEnable = false;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;
    auto res = Device::Instance().GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&PiPline_));
    if (FAILED(res)) {
        assert(false && "パイプラインステートの作成に失敗");
    }

    return true;
}

//@brief	パイプラインステート取得関数
[[nodiscard]] ID3D12PipelineState* PiPlineState::Get()const noexcept {
    if (!PiPline_) {
        assert(false && "パイプラインステートが未作成です");
    }
    return PiPline_.Get();
}