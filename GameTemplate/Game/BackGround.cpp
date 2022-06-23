#include "stdafx.h"
#include "BackGround.h"

bool BackGround::Start()
{
	// モデルの初期化。
	m_inGameStage.Init(
		"Assets/modelData/stage/2ndFloor/Floor2.tkm",
		nullptr,
		0,
		enModelUpAxisZ,
		0
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
