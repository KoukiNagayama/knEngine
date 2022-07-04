#include "stdafx.h"
#include "TitleCamera.h"
#include "TitleText.h"

namespace
{
	const float NEAR_CLIP = 3.0f;					//ニアークリップ
	const float FAR_CLIP = 10000.0f;				//ファークリップ
}

bool TitleCamera::Start()
{
	m_tText = FindGO<TitleText>("titleText");
	Vector3	target = m_tText->GetPosition();
	// カメラの初期設定
	// カメラの近平面と遠平面を設定
	g_camera3D->SetNear(NEAR_CLIP);
	g_camera3D->SetFar(FAR_CLIP);

	// 注視点を設定。
	g_camera3D->SetTarget(target);

	// 視点を設定。
	g_camera3D->SetPosition(m_cameraPos);

	return true;
}

void TitleCamera::Update()
{
	// カメラを更新する。
	g_camera3D->Update();
}


