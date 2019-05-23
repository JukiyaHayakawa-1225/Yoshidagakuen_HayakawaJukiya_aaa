//=============================================================================
//
// ライト処理 [light.h]
// Author :
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

#define MAX_LIGHT (3)
//*****************************************************************************
// カメラのクラス構造体
//*****************************************************************************
class CLight
{
public:
	CLight();	//コンストラクタ
	~CLight();	//デストラクタ
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
private:
	static CManager *m_pManager;//マネージャ
	D3DLIGHT9 m_Light[MAX_LIGHT];	//ライト

};


#endif
