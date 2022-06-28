#include "stdafx.h"
#include "BackGround.h"
#include "EdgeManagement.h"

namespace
{
	const int EDGE_COLOR_WHITE = 0;					// 輪郭線の色
	const int NUM_ANIMATIONCLIP = 0;				// アニメーションクリップの数
	const int MAX_INSTANCE = 0;						// インスタンスの最大数
}

BackGround::~BackGround()
{
	m_physicsStaticObject.Release();
}

bool BackGround::Start()
{
	// モデルの初期化。
	m_inGameStage.Init(
		"Assets/modelData/stage/2ndFloor/Floor2.tkm",
		nullptr,
		NUM_ANIMATIONCLIP,
		enModelUpAxisZ,
		MAX_INSTANCE,
		EDGE_COLOR_WHITE,
		m_edgeManagement->GetEdgeControl()
	);

	// モデルから静的物理オブジェクトを生成。
	m_physicsStaticObject.CreateFromModel(
		m_inGameStage.GetModel(),
		m_inGameStage.GetModel().GetWorldMatrix()
	);

	return true;
}

void BackGround::Update()
{
	// モデルの更新。
	m_inGameStage.Update();
}

void BackGround::Render(RenderContext& rc)
{
	// モデルの描画。
	m_inGameStage.Draw();
}
