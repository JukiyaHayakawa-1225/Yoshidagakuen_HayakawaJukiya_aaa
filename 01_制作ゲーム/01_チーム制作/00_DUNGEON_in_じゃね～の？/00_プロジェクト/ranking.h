//=============================================================================
//
// ランキング処理[2D ポリゴン] [ranking.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

class CUiRanking;
#include "main.h"

class CRanking
{
public:
	CRanking();	//コンストラクタ
	~CRanking();	//デストラクタ
				//static CRanking *Create();	//リザルトの生成
	static HRESULT Init(void);	//初期化処理
	static void Uninit(void);	//終了処理
	static void Update(void);	//更新処理
	static void Draw(void);	//描画処理
private:
	static int m_nCntFlame;

	static CUiRanking *m_pUiRanking;
	static CObject *m_pObject;				//オブジェクト
	static CMeshField *m_pMeshField;		//メッシュフィールド
	static int m_nJumpTitleFrame;			//タイトル画面へ遷移するときのフレームカウンタ

};

#endif