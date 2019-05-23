//=============================================================================
//
// タイマー処理[2D ポリゴン] [timer.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;				// ナンバー

							//*****************************************************************************
							// マクロ定義
							//*****************************************************************************
#define TIMER_NUMBER			(6)		// タイムの桁数
#define RANKING_NUMBER		(5)		// 順位の数

							//*****************************************************************************
							// タイマークラス
							//*****************************************************************************
class CTimer : public CScene
{
public:
	typedef enum
	{
		TIMERTYPE_TENMINUTE = 0,	//10分台
		TIMERTYPE_ONEMINUTE,	//1分台
		TIMERTYPE_TENSECONDS,	//10秒台
		TIMERTYPE_ONESECONDS,	//1秒台
		TIMERTYPE_TENCOMMASECONDS,	//コンマ10秒台
		TIMERTYPE_ONECOMMASECONDS,	//コンマ1秒台
	} TIMETYPE;

	typedef enum
	{
		TIMEMODE_NONE = 0,				// 初期化
		TIMEMODE_GAME,					// ゲーム
		TIMEMODE_RESULT,					// リザルト
		TIMEMODE_RANKING,				// ランキング
		TIMEMODE_MAX						// モードの種類
	} TIMEMODE;

	CTimer();	//コンストラクタ
	virtual ~CTimer();	//デストラクタ
	static CTimer *Create(D3DXVECTOR3 pos, TIMEMODE mode);	//数字の生成
	virtual HRESULT Init(void) = 0;	//初期化処理
	virtual void Uninit(void) = 0;	//終了処理
	virtual void Update(void) = 0;	//更新処理
	virtual void Draw(void) = 0;	//描画処理

protected:
	D3DXVECTOR3 m_pos;	//位置情報
	static int m_nTimer;		 //タイマー
	static int m_nTenMinutes;	 //10分台
	static int m_nOneMinutes;	 //1分台
	static int m_nTenSeconds;	 //10秒台
	static int m_nOneSeconds;	 //10秒台
	static int m_nTenComma;	//コンマ秒
	static int m_nOneComma;	//コンマ秒
};

//*****************************************************************************
// ゲームのタイマー
//*****************************************************************************
class CGameTimer : public CTimer
{
public:
	CGameTimer();			//コンストラクタ
	~CGameTimer();			//デストラクタ
	HRESULT Init(void);	//初期化処理
	void Uninit(void);			//終了処理
	void Update(void);		//更新処理
	void Draw(void);			//描画処理

private:
	CNumber *m_apNumber[TIMER_NUMBER];	//数字
	int m_nCntFrame;	//フレーム数カウント用
	int m_nCntColor;	//カラーカウント用
	bool bUse;//カウントするか
};

//*****************************************************************************
// リザルトのタイマー
//*****************************************************************************
class CResultTimer : public CTimer
{
public:
	CResultTimer();			//コンストラクタ
	~CResultTimer();		//デストラクタ
	HRESULT Init(void);	//初期化処理
	void Uninit(void);			//終了処理
	void Update(void);		//更新処理
	void Draw(void);			//描画処理
	void SetResultTimer(int nTenMinutes, int nOneMinutes, int nTenSeconds, int nOneSeconds, int nTenComma, int nOneComma);	//スコアの設定

private:
	CNumber *m_apNumber[TIMER_NUMBER];	//数字
};

//*****************************************************************************
// ランキングのタイマー
//*****************************************************************************
class CRankingTimer : public CTimer
{
public:
	CRankingTimer();		//コンストラクタ
	~CRankingTimer();		//デストラクタ
	HRESULT Init(void);	//初期化処理
	void Uninit(void);			//終了処理
	void Update(void);		//更新処理
	void Draw(void);			//描画処理
	void SetRankingTimer();	//スコアの設定

private:
	static int m_nRank[RANKING_NUMBER][6];
	CNumber *m_apNumber[RANKING_NUMBER][TIMER_NUMBER];	//順位、数字
	int m_nChange;		// 計算用1
};

#endif