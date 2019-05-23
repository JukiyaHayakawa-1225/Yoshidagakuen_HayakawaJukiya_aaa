//=============================================================================
//
// ゲーム処理[2D ポリゴン] [game.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
class CModel;
class CMeshCylinder;
class CItemCounter;
class CUi;
class CNumber;
class CTimer;
//*****************************************************************************
// ゲームのクラス構造体
//*****************************************************************************
class CGame
{
public:
	CGame();										//コンストラクタ
	~CGame();										//デストラクタ
	 HRESULT Init(void);							//初期化処理
	 void Uninit(void);								//終了処理
	 void Update(void);								//更新処理
	 void Draw(void);								//描画処理
	 static CMeshField *GetSceneMeshField(void);	//メッシュフィールドシーンの取得
	 static CPlayer *GetPlayer(void);				//プレイヤーの取得
	 static CEnemy *GetEnemy(int nIndex);			//敵の取得


private:
	static CGame *m_pGame;					//ゲーム
	static CMeshField *m_pMeshField;		//メッシュフィールド
	static CMeshCylinder *m_pMeshCylinder;	//シリンダー
	static CPlayer *m_pPlayer;				//プレイヤー
	static CEnemy *m_pEnemy[15];			//敵
	static CModel *m_pModel;				//モデル
	static CObject *m_pObject;				//オブジェクト
	static CItemCounter *m_pItemCounter;	//アイテムカウンタ
	static CUi *m_pUi;	//アイテムカウンタ
	static CNumber *m_pNumber;//数字
	static CTimer *m_pTimer;//数字
	static CCamera *m_pCamera;					//カメラ
	static CLight *m_pLight;					//ライト

};

#endif