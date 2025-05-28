/*
 *  Copyright 2019-2025 Diligent Graphics LLC
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

#pragma once

#include <array>
#include <random>
#include "SampleBase.hpp"
#include "BasicMath.hpp"

namespace Diligent
{

class Tutorial11_ResourceUpdates final : public SampleBase
{
public:
    virtual void Initialize(const SampleInitInfo& InitInfo) override final;

    virtual void Render() override final;
    virtual void Update(double CurrTime, double ElapsedTime, bool DoUpdateUI) override final;

    virtual const Char* GetSampleName() const override final { return "Tutorial11: Resource Updates"; }

private:
    void CreatePipelineStates();
    void CreateVertexBuffers();
    void CreateIndexBuffer();
    void LoadTextures();    

    void UpdateBuffer(Uint32 BufferIndex);
    void MapDynamicBuffer(Uint32 BufferIndex);

    // #Crea el jugador que se mueve como tal funcitones diferentes que el pasto
    void CreatePlayerCube();
    void DrawPlayerCube(const float4x4& WVPMatrix, IShaderResourceBinding* pSRB);

    // #Actualiza la posicion del jugador
    void UpdatePlayerVelocity(float dt);

    // #El piso
    void CreateGroundPlane();
    void DrawGroundPlane(const float4x4& WVPMatrix, IShaderResourceBinding* pSRB);
    RefCntAutoPtr<IBuffer> m_GroundPlaneVertexBuffer;
    RefCntAutoPtr<IBuffer> m_GroundPlaneIndexBuffer;

    RefCntAutoPtr<IPipelineState> m_pPSO, m_pPSO_NoCull;
    RefCntAutoPtr<IBuffer>        m_CubeVertexBuffer[3];
    RefCntAutoPtr<IBuffer>        m_CubeIndexBuffer;
    RefCntAutoPtr<IBuffer>        m_VSConstants;
    RefCntAutoPtr<IBuffer>        m_TextureUpdateBuffer;

    // #Funcion de creacion del pasto
    void DrawCube(const float4x4& WVPMatrix, IBuffer* pVertexBuffer, IShaderResourceBinding* pSRB);

    static constexpr const size_t NumTextures         = 4;
    static constexpr const Uint32 MaxUpdateRegionSize = 128;
    static constexpr const Uint32 MaxMapRegionSize    = 128;

    std::array<RefCntAutoPtr<ITexture>, NumTextures>               m_Textures;
    std::array<RefCntAutoPtr<IShaderResourceBinding>, NumTextures> m_SRBs;

    double       m_LastTextureUpdateTime = 0;
    double       m_LastBufferUpdateTime  = 0;
    double       m_LastMapTime           = 0;
    std::mt19937 m_gen{0}; //Use 0 as the seed to always generate the same sequence
    double       m_CurrTime = 0;

    // #Vamos a usar esto para el movimiento del pasto
    int m_MovementDirection = 0;

    // #Camara 
    Diligent::float3 m_CamPos    = {0, 15, 25};
    float            m_Pitch     = -0.2f;      
    float            m_Yaw       = -3.14f / 2; 
    float            m_MoveSpeed = 10.0f;      
    float            m_MouseSens = 0.0025f;   

    // #Info del jugador
    float m_PlayerX = 0.0f;
    float m_PlayerZ = 0.0f;

    RefCntAutoPtr<IBuffer> m_PlayerCubeVertexBuffer;
    RefCntAutoPtr<IBuffer> m_PlayerCubeIndexBuffer;

    // #Info para movpasto
    float m_PrevPlayerX = 0.0f;
    float m_PrevPlayerZ = 0.0f;
    float m_PlayerMoveX = 0.0f;
    float m_PlayerMoveZ = 0.0f;
    float3 m_PlayerVel{0, 0, 0};

    std::vector<bool>  m_GrassDeformed;
    std::vector<float> m_GrassDeformX;
    std::vector<float> m_GrassDeformY;
    std::vector<float> m_GrassDeformZ;
};

} // namespace Diligent
