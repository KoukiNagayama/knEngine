/*!
 * @brief 輪郭線描画(テスト)
 */

////////////////////////////////////////////////
// 定数
////////////////////////////////////////////////
// 音源の最大個数
#define MAXIMUM_SIZE_OF_SOUND_SOURCE 50

////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////

//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn
{
    int4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};

// 頂点シェーダーへの入力
struct SVSIn
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    SSkinVSIn skinVert;
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
};

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float4 posInProj : TEXCOORD1;
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
};

// 音源データ
struct SoundSourceData
{
    float3 pos;             // 座標
    float range;            // 影響範囲
    float rateByTime;       // 時間経過による影響率
    float3 pad;
    int isSound;            // 音源が鳴っているか
    int3 pad2;
};

///////////////////////////////////////////
// 定数バッファー
///////////////////////////////////////////
// モデル用の定数バッファー
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};
// 音源用の定数バッファー
cbuffer SoundSourceCb : register(b1)
{
    SoundSourceData soundSourceData[MAXIMUM_SIZE_OF_SOUND_SOURCE]; // 音源データ
    int numSoundSource; // 音源データの個数
    float sizeOfColorValue; // 色の数値の大きさ
    
};



///////////////////////////////////////////
// シェーダーリソース
///////////////////////////////////////////
Texture2D<float4> g_texture : register(t0); // モデルテクスチャ
StructuredBuffer<float4x4> g_boneMatrix : register(t3); //ボーン行列。
Texture2D<float4> g_depthValueTexture : register(t10); // 深度テクスチャ
Texture2D<float4> g_worldCoordinateTexture : register(t11); // ワールド座標テクスチャ
Texture2D<float4> g_normalTexture : register(t12); // 法線テクスチャ

///////////////////////////////////////////
// サンプラーステート
///////////////////////////////////////////
sampler g_sampler : register(s0); // サンプラー


/// <summary>
//スキン行列を計算する。
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
/// モデル用の頂点シェーダーのエントリーポイント
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
    psIn.pos = mul(m, vsIn.pos); // モデルの頂点をワールド座標系に変換
    psIn.pos = mul(mView, psIn.pos); // ワールド座標系からカメラ座標系に変換
    psIn.pos = mul(mProj, psIn.pos); // カメラ座標系からスクリーン座標系に変換
    psIn.uv = vsIn.uv;
    psIn.posInProj = psIn.pos; // 頂点の正規化スクリーン座標系の座標をピクセルシェーダーに渡す
    return psIn;
}
/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, false);
}
/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, true);
}

/// <summary>
/// モデル用のピクセルシェーダーのエントリーポイント
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    // 正規化スクリーン座標系からUV座標系に変換する
    float2 uv = (psIn.posInProj.xy / psIn.posInProj.w) * float2(0.5f, -0.5f) + 0.5f;

    float4 tex = g_texture.Sample(g_sampler, psIn.uv);
    
    // 近傍8テクセルへのUVオフセット
    float2 uvOffset[8] =
    {
        float2(0.0f, 1.0f / 900.0f), //上
        float2(0.0f, -1.0f / 900.0f), //下
        float2(1.0f / 1600.0f, 0.0f), //右
        float2(-1.0f / 1600.0f, 0.0f), //左
        float2(1.0f / 1600.0f, 1.0f / 900.0f), //右上
        float2(-1.0f / 1600.0f, 1.0f / 900.0f), //左上
        float2(1.0f / 1600.0f, -1.0f / 900.0f), //右下
        float2(-1.0f / 1600.0f, -1.0f / 900.0f) //左下
    };
    
    // そのピクセルの座標が含まれる影響範囲をもつ音源の数
    int insideRange = 0;
    // 配列の番号
    int num = 0;  
    // 同じピクセルに被っている輪郭線の範囲用配列
    int edgeArray[5];
    // 色の最大値
    float color = 1.0f;
    // 計算後の色の値の最大値格納用
    float maxColor = 0.0000f;
    
    // ワールド座標
    float3 worldPos = g_worldCoordinateTexture.Sample(g_sampler, uv);

    for (int i = 0; i < numSoundSource; i++)
    {
        // 音源が鳴っているか時間経過による影響率が残っているならば
        if(soundSourceData[i].isSound == 1 || soundSourceData[i].rateByTime > 0.00f)
        {
            // 音源からの距離
            float dist = length(worldPos - soundSourceData[i].pos);
            // 音源からの距離が音源の影響範囲よりも小さいならば
            if(dist < soundSourceData[i].range)
            {
                insideRange++;
                edgeArray[insideRange - 1] = i;
            }
        }
    }
    

    // ここから輪郭線描画
    if (insideRange >= 1)
    {
        // 深度値
        // このピクセルの深度値を取得
        float depth = g_depthValueTexture.Sample(g_sampler, uv).x;
  
        // 近傍8テクセルの深度値の平均値を計算する
        float depth2 = 0.0f;
        for (int i = 0; i < 8; i++)
        {
            depth2 += g_depthValueTexture.Sample(g_sampler, uv + uvOffset[i]).x;
        }
        depth2 /= 8.0f;
        
        
        // 法線
        // このピクセルの法線を取得
        float3 normal = g_normalTexture.Sample(g_sampler, uv).xyz * -8.0f;
        
        // 近傍8テクセルの法線の平均値を計算する
        for (i = 0; i < 8; i++)
        {
            normal += g_normalTexture.Sample(g_sampler, uv + uvOffset[i]).xyz;
        }
        
        // 自身の深度値・法線と近傍8テクセルの深度値の差・法線の差を調べる
        if (abs(depth - depth2) > 0.0000455f || length(normal) >= 0.4f)
        {
            for (int i = 0; i < insideRange; i++)
            {
                // 音源からピクセルまでの距離
                float dist = length(worldPos - soundSourceData[edgeArray[i]].pos);
                // 距離による影響率
                float rateByDist = 1.0 - pow((dist / soundSourceData[edgeArray[i]].range), 2.5f);
                // 輪郭線の色を計算
                color = 1.0 * soundSourceData[edgeArray[i]].rateByTime * rateByDist;
                // 計算後の色が一番明るいか
                if (color > maxColor)
                {
                    maxColor = color;
                }
            }
            // ピクセルを輪郭線として塗りつぶす
            return float4(maxColor * 0.7f, maxColor * 0.7f, maxColor * 0.7f, 1.0f);
        }

    }
    // ピクセルを黒色に塗りつぶす
    return float4(0.0f, 0.0f, 0.0f, 1.0f);
    // デバッグ用 
    //return tex;
}