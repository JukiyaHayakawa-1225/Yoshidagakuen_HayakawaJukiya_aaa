//=============================================================================
//
// アイテムカウンタ処理[シーン] [itemcounter.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _ITEMCOUNTER_H_
#define _ITEMCOUNTER_H_

#include "main.h"
#include "scene.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define COUNTER_NUMBER (3)

class CNumber;
class CItemCounter : public CScene
{
public:
	typedef enum
	{
		COUNTERTYPE_KEY = 0,
		COUNTERTYPE_COIN,
		COUNTERTYPE_MAX
	}COUNTERTYPE;
	CItemCounter(int nPriority = 7, OBJTYPE type = OBJTYPE_ITEMCOUNTER);	//コンストラクタ
	~CItemCounter();	//デストラクタ
	static CItemCounter *Create(void);	//数字の生成
	HRESULT Init(void);								//初期化処理
	void Uninit(void);								//終了処理
	void Update(void);								//更新処理
	void Draw(void);								//描画処理
	static int GetCoin(void);						//コインの取得
	static int GetKey(void);						//カギの取得
	static int GetMaxKey(void);						//カギの
	static void AddCoin(int nValue);				//コインの加算
	static void AddKey(int nValue);					//カギの加算
	static void AddKeyMax(int nValue);				//カギ最大数の加算
	static void SetCoinNum(int nValue);				//コインの数の設定
private:
	static CNumber *m_apNumber[COUNTER_NUMBER];		//数字
	static int m_nCoin;								//コインの数
	static int m_nKey;								//カギの数
	static int m_nKeyMax;							//カギの最大数

};

#endif