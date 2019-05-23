//=============================================================================
//
// 敵の処理[2D ポリゴン] [enemy.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMY_PARTS (15)	//プレイヤーのパーツ数
#define MAX_KEY (10)			//最大キー数
//*****************************************************************************
//前方宣言 --スペルミスに注意！(なんでも通るから！)
//*****************************************************************************
class CModel;


//*****************************************************************************
//敵のクラス宣言
//*****************************************************************************
class CEnemy : public CScene
{
	//キー要素
	typedef struct
	{
		int nFrame;								//フレーム数
		D3DXVECTOR3 pos[MAX_ENEMY_PARTS];		//位置情報
		D3DXVECTOR3 rot[MAX_ENEMY_PARTS];		//角度情報
	}KEY;

	typedef struct
	{
		int nLoop;								//ループするか
		int nNumKey;							//キー数
		KEY aKey[MAX_KEY];						//キー
	}KEY_INFO;


public:
	typedef enum
	{//敵の状態
		ENEMYSTATE_NORMAL = 0,	//通常状態
		ENEMYSTATE_LOITERING,	//徘徊状態
		ENEMYSTATE_CHASE,		//追跡状態
		ENEMYSTATE_MAX			//敵の状態の最大数
	}ENEMYSTATE;

	typedef enum
	{//敵の種類
		ENEMYTYPE_LOITERING_Z = 0,	// Z軸移動タイプ
		ENEMYTYPE_LOITERING_X,		// X軸移動タイプ
		ENEMYTYPE_CHASE,			// 追跡タイプ
		ENEMYTYPE_TEST,				// テスト用タイプ
		ENEMYTYPE_MAX				// 敵の種類の最大数
	}ENEMYTYPE;

	typedef enum
	{// モーションの種類
		MOTION_NEUTRAL = 0,			// ニュートラル時
		MOTION_MOVE,				// 移動時
		MOTION_MOVE_2,				// 移動時2
		MOTION_JUMP,				// ジャンプ時
		MOTION_LANDING,				// 着地時
		MOTION_MAX					// モーションの総数
	}MOTIONSTATE;

	typedef enum
	{// フラグの種類
		ENEMYBOOLTYPE_JUMP = 0,		// ジャンプ
		ENEMYBOOLTYPE_LANDING,		// 着地
		ENEMYBOOLTYPE_DAMAGE,		// ダメージ
		ENEMYBOOLTYPE_DEAD,			// 死亡
		ENEMYBOOLTYPE_MAX			// 総数
	}ENEMYBOOLTYPE;

	CEnemy(int nPriority = 5, OBJTYPE type = OBJTYPE_PLAYER);						//コンストラクタ
	~CEnemy();																		//デストラクタ
	static HRESULT Load(void);														//テクスチャの読み込み
	static void Unload(void);														//テクスチャの破棄
	static CEnemy *Create(D3DXVECTOR3 pos, ENEMYTYPE type);							//プレイヤーの生成
	HRESULT Init(void);																//初期化処理
	void Uninit(void);																//終了処理
	void Update(void);																//更新処理
	void Draw(void);																//描画処理
	void EnemyMotion(void);															//プレイヤーのモーション
	void EnemyMotionState(bool *bAction);											//モーションの状態設定
	void EnemyRotFixes(D3DXVECTOR3 *rot);											//角度修正
	void EnemyRotFixes(float *rot);													//角度修正
	bool FindPlayer(D3DXVECTOR3 *pos, float fRange);								//プレイヤー探索
	D3DXVECTOR3 GetPos(void);														//位置情報の取得
	D3DXVECTOR3 GetRot(void);														//角度情報の取得
	bool CollisionEnemy(D3DXVECTOR3 *pos, D3DXVECTOR3 *move, D3DXVECTOR3 *rot);		//敵との当たり判定


private:
	 D3DXVECTOR3 m_Partspos[MAX_ENEMY_PARTS];		//パーツの位置
	 D3DXVECTOR3 m_Partsrot[MAX_ENEMY_PARTS];		//パーツの角度
	 D3DXVECTOR3 m_Motionpos[MAX_ENEMY_PARTS];	//モーション差分代入用位置
	 D3DXVECTOR3 m_Motionrot[MAX_ENEMY_PARTS];	//モーション差分代入用角度
	 D3DXVECTOR3 m_PartsDefaultpos[MAX_ENEMY_PARTS];	//パーツの位置
	 D3DXVECTOR3 m_PartsDefaultrot[MAX_ENEMY_PARTS];	//パーツの角度

	static int m_aParent[MAX_ENEMY_PARTS];	//パーツの親
	static int m_aIndex[MAX_ENEMY_PARTS];	//パーツの番号

	//プレイヤーの基本情報//
	static LPDIRECT3DTEXTURE9 m_pTexture; //共有テクスチャのポインタ
	float m_fPlayerMove;					//プレイヤーの移動量
	float m_fDestAngle;						//目的の角度
	float m_fAngle;							//角度
	D3DXMATRIX m_mtxWorld;					//ワールドマトリックス
	D3DXVECTOR3 m_pos;						//位置
	D3DXVECTOR3 m_posold;					//位置
	D3DXVECTOR3 m_move;						//移動量
	D3DXVECTOR3 m_rot;						//向き
	D3DXVECTOR3 m_jumppower;				//ジャンプ量
	CModel *m_apModel[MAX_ENEMY_PARTS];		//キャラクターのパーツ
	D3DXVECTOR3 m_Startpos;					//スタート位置
	bool m_bSwitch;							//スイッチ
	MOTIONSTATE m_state;					//敵の状態
	ENEMYTYPE m_type;						//敵の種類


	////モーション関係の変数////
	int m_nNumKey;							//キーの総数
	KEY_INFO *m_pKeyInfo;					//キー情報のポインタ
	KEY_INFO m_aKeyInfo[MOTION_MAX];		//キー情報のポインタ
	MOTIONSTATE m_Motionstate;				//モーションの状態
	MOTIONSTATE m_MotionstateOld;			//以前のモーションの状態
	bool m_bAction[ENEMYBOOLTYPE_MAX];		// 行動中のフラグ
	bool m_bMotionBlend;					//ブレンドするか
	int m_nKey;								//現在のキーナンバー
	int m_nAllFrame;						//現在のキーナンバー
	int m_nCountMotion;						//モーションカウンター
	int m_nCounterFrame;					//フレームのカウンター
	int m_nCounterAllFrame;					//モーションカウンター
	int m_nCounterKey;						//キーのカウンター
	int m_nFrame;							//今のフレーム
	float m_fHeight;						//敵の高さ
};


#endif