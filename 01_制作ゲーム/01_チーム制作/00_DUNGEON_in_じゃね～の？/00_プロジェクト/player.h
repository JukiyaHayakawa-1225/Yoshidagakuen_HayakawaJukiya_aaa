//=============================================================================
//
// プレイヤー処理[2D ポリゴン] [player.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "scene.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PLAYER_PARTS (14)	//プレイヤーのパーツ数
#define MAX_KEY (10)			//最大キー数
//*****************************************************************************
//前方宣言 --スペルミスに注意！(なんでも通るから！)
//*****************************************************************************
class CModel;
//キー要素
typedef struct
{
	int nFrame;								//フレーム数
	D3DXVECTOR3 pos[MAX_PLAYER_PARTS];		//位置情報
	D3DXVECTOR3 rot[MAX_PLAYER_PARTS];		//角度情報
}KEY;

typedef struct
{
	int nLoop;								//ループするか
	int nNumKey;							//キー数
	KEY aKey[MAX_KEY];						//キー
}KEY_INFO;

//*****************************************************************************
//プレイヤーのクラス宣言
//*****************************************************************************
class CPlayer : public CScene
{

public:
	typedef enum
	{//プレイヤーの状態
		PLAYERSTATE_NORMAL = 0,	//通常状態
		PLAYERSTATE_DAMAGE,		//ダメージ状態
		PLAYERSTATE_INVISIBLE,	//無敵状態
		PLAYERSTATE_APPEAR,		//出現時状態
		PLAYERSTATE_MAX
	}PLAYERSTATE;
	typedef enum
	{// モーションの種類
		MOTION_NEUTRAL = 0,			// ニュートラル時
		MOTION_MOVE,				// 移動時
		MOTION_ATTACK,				// 通常攻撃00
		MOTION_JUMP,				// ジャンプ時
		MOTION_LANDING,				// 着地時
		MOTION_DAMAGE,				//ダメージ
		MOTION_DEATH,				// 死亡時
		MOTION_MAX					// モーションの総数
	}MOTIONSTATE;

	typedef enum
	{// フラグの種類
		PLAYERBOOLTYPE_JUMP = 0,	// ジャンプ
		PLAYERBOOLTYPE_LANDING,		// 着地
		PLAYERBOOLTYPE_DAMAGE,		// ダメージ
		PLAYERBOOLTYPE_DEAD,		// 死亡
		PLAYERBOOLTYPE_MAX			// 総数
	}PLAYERBOOLTYPE;

	CPlayer(int nPriority = 3, OBJTYPE type = OBJTYPE_PLAYER);	//コンストラクタ
	~CPlayer();													//デストラクタ
	static HRESULT Load(void);									//テクスチャの読み込み
	static void Unload(void);									//テクスチャの破棄
	static CPlayer *Create(D3DXVECTOR3 pos);					//プレイヤーの生成
	HRESULT Init(void);											//初期化処理
	void Uninit(void);											//終了処理
	void Update(void);											//更新処理
	void Draw(void);											//描画処理
	void PlayerMotion(void);									//プレイヤーのモーション
	void PlayerMotionState(bool *bAction);						//モーションの状態設定
	void PlayerRotFixes(D3DXVECTOR3 rot);						//角度修正
	void PlayerRotFixes(float rot);								//角度修正
	D3DXVECTOR3 GetPos(void);									//位置情報の取得
	D3DXVECTOR3 GetRot(void);									//角度情報の取得
	D3DXVECTOR3 GetMove(void);									//速度情報の取得

private:
	static D3DXVECTOR3 m_Partspos[MAX_PLAYER_PARTS];			//パーツの位置
	static D3DXVECTOR3 m_Partsrot[MAX_PLAYER_PARTS];			//パーツの角度
	static D3DXVECTOR3 m_PartsDefaultpos[MAX_PLAYER_PARTS];		//パーツの位置
	static D3DXVECTOR3 m_PartsDefaultrot[MAX_PLAYER_PARTS];		//パーツの角度
	static D3DXVECTOR3 m_Motionpos[MAX_PLAYER_PARTS];			//モーション差分代入用位置
	static D3DXVECTOR3 m_Motionrot[MAX_PLAYER_PARTS];			//モーション差分代入用角度

	static int m_aParent[MAX_PLAYER_PARTS];						//パーツの親
	static int m_aIndex[MAX_PLAYER_PARTS];						//パーツの番号

	//プレイヤーの基本情報//
	static LPDIRECT3DTEXTURE9 m_pTexture; //共有テクスチャのポインタ
	float m_fPlayerMove;					//プレイヤーの移動量
	float m_fDestAngle;						//目的の角度
	float m_fAngle;							//角度
	bool m_bUseDraw;						//描画するか
	int m_StateCnt;							//状態変化のカウンタ
	D3DXMATRIX m_mtxWorld;					//ワールドマトリックス
	D3DXVECTOR3 m_pos;						//位置
	D3DXVECTOR3 m_posold;					//位置
	D3DXVECTOR3 m_move;						//移動量
	D3DXVECTOR3 m_rot;						//向き
	D3DXVECTOR3 m_jumppower;				//ジャンプ量
	PLAYERSTATE m_state;					//プレイヤーの状態
	CModel *m_apModel[MAX_PLAYER_PARTS];	//キャラクターのパーツ
	////モーション関係の変数////
	int m_nNumKey;							//キーの総数
	KEY_INFO *m_pKeyInfo;					//キー情報のポインタ
	KEY_INFO m_aKeyInfo[MOTION_MAX];		//キー情報のポインタ
	MOTIONSTATE m_Motionstate;				//モーションの状態
	MOTIONSTATE m_MotionstateOld;			//以前のモーションの状態
	bool m_bAction[PLAYERBOOLTYPE_MAX];		// 行動中のフラグ
	bool m_bMotionBlend;					//ブレンドするか
	int m_nKey;								//現在のキーナンバー
	int m_nAllFrame;						//現在のキーナンバー
	int m_nCountMotion;						//モーションカウンター
	int m_nCounterFrame;					//フレームのカウンター
	int m_nCounterAllFrame;					//モーションカウンター
	int m_nCounterKey;						//キーのカウンター
	int m_nFrame;							//今のフレーム
	float m_fRadius;						//当たり判定の半径
};


#endif