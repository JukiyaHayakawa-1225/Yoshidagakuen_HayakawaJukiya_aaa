//=============================================================================
//
// ランキングの処理 [ranking.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CRanking
{
public:	//誰からもアクセス可能
	//メンバ関数
	CRanking();
	~CRanking();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	//静的メンバ関数
	static CRanking *Create(void);

private:	//自分だけがアクセス可能
	int m_nTimer;	//タイマー
};
#endif