#pragma once
#include "LevelRender.h"

/// <summary>
/// パス移動のポイント情報
/// </summary>
struct Point {
	Vector3 s_position;				// ポイントの座標
	int     s_number;				// ポイントの番号
};

/// <summary>
/// エネミーパスクラス
/// </summary>
class  EnemyPath
{
public:
	/// <summary>
	/// パス移動の最初のポイント情報を取得
	/// </summary>
	/// <returns>パス移動の最初のポイント情報</returns>
	Point* GetFirstPoint()
	{
		return &m_pointlist[0];
	}
	/// <summary>
	/// パス移動の次のポイント情報を取得
	/// </summary>
	/// <param name="number">現在地のポイントの番号</param>
	/// <returns>パス移動の次のポイント情報</returns>
	Point* GetNextPoint(const int number)
	{
		return &m_pointlist[number + 1];
	}
	/// <summary>
	/// パス移動用のポイントの数を取得
	/// </summary>
	/// <returns>パス移動用のポイントの数</returns>
	int GetPointListSize()
	{
		return m_pointlist.size();
	}
	/// <summary>
	/// 現在の座標から一番近いポイントを取得
	/// </summary>
	/// <param name="position">座標</param>
	/// <returns>ポイント</returns>
	Point* GetNearPoint(const Vector3& position);
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="filePath">ポイント登録に使用したレベルのファイルパス</param>
	void Init(const char* filePath);
private:
	Point* m_point;					// ポイント構造体
	std::map<int, Point>		m_pointlist;				// パス移動用のポイントリスト
	LevelRender					m_level;					// レベルレンダー
};

