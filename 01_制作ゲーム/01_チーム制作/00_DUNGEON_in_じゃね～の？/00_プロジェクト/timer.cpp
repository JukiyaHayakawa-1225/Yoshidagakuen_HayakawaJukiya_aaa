//=============================================================================
//
// タイマーの処理[2Dポリゴン] [timer.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================5
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "player.h"
#include "number.h"
#include "timer.h"

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int CTimer::m_nTimer = 0;	//タイマー
int CTimer::m_nTenMinutes = 0;
int CTimer::m_nOneMinutes = 0;
int CTimer::m_nTenSeconds = 0;
int CTimer::m_nOneSeconds = 0;
int CTimer::m_nTenComma = 0;
int CTimer::m_nOneComma = 0;

// ランキングの初期値設定
int CRankingTimer::m_nRank[RANKING_NUMBER][6] = { { 0, 0, 5, 0, 0, 0 },{ 0, 1, 0, 0, 0, 0 },{ 0, 1, 3, 0, 0, 0 },{ 0, 2, 0, 0, 0, 0 },{ 0, 2, 3, 0, 0, 0 } };

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MINUTES_WIDTH (60.0f)
#define MINUTES_HEIGHT (80.0f)

#define SECONDS_WIDTH (50.0f)
#define SECONDS_HEIGHT (70.0f)

#define COMMA_WIDTH (35.0f)
#define COMMA_HEIGHT (55.0f)

//=============================================================================
//コンストラクタ
//=============================================================================
CTimer::CTimer() :CScene(7)
{//クリアな値を代入
	m_nTimer = 0;	//タイマー
	m_nTenMinutes = 0;
	m_nOneMinutes = 0;
	m_nTenSeconds = 0;
	m_nOneSeconds = 0;
	m_nTenComma = 0;
	m_nOneComma = 0;
}

//=============================================================================
//デストラクタ
//=============================================================================
CTimer::~CTimer()
{
}

//=============================================================================
//タイマーの生成
//=============================================================================
CTimer *CTimer::Create(D3DXVECTOR3 pos, TIMEMODE mode)
{
	//タイマーの生成
	CTimer *pTimer = NULL;

	switch (mode)
	{
	case TIMEMODE_GAME:					// ゲームで使用
		pTimer = new CGameTimer;
		pTimer->m_pos = pos;
		pTimer->Init();
		break;

	case TIMEMODE_RESULT:				// リザルトで使用
		pTimer = new CResultTimer;
		pTimer->m_pos = pos;
		pTimer->Init();
		break;

	case TIMEMODE_RANKING:				// ランキングで使用
		pTimer = new CRankingTimer;
		pTimer->m_pos = pos;
		pTimer->Init();
		break;

	}

	return pTimer;
}

//=============================================================================
//ゲームのコンストラクタ
//=============================================================================
CGameTimer::CGameTimer()
{
	for (int nCntTimer = 0; nCntTimer < TIMER_NUMBER; nCntTimer++)
	{
		m_apNumber[nCntTimer] = NULL;
	}
}

//=============================================================================
//ゲームのデストラクタ
//=============================================================================
CGameTimer::~CGameTimer()
{
	for (int nCntTimer = 0; nCntTimer < TIMER_NUMBER; nCntTimer++)
	{
		m_apNumber[nCntTimer] = 0;
	}
}

//=============================================================================
//ゲームの初期化処理
//=============================================================================
HRESULT CGameTimer::Init(void)
{
	//float fSpace;
	m_nCntFrame = 0;
	bUse = true;

	for (int nCntNumber = 0; nCntNumber < TIMER_NUMBER; nCntNumber++)
	{
		switch (nCntNumber)
		{
		case CTimer::TIMERTYPE_TENMINUTE:
			//10分台
			//数字の生成
			m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z), MINUTES_WIDTH, MINUTES_HEIGHT);
			break;
		case CTimer::TIMERTYPE_ONEMINUTE:
			//1分台
			//数字の生成
			m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 100.0f, m_pos.y, m_pos.z), MINUTES_WIDTH, MINUTES_HEIGHT);
			break;

		case CTimer::TIMERTYPE_TENSECONDS:
			//10秒台
			//数字の生成
			m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 270.0f, m_pos.y + 5.0f, m_pos.z), SECONDS_WIDTH, SECONDS_HEIGHT);
			break;
		case CTimer::TIMERTYPE_ONESECONDS:
			//1秒台
			//数字の生成
			m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 350.0f, m_pos.y + 5.0f, m_pos.z), SECONDS_WIDTH, SECONDS_HEIGHT);
			break;
		case CTimer::TIMERTYPE_TENCOMMASECONDS:
			//10コンマ秒
			//数字の生成
			m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 480.0f, m_pos.y + 10.0f, m_pos.z), COMMA_WIDTH, COMMA_HEIGHT);
			break;
		case CTimer::TIMERTYPE_ONECOMMASECONDS:
			//1コンマ秒
			//数字の生成
			m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 540.0f, m_pos.y + 10.0f, m_pos.z), COMMA_WIDTH, COMMA_HEIGHT);
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//ゲームの終了処理
//=============================================================================
void CGameTimer::Uninit(void)
{
	// ゲームが終了した時に、リザルトにスコアを渡す
	CManager::SetTimer(m_nTenMinutes, m_nOneMinutes, m_nTenSeconds, m_nOneSeconds, m_nTenComma, m_nOneComma);

	for (int nCntTimer = 0; nCntTimer < TIMER_NUMBER; nCntTimer++)
	{
		if (m_apNumber[nCntTimer] != NULL)
		{
			m_apNumber[nCntTimer]->Uninit();
			delete m_apNumber[nCntTimer];
			m_apNumber[nCntTimer] = NULL;
		}
	}

	// 自分自身の破棄
	Release();
}

//=============================================================================
//ゲームの更新処理
//=============================================================================
void CGameTimer::Update(void)
{
	//カウント用フレームの加算
	m_nCntFrame++;

	if (bUse == true)
	{
		m_nOneComma++;
	}
	//１０台コンマ秒を増加
	if (m_nCntFrame % 6 == 0)
	{//6フレームで更新
		m_nTenComma++;
	}

	if (m_nTenComma > 9)
	{//コンマ10秒台が最大までいったら
		m_nTenComma = 0;
		m_nOneSeconds++;
	}

	if (m_nOneSeconds > 9)
	{
		m_nTenSeconds++;
		m_nOneSeconds = 0;
	}

	if (m_nTenSeconds > 5)
	{
		m_nOneMinutes++;

		m_nTenSeconds = 0;
	}
	if (m_nOneMinutes > 9)
	{
		m_nTenMinutes++;

		m_nOneMinutes = 0;
	}

	//=======================================================
	//番号のテクスチャ設定
	//=======================================================
	//分
	m_apNumber[0]->SetNumber(m_nTenMinutes);
	m_apNumber[1]->SetNumber(m_nOneMinutes);

	//秒
	m_apNumber[2]->SetNumber(m_nTenSeconds);
	m_apNumber[3]->SetNumber(m_nOneSeconds);

	//コンマ秒
	m_apNumber[4]->SetNumber(m_nTenComma);
	m_apNumber[5]->SetNumber(m_nOneComma);
}

//=============================================================================
//ゲームの描画処理
//=============================================================================
void CGameTimer::Draw(void)
{
	for (int nCntTimer = 0; nCntTimer < TIMER_NUMBER; nCntTimer++)
	{
		m_apNumber[nCntTimer]->Draw();
	}
}

//=============================================================================
//リザルトのコンストラクタ
//=============================================================================
CResultTimer::CResultTimer()
{
	for (int nCntTimer = 0; nCntTimer < TIMER_NUMBER; nCntTimer++)
	{
		m_apNumber[nCntTimer] = NULL;
	}
}

//=============================================================================
//リザルトのデストラクタ
//=============================================================================
CResultTimer::~CResultTimer()
{
}

//=============================================================================
//リザルトの初期化処理
//=============================================================================
HRESULT CResultTimer::Init()
{
	for (int nCntNumber = 0; nCntNumber < TIMER_NUMBER; nCntNumber++)
	{
		switch (nCntNumber)
		{
		case CTimer::TIMERTYPE_TENMINUTE:
			//10分台
			//数字の生成
			m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z), MINUTES_WIDTH, MINUTES_HEIGHT);
			break;
		case CTimer::TIMERTYPE_ONEMINUTE:
			//1分台
			//数字の生成
			m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 100.0f, m_pos.y, m_pos.z), MINUTES_WIDTH, MINUTES_HEIGHT);
			break;

		case CTimer::TIMERTYPE_TENSECONDS:
			//10秒台
			//数字の生成
			m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 290.0f, m_pos.y + 5.0f, m_pos.z), SECONDS_WIDTH, SECONDS_HEIGHT);
			break;
		case CTimer::TIMERTYPE_ONESECONDS:
			//1秒台
			//数字の生成
			m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 370.0f, m_pos.y + 5.0f, m_pos.z), SECONDS_WIDTH, SECONDS_HEIGHT);
			break;
		case CTimer::TIMERTYPE_TENCOMMASECONDS:
			//10コンマ秒
			//数字の生成
			m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 470.0f, m_pos.y + 10.0f, m_pos.z), COMMA_WIDTH, COMMA_HEIGHT);
			break;
		case CTimer::TIMERTYPE_ONECOMMASECONDS:
			//1コンマ秒
			//数字の生成
			m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 530.0f, m_pos.y + 10.0f, m_pos.z), COMMA_WIDTH, COMMA_HEIGHT);
			break;
		}
	}

	// ゲームのスコアを取得
	SetResultTimer(CManager::GetTenMinutes(), CManager::GetOneMinutes(), CManager::GetTenSeconds(), CManager::GetOneSeconds(), CManager::GetTenComma(), CManager::GetOneComma());

	return S_OK;
}

//=============================================================================
//リザルトの終了処理
//=============================================================================
void CResultTimer::Uninit(void)
{
	// ゲームが終了した時に、リザルトにスコアを渡す
	CManager::SetTimer(m_nTenMinutes, m_nOneMinutes, m_nTenSeconds, m_nOneSeconds, m_nTenComma, m_nOneComma);

	for (int nCntTimer = 0; nCntTimer < TIMER_NUMBER; nCntTimer++)
	{
		if (m_apNumber[nCntTimer] != NULL)
		{
			m_apNumber[nCntTimer]->Uninit();
			delete m_apNumber[nCntTimer];
			m_apNumber[nCntTimer] = NULL;
		}
	}

	// 自分自身の破棄
	Release();
}

//=============================================================================
//リザルトの更新処理
//=============================================================================
void CResultTimer::Update(void)
{
}

//=============================================================================
//リザルトの描画処理
//=============================================================================
void CResultTimer::Draw(void)
{
	for (int nCntTimer = 0; nCntTimer < TIMER_NUMBER; nCntTimer++)
	{
		m_apNumber[nCntTimer]->Draw();
	}
}

//=============================================================================
//スコアの設定
//=============================================================================
void CResultTimer::SetResultTimer(int nTenMinutes, int nOneMinutes, int nTenSeconds, int nOneSeconds, int nTenComma, int nOneComma)
{
	m_nTenMinutes = nTenMinutes;
	m_nOneMinutes = nOneMinutes;
	m_nTenSeconds = nTenSeconds;
	m_nOneSeconds = nOneSeconds;
	m_nTenComma = nTenComma;
	m_nOneComma = nOneComma;

	//=======================================================
	//番号のテクスチャ設定
	//=======================================================
	//分
	m_apNumber[0]->SetNumber(m_nTenMinutes);
	m_apNumber[1]->SetNumber(m_nOneMinutes);

	//秒
	m_apNumber[2]->SetNumber(m_nTenSeconds);
	m_apNumber[3]->SetNumber(m_nOneSeconds);

	//コンマ秒
	m_apNumber[4]->SetNumber(m_nTenComma);
	m_apNumber[5]->SetNumber(m_nOneComma);

	//スコアの初期化
	//CManager::SetTimer(0, 0, 0, 0, 0, 0);
}

//=============================================================================
//ランキングのコンストラクタ
//=============================================================================
CRankingTimer::CRankingTimer()
{
	for (int nCntRank = 0; nCntRank < RANKING_NUMBER; nCntRank++)
	{
		for (int nCntNumber = 0; nCntNumber < TIMER_NUMBER; nCntNumber++)
		{
			m_apNumber[nCntRank][nCntNumber] = NULL;
		}
	}

	// 変数の初期化
	m_nChange = 0;
}

//=============================================================================
//ランキングのデストラクタ
//=============================================================================
CRankingTimer::~CRankingTimer()
{
}

//=============================================================================
//ランキングの初期化処理
//=============================================================================
HRESULT CRankingTimer::Init(void)
{
	for (int nCntRank = 0; nCntRank < RANKING_NUMBER; nCntRank++)
	{
		for (int nCntNumber = 0; nCntNumber < TIMER_NUMBER; nCntNumber++)
		{
			switch (nCntNumber)
			{
			case CTimer::TIMERTYPE_TENMINUTE:
				//10分台
				//数字の生成
				m_apNumber[nCntRank][nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x, m_pos.y + (120.0f * nCntRank), m_pos.z), 40.0f, 60.0f);
				break;
			case CTimer::TIMERTYPE_ONEMINUTE:
				//1分台
				//数字の生成
				m_apNumber[nCntRank][nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 80.0f, m_pos.y + (120.0f * nCntRank), m_pos.z), 40.0f, 60.0f);
				break;

			case CTimer::TIMERTYPE_TENSECONDS:
				//10秒台
				//数字の生成
				m_apNumber[nCntRank][nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 200.0f, m_pos.y + 5.0f + (120.0f * nCntRank), m_pos.z), 30.0f, 50.0f);
				break;
			case CTimer::TIMERTYPE_ONESECONDS:
				//1秒台
				//数字の生成
				m_apNumber[nCntRank][nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 270.0f, m_pos.y + 5.0f + (120.0f * nCntRank), m_pos.z), 30.0f, 50.0f);
				break;
			case CTimer::TIMERTYPE_TENCOMMASECONDS:
				//10コンマ秒
				//数字の生成
				m_apNumber[nCntRank][nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 380.0f, m_pos.y + 10.0f + (120.0f * nCntRank), m_pos.z), COMMA_WIDTH, COMMA_HEIGHT);
				break;
			case CTimer::TIMERTYPE_ONECOMMASECONDS:
				//1コンマ秒
				//数字の生成
				m_apNumber[nCntRank][nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 450.0f, m_pos.y + 10.0f + (120.0f * nCntRank), m_pos.z), COMMA_WIDTH, COMMA_HEIGHT);
				break;
			}
		}
	}

	SetRankingTimer();

	return S_OK;
}

//=============================================================================
//ランキングの終了処理
//=============================================================================
void CRankingTimer::Uninit(void)
{
	for (int nCntRank = 0; nCntRank < RANKING_NUMBER; nCntRank++)
	{
		for (int nCntTimer = 0; nCntTimer < TIMER_NUMBER; nCntTimer++)
		{
			if (m_apNumber[nCntRank][nCntTimer] != NULL)
			{
				m_apNumber[nCntRank][nCntTimer]->Uninit();
				delete m_apNumber[nCntRank][nCntTimer];
				m_apNumber[nCntRank][nCntTimer] = NULL;
			}
		}
	}

	// 自分自身の破棄
	Release();
}

//=============================================================================
//ランキングの更新処理
//=============================================================================
void CRankingTimer::Update(void)
{

}

//=============================================================================
//ランキングの描画処理
//=============================================================================
void CRankingTimer::Draw(void)
{
	for (int nCntRank = 0; nCntRank < RANKING_NUMBER; nCntRank++)
	{
		for (int nCntNumber = 0; nCntNumber < TIMER_NUMBER; nCntNumber++)
		{	//描画処理
			m_apNumber[nCntRank][nCntNumber]->Draw();
		}
	}
}

//=============================================================================
//ランキングの設定
//=============================================================================
void CRankingTimer::SetRankingTimer()
{
	int nRank[6];

	// 配列に現在のスコアを代入
	nRank[0] = CManager::GetTenMinutes();
	nRank[1] = CManager::GetOneMinutes();
	nRank[2] = CManager::GetTenSeconds();
	nRank[3] = CManager::GetOneSeconds();
	nRank[4] = CManager::GetTenComma();
	nRank[5] = CManager::GetOneComma();

	// ランキングの設定
	for (int nCntRank = 0; nCntRank < RANKING_NUMBER; nCntRank++)
	{
		for (int nCntNum = 0; nCntNum < TIMER_NUMBER; )
		{
			if (nRank[nCntNum] == m_nRank[nCntRank][nCntNum])
			{
				nCntNum++;
			}
			else if (nRank[nCntNum] < m_nRank[nCntRank][nCntNum])
			{
				m_nChange = m_nRank[nCntRank][0];
				m_nRank[nCntRank][0] = nRank[0];
				nRank[0] = m_nChange;

				m_nChange = m_nRank[nCntRank][1];
				m_nRank[nCntRank][1] = nRank[1];
				nRank[1] = m_nChange;

				m_nChange = m_nRank[nCntRank][2];
				m_nRank[nCntRank][2] = nRank[2];
				nRank[2] = m_nChange;

				m_nChange = m_nRank[nCntRank][3];
				m_nRank[nCntRank][3] = nRank[3];
				nRank[3] = m_nChange;

				m_nChange = m_nRank[nCntRank][4];
				m_nRank[nCntRank][4] = nRank[4];
				nRank[4] = m_nChange;

				m_nChange = m_nRank[nCntRank][5];
				m_nRank[nCntRank][5] = nRank[5];
				nRank[5] = m_nChange;
				break;
			}
			else
			{
				break;
			}
		}

		//=======================================================
		//番号のテクスチャ設定
		//=======================================================
		//分
		m_apNumber[nCntRank][0]->SetNumber(m_nRank[nCntRank][0]);
		m_apNumber[nCntRank][1]->SetNumber(m_nRank[nCntRank][1]);

		//秒
		m_apNumber[nCntRank][2]->SetNumber(m_nRank[nCntRank][2]);
		m_apNumber[nCntRank][3]->SetNumber(m_nRank[nCntRank][3]);

		//コンマ秒
		m_apNumber[nCntRank][4]->SetNumber(m_nRank[nCntRank][4]);
		m_apNumber[nCntRank][5]->SetNumber(m_nRank[nCntRank][5]);
	}

	//スコアの初期化
	CManager::SetTimer(0, 0, 0, 0, 0, 0);
}