//=============================================================================
//
// プレイヤーの処理 [player.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "scene.h"

#define MAX_PLAYER_MODEL (1)
#define MAX_PLAYER_PARTS (1)
#define MAX_MOTION		 (10)
#define MAX_KEY			 (10)

//前方宣言
class CModel;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPlayer : public CScene
{
public:	//誰からもアクセス可能
	//*************************************************************************
	// 構造体定義
	//*************************************************************************
	typedef struct
	{
		float fPosX;
		float fPosY;
		float fPosZ;
		float fRotX;
		float fRotY;
		float fRotZ;
	}KEY;

	typedef struct
	{
		int nFrame;
		KEY aKey[MAX_PLAYER_MODEL];
	}KEY_INFO;

	typedef struct
	{
		bool bLoop;
		int nNumKey;
		KEY_INFO aKeyInfo[MAX_KEY];
	}MOTION_INFO;

	typedef enum
	{
		MOTION_NEUTRAL = 0,		//ニュートラルモーション
		MOTION_MOVE,			//移動モーション
		MOTION_PANCH,			//パンチモーション
		MOTION_JUMP,			//ジャンプモーション
		MOTION_LANDING,			//着地モーション
		MOTION_MAX,
	}MOTION_TYPE;

	typedef enum
	{
		PLAYER_STATE_START = 0,
		PLAYER_STATE_GOAL,
		PLAYER_STATE_MAX,
	}PLAYER_STATE;

	//メンバ関数
	CPlayer();
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void UpdateMotion(void);
	void LoadMotion(void);
	void SetMotion(MOTION_TYPE type);
	bool GetJump(void);
	D3DXVECTOR3 GetPosition(void) { return m_pos; };
	D3DXVECTOR3 SetScale(D3DXVECTOR3 scale);
	PLAYER_STATE GetState(void);

	//静的メンバ関数
	static CPlayer *Create(D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);

private:	//自分だけがアクセス可能
	//メンバ関数
	void Move(void);
	void Collision(void);

	//メンバ変数
	D3DXVECTOR3 m_pos;											//位置
	D3DXVECTOR3 m_posOld;										//過去の位置
	D3DXVECTOR3 m_rot;											//向き
	D3DXVECTOR3	m_move;											//移動量
	D3DXMATRIX	m_mtxWorld;										//ワールドマトリックス
	D3DXVECTOR3	m_VtxMin;						//最小値
	D3DXVECTOR3	m_VtxMax;						//最大値
	bool		m_bJump;										//ジャンプしているかどうか
	int			m_nNumKey;										//キーの総数
	KEY_INFO	*m_pKeyInfo;									//キー情報のポインタ
	int			m_nKey;											//現在のキーNo.
	int			m_nCountFrame;									//フレームカウンター
	int			m_nNumParts;									//パーツの総数
	int			m_nIndexParent[MAX_PLAYER_PARTS];				//プレイヤーのパーツ番号
	KEY			m_aKeyOffset[MAX_PLAYER_PARTS];					//プレイヤーのオフセット
	MOTION_INFO	m_apMotionInfo[MAX_KEY];						//モーションの情報
	MOTION_TYPE	m_motionType;									//モーションの種類
	D3DXVECTOR3	 m_scale;										//大きさ
	int m_OldMotion;											//過去のモーション
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;						// 頂点バッファへのポインタ
	CModel						*m_apModel[MAX_PLAYER_MODEL];	//モデルのポインタ
	D3DXMATRIX	m_mtxRot;										//回転マトリックス（保存用）
	D3DXQUATERNION	m_quat;										//クォータニオン
	D3DXVECTOR3	m_vecAxis;										//回転軸
	float		m_fValueRot;									//回転角（回転量）
	PLAYER_STATE m_state;										//プレイヤーの状態
	bool		m_bHitFlag;										//オブジェクトの手前にヒットしたか
	int			m_nAppearTimer;									//無敵時間タイマー

	//静的メンバ変数
	static LPDIRECT3DTEXTURE9	m_pTexture;								// テクスチャへのポインタ

	//静的メンバ変数
	static LPD3DXMESH			m_pMesh[MAX_PLAYER_MODEL];		//メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMat[MAX_PLAYER_MODEL];	//マテリアル情報へのポインタ
	static DWORD				m_nNumMat[MAX_PLAYER_MODEL];	//マテリアル情報の数
};
#endif