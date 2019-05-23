//=============================================================================
//
// リザルト処理[2D ポリゴン] [result.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
class CUiResult;
class CResult
{
public:
	CResult();	//コンストラクタ
	~CResult();	//デストラクタ
	//static CResult *Create();	//リザルトの生成
	static HRESULT Init(void);	//初期化処理
	static void Uninit(void);	//終了処理
	static void Update(void);	//更新処理
	static void Draw(void);	//描画処理
private:

	static CUiResult *m_pUiResult;
	static CObject *m_pObject;				//オブジェクト
	static CMeshField *m_pMeshField;		//メッシュフィールド

};

#endif