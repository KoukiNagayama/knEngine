/*!
 * @brief �֊s���`��(�e�X�g)
 */

////////////////////////////////////////////////
// �萔
////////////////////////////////////////////////
// �����̍ő��
#define MAXIMUM_SIZE_OF_SOUND_SOURCE 50

////////////////////////////////////////////////
// �\����
////////////////////////////////////////////////

//�X�L�j���O�p�̒��_�f�[�^���ЂƂ܂Ƃ߁B
struct SSkinVSIn
{
    int4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};

// ���_�V�F�[�_�[�ւ̓���
struct SVSIn
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    SSkinVSIn skinVert;
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
};

// �s�N�Z���V�F�[�_�[�ւ̓���
struct SPSIn
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float4 posInProj : TEXCOORD1;
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
};

// �����f�[�^
struct SoundSourceData
{
    float3 pos; // ���W
    float range; // �e���͈�
    float rateByTime; // ���Ԍo�߂ɂ��e����
    float3 pad;
    int isSound; // ���������Ă��邩
    int3 pad2;
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
// �����p�̒萔�o�b�t�@�[
cbuffer SoundSourceCb : register(b1)
{
    SoundSourceData soundSourceData[MAXIMUM_SIZE_OF_SOUND_SOURCE]; // �����f�[�^
    int numSoundSource; // �����f�[�^�̌�
    float sizeOfColorValue; // �F�̐��l�̑傫��
    
};



///////////////////////////////////////////
// �V�F�[�_�[���\�[�X
///////////////////////////////////////////
Texture2D<float4> g_texture : register(t0); // ���f���e�N�X�`��
StructuredBuffer<float4x4> g_boneMatrix : register(t3); //�{�[���s��B
Texture2D<float4> g_depthValueTexture : register(t10); // �[�x�e�N�X�`��
Texture2D<float4> g_worldCoordinateTexture : register(t11); // ���[���h���W�e�N�X�`��
Texture2D<float4> g_normalTexture : register(t12); // �@���e�N�X�`��

///////////////////////////////////////////
// �T���v���[�X�e�[�g
///////////////////////////////////////////
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
/// ���f���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g
/// </summary>
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
    psIn.pos = mul(m, vsIn.pos); // ���f���̒��_�����[���h���W�n�ɕϊ�
    psIn.pos = mul(mView, psIn.pos); // ���[���h���W�n����J�������W�n�ɕϊ�
    psIn.pos = mul(mProj, psIn.pos); // �J�������W�n����X�N���[�����W�n�ɕϊ�
    psIn.uv = vsIn.uv;
    psIn.posInProj = psIn.pos; // ���_�̐��K���X�N���[�����W�n�̍��W���s�N�Z���V�F�[�_�[�ɓn��
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
/// ���f���p�̃s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    // ���K���X�N���[�����W�n����UV���W�n�ɕϊ�����
    float2 uv = (psIn.posInProj.xy / psIn.posInProj.w) * float2(0.5f, -0.5f) + 0.5f;

    float4 tex = g_texture.Sample(g_sampler, psIn.uv);
    
    // �ߖT8�e�N�Z���ւ�UV�I�t�Z�b�g
    float2 uvOffset[8] =
    {
        float2(0.0f, 1.0f / 900.0f), //��
        float2(0.0f, -1.0f / 900.0f), //��
        float2(1.0f / 1600.0f, 0.0f), //�E
        float2(-1.0f / 1600.0f, 0.0f), //��
        float2(1.0f / 1600.0f, 1.0f / 900.0f), //�E��
        float2(-1.0f / 1600.0f, 1.0f / 900.0f), //����
        float2(1.0f / 1600.0f, -1.0f / 900.0f), //�E��
        float2(-1.0f / 1600.0f, -1.0f / 900.0f) //����
    };
    
    // ���̃s�N�Z���̍��W���܂܂��e���͈͂��������̐�
    int insideRange = 0;
    // �z��̔ԍ�
    int num = 0;
    // �����s�N�Z���ɔ���Ă���֊s���͈̔͗p�z��
    int edgeArray[5];
    // �F�̍ő�l
    float color = 1.0f;
    // �v�Z��̐F�̒l�̍ő�l�i�[�p
    float maxColor = 0.0000f;
    
    // ���[���h���W
    float3 worldPos = g_worldCoordinateTexture.Sample(g_sampler, uv);

    for (int i = 0; i < numSoundSource; i++)
    {
        // ���������Ă��邩���Ԍo�߂ɂ��e�������c���Ă���Ȃ��
        if (soundSourceData[i].isSound == 1 || soundSourceData[i].rateByTime > 0.00f)
        {
            // ��������̋���
            float dist = length(worldPos - soundSourceData[i].pos);
            // ��������̋����������̉e���͈͂����������Ȃ��
            if (dist < soundSourceData[i].range)
            {
                insideRange++;
                edgeArray[insideRange - 1] = i;
            }
        }
    }
    

    // ��������֊s���`��
    if (insideRange >= 1)
    {
        // �[�x�l
        // ���̃s�N�Z���̐[�x�l���擾
        float depth = g_depthValueTexture.Sample(g_sampler, uv).x;
  
        // �ߖT8�e�N�Z���̐[�x�l�̕��ϒl���v�Z����
        float depth2 = 0.0f;
        for (int i = 0; i < 8; i++)
        {
            depth2 += g_depthValueTexture.Sample(g_sampler, uv + uvOffset[i]).x;
        }
        depth2 /= 8.0f;
        
        
        // �@��
        // ���̃s�N�Z���̖@�����擾
        float3 normal = g_normalTexture.Sample(g_sampler, uv).xyz * -8.0f;
        
        // �ߖT8�e�N�Z���̖@���̕��ϒl���v�Z����
        for (i = 0; i < 8; i++)
        {
            normal += g_normalTexture.Sample(g_sampler, uv + uvOffset[i]).xyz;
        }
        
        // ���g�̐[�x�l�E�@���ƋߖT8�e�N�Z���̐[�x�l�̍��E�@���̍��𒲂ׂ�
        if (abs(depth - depth2) > 0.0000455f || length(normal) >= 0.4f)
        {
            for (int i = 0; i < insideRange; i++)
            {
                // ��������s�N�Z���܂ł̋���
                float dist = length(worldPos - soundSourceData[edgeArray[i]].pos);
                // �����ɂ��e����
                float rateByDist = 1.0 - pow((dist / soundSourceData[edgeArray[i]].range), 2.5f);
                // �֊s���̐F���v�Z
                color = 1.0 * soundSourceData[edgeArray[i]].rateByTime * rateByDist;
                // �v�Z��̐F����Ԗ��邢��
                if (color > maxColor)
                {
                    maxColor = color;
                }
            }
            // �s�N�Z����֊s���Ƃ��ēh��Ԃ�
            return float4(maxColor * 40.0f, maxColor * 40.0f, 0.0f, 1.0f);
        }

    }
    // �s�N�Z�������F�ɓh��Ԃ�
    return float4(0.0f, 0.0f, 0.0f, 1.0f);
    // �f�o�b�O�p 
    //return tex;
}