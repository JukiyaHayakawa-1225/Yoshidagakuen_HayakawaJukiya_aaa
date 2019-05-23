//=============================================================================
//
// チュートリアル処理[2D ポリゴン] [tutorial.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMY (50)
#define MAX_TUTORIAL_UI (12)

#include "main.h"
class CTutoLogo;
class CTutorial
{
public:

	CTutorial();	//コンストラクタ
	~CTutorial();	//デストラクタ
	static HRESULT Init(void);	//初期化処理
	static void Uninit(void);	//終了処理
	static void Update(void);	//更新処理
	static void Draw(void);	//描画処理
private:
	static CTutoLogo *m_pTutoLogo;
};

#endif