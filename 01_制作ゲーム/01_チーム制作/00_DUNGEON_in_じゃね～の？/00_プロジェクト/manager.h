//=============================================================================
//
// マネージャー処理 [manager.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"
//前方宣言(名称に注意！なんでも通るから！)
class CScene2D;
class CScene3D;
class CScene;
class CSceneX;
class CSceneBillBoard;
class CMeshField;
class CSound;
class CLight;
class CCamera;
class CDebugProc;
class CPlayer;
class CEnemy;
class CModel;
class CObject;
class CTitle;
class CTutorial;
class CGame;
class CResult;
class CRanking;
class CSound;
class CInputX;
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//*****************************************************************************
// マネージャーのクラス構造体
//*****************************************************************************
class CManager
{
public:
	typedef enum
	{
		MODE_TITLE = 0,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_RESULT,
		MODE_RANKING,
		MODE_MAX
	}MODE;
	CManager();	//コンストラクタ
	~CManager();//デストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);	//初期化処理
	void Uninit(void);								//終了処理
	void Update(void);								//更新処理
	void Draw(void);								//描画処理

	//静的メンバ関数
	static CInputKeyboard *GetInputKeyboard(void);	//キーボードの取得
	static CRenderer *GetRenderer(void);			//レンダラーの取得
	static CCamera *GetCamera(void);				//カメラの取得
	static CScene3D *GetScene3D(void);				//シーン3Dの取得
	static CSound *GetSound(void);					//サウンドの取得
	static CInputX *GetInput(void);					//ジョイパッド
	static void SetMode(MODE mode);					//モードの設定
	static MODE GetMode(void);						//モードの取得


	// タイマーで使用
	static void SetTimer(int nTenMinutes, int nOneMinutes, int nTenSeconds, int nOneSeconds, int nTenComma, int nOneComma);
	static int GetTimer(void);
	static int GetTenMinutes(void) { return m_nTenMinutes; }			//10分台
	static int GetOneMinutes(void) { return m_nOneMinutes; }		  //1分台
	static int GetTenSeconds(void) { return m_nTenSeconds; }		  //10秒台
	static int GetOneSeconds(void) { return m_nOneSeconds; }		 //10秒台
	static int GetTenComma(void) { return m_nTenComma; }			 //コンマ秒
	static int GetOneComma(void) { return m_nOneComma; }		//コンマ秒


private:
	static CTitle *m_pTitle;					//タイトル
	static CTutorial *m_pTutorial;				//チュートリアル
	static CGame *m_pGame;						//ゲーム
	static CResult *m_pResult;					//リザルト
	static CRanking *m_pRanking;				//ランキング
	static CSound *m_pSound; //サウンドのポインタ
	static CRenderer *m_pRenderer;				//レンダラのポインタ
	static CInputKeyboard *m_pInputKeyboard;	//キーボードのポインタ
	static MODE m_mode;							//モード
	static CScene3D *m_pScene3D;				//3Dシーン
	static CDebugProc *m_pDebugProc;			//デバッグ表示
	static CCamera *m_pCamera;					//カメラ
	static CLight *m_pLight;					//ライト
	static CInputX *m_pInputX;					//ジョイパッド
	bool m_bDebug;								//デバッグ用


	// タイマーの値を保存する為に使用
	static int m_nTimer;							//タイマー
	static int m_nTenMinutes;						//10分台
	static int m_nOneMinutes;						//1分台
	static int m_nTenSeconds;						//10秒台
	static int m_nOneSeconds;						//10秒台
	static int m_nTenComma;							//コンマ秒
	static int m_nOneComma;							//コンマ秒

	// コインの値を保存する為に使用
	static int m_nCoin;									// コインに値を返す
};


#endif