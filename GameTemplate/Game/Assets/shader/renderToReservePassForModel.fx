/*!
 * @brief ReservePass�p���f���`��
 */

///////////////////////////////////////////////////
// �\����
///////////////////////////////////////////////////

//�X�L�j���O�p�̒��_�f�[�^���ЂƂ܂Ƃ߁B
struct SSkinVSIn
{
    int4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};

// ���_�V�F�[�_�[�ւ̓���
struct SVSIn
{
    float4 pos : POSITION; // ���f���̒��_���W
    float3 normal : NORMAL; // �@��
    float3 tangent : TANGENT;
    SSkinVSIn skinVert;
    float3 biNormal : BINORMAL;
    float2 uv : TEXCOORD0; // UV���W
};

// �s�N�Z���V�F�[�_�[�ւ̓���
struct SPSIn
{
    float4 pos : SV_POSITION; // �X�N���[����Ԃł̃s�N�Z���̍��W
    float3 normal : NORMAL; // �@��
    float3 tangent : TANGENT; // �ڃx�N�g��
    float3 biNormal : BINORMAL; // �]�@���x�N�g��
    float2 uv : TEXCOORD0; // UV���W
    float3 worldPos : TEXCOORD1; // ���[���h��Ԃł̃s�N�Z���̍��W
    float3 depthInView : TEXCOORD2; // �J������Ԃł̃s�N�Z����Z�l
};

// �s�N�Z���V�F�[�_�[����̏o��
struct SPSOut
{
    float depth : SV_TARGET0; // �����_�����O�^�[�Q�b�g0�ɕ`��
    float3 normal : SV_TARGET1; // �����_�����O�^�[�Q�b�g1�ɕ`��
    float3 worldPos : SV_TARGET2; // �����_�����O�^�[�Q�b�g2�ɕ`��

};

///////////////////////////////////////////
// �萔�o�b�t�@�[
///////////////////////////////////////////
// ���f���p�̒萔�o�b�t�@�[
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

///////////////////////////////////////////
// �O���[�o���ϐ�
///////////////////////////////////////////
Texture2D<float4> g_normalMap : register(t1);
StructuredBuffer<float4x4> g_boneMatrix : register(t3);
sampler g_sampler : register(s0); // �T���v���[

/// <summary>
//�X�L���s����v�Z����B
/// </summary>
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
    float4x4 skinning = 0;
    float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}

/// <summary>
/// ���_�V�F�[�_�[
/// <summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
    SPSIn psIn;
    float4x4 m;
    if (hasSkin)
    {
        m = CalcSkinMatrix(vsIn.skinVert);
    }
    else
    {
        m = mWorld;
    }
    psIn.pos = mul(m, vsIn.pos);
    psIn.worldPos = psIn.pos;
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);
    
    psIn.normal = normalize(mul(m, vsIn.normal));
    psIn.tangent = normalize(mul(m, vsIn.tangent));
    psIn.biNormal = normalize(mul(m, vsIn.biNormal));
    psIn.uv = vsIn.uv;
    
    return psIn;
}

/// <summary>
/// �X�L���Ȃ����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�֐��B
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, false);
}

/// <summary>
/// �X�L�����胁�b�V���̒��_�V�F�[�_�[�̃G���g���[�֐��B
/// </summary>
SPSIn VSSkinMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, true);
}

/// <summary>
/// �s�N�Z���V�F�[�_�[
/// <summary>
SPSOut PSMain(SPSIn psIn)
{
    SPSOut psOut;
    
    // �[�x�l
    // �J������Ԃł̐[�x�l��ݒ�
    psOut.depth = psIn.pos.z;
    
    // ���[���h���W
    // ���[���h���W��ݒ�
    psOut.worldPos = psIn.worldPos;
    
    // �@��
    float3 normal = psIn.normal;
    // �@���}�b�v����^���W�F���g�X�y�[�X�̖@�����T���v�����O����
    float3 localNormal = g_normalMap.Sample(g_sampler, psIn.uv).xyz;
    // �^���W�F���g�X�y�[�X�̖@����0�`1�͈̔͂���-1�`1�͈̔͂ɕ�������
    localNormal = (localNormal - 0.5f) * 2.0f;
    // �^���W�F���g�X�y�[�X�̖@�������[���h�X�y�[�X�ɕϊ�����
    normal = psIn.tangent * localNormal.x + psIn.biNormal * localNormal.y + normal * localNormal.z;
    // �@����ݒ�
    psOut.normal = normal;
    
    return psOut;
}