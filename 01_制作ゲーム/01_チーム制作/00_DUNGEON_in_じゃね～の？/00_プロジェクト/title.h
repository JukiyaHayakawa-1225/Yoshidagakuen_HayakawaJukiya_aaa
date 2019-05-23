//=============================================================================
//
// タイトル処理[2D ポリゴン] [title.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
//*****************************************************************************
// タイトル画面のクラス構造体
//*****************************************************************************
class CTitleLogo;
class CTitle
{
public:
	CTitle();	//コンストラクタ
	~CTitle();	//デストラクタ
	//static CTitle *Create(void);	//タイトルの生成
	static HRESULT Init(void);	//初期化処理
	static void Uninit(void);	//終了処理
	static void Update(void);	//更新処理
	static void Draw(void);	//描画処理
private:
	static CTitleLogo *m_pTitleLogo;//タイトルロゴ
	static CObject *m_pObject;				//オブジェクト
	static CMeshField *m_pMeshField;		//メッシュフィールド
};

#endif