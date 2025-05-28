/*
 *  Copyright 2019-2024 Diligent Graphics LLC
 *  Copyright 2015-2019 Egor Yusov
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  In no event and under no legal theory, whether in tort (including negligence),
 *  contract, or otherwise, unless required by applicable law (such as deliberate
 *  and grossly negligent acts) or agreed to in writing, shall any Contributor be
 *  liable for any damages, including any direct, indirect, special, incidental,
 *  or consequential damages of any character arising as a result of this License or
 *  out of the use or inability to use the software (including but not limited to damages
 *  for loss of goodwill, work stoppage, computer failure or malfunction, or any and
 *  all other commercial damages or losses), even if such Contributor has been advised
 *  of the possibility of such damages.
 */

#include <math.h>
#include <cmath>

#include "Tutorial11_ResourceUpdates.hpp"
#include "MapHelper.hpp"
#include "GraphicsUtilities.h"
#include "TextureUtilities.h"
#include "ColorConversion.h"
#include "imgui.h"

namespace Diligent
{

SampleBase* CreateSample()
{
    return new Tutorial11_ResourceUpdates();
}

namespace
{

// Layout of this structure matches the one we defined in the pipeline state

struct Vertex
{
    Diligent::float3 Pos;
    Diligent::float2 UV;
};

constexpr float A  = 0.15f; // #Lado
constexpr float Hc = 2.8f;  // #Altura
constexpr float Hb = 1.6f;  // #Base

// # Vertices del pasto
const Vertex CubeVerts[] =
    {

        {float3(-1, 0.00f, -1), float2(0, 1)}, // 0
        {float3(+1, 0.00f, -1), float2(1, 1)}, // 1
        {float3(+1, 0.00f, +1), float2(1, 0)}, // 2
        {float3(-1, 0.00f, +1), float2(0, 0)}, // 3


        {float3(-A, 0.00f, 0), float2(0, 1)}, // 4
        {float3(+A, 0.00f, 0), float2(1, 1)}, // 5
        {float3(+A, Hc, 0), float2(1, 0)},    // 6
        {float3(-A, Hc, 0), float2(0, 0)},    // 7

        {float3(0, 0.00f, -A), float2(0, 1)}, // 8
        {float3(0, 0.00f, +A), float2(1, 1)}, // 9
        {float3(0, Hc, +A), float2(1, 0)},    // 10
        {float3(0, Hc, -A), float2(0, 0)},    // 11

        {float3(-A, 0.00f, 0), float2(0, 1)}, // 12
        {float3(+A, 0.00f, 0), float2(1, 1)}, // 13
        {float3(+A, Hb, 0), float2(1, 0)},    // 14
        {float3(-A, Hb, 0), float2(0, 0)},    // 15

        {float3(0, 0.00f, -A), float2(0, 1)}, // 16
        {float3(0, 0.00f, +A), float2(1, 1)}, // 17
        {float3(0, Hb, +A), float2(1, 0)},    // 18
        {float3(0, Hb, -A), float2(0, 0)},    // 19

        {float3(+1.1f, 0.30f, 0.0f), float2(0, 1)}, // 20 
        {float3(+1.4f, 1.40f, 0.0f), float2(0, 0)}, // 21 

        {float3(-1.1f, 0.35f, 0.0f), float2(1, 1)}, // 22 
        {float3(-1.4f, 1.45f, 0.0f), float2(1, 0)}, // 23 

        {float3(0.0f, 0.30f, +1.1f), float2(0, 1)}, // 24 
        {float3(0.0f, 1.35f, +1.4f), float2(0, 0)}, // 25 

        {float3(0.0f, 0.35f, -1.1f), float2(1, 1)}, // 26 
        {float3(0.0f, 1.45f, -1.4f), float2(1, 0)}, // 27 
};
} 

// #Vertices del jugador
const Vertex SimpleCubeVerts[] =
    {
        {float3(-0.5f, -0.5f, 0.5f), float2(0, 1)},
        {float3(0.5f, -0.5f, 0.5f), float2(1, 1)},
        {float3(0.5f, 0.5f, 0.5f), float2(1, 0)},
        {float3(-0.5f, 0.5f, 0.5f), float2(0, 0)},

        {float3(-0.5f, -0.5f, -0.5f), float2(1, 1)},
        {float3(-0.5f, 0.5f, -0.5f), float2(1, 0)},
        {float3(0.5f, 0.5f, -0.5f), float2(0, 0)},
        {float3(0.5f, -0.5f, -0.5f), float2(0, 1)},

        {float3(-0.5f, 0.5f, -0.5f), float2(0, 1)},
        {float3(-0.5f, 0.5f, 0.5f), float2(0, 0)},
        {float3(0.5f, 0.5f, 0.5f), float2(1, 0)},
        {float3(0.5f, 0.5f, -0.5f), float2(1, 1)},

        {float3(-0.5f, -0.5f, -0.5f), float2(1, 0)},
        {float3(0.5f, -0.5f, -0.5f), float2(0, 0)},
        {float3(0.5f, -0.5f, 0.5f), float2(0, 1)},
        {float3(-0.5f, -0.5f, 0.5f), float2(1, 1)},

        {float3(0.5f, -0.5f, -0.5f), float2(1, 1)},
        {float3(0.5f, 0.5f, -0.5f), float2(1, 0)},
        {float3(0.5f, 0.5f, 0.5f), float2(0, 0)},
        {float3(0.5f, -0.5f, 0.5f), float2(0, 1)},

        {float3(-0.5f, -0.5f, -0.5f), float2(0, 1)},
        {float3(-0.5f, -0.5f, 0.5f), float2(1, 1)},
        {float3(-0.5f, 0.5f, 0.5f), float2(1, 0)},
        {float3(-0.5f, 0.5f, -0.5f), float2(0, 0)}};

// #Indices del jugador
const Uint32 SimpleCubeIndices[] =
    {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20};

// #Piso
const Vertex GroundPlaneVerts[] =
    {
        {float3(-60.f, -0.30f, -60.f), float2(0.0f, 0.0f)},
        {float3(60.f, -0.30f, -60.f), float2(1.0f, 0.0f)},
        {float3(60.f, -0.30f, 60.f), float2(1.0f, 1.0f)},
        {float3(-60.f, -0.30f, 60.f), float2(0.0f, 1.0f)}};

const Uint32 GroundPlaneIndices[] =
    {
        0, 2, 1, 0, 3, 2  
};

void Tutorial11_ResourceUpdates::CreatePipelineStates()
{
    // Pipeline state object encompasses configuration of all GPU stages

    GraphicsPipelineStateCreateInfo PSOCreateInfo;

    // Pipeline state name is used by the engine to report issues.
    // It is always a good idea to give objects descriptive names.
    PSOCreateInfo.PSODesc.Name = "Cube PSO";

    // This is a graphics pipeline
    PSOCreateInfo.PSODesc.PipelineType = PIPELINE_TYPE_GRAPHICS;

    // clang-format off
    // This tutorial will render to a single render target
    PSOCreateInfo.GraphicsPipeline.NumRenderTargets             = 1;
    // Set render target format which is the format of the swap chain's color buffer
    PSOCreateInfo.GraphicsPipeline.RTVFormats[0]                = m_pSwapChain->GetDesc().ColorBufferFormat;
    // Set depth buffer format which is the format of the swap chain's back buffer
    PSOCreateInfo.GraphicsPipeline.DSVFormat                    = m_pSwapChain->GetDesc().DepthBufferFormat;
    // Primitive topology defines what kind of primitives will be rendered by this pipeline state
    PSOCreateInfo.GraphicsPipeline.PrimitiveTopology            = PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    // Cull back faces
    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode      = CULL_MODE_BACK;
    // Enable depth testing
    PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = True;
    // clang-format on

    ShaderCreateInfo ShaderCI;
    // Tell the system that the shader source code is in HLSL.
    // For OpenGL, the engine will convert this into GLSL under the hood.
    ShaderCI.SourceLanguage = SHADER_SOURCE_LANGUAGE_HLSL;

    // OpenGL backend requires emulated combined HLSL texture samplers (g_Texture + g_Texture_sampler combination)
    ShaderCI.Desc.UseCombinedTextureSamplers = true;

    // Pack matrices in row-major order
    ShaderCI.CompileFlags = SHADER_COMPILE_FLAG_PACK_MATRIX_ROW_MAJOR;

    // Presentation engine always expects input in gamma space. Normally, pixel shader output is
    // converted from linear to gamma space by the GPU. However, some platforms (e.g. Android in GLES mode,
    // or Emscripten in WebGL mode) do not support gamma-correction. In this case the application
    // has to do the conversion manually.
    ShaderMacro Macros[] = {{"CONVERT_PS_OUTPUT_TO_GAMMA", m_ConvertPSOutputToGamma ? "1" : "0"}};
    ShaderCI.Macros      = {Macros, _countof(Macros)};

    // Create a shader source stream factory to load shaders from files.
    RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderSourceFactory;
    m_pEngineFactory->CreateDefaultShaderSourceStreamFactory(nullptr, &pShaderSourceFactory);
    ShaderCI.pShaderSourceStreamFactory = pShaderSourceFactory;
    // Create a vertex shader
    RefCntAutoPtr<IShader> pVS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_VERTEX;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.Desc.Name       = "Cube VS";
        ShaderCI.FilePath        = "cube.vsh";
        m_pDevice->CreateShader(ShaderCI, &pVS);
        // Create dynamic uniform buffer that will store our transformation matrix
        // Dynamic buffers can be frequently updated by the CPU
        CreateUniformBuffer(m_pDevice, sizeof(float4x4), "VS constants CB", &m_VSConstants);
    }

    // Create a pixel shader
    RefCntAutoPtr<IShader> pPS;
    {
        ShaderCI.Desc.ShaderType = SHADER_TYPE_PIXEL;
        ShaderCI.EntryPoint      = "main";
        ShaderCI.Desc.Name       = "Cube PS";
        ShaderCI.FilePath        = "cube.psh";
        m_pDevice->CreateShader(ShaderCI, &pPS);
    }

    // clang-format off
    // Define vertex shader input layout
    LayoutElement LayoutElems[] =
    {
        // Attribute 0 - vertex position
        LayoutElement{0, 0, 3, VT_FLOAT32, False},
        // Attribute 1 - texture coordinates
        LayoutElement{1, 0, 2, VT_FLOAT32, False}
    };
    // clang-format on

    PSOCreateInfo.pVS = pVS;
    PSOCreateInfo.pPS = pPS;

    PSOCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = LayoutElems;
    PSOCreateInfo.GraphicsPipeline.InputLayout.NumElements    = _countof(LayoutElems);

    // Define variable type that will be used by default
    PSOCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = SHADER_RESOURCE_VARIABLE_TYPE_STATIC;

    // clang-format off
    // Shader variables should typically be mutable, which means they are expected
    // to change on a per-instance basis
    ShaderResourceVariableDesc Vars[] = 
    {
        {SHADER_TYPE_PIXEL, "g_Texture", SHADER_RESOURCE_VARIABLE_TYPE_MUTABLE}
    };
    PSOCreateInfo.PSODesc.ResourceLayout.Variables    = Vars;
    PSOCreateInfo.PSODesc.ResourceLayout.NumVariables = _countof(Vars);

    // Define immutable sampler for g_Texture. Immutable samplers should be used whenever possible
    SamplerDesc SamLinearClampDesc
    {
        FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, FILTER_TYPE_LINEAR, 
        TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP, TEXTURE_ADDRESS_CLAMP
    };
    ImmutableSamplerDesc ImtblSamplers[] = 
    {
        {SHADER_TYPE_PIXEL, "g_Texture", SamLinearClampDesc}
    };
    // clang-format on
    PSOCreateInfo.PSODesc.ResourceLayout.ImmutableSamplers    = ImtblSamplers;
    PSOCreateInfo.PSODesc.ResourceLayout.NumImmutableSamplers = _countof(ImtblSamplers);
    m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO);

    // Since we did not explicitly specify the type for 'Constants' variable, default
    // type (SHADER_RESOURCE_VARIABLE_TYPE_STATIC) will be used. Static variables never
    // change and are bound directly to the pipeline state object.
    m_pPSO->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(m_VSConstants);

    PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = CULL_MODE_NONE;
    m_pDevice->CreateGraphicsPipelineState(PSOCreateInfo, &m_pPSO_NoCull);
    m_pPSO_NoCull->GetStaticVariableByName(SHADER_TYPE_VERTEX, "Constants")->Set(m_VSConstants);
}

void Tutorial11_ResourceUpdates::CreateVertexBuffers()
{
    for (Uint32 i = 0; i < _countof(m_CubeVertexBuffer); ++i)
    {
        auto& VertexBuffer = m_CubeVertexBuffer[i];

        // Create vertex buffer that stores cube vertices
        BufferDesc VertBuffDesc;
        VertBuffDesc.Name = "Cube vertex buffer";
        if (i == 0)
            VertBuffDesc.Usage = USAGE_IMMUTABLE;
        else if (i == 1)
            VertBuffDesc.Usage = USAGE_DEFAULT;
        else
        {
            VertBuffDesc.Usage          = USAGE_DYNAMIC;
            VertBuffDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
        }

        VertBuffDesc.BindFlags = BIND_VERTEX_BUFFER;
        VertBuffDesc.Size      = sizeof(CubeVerts);
        BufferData VBData;
        VBData.pData    = CubeVerts;
        VBData.DataSize = sizeof(CubeVerts);
        m_pDevice->CreateBuffer(VertBuffDesc, i < 2 ? &VBData : nullptr, &VertexBuffer);
    }
}

void Tutorial11_ResourceUpdates::CreateIndexBuffer()
{
    // #Indices del pasto
    const Uint32 Indices[] =
    {
         4,  5,  6,   4,  6,  7,
         8,  9, 10,   8, 10, 11,
        12, 13, 14,  12, 14, 15,
        16, 17, 18,  16, 18, 19,
        20, 21,  5,   20,  5,  4,   
        22, 23,  8,   22,  8,  9,
        24, 25,  4,   24,  4,  8,
        26, 27, 13,   26, 13, 17
    };
    // clang-format on

    // Create index buffer
    BufferDesc IndBuffDesc;
    IndBuffDesc.Name      = "Cube index buffer";
    IndBuffDesc.Usage     = USAGE_IMMUTABLE;
    IndBuffDesc.BindFlags = BIND_INDEX_BUFFER;
    IndBuffDesc.Size      = sizeof(Indices);
    BufferData IBData;
    IBData.pData    = Indices;
    IBData.DataSize = sizeof(Indices);
    m_pDevice->CreateBuffer(IndBuffDesc, &IBData, &m_CubeIndexBuffer);
}


// #Creacion del vertex e index buffer para el jugador
void Tutorial11_ResourceUpdates::CreatePlayerCube()
{
    BufferDesc VertBuffDesc;
    VertBuffDesc.Name      = "Player cube vertex buffer";
    VertBuffDesc.Usage     = USAGE_IMMUTABLE;
    VertBuffDesc.BindFlags = BIND_VERTEX_BUFFER;
    VertBuffDesc.Size      = sizeof(SimpleCubeVerts);
    BufferData VBData;
    VBData.pData    = SimpleCubeVerts;
    VBData.DataSize = sizeof(SimpleCubeVerts);
    m_pDevice->CreateBuffer(VertBuffDesc, &VBData, &m_PlayerCubeVertexBuffer);

    BufferDesc IndexBuffDesc;
    IndexBuffDesc.Name      = "Player cube index buffer";
    IndexBuffDesc.Usage     = USAGE_IMMUTABLE;
    IndexBuffDesc.BindFlags = BIND_INDEX_BUFFER;
    IndexBuffDesc.Size      = sizeof(SimpleCubeIndices);
    BufferData IBData;
    IBData.pData    = SimpleCubeIndices;
    IBData.DataSize = sizeof(SimpleCubeIndices);
    m_pDevice->CreateBuffer(IndexBuffDesc, &IBData, &m_PlayerCubeIndexBuffer);
}

// #Dibuja el cubo del jugador
void Tutorial11_ResourceUpdates::DrawPlayerCube(const float4x4& WVPMatrix, IShaderResourceBinding* pSRB)
{
    // Bind vertex buffer
    IBuffer* pBuffs[] = {m_PlayerCubeVertexBuffer};
    m_pImmediateContext->SetVertexBuffers(0, 1, pBuffs, nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
    m_pImmediateContext->SetIndexBuffer(m_PlayerCubeIndexBuffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    // Commit shader resources
    m_pImmediateContext->CommitShaderResources(pSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    {
        // #Informacion del mapaaa
        MapHelper<float4x4> CBConstants(m_pImmediateContext, m_VSConstants, MAP_WRITE, MAP_FLAG_DISCARD);
        *CBConstants = WVPMatrix;
    }

    DrawIndexedAttribs DrawAttrs;
    DrawAttrs.IndexType  = VT_UINT32;
    DrawAttrs.NumIndices = 36; 
    DrawAttrs.Flags      = DRAW_FLAG_VERIFY_ALL;
    m_pImmediateContext->DrawIndexed(DrawAttrs);
}

// #Lo mismo pero para el piso
void Tutorial11_ResourceUpdates::CreateGroundPlane()
{
    // Create ground plane vertex buffer
    BufferDesc VertBuffDesc;
    VertBuffDesc.Name      = "Ground plane vertex buffer";
    VertBuffDesc.Usage     = USAGE_IMMUTABLE;
    VertBuffDesc.BindFlags = BIND_VERTEX_BUFFER;
    VertBuffDesc.Size      = sizeof(GroundPlaneVerts);
    BufferData VBData;
    VBData.pData    = GroundPlaneVerts;
    VBData.DataSize = sizeof(GroundPlaneVerts);
    m_pDevice->CreateBuffer(VertBuffDesc, &VBData, &m_GroundPlaneVertexBuffer);

    // Create ground plane index buffer
    BufferDesc IndexBuffDesc;
    IndexBuffDesc.Name      = "Ground plane index buffer";
    IndexBuffDesc.Usage     = USAGE_IMMUTABLE;
    IndexBuffDesc.BindFlags = BIND_INDEX_BUFFER;
    IndexBuffDesc.Size      = sizeof(GroundPlaneIndices);
    BufferData IBData;
    IBData.pData    = GroundPlaneIndices;
    IBData.DataSize = sizeof(GroundPlaneIndices);
    m_pDevice->CreateBuffer(IndexBuffDesc, &IBData, &m_GroundPlaneIndexBuffer);
}

void Tutorial11_ResourceUpdates::DrawGroundPlane(const float4x4& WVPMatrix, IShaderResourceBinding* pSRB)
{
    // Bind vertex and index buffers
    IBuffer* pBuffs[] = {m_GroundPlaneVertexBuffer};
    m_pImmediateContext->SetVertexBuffers(0, 1, pBuffs, nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
    m_pImmediateContext->SetIndexBuffer(m_GroundPlaneIndexBuffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    // Commit shader resources
    m_pImmediateContext->CommitShaderResources(pSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    {
        // Map the buffer and write current world-view-projection matrix
        MapHelper<float4x4> CBConstants(m_pImmediateContext, m_VSConstants, MAP_WRITE, MAP_FLAG_DISCARD);
        *CBConstants = WVPMatrix;
    }

    DrawIndexedAttribs DrawAttrs;
    DrawAttrs.IndexType  = VT_UINT32;
    DrawAttrs.NumIndices = 6; // 2 triangles, 3 vertices each
    DrawAttrs.Flags      = DRAW_FLAG_VERIFY_ALL;
    m_pImmediateContext->DrawIndexed(DrawAttrs);
}

void Tutorial11_ResourceUpdates::LoadTextures()
{
    for (size_t i = 0; i < m_Textures.size(); ++i)
    {
        // Load texture
        TextureLoadInfo   loadInfo;
        std::stringstream FileNameSS;
        FileNameSS << "grass" << i << ".png";
        auto FileName   = FileNameSS.str();
        loadInfo.IsSRGB = true;
        loadInfo.Usage  = USAGE_IMMUTABLE;
        if (i == 2)
        {
            loadInfo.Usage = USAGE_DEFAULT;
            // Disable mipmapping for simplicity as we will only update mip level 0
            loadInfo.MipLevels = 1;
        }
        else if (i == 3)
        {
            // Disable mipmapping
            loadInfo.MipLevels      = 1;
            loadInfo.Usage          = USAGE_DYNAMIC;
            loadInfo.CPUAccessFlags = CPU_ACCESS_WRITE;
        }

        auto& Tex = m_Textures[i];
        CreateTextureFromFile(FileName.c_str(), loadInfo, m_pDevice, &Tex);
        // Get shader resource view from the texture
        auto TextureSRV = Tex->GetDefaultView(TEXTURE_VIEW_SHADER_RESOURCE);

        // Since we are using mutable variable, we must create shader resource binding object
        // http://diligentgraphics.com/2016/03/23/resource-binding-model-in-diligent-engine-2-0/
        m_pPSO->CreateShaderResourceBinding(&(m_SRBs[i]), true);
        // Set texture SRV in the SRB
        m_SRBs[i]->GetVariableByName(SHADER_TYPE_PIXEL, "g_Texture")->Set(TextureSRV);
    }
}


void Tutorial11_ResourceUpdates::Initialize(const SampleInitInfo& InitInfo)
{
    SampleBase::Initialize(InitInfo);

    CreatePipelineStates();
    CreateVertexBuffers();
    CreateIndexBuffer();
    //# Crea el jugador y piso
    CreatePlayerCube();
    CreateGroundPlane();
    LoadTextures();

    {
        BufferDesc VertBuffDesc;
        VertBuffDesc.Name           = "Texture update buffer";
        VertBuffDesc.Usage          = USAGE_DYNAMIC;
        VertBuffDesc.BindFlags      = BIND_VERTEX_BUFFER; // We do not really bind the buffer, but D3D11 wants at least one bind flag bit
        VertBuffDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
        VertBuffDesc.Size           = MaxUpdateRegionSize * MaxUpdateRegionSize * 4;
        m_pDevice->CreateBuffer(VertBuffDesc, nullptr, &m_TextureUpdateBuffer);
    }

    // #Infor necesarioa para deformar
    constexpr int GRID = 50;
    m_GrassDeformed.resize(GRID * GRID, false);
    m_GrassDeformX.resize(GRID * GRID, 0.0f);
    m_GrassDeformZ.resize(GRID * GRID, 0.0f);
}

void Tutorial11_ResourceUpdates::DrawCube(const float4x4& WVPMatrix, Diligent::IBuffer* pVertexBuffer, Diligent::IShaderResourceBinding* pSRB)
{
    // Bind vertex buffer
    IBuffer* pBuffs[] = {pVertexBuffer};
    m_pImmediateContext->SetVertexBuffers(0, 1, pBuffs, nullptr, RESOURCE_STATE_TRANSITION_MODE_TRANSITION, SET_VERTEX_BUFFERS_FLAG_RESET);
    m_pImmediateContext->SetIndexBuffer(m_CubeIndexBuffer, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    // Commit shader resources. RESOURCE_STATE_TRANSITION_MODE_TRANSITION mode
    // makes sure that resources are transitioned to required states.
    m_pImmediateContext->CommitShaderResources(pSRB, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    {
        // Map the buffer and write current world-view-projection matrix
        MapHelper<float4x4> CBConstants(m_pImmediateContext, m_VSConstants, MAP_WRITE, MAP_FLAG_DISCARD);
        *CBConstants = WVPMatrix;
    }

    DrawIndexedAttribs DrawAttrs;     // This is an indexed draw call
    DrawAttrs.IndexType  = VT_UINT32; // Index type
    DrawAttrs.NumIndices = 45;        // Updated for the new triangles (16 triangles * 3 vertices)
    // Verify the state of vertex and index buffers
    DrawAttrs.Flags = DRAW_FLAG_VERIFY_ALL;
    m_pImmediateContext->DrawIndexed(DrawAttrs);
}

// #Matriz para facilitar la vista (antes la usaba para la camara que se movia pero eso se quito)
static Diligent::float4x4 MakeViewMatrix(const Diligent::float3& eye,
                                         const Diligent::float3& target,
                                         const Diligent::float3& up)
{
    using namespace Diligent;
    float3 z = normalize(target - eye);
    float3 x = normalize(cross(up, z));
    float3 y = cross(z, x);

    return float4x4{
        x.x, y.x, z.x, 0,
        x.y, y.y, z.y, 0,
        x.z, y.z, z.z, 0,
        -dot(x, eye),
        -dot(y, eye),
        -dot(z, eye),
        1};
}

// #Adicional para direccionar
static Diligent::float2 DirAwayFrom(const Diligent::float3& player,
                                    const Diligent::float3& tuft)
{
    float dx = tuft.x - player.x;
    float dz = tuft.z - player.z;
    float d2 = dx * dx + dz * dz;
    if (d2 < 1e-6f)
        return {0, 0};
    float invLen = 1.0f / std::sqrt(d2);
    return {dx * invLen, dz * invLen};
}


void Tutorial11_ResourceUpdates::Render()
{
    auto*  pRTV       = m_pSwapChain->GetCurrentBackBufferRTV();
    auto*  pDSV       = m_pSwapChain->GetDepthBufferDSV();
    float4 ClearColor = {0.35f, 0.35f, 0.35f, 1.0f};
    if (m_ConvertPSOutputToGamma)
        ClearColor = LinearToSRGB(ClearColor);
    m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor.Data(), RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

    m_pImmediateContext->SetPipelineState(m_pPSO);

    // #CVariables varias, para camara, movimiento, tracking, etc
    auto SrfPre = GetSurfacePretransformMatrix({0, 0, 1});
    auto Proj   = GetAdjustedProjectionMatrix(PI_F / 5.f, 0.1f, 200.f);

    constexpr float DEG2RAD = PI_F / 180.f;
    float           pitch   = -42.f * DEG2RAD;
    float           yaw     = 180.f * DEG2RAD;
    float3          eye     = {0.f, 29.f, 32.f};

    float  cp = std::cos(pitch), sp = std::sin(pitch);
    float  cy = std::cos(yaw), sy = std::sin(yaw);
    float3 fwd = {sy * cp, sp, cy * cp};

    auto View     = MakeViewMatrix(eye, eye + fwd, {0, 1, 0});
    auto ViewProj = SrfPre * View * Proj;


    constexpr int   GRID = 50;
    constexpr float STEP = 1.4f;
    constexpr float HALF = STEP * (GRID - 1) * 0.5f;

    constexpr float RADIUS   = 3.4f;  
    constexpr float MAX_BEND = 0.45f; 

    m_pImmediateContext->SetPipelineState(m_pPSO_NoCull);
    float4x4 GroundWorld = float4x4::Identity();
    DrawGroundPlane(GroundWorld * ViewProj, m_SRBs[1]);

    float3 vel    = m_PlayerVel;
    float  velLen = std::sqrt(vel.x * vel.x + vel.z * vel.z);
    if (velLen > 1e-4f)
        vel /= velLen;

    const float VELOCITY_INFLUENCE = 2.0f;
    const float POSITION_INFLUENCE = 4.5f;

    // #Generacion del grid de pasto
    for (int gz = 0; gz < GRID; ++gz)
    {
        for (int gx = 0; gx < GRID; ++gx)
        {
            float xPos = -HALF + gx * STEP;
            float zPos = -HALF + gz * STEP;

            float dx = xPos - m_PlayerX;
            float dz = zPos - m_PlayerZ;
            float d2 = dx * dx + dz * dz;

            float bendX = 0.f, bendZ = 0.f;

            if (d2 < RADIUS * RADIUS)
            {
                float dist = std::sqrt(d2);
                float w    = 1.f - dist / RADIUS;
                w          = w * w; 

                float inv = dist > 1e-4f ? 1.f / dist : 0.f;
                float ux  = dx * inv;
                float uz  = dz * inv;

                // #Bend dependiendo de la pos
                float posBendX = uz * MAX_BEND * w * POSITION_INFLUENCE;
                float posBendZ = -ux * MAX_BEND * w * POSITION_INFLUENCE;

                // #Velocidad
                float velBendX = -vel.z * MAX_BEND * w * VELOCITY_INFLUENCE;
                float velBendZ = vel.x * MAX_BEND * w * VELOCITY_INFLUENCE;

                bendX = posBendX + velBendX;
                bendZ = posBendZ + velBendZ;
            }

            float4x4 World = float4x4::RotationX(bendX) *
                float4x4::RotationZ(bendZ) *
                float4x4::Translation(xPos, 0.f, zPos);

            DrawCube(World * ViewProj,
                     m_CubeVertexBuffer[2],
                     m_SRBs[1]);
        }
    }

    float4x4 PlayerWorld = float4x4::Translation(m_PlayerX, 1.0f, m_PlayerZ);
    PlayerWorld *= float4x4::Scale(1.5f, 1.5f, 1.5f);
    DrawPlayerCube(PlayerWorld * ViewProj, m_SRBs[0]);
}

void Tutorial11_ResourceUpdates::UpdateBuffer(Diligent::Uint32 BufferIndex)
{
    Uint32 NumVertsToUpdate  = std::uniform_int_distribution<Uint32>{2, 5}(m_gen);
    Uint32 FirstVertToUpdate = std::uniform_int_distribution<Uint32>{0, static_cast<Uint32>(28) - NumVertsToUpdate}(m_gen);
    Vertex Vertices[28];
    for (Uint32 v = 0; v < NumVertsToUpdate; ++v)
    {
        auto        SrcInd  = FirstVertToUpdate + v;
        const auto& SrcVert = CubeVerts[SrcInd];
        Vertices[v].UV      = SrcVert.UV;
        Vertices[v].Pos     = SrcVert.Pos * static_cast<float>(1 + 0.2 * sin(m_CurrTime * (1.0 + SrcInd * 0.2)));
    }
    m_pImmediateContext->UpdateBuffer(
        m_CubeVertexBuffer[BufferIndex],    // Device context to use for the operation
        FirstVertToUpdate * sizeof(Vertex), // Start offset in bytes
        NumVertsToUpdate * sizeof(Vertex),  // Data size in bytes
        Vertices,                           // Data pointer
        RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
}

// #FUncion para calcular la velocidad y tener informacion relevante
void Tutorial11_ResourceUpdates::UpdatePlayerVelocity(float dt)
{
    float vx = (m_PlayerX - m_PrevPlayerX) / dt;
    float vz = (m_PlayerZ - m_PrevPlayerZ) / dt;

    const float SMOOTH = 10.0f; 

    const float threshold = 0.2f;
    if (std::abs(vx) > threshold || std::abs(vz) > threshold)
    {
        m_PlayerVel.x = vx * 0.7f + m_PlayerVel.x * 0.3f;
        m_PlayerVel.z = vz * 0.7f + m_PlayerVel.z * 0.3f;
    }
    else
    {
        m_PlayerVel.x += (vx - m_PlayerVel.x) * (1.f - expf(-SMOOTH * dt));
        m_PlayerVel.z += (vz - m_PlayerVel.z) * (1.f - expf(-SMOOTH * dt));
    }

    if (std::abs(vx) < 0.01f && std::abs(vz) < 0.01f)
    {
        m_PlayerVel.x *= 0.85f;
        m_PlayerVel.z *= 0.85f;
    }
}

// #Mapea el buffer dinamico y actualiza las posiciones de los vertices
constexpr float kIdleF = 1.7f;  
constexpr float kIdleA  = 1.35f; 
void Tutorial11_ResourceUpdates::MapDynamicBuffer(Diligent::Uint32 BufferIndex)
{
    MapHelper<Vertex> Vtx(m_pImmediateContext,
                          m_CubeVertexBuffer[BufferIndex],
                          MAP_WRITE,
                          MAP_FLAG_DISCARD);

    for (Uint32 v = 0; v < _countof(CubeVerts); ++v)
    {
        const auto& src = CubeVerts[v];
        Vtx[v].UV       = src.UV;

        if (v < 4)
        {
            Vtx[v].Pos = src.Pos;
            continue;
        }

        float phase        = 0.30f * v;
        float baseAmp      = 0.06f;
        float heightFactor = src.Pos.y / 3.0f; 
        float amp          = baseAmp * (heightFactor * 1.5f + 0.1f);

        if (v >= 4 && v <= 11) amp *= 1.3f;

        float disp = (amp * kIdleA) *
            std::sin(m_CurrTime * 0.8f * kIdleF + phase);

        float tilt  = src.Pos.y * 0.02f; 
        float swayX = 0, swayY = 0, swayZ = 0;

        if (m_MovementDirection == 0) 
        {
            swayX = disp;
            swayY = -disp * tilt;
        }
        else 
        {
            swayZ = disp;
            swayY = -disp * tilt;
        }

        Vtx[v].Pos = float3{src.Pos.x + swayX,
                            src.Pos.y + swayY,
                            src.Pos.z + swayZ};
    }
}



void Tutorial11_ResourceUpdates::Update(double CurrTime, double ElapsedTime, bool DoUpdateUI)
{
    SampleBase::Update(CurrTime, ElapsedTime, DoUpdateUI);

    m_CurrTime = CurrTime;

    m_PrevPlayerX = m_PlayerX;
    m_PrevPlayerZ = m_PlayerZ;

    // #Velocidad del jugador
    float moveSpeed = 2.0f * static_cast<float>(ElapsedTime);

    const auto& inputController = GetInputController();

    // #Inputs del jugador
    if (inputController.IsKeyDown(InputKeys::MoveForward))
        m_PlayerZ -= moveSpeed;
    if (inputController.IsKeyDown(InputKeys::MoveBackward))
        m_PlayerZ += moveSpeed;

    if (inputController.IsKeyDown(InputKeys::MoveRight))
        m_PlayerX -= moveSpeed;
    if (inputController.IsKeyDown(InputKeys::MoveLeft))
        m_PlayerX += moveSpeed;

    m_PlayerMoveX = m_PlayerX - m_PrevPlayerX;
    m_PlayerMoveZ = m_PlayerZ - m_PrevPlayerZ;

    float moveLength = std::sqrt(m_PlayerMoveX * m_PlayerMoveX + m_PlayerMoveZ * m_PlayerMoveZ);
    if (moveLength > 0.001f)
    {
        m_PlayerMoveX /= moveLength;
        m_PlayerMoveZ /= moveLength;
    }

    static constexpr const double UpdateBufferPeriod = 0.1;
    if (CurrTime - m_LastBufferUpdateTime > UpdateBufferPeriod)
    {
        m_LastBufferUpdateTime = CurrTime;
        UpdateBuffer(1);
    }

    MapDynamicBuffer(2);
}

} // namespace Diligent
