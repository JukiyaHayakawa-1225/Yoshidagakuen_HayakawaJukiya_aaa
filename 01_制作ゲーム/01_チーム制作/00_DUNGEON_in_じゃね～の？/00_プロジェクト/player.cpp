//=============================================================================
//
// プレイヤー処理[モデル] [player.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene3D.h"
#include "manager.h"
#include "input.h"
#include "sceneX.h"
#include "scene2D.h"
#include "debugproc.h"
#include "player.h"
#include "camera.h"
#include "model.h"
#include "enemy.h"
#include "object.h"
#include "meshfield.h"
#include "game.h"
#include "sound.h"
#include "inputx.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//=====================================
// プレイヤー情報
//=====================================
#define PLAYER_MOVE (0.28f)										//移動量
#define PLAYER_MOVE_DEFAULT		(0.2f)							// プレイヤーが動いてる基準の移動量
#define MAX_CHAR (256)											//文字の最大数
#define GRAVITY_NUM (0.8f)										//重力の値
#define PLAYER_MOTION_BLEND_FRAME	(5)							// モーションブレンドのフレーム数
#define JUMP_POWER (22.0f)										//ジャンプ力
#define PLAYER_COLLISION_RADIUS		(20.0f)						// プレイヤーの当たり判定の半径

#define SCREEN_LIMIT_MAX_X (690.0f )	//画面端(右端)
#define SCREEN_LIMIT_MIN_X (80.0f)			//画面(左端)

#define SCREEN_LIMIT_MIN_Z (-1100)			//画面(上端)


//=====================================
// 優先順位
//=====================================
#define ENEMY_PRIORITY (5)										//敵の優先順位
#define OBJECT_PRIORITY (4)										//オブジェクトの優先順位

//=====================================
// 読み込むテキストファイル
//=====================================
#define MODEL_FILENAME ("data/TEXT/motion_player.txt")			//読み込むファイル名

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL; //共有テクスチャのポインタ
D3DXVECTOR3 CPlayer::m_Partspos[MAX_PLAYER_PARTS] = {};			//パーツの位置
D3DXVECTOR3 CPlayer::m_Partsrot[MAX_PLAYER_PARTS] = {};			//パーツの位置
D3DXVECTOR3 CPlayer::m_PartsDefaultpos[MAX_PLAYER_PARTS] = {};	//デフォルトのパーツ位置
D3DXVECTOR3 CPlayer::m_PartsDefaultrot[MAX_PLAYER_PARTS] = {};	//デフォルトのパーツ角度
D3DXVECTOR3 CPlayer::m_Motionpos[MAX_PLAYER_PARTS] = {};		//モーション差分代入用位置
D3DXVECTOR3 CPlayer::m_Motionrot[MAX_PLAYER_PARTS] = {};		//モーション差分代入用角度
int CPlayer::m_aParent[MAX_PLAYER_PARTS] = {};					//パーツの親
int CPlayer::m_aIndex[MAX_PLAYER_PARTS] = {};					//パーツ番号


//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CPlayer::CPlayer(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{
	m_bUseDraw = true;
}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CPlayer::~CPlayer()
{
}


//=============================================================================
// 関数名：使用するテクスチャの読み込み
// 関数の概要：--
//=============================================================================
HRESULT CPlayer::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/player.png", &m_pTexture);

	return S_OK;

}

//=============================================================================
// 関数名：使用したテクスチャの破棄
// 関数の概要：--
//=============================================================================
void CPlayer::Unload(void)
{

		m_pTexture->Release();
		m_pTexture = NULL;

}


//=============================================================================
// 関数名：プレイヤーシーンの生成
// 関数の概要：プレイヤーシーンを生成する
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	CPlayer  *pPlayer;
	pPlayer = new CPlayer;//シーンクラスの生成
	if (pPlayer != NULL)
	{
		pPlayer->m_pos = pos;
		pPlayer->m_bAction[PLAYERBOOLTYPE_JUMP] = false;
		pPlayer->Init();
	}
	return pPlayer;
}

//=============================================================================
// 関数名：プレイヤーシーンの初期化処理
// 関数の概要：プレイヤー情報の設定
//=============================================================================
HRESULT CPlayer::Init(void)
{
#if 1
		CRenderer *pRenderer;
		pRenderer = CManager::GetRenderer();

		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice;
		pDevice = pRenderer->GetDevice();


		FILE *pFile = fopen(MODEL_FILENAME, "r");		// 読み込むテキストファイルのアドレス

		char acLine[MAX_CHAR];							// 1行分読み込む変数
		char acData[MAX_CHAR];							// 1文を読み込む変数
		int nCntMotion = 0;								// モーションのカウンタ
		int nCntKey = 0;								// キーのカウンタ
		int nCntParts = 0;								// パーツのカウンタ
		int nCntModel = 0;								// 読み込むモデルのカウンタ
		int nCntTexture = 0;							// 読み込むテクスチャのカウンタ
		int nIndex = 0;									// 使用するモデルのタイプ
		int nParent = 0;								// 親の数字
		int nMaxParts = 0;								// パーツ数の最大数
		D3DXVECTOR3 pos;								// 位置を入れる変数
		D3DXVECTOR3 rot;								// 向きを入れる変数

		if (pFile != NULL)
		{
			while (fgets(&acLine[0], MAX_CHAR, pFile) != NULL)
			{
				sscanf(&acLine[0], "%s", &acData);				// 文をデータにコピー
				if (memcmp(acData, "CHARACTERSET", 12) == 0)
				{// "CHARACTERSET"の文字列があったら
					nCntModel = 0;
					while (strcmp(acData, "END_CHARACTERSET") != 0)
					{// "END_CHARACTERSET"がまで情報を取得し続ける
						fgets(&acLine[0], MAXCHAR, pFile);					// 行を取得
						sscanf(&acLine[0], "%s", &acData);					// データをコピー
						if (strcmp(acData, "PARTSSET") == 0)
						{// "MODELSET"の文字列があったら
							while (strcmp(acData, "END_PARTSSET") != 0)
							{// "END_PARTSSET"がまで情報を取得し続ける
								fgets(&acLine[0], MAXCHAR, pFile);					// 行を取得
								sscanf(&acLine[0], "%s", &acData);					// データをコピー
								if (strcmp(acData, "INDEX") == 0)
								{// "TYPE"の文字列があったら
									sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nIndex);					// 使用するモデルの番号を取得
									m_aIndex[nCntModel] = nIndex;
								}
								else if (strcmp(acData, "PARENT") == 0)
								{// "TYPE"の文字列があったら
									sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nParent);					// 使用するモデルの番号を取得
									m_aParent[nCntModel] = nParent;
								}
								else if (strcmp(acData, "POS") == 0)
								{// "POS"の文字列があったら
									sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &pos.x, &pos.y, &pos.z);	// 位置を取得
									m_Partspos[nCntModel] = pos;
								}
								else if (strcmp(acData, "ROT") == 0)
								{// "ROT"の文字列があったら
									sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &rot.x, &rot.y, &rot.z);	// 向きを取得
									//テキストの角度情報通りに入れる計算
									rot.x = (D3DX_PI * 2 / 360.0f * rot.x);
									rot.y = (D3DX_PI * 2 / 360.0f * rot.y);
									rot.z = (D3DX_PI * 2 / 360.0f * rot.z);
									m_Partsrot[nCntModel] = rot;
								}
							}
							m_apModel[nCntModel] = CModel::Create(m_Partspos[nCntModel], m_Partsrot[nCntModel], m_aIndex[nCntModel], CModel::MODELTYPE_PLAYER);	//パーツの生成
							if (nCntModel == 0)
							{//体のパーツのとき
								m_apModel[nCntModel]->SetParent(NULL);
							}
							else
							{//それ以外のパーツのとき
								m_apModel[nCntModel]->SetParent(m_apModel[m_aParent[nCntModel]]);
							}
							m_apModel[nCntModel]->BindTexture(m_pTexture);
							nCntModel++;
						}//PARTSSET
					}//END_CHARACTERSET
				}//CHARACTERSET
				if (nCntMotion < MOTION_MAX)
				{// モーションの総数以上は設定しない
					if (strcmp(acData, "MOTIONSET") == 0)
					{// MOTIONSETの文字列があったら
						nCntKey = 0;
						while (strcmp(acData, "END_MOTIONSET") != 0)
						{// END_MOTIONSETが見つかるまでループする
							fgets(&acLine[0], MAXCHAR, pFile);					// 行を取得
							sscanf(&acLine[0], "%s", &acData);					// データをコピー

							if (strcmp(acData, "LOOP") == 0)
							{// LOOPがあったら
								sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &m_aKeyInfo[nCntMotion].nLoop);	// ループするかどうかを取得

							}
							else if (strcmp(acData, "NUM_KEY") == 0)
							{// NUM_KEYがあったら
								sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &m_aKeyInfo[nCntMotion].nNumKey);	// キー数を取得
							}
							else if (strcmp(acData, "KEYSET") == 0)
							{// KEYSETがあったら

								nCntParts = 0;// パーツのカウント変数を初期化

								while (strcmp(acData, "END_KEYSET") != 0)
								{// END_KEYSETがあるまでループ
									fgets(&acLine[0], MAXCHAR, pFile);					// 行を取得
									sscanf(&acLine[0], "%s", &acData);					// データをコピー

									if (strcmp(acData, "FRAME") == 0)
									{// FRAMEがあったら
										sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &m_aKeyInfo[nCntMotion].aKey[nCntKey].nFrame);	// キーのフレーム数を取得
									}
									else if (strcmp(acData, "KEY") == 0)
									{// KEYがあったら
										while (strcmp(acData, "END_KEY") != 0)
										{// END_KEYがあるまでループ
											fgets(&acLine[0], MAXCHAR, pFile);					// 行を取得
											sscanf(&acLine[0], "%s", &acData);					// データをコピー
											if (strcmp(acData, "POS") == 0)
											{// POSがあったら
												sscanf(&acLine[0],
													"%s %s %f %f %f",
													&acData[0],
													&acData[0],
													&m_aKeyInfo[nCntMotion].aKey[nCntKey].pos[nCntParts].x,		// 各パーツのモーションの位置を取得
													&m_aKeyInfo[nCntMotion].aKey[nCntKey].pos[nCntParts].y,		// 各パーツのモーションの位置を取得
													&m_aKeyInfo[nCntMotion].aKey[nCntKey].pos[nCntParts].z);		// 各パーツのモーションの位置を取得
											}
											else if (strcmp(acData, "ROT") == 0)
											{// ROTがあったら
												sscanf(&acLine[0],
													"%s %s %f %f %f",
													&acData[0],
													&acData[0],
													&m_aKeyInfo[nCntMotion].aKey[nCntKey].rot[nCntParts].x,		// 各パーツのモーションの回転を取得
													&m_aKeyInfo[nCntMotion].aKey[nCntKey].rot[nCntParts].y,		// 各パーツのモーションの回転を取得
													&m_aKeyInfo[nCntMotion].aKey[nCntKey].rot[nCntParts].z);		// 各パーツのモーションの回転を取得
											}//ROT
										}// END_KEY
										nCntParts++;// パーツのカウントを進める
									}// KEY
								}// END_KEYSET
								nCntKey++;// キーフレームを進める
							}//KEYSET
						}// END_MOTIONSET
						nCntMotion++;// モーションを変える
					}// MOTIONSET
				}//モーションの総数以上は設定しない
			}//while
				fclose(pFile);// 開いたファイルを閉じる
		}//ファイルが開けてたら

		//モーション関係
		m_nNumKey = 2;
		m_pKeyInfo = m_aKeyInfo;
		m_nKey = 0;
		m_nCountMotion = 0;
		m_Motionstate = MOTION_NEUTRAL;


		//プレイヤー情報
		m_fRadius = PLAYER_COLLISION_RADIUS; // プレイヤーの当たり判定の半径
		m_StateCnt = 20;
		m_state = PLAYERSTATE_APPEAR;

		for (int nCntKey = 0; nCntKey < m_aKeyInfo[m_Motionstate].nNumKey; nCntKey++)
		{//総フレーム数の計算
			m_nAllFrame += m_aKeyInfo[m_Motionstate].aKey[nCntKey].nFrame;
		}

		for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
		{// パーツ数分ループ
			 // モデルの位置をニュートラルモーションに初期化
			m_PartsDefaultpos[nCntParts] = m_Partspos[nCntParts] + m_aKeyInfo[m_Motionstate].aKey[0].pos[nCntParts];
			m_PartsDefaultrot[nCntParts] = m_aKeyInfo[m_Motionstate].aKey[0].rot[nCntParts];
		}
		//2Dシーンのオブジェクトの設定
		SetObjType(CScene::OBJTYPE_PLAYER);
#endif
		return S_OK;
}
//=============================================================================
// 関数名：プレイヤーシーンの終了処理
// 関数の概要：メッシュ、マテリアル、自身の破棄
//=============================================================================
void CPlayer::Uninit(void)
{
	for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
	{
		//モデルの終了処理
		if (m_apModel[nCntParts] != NULL)
		{
			m_apModel[nCntParts]->Uninit();
			delete m_apModel[nCntParts];
			m_apModel[nCntParts] = NULL;
		}
	}
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 関数名：プレイヤーシーンの更新処理
// 関数の概要：プレイヤーの移動、パーツのモーション更新
//=============================================================================
void CPlayer::Update(void)
{

#if 1
	CInputX::InputJoyState JoyState = CInputX::GetJoystate();
	bool bConnect = CInputX::GetbConnect();			//ジョイパッドの接続状態

	//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);


	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//カメラの取得
	CCamera *pCamera;
	pCamera = CManager::GetCamera();

	//カメラの角度取得
	D3DXVECTOR3 CameraRot = pCamera->GetRot();

	//メッシュフィールド取得
	CMeshField *pFieldField;
	pFieldField = CGame::GetSceneMeshField();


	//敵
	CEnemy *pEnemyTop;

	//オブジェクト
	CObject *pObjectTop;

	//高さを取得
	float fHeight = pFieldField->GetHeight(m_pos);

	//モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();
	switch (mode)
	{
	case CManager::MODE_TITLE:
		break;
	case CManager::MODE_GAME:
		break;
	}

		//===========================================================================
		//プレイヤーの状態
		//===========================================================================
		switch (m_state)
		{
		case PLAYERSTATE_NORMAL:
			break;

		case PLAYERSTATE_DAMAGE:
			m_StateCnt--;
			if (m_StateCnt <= 0)
			{
				m_StateCnt = 120;
				m_state = PLAYERSTATE_INVISIBLE;
			}
			break;

		case PLAYERSTATE_INVISIBLE:
			m_StateCnt--;
			if (m_StateCnt % 2 == 0)
			{// 点滅
				m_bUseDraw = m_bUseDraw ? false : true;
			}
			if (m_StateCnt <= 0)
			{
				m_bUseDraw = true;
				m_state = PLAYERSTATE_NORMAL;
			}
			break;
		case PLAYERSTATE_APPEAR:
			m_StateCnt--;
			if (m_StateCnt <= 0)
			{
				m_state = PLAYERSTATE_NORMAL;

			}
		}

		//===========================================================================
		//プレイヤーの移動
		//===========================================================================
		if (m_state == PLAYERSTATE_NORMAL || m_state == PLAYERSTATE_INVISIBLE)
		{//プレイヤーの状態で移動できるか
			//=========================================
			//任意のキー(→キー)が押されたかどうか    //
			//=========================================
			if (pInputKeyboard->GetKeyboardPress(DIK_A) == true || (state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && bConnect == true))
			{//Aキーが押されたとき
				if (pInputKeyboard->GetKeyboardPress(DIK_W) == true || (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && bConnect == true))
				{//上キー(Wキー)が押された
					m_move.x += sinf(-D3DX_PI * 0.25f + CameraRot.y) * PLAYER_MOVE;
					m_move.z += cosf(-D3DX_PI * 0.25f + CameraRot.y) * PLAYER_MOVE;
					m_fDestAngle = (D3DX_PI * 0.75f) + CameraRot.y;


				}
				else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true || (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && bConnect == true))
				{//下キー(↓キー)が押された
					m_move.x += sinf(-D3DX_PI * 0.75f + CameraRot.y) * PLAYER_MOVE;
					m_move.z += cosf(-D3DX_PI * 0.75f + CameraRot.y) * PLAYER_MOVE;
					m_fDestAngle = (D3DX_PI * 0.25f) + CameraRot.y;


				}
				else
				{//左キー(←キー)のみ
					m_move.x += sinf((-D3DX_PI *  0.5f) + CameraRot.y) * PLAYER_MOVE;
					m_move.z += cosf((-D3DX_PI *  0.5f) + CameraRot.y) * PLAYER_MOVE;

					m_fDestAngle = (D3DX_PI * 0.5f + CameraRot.y);
				}
			}
			//=========================================
			//任意のキー(Dキー)が押されたかどうか    //
			//=========================================
			else if (pInputKeyboard->GetKeyboardPress(DIK_D) == true || (state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && bConnect == true))
			{
				if (pInputKeyboard->GetKeyboardPress(DIK_W) == true || (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && bConnect == true))
				{//上キー(↑キー)が押された
					m_move.x += sinf(D3DX_PI *  0.25f + CameraRot.y) * PLAYER_MOVE;
					m_move.z += cosf(D3DX_PI *  0.25f + CameraRot.y) * PLAYER_MOVE;
					m_fDestAngle = (D3DX_PI * -0.75f) + CameraRot.y;

				}
				else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true || (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && bConnect == true))
				{//下キー(↓キー)が押された
					m_move.x += sinf(D3DX_PI * 0.75f + CameraRot.y) * PLAYER_MOVE;
					m_move.z += cosf(D3DX_PI * 0.75f + CameraRot.y) * PLAYER_MOVE;
					m_fDestAngle = (D3DX_PI * -0.25f) + CameraRot.y;

				}
				else
				{//右キー(→キー)のみ
					m_move.x += sinf((D3DX_PI *  0.5f) + CameraRot.y) * PLAYER_MOVE;
					m_move.z += cosf((D3DX_PI *  0.5f) + CameraRot.y) * PLAYER_MOVE;
					m_fDestAngle = (D3DX_PI * -0.5f) + CameraRot.y;

				}
			}
			//=========================================
			//任意のキー(Wキー)が押されたかどうか    //
			//=========================================
			else if (pInputKeyboard->GetKeyboardPress(DIK_W) == true || (state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && bConnect == true))
			{//↑キーが押されたとき
				m_move.x += sinf(D3DX_PI * 0.0f + CameraRot.y) * PLAYER_MOVE;
				m_move.z += cosf(D3DX_PI * 0.0f + CameraRot.y) * PLAYER_MOVE;

				m_fDestAngle = (D3DX_PI)+CameraRot.y;
			}
			//=========================================
			//任意のキー(Sキー)が押されたかどうか    //
			//=========================================
			else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true || (state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && bConnect == true))
			{//↓キーが押されたとき
				m_move.x -= sinf(D3DX_PI * 0.0f + CameraRot.y) * PLAYER_MOVE;
				m_move.z -= cosf(D3DX_PI * 0.0f + CameraRot.y) * PLAYER_MOVE;

				m_fDestAngle = (D3DX_PI * 0.0f) + CameraRot.y;
			}


			//=============================================
			//任意のキー(エンターキー)が押されたかどうか //
			//=============================================
			if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
			{//エンターキーが押されたとき
			}

			//=============================================
			//任意のキー(スペースキー)が押されたかどうか //
			//=============================================
			if (pInputKeyboard->GetKeyboardTrigger(DIK_SPACE) == true )
			{//スペースキーが押されたとき
					if (m_bAction[PLAYERBOOLTYPE_JUMP] == false)
					{
						m_bAction[PLAYERBOOLTYPE_JUMP] = true;
						m_move.y += JUMP_POWER;
					}
			}

		}//(閉じ)プレイヤーの状態で移動できるか


		if (JoyState == CInputX::INPUT_JOYSTATE_PUSH)
		{
			JoyState = CInputX::INPUT_JOYSTATE_NONE;
		}
		else if (JoyState == CInputX::INPUT_JOYSTATE_NONE)
		{
			JoyState = CInputX::INPUT_JOYSTATE_NOTPUSH;
		}


	 //=============================================
	 // 当たり判定		(敵)
	 //=============================================
	pEnemyTop = (CEnemy*)GetSceneTop(ENEMY_PRIORITY);
	while (pEnemyTop != NULL)//NULLになるまで
	{
		CEnemy *pEnemyNext;
		pEnemyNext = (CEnemy*)pEnemyTop->GetSceneNext(ENEMY_PRIORITY);
		if (pEnemyTop->GetObjType() == CScene::OBJTYPE_ENEMY)
		{
			if (m_state == PLAYERSTATE_NORMAL)
			{
				if (pEnemyTop->CollisionEnemy(&m_pos, &m_move, &m_rot) == true)
				{
					pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT);
					m_bAction[PLAYERBOOLTYPE_DAMAGE] = true;
					m_StateCnt = 30;
					m_state = PLAYERSTATE_DAMAGE;
				}
			}
		}
		pEnemyTop = pEnemyNext;
	}


	 //=============================================
	 // モーションの状態更新
	 //=============================================
	PlayerMotionState(m_bAction);
	PlayerMotion();

	//位置情報を代入
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
	m_pos.z += m_move.z;
	//慣性移動
	m_move.x += (0.0f - m_move.x) * 0.12f;
	m_move.y += (0.0f - m_move.y) * 0.12f;
	m_move.z += (0.0f - m_move.z) * 0.12f;


	//=============================================
	// 当たり判定		(オブジェクト)
	//=============================================
	pObjectTop = (CObject*)GetSceneTop(OBJECT_PRIORITY);
	while (pObjectTop != NULL)//NULLになるまで
	{
		CObject *pObjectNext;
		pObjectNext = (CObject*)pObjectTop->GetSceneNext(OBJECT_PRIORITY);
		if (pObjectTop->GetObjType() == CScene::OBJTYPE_OBJECT)
		{
			if (pObjectTop->CollisionObject(&m_pos, &m_posold, &m_move, m_fRadius) == true)
			{
				m_bAction[PLAYERBOOLTYPE_JUMP] = false;
			}
		}
		pObjectTop = pObjectNext;
	}

	//目的の角度まで
	m_fAngle = m_fDestAngle - m_rot.y;
	//角度調整
	if (m_fAngle > D3DX_PI)
	{
		m_fAngle -= D3DX_PI * 2;
	}

	if (m_fAngle < -D3DX_PI)
	{
		m_fAngle += D3DX_PI * 2;
	}
	//プレイヤーの角度調整
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}

	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
	//角度情報
	m_rot.y += m_fAngle * 0.1f;

	//カメラの角度調整
	if (CameraRot.y > D3DX_PI)
	{
		CameraRot.y -= D3DX_PI * 2;
	}

	if (CameraRot.y < -D3DX_PI)
	{
		CameraRot.y += D3DX_PI * 2;
	}


	//落下
	m_move.y -= GRAVITY_NUM;
	if (m_pos.y <= fHeight)
	{//床の高さ以下だったら
		m_pos.y = fHeight;//高さを地面の高さに
		m_move.y = 0.0f;
		if (m_bAction[PLAYERBOOLTYPE_JUMP] == true)
		{
			if (m_move.x < 0.2f && m_move.x > -0.2f && m_move.z < 0.2f && m_move.z > -0.2f)
			{
				m_bAction[PLAYERBOOLTYPE_LANDING] = true;//着地状態に
				m_bAction[PLAYERBOOLTYPE_JUMP] = false;	//ジャンプ状態を解除
			}
			m_bAction[PLAYERBOOLTYPE_JUMP] = false;//ジャンプできる状態に
		}
		if (m_bAction[PLAYERBOOLTYPE_DAMAGE] == true)
		{// ダメージ中に着地したら
			m_StateCnt = 120;
			m_state = PLAYERSTATE_INVISIBLE;
			m_bAction[PLAYERBOOLTYPE_DAMAGE] = false;				//ダメージ状態を解除
		}
	}


	if (m_pos.x >= SCREEN_LIMIT_MAX_X)
	{
		m_pos.x = SCREEN_LIMIT_MAX_X;
	}
	if (m_pos.x <= SCREEN_LIMIT_MIN_X)
	{
		m_pos.x = SCREEN_LIMIT_MIN_X;

	}



	//==================================================================================================
	// デバッグログ関係
	//==================================================================================================
	//CDebugProc::Print("\n");
	//CDebugProc::Print("[%d]プレイヤー\n", GetPriority());
	//CDebugProc::Print("プレイヤー位置(XYZ): %.1f, %.1f, %.1f：\n",m_pos.x, m_pos.y, m_pos.z);
	//CDebugProc::Print("プレイヤー角度(XYZ): %.1f, %.1f, %.1f：\n", m_rot.x, m_rot.y, m_rot.z);
	//CDebugProc::Print("プレイヤー移動量(XYZ): %.1f, %.1f, %.1f：\n", m_move.x, m_move.y, m_move.z);
	//CDebugProc::Print("状態変化のカウンタ：%d\n", m_StateCnt);

	//switch (m_state)
	//{
	//case PLAYERSTATE_NORMAL://通常状態のとき
	//	CDebugProc::Print("STATE：[0]NORMAL\n");
	//	break;
	//case PLAYERSTATE_DAMAGE://ダメージ状態のとき
	//	CDebugProc::Print("STATE：[1]DAMAGE\n");
	//	break;
	//case PLAYERSTATE_INVISIBLE://無敵のとき
	//	CDebugProc::Print("STATE：[2]INVISIBLE\n");
	//	break;
	//case PLAYERSTATE_APPEAR://出現時のとき
	//	CDebugProc::Print("STATE：[3]PLAYERSTATE_APPEAR\n");
	//	break;
	//}

	//switch (m_Motionstate)
	//{
	//case MOTION_NEUTRAL://ニュートラルのとき
	//	CDebugProc::Print("MOTION：[0]NEUTRAL\n");
	//	break;
	//case MOTION_MOVE://移動のとき
	//	CDebugProc::Print("MOTION：[1]MOVE\n");
	//	break;
	//case MOTION_JUMP://ジャンプのとき
	//	CDebugProc::Print("MOTION：[2]JUMP\n");
	//	break;
	//case MOTION_LANDING://着地のとき
	//	CDebugProc::Print("MOTION：[3]LANDING\n");
	//	break;
	//}

	//CDebugProc::Print("KEY：%d / %d\n", m_nCounterKey, m_aKeyInfo[m_Motionstate].nNumKey);
	//CDebugProc::Print("FRAME：%d / %d (%d / %d)\n", m_nCounterFrame, m_nFrame, m_nCounterAllFrame, m_nAllFrame);
	//CDebugProc::Print("AllFrame：%d\n", m_nCounterAllFrame);

	//以前の位置に今の位置を代入
	m_posold = m_pos;

	//以前のモーションを代入
	m_MotionstateOld = m_Motionstate;

#endif


}

//=============================================================================
// 関数名：プレイヤーシーンの描画処理
// 関数の概要：プレイヤーのマトリックス設定、各パーツの描画
//=============================================================================
void CPlayer::Draw(void)
{
#if 1
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();


	//計算用マトリックス
	D3DXMATRIX mtxrot, mtxtrans;


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxrot,
		m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxrot);


	// 位置を反映
	D3DXMatrixTranslation(&mtxtrans,
		m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxtrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);


	//計算用マトリックス
	D3DXMATRIX mtxPartsrot, mtxPartstrans;

	if (m_bUseDraw == true)
	{
		for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
		{
			//モデルの描画
			m_apModel[nCntParts]->Draw();
		}
	}
#endif
}

//=============================================================================
// 関数名：プレイヤーのモーション
// 関数の概要：ブレンド設定や、モーション
//=============================================================================
void CPlayer::PlayerMotion(void)
{
	//-----------------------------------------------------------------------
	// モーションブレンドの設定
	//-----------------------------------------------------------------------
	if (m_Motionstate != m_MotionstateOld)
	{// モーションのステートが変わったら
		m_nCounterFrame = 0;						// 現在のキーフレームのフレーム数を初期化
		m_nCounterKey = 0;							// キーフレームの初期化
		m_nCounterAllFrame = 0;						// モーションのフレーム数を初期化
		m_nFrame = PLAYER_MOTION_BLEND_FRAME;		// モーションブレンドのフレーム数を設定
		m_nAllFrame = 0;
		for (int nCntKey = 0; nCntKey < m_aKeyInfo[m_Motionstate].nNumKey; nCntKey++)
		{//総フレーム数の計算
			m_nAllFrame += m_aKeyInfo[m_Motionstate].aKey[nCntKey].nFrame;
		}
		m_bMotionBlend = true;						// モーションブレンドをtrueにする
	}

	//-----------------------------------------------------------------------
	// キーフレームの切り替わり
	//-----------------------------------------------------------------------
	if (m_nCounterFrame == m_nFrame)
	{// モーションブレンドの時、フレーム数が最後になったら
		m_nCounterFrame = 0; // 現在のキーフレームのフレーム数を初期化
		m_nFrame = m_aKeyInfo[m_Motionstate].aKey[m_nCounterKey].nFrame; // モーションのフレーム数を設定

		m_nCounterKey++; // キーフレームを進める

		if (m_nCounterKey == m_aKeyInfo[m_Motionstate].nNumKey)
		{// キーフレームが最後まで来たら
			if (m_aKeyInfo[m_Motionstate].nLoop == 0)
			{// ループしない場合
				switch (m_Motionstate)
				{// プレイヤーのモーションが
				case MOTION_JUMP: // ジャンプモーションだったら
					m_nCounterKey--; // キーフレームを戻す
					break;

				case MOTION_LANDING: // 着地モーションだったら
					m_bAction[PLAYERBOOLTYPE_LANDING] = false;
					m_nCounterKey--; // キーフレームを戻す
					break;

				case MOTION_DAMAGE: // ダメージモーションだったら
					m_bAction[PLAYERBOOLTYPE_DAMAGE] = false;// ダメージ中じゃない状態にする
					m_nCounterKey--; // キーフレームを戻す
					break;
				}
			}
			else
			{// ループする
				m_nCounterKey = m_nCounterKey % m_aKeyInfo[m_Motionstate].nNumKey; // キーフレームを進める
				m_nCounterAllFrame = 0; // モーションのフレーム数(ALL)を初期化
			}
		}
	}

	//-----------------------------------------------------------------------
	// モーションの差分を代入
	//-----------------------------------------------------------------------
	if (m_nCounterFrame == 0)
	{// フレーム数が最初の時
		for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
		{// パーツ数分ループ
		 // モーションの位置差分を代入
			m_Motionpos[nCntParts] = (m_aKeyInfo[m_Motionstate].aKey[m_nCounterKey].pos[nCntParts] + m_Partspos[nCntParts]) - m_PartsDefaultpos[nCntParts];
			// モーションの回転差分を代入
			m_Motionrot[nCntParts] = m_aKeyInfo[m_Motionstate].aKey[m_nCounterKey].rot[nCntParts] - m_PartsDefaultrot[nCntParts];

			// 回転の差分を修正する
			// X軸の修正
			if (m_Motionrot[nCntParts].x > D3DX_PI)
			{// +3.14より、超えてしまったら
				m_Motionrot[nCntParts].x -= D3DX_PI * 2.0f;
			}
			else if (m_Motionrot[nCntParts].x < -D3DX_PI)
			{// -3.14より、下回ったら
				m_Motionrot[nCntParts].x += D3DX_PI * 2.0f;
			}
			// Y軸の修正
			if (m_Motionrot[nCntParts].y > D3DX_PI)
			{// +3.14より、超えてしまったら
				m_Motionrot[nCntParts].y -= D3DX_PI * 2.0f;
			}
			else if (m_Motionrot[nCntParts].y < -D3DX_PI)
			{// -3.14より、下回ったら
				m_Motionrot[nCntParts].y += D3DX_PI * 2.0f;
			}
			// Z軸の修正
			if (m_Motionrot[nCntParts].z > D3DX_PI)
			{// +3.14より、超えてしまったら
				m_Motionrot[nCntParts].z -= D3DX_PI * 2.0f;
			}
			else if (m_Motionrot[nCntParts].z < -D3DX_PI)
			{// -3.14より、下回ったら
				m_Motionrot[nCntParts].z += D3DX_PI * 2.0f;
			}
		}
	}

	//----------------------------------------------
	// モーションの動作
	//----------------------------------------------
	for (int nCntParts = 0; nCntParts < MAX_PLAYER_PARTS; nCntParts++)
	{// パーツ数分ループ
	 // 位置差分をフレーム数で割った値を加算していく
		m_PartsDefaultpos[nCntParts].x += m_Motionpos[nCntParts].x / m_nFrame;		// 位置の差分をフレーム数で割った値を加算していく
		m_PartsDefaultpos[nCntParts].y += m_Motionpos[nCntParts].y / m_nFrame;		// 位置の差分をフレーム数で割った値を加算していく
		m_PartsDefaultpos[nCntParts].z += m_Motionpos[nCntParts].z / m_nFrame;		// 位置の差分をフレーム数で割った値を加算していく
		//回転値の代入
		m_apModel[nCntParts]->SetPos(m_PartsDefaultpos[nCntParts]);

		// 回転差分をフレーム数で割った値を加算していく
		m_PartsDefaultrot[nCntParts].x += m_Motionrot[nCntParts].x / m_nFrame;				// 回転の差分をフレーム数で割った値を加算していく
		m_PartsDefaultrot[nCntParts].y += m_Motionrot[nCntParts].y / m_nFrame;				// 回転の差分をフレーム数で割った値を加算していく
		m_PartsDefaultrot[nCntParts].z += m_Motionrot[nCntParts].z / m_nFrame;				// 回転の差分をフレーム数で割った値を加算していく
		//回転値の代入
		m_apModel[nCntParts]->SetRot(m_PartsDefaultrot[nCntParts]);

		// モデルの回転を修正する
		// X軸の修正
		if (m_Partsrot[nCntParts].x > D3DX_PI)
		{// +3.14より、超えてしまったら
			m_Partsrot[nCntParts].x -= D3DX_PI * 2.0f;
		}
		else if (m_Partsrot[nCntParts].x < -D3DX_PI)
		{// -3.14より、下回ったら
			m_Partsrot[nCntParts].x += D3DX_PI * 2.0f;
		}
		// Y軸の修正
		if (m_Partsrot[nCntParts].y > D3DX_PI)
		{// +3.14より、超えてしまったら
			m_Partsrot[nCntParts].y -= D3DX_PI * 2.0f;
		}
		else if (m_Partsrot[nCntParts].y < -D3DX_PI)
		{// -3.14より、下回ったら
			m_Partsrot[nCntParts].y += D3DX_PI * 2.0f;
		}
		// Z軸の修正
		if (m_Partsrot[nCntParts].z > D3DX_PI)
		{// +3.14より、超えてしまったら
			m_Partsrot[nCntParts].z -= D3DX_PI * 2.0f;
		}
		else if (m_Partsrot[nCntParts].z < -D3DX_PI)
		{// -3.14より、下回ったら
			m_Partsrot[nCntParts].z += D3DX_PI * 2.0f;
		}
	}

	m_nCounterFrame++;		// 現在キーフレームのフレーム数を加算
	m_nCounterAllFrame++;	// モーションのフレーム数を加算
}

//=============================================================================
// 関数名：プレイヤーのモーションステートの変更
// 関数の概要：プレイヤーの状態遷移
//=============================================================================
void CPlayer::PlayerMotionState(bool *bAction)
{
	if (bAction[PLAYERBOOLTYPE_DEAD] == true)
	{// 死んだなら
	 m_Motionstate = MOTION_DEATH; // 死亡モーションに設定
	}
	else if (bAction[PLAYERBOOLTYPE_DAMAGE] == true)
	{// ダメージ中なら
	 m_Motionstate = MOTION_DAMAGE; // ダメージモーションに設定
	}
	else if (bAction[PLAYERBOOLTYPE_LANDING] == true)
	{// ダメージ着地
		m_Motionstate = MOTION_LANDING; // 着地モーションに設定
	}
	else if (bAction[PLAYERBOOLTYPE_JUMP] == true)
	{// プレイヤーがジャンプ中なら
		m_Motionstate = MOTION_JUMP; // ジャンプモーションに設定
	}
	else if (m_move.x >  PLAYER_MOVE_DEFAULT ||
		m_move.x < -PLAYER_MOVE_DEFAULT ||
		m_move.z >  PLAYER_MOVE_DEFAULT ||
		m_move.z < -PLAYER_MOVE_DEFAULT)
	{// プレイヤーが動いているとき
		m_Motionstate = MOTION_MOVE; // 移動状態にする
	}
	else
	{// プレイヤーが動いてないとき
		m_Motionstate = MOTION_NEUTRAL; // ニュートラル状態にする
	}
}


//=============================================================================
// 関数名：角度の修正
// 関数の概要：大きければ、引く、小さければ、足す
//=============================================================================
void CPlayer::PlayerRotFixes(D3DXVECTOR3 rot)
{
	// X軸の修正
	if (rot.x > D3DX_PI)
	{// +3.14より、超えてしまったら
		rot.x -= D3DX_PI * 2.0f;
	}
	else if (rot.x < -D3DX_PI)
	{// -3.14より、下回ったら
		rot.x += D3DX_PI * 2.0f;
	}
	// Y軸の修正
	if (rot.y > D3DX_PI)
	{// +3.14より、超えてしまったら
		rot.y -= D3DX_PI * 2.0f;
	}
	else if (rot.y < -D3DX_PI)
	{// -3.14より、下回ったら
		rot.y += D3DX_PI * 2.0f;
	}
	// Z軸の修正
	if (rot.z > D3DX_PI)
	{// +3.14より、超えてしまったら
		rot.z -= D3DX_PI * 2.0f;
	}
	else if (rot.z < -D3DX_PI)
	{// -3.14より、下回ったら
		rot.z += D3DX_PI * 2.0f;
	}
}
//=============================================================================
// 関数名：角度の修正
// 関数の概要：大きければ、引く、小さければ、足す
//=============================================================================
void CPlayer::PlayerRotFixes(float rot)
{
	if (rot > D3DX_PI)
	{// +3.14より、超えてしまったら
		rot -= D3DX_PI * 2.0f;
	}
	else if (rot < -D3DX_PI)
	{// -3.14より、下回ったら
		rot += D3DX_PI * 2.0f;
	}

}

//=============================================================================
// 関数名：位置情報の取得
// 関数の概要：プレイヤーの位置情報を返す
//=============================================================================
D3DXVECTOR3 CPlayer::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// 関数名：角度情報の取得
// 関数の概要：プレイヤーの角度情報を返す
//=============================================================================
D3DXVECTOR3 CPlayer::GetRot(void)
{
	return m_rot;
}

D3DXVECTOR3 CPlayer::GetMove(void)
{
	return m_move;
}


