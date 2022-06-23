#include "stdafx.h"
#include "EnemyPath.h"

void EnemyPath::Init(const char* filePath)
{
	m_level.Init(filePath, [&](LevelObjectData& objData)
		{
			if (objData.ForwardMatchName(L"path") == true)
			{
				Point point;
				point.s_number = objData.number;
				point.s_position = objData.position;
				int number = objData.number;
				m_pointlist.emplace(number, point);
				return true;
			}
			return true;
		});
}

Point* EnemyPath::GetNearPoint(const Vector3& position)
{
	Point* point = &m_pointlist[0];
	Vector3 diff = m_pointlist[0].s_position - position;
	//¡‚ÌêŠ‚©‚çˆê”Ô‹ß‚¢ƒ|ƒCƒ“ƒg’T‚·
	for (int i = 1; i < m_pointlist.size(); i++) {
		Vector3 diff2 = m_pointlist[i].s_position - position;
		if (diff.LengthSq() > diff2.LengthSq()) {
			diff = diff2;
			point = &m_pointlist[i];
		}
	}
	return point;
}
