#include "stdafx.h"
#include "TitleText.h"
#include "EdgeManagement.h"

namespace
{
	const float VALUE_OF_SCALE = 0.90f;						// Šg‘å—¦‚Ì’l
	const float ADJUSTMENT_VALUE_OF_POSITION = 4.0f;		// À•W‚Ì’²®’l
	const float ADJUSTMENT_VALUE_OF_ROTATION = -26.0f;		// ‰ñ“]‚Ì’²®’l
	const int NUM_ANIMATIONCLIP = 0;
	const int MAX_INSTANCE = 0;	
	const int EDGE_COLOR_WHITE = 0;
}
bool TitleText::Start()
{
	m_textModel.Init(
		"Assets/modelData/title/title.tkm",
		nullptr,
		NUM_ANIMATIONCLIP,
		enModelUpAxisZ,
		MAX_INSTANCE,
		EDGE_COLOR_WHITE,
		m_edgeManagement->GetEdgeControl()
	);

	// Šg‘å—¦
	m_scale = Vector3(VALUE_OF_SCALE, VALUE_OF_SCALE, VALUE_OF_SCALE);


	m_textModel.SetTRS(m_position, m_rotation, m_scale);

	return true;
}

void TitleText::Update()
{
	m_textModel.Update();
}

void TitleText::Render(RenderContext& rc)
{
	m_textModel.Draw();
}
