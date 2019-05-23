//=============================================================================
//
// 敵処理[モデル] [player.cpp]
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
#include "enemy.h"
#include "camera.h"
#include "meshfield.h"
#include "model.h"
#include "player.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_MOVE (0.35f)										//移動量
#define ENEMY_WALK_DEFAULT		(0.15f)							// 敵が動いてる基準の移動量
#define ENEMY_DASH_DEFAULT		(1.5f)							// 敵が動いてる基準の移動量
#define MAX_CHAR (256)											//文字の最大数
#define GRAVITY_NUM (0.8f)										//重力の値
#define PLAYER_MOTION_BLEND_FRAME	(5)							// モーションブレンドのフレーム数
#define JUMP_POWER (20.0f)										//ジャンプ力
#define ENEMY_INDEX (14)
#define MODEL_FILENAME ("data/TEXT/motion_enemy.txt")			//読み込むファイル名


//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CEnemy::m_pTexture = NULL; //共有テクスチャのポインタ
int CEnemy::m_aParent[MAX_ENEMY_PARTS] = {};				//パーツの親
int CEnemy::m_aIndex[MAX_ENEMY_PARTS] = {};				//パーツ番号


//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CEnemy::CEnemy(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{
}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CEnemy::~CEnemy()
{
}

//=============================================================================
// 関数名：使用するテクスチャの読み込み
// 関数の概要：--
//=============================================================================
HRESULT CEnemy::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/skeleton.png", &m_pTexture);

	return S_OK;

}

//=============================================================================
// 関数名：使用したテクスチャの破棄
// 関数の概要：--
//=============================================================================
void CEnemy::Unload(void)
{

	m_pTexture->Release();
	m_pTexture = NULL;

}


//=============================================================================
// 関数名：敵シーンの生成
// 関数の概要：敵シーンを生成する
//=============================================================================
CEnemy * CEnemy::Create(D3DXVECTOR3 pos, ENEMYTYPE type)
{
	CEnemy  *pEnemy;
	pEnemy = new CEnemy;//シーンクラスの生成
	if (pEnemy != NULL)
	{
		pEnemy->m_pos = pos;
		pEnemy->m_Startpos = pos;
		pEnemy->m_type = type;
		pEnemy->m_fHeight = 25.0f;
		pEnemy->m_bAction[ENEMYBOOLTYPE_JUMP] = false;
		pEnemy->Init();

	}
	return pEnemy;
}

//=============================================================================
// 関数名：敵シーンの初期化処理
// 関数の概要：敵情報の設定
//=============================================================================
HRESULT CEnemy::Init(void)
{

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
								m_aIndex[nCntModel] = nIndex + ENEMY_INDEX;
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
						m_apModel[nCntModel] = CModel::Create(m_Partspos[nCntModel], m_Partsrot[nCntModel], m_aIndex[nCntModel], CModel::MODELTYPE_ENEMY);
						if (nCntModel == 0)
						{
							m_apModel[nCntModel]->SetParent(NULL);
						}
						else
						{
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
	for (int nCntKey = 0; nCntKey < m_aKeyInfo[m_Motionstate].nNumKey; nCntKey++)
	{//総フレーム数の計算
		m_nAllFrame += m_aKeyInfo[m_Motionstate].aKey[nCntKey].nFrame;
	}
	for (int nCntParts = 0; nCntParts < MAX_ENEMY_PARTS; nCntParts++)
	{// パーツ数分ループ
		 // モデルの位置をニュートラルモーションに初期化
		m_PartsDefaultpos[nCntParts] = m_Partspos[nCntParts] + m_aKeyInfo[m_Motionstate].aKey[0].pos[nCntParts];
		m_PartsDefaultrot[nCntParts] = m_aKeyInfo[m_Motionstate].aKey[0].rot[nCntParts];

	}

	//2Dシーンのオブジェクトの設定
	SetObjType(CScene::OBJTYPE_ENEMY);
	return S_OK;
}
//=============================================================================
// 関数名：敵シーンの終了処理
// 関数の概要：メッシュ、マテリアル、自身の破棄
//=============================================================================
void CEnemy::Uninit(void)
{
	for (int nCntParts = 0; nCntParts < MAX_ENEMY_PARTS; nCntParts++)
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
// 関数名：敵シーンの更新処理
// 関数の概要：敵の移動、パーツのモーション更新
//=============================================================================
void CEnemy::Update(void)
{

	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//カメラの取得
	CCamera *pCamera;
	pCamera = CManager::GetCamera();

	//カメラの角度取得
	D3DXVECTOR3 CameraRot = pCamera->GetRot();

	//メッシュフィールド取得
	CMeshField *pMeshField;
	pMeshField = CGame::GetSceneMeshField();

	//敵の取得
	CPlayer *pPlayer;
	pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 playerpos = pPlayer->GetPos();

	switch (m_type)
	{
	case ENEMYTYPE_LOITERING_Z:
		if (m_pos.z <= m_Startpos.z - 150.0f)
		{//手前側に移動
			m_bSwitch = false;
		}
		if (m_pos.z >= m_Startpos.z + 150.0f)
		{//奥側に移動
			m_bSwitch = true;
		}
		if (m_bSwitch == false)
		{
			m_move.z += 0.18f;
			m_fDestAngle = (D3DX_PI);
		}
		if (m_bSwitch == true)
		{
			m_move.z -= 0.18f;
			m_fDestAngle = (D3DX_PI * 0.0f);
		}
		break;
	case ENEMYTYPE_LOITERING_X:
		if (m_pos.x <= m_Startpos.x - 150.0f)
		{//手前側に移動
			m_bSwitch = false;
		}
		if (m_pos.x >= m_Startpos.x + 150.0f)
		{//奥側に移動
			m_bSwitch = true;
		}
		if (m_bSwitch == false)
		{
			m_move.x += 0.18f;
			m_fDestAngle = (D3DX_PI * -0.5f);

		}
		if (m_bSwitch == true)
		{
			m_move.x -= 0.18f;
			m_fDestAngle = (D3DX_PI * 0.5f);
		}
		break;
	case ENEMYTYPE_CHASE:
		if (FindPlayer(&playerpos, 12.0f) == true)
		{//プレイヤーが範囲内に入ったら
			D3DXVECTOR3 DiffPos;

			//プレイヤーと敵のベクトルを算出
			DiffPos = m_pos - playerpos;
			D3DXVec3Normalize(&DiffPos, &DiffPos);

			//ベクトル方向に向かって移動
			m_move.x -= DiffPos.x * 0.21f;
			m_move.z -= DiffPos.z * 0.21f;

			//敵の角度設定
			m_fDestAngle = atan2f(m_pos.x - playerpos.x, m_pos.z - playerpos.z);
		}
		else
		{//プレイヤーが範囲外に出たら
			D3DXVECTOR3 DiffPos;
			//敵の初期位置とのベクトルを算出
			DiffPos = m_pos - m_Startpos;
			D3DXVec3Normalize(&DiffPos, &DiffPos);

			if (m_pos.x >= m_Startpos.x - 5.0f && m_pos.x <= m_Startpos.x + 5.0f &&
				m_pos.z >= m_Startpos.z - 5.0f && m_pos.z <= m_Startpos.z + 5.0f)
			{//初期位置に戻ったら
				m_move.x = 0.0f;
				m_move.z = 0.0f;

			}
			else
			{//初期位置に戻っていないとき
				//ベクトル方向に向かって移動
				m_move.x -= DiffPos.x * 0.15f;
				m_move.z -= DiffPos.z * 0.15f;
			}

			//敵の角度設定
			m_fDestAngle = atan2f(m_pos.x - m_Startpos.x, m_pos.z - m_Startpos.z);
		}
		break;
	case ENEMYTYPE_TEST:

		break;

	}


	//高さを取得
	float fHeight = pMeshField->GetHeight(m_pos);


	//---------------------------------------------------
	// モーションの状態更新
	//-------------------------------------------------
	EnemyMotionState(m_bAction);
	EnemyMotion();

	//位置情報を代入
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
	m_pos.z += m_move.z;
	//慣性移動
	m_move.x += (0.0f - m_move.x) * 0.12f;
	m_move.y += (0.0f - m_move.y) * 0.12f;
	m_move.z += (0.0f - m_move.z) * 0.12f;

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
	//敵の角度調整
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
	if (m_pos.y <= 0.0)
	{
		m_pos.y = 0.0f;//高さを地面の高さに
		m_move.y = 0.0f;
		if (m_bAction[ENEMYBOOLTYPE_JUMP] == true)
		{
			if (m_move.x < 1.0f && m_move.x > -1.0f && m_move.z < 1.0f && m_move.z > -1.0f)
			{
				m_bAction[ENEMYBOOLTYPE_LANDING] = true;//着地状態に
			}
			m_bAction[ENEMYBOOLTYPE_JUMP] = false;//ジャンプできる状態に

		}
	}
	if (m_pos.y <= fHeight)
	{//床の高さ以下だったら
		m_pos.y = fHeight;//高さを地面の高さに
		m_move.y = 0.0f;
		if (m_bAction[ENEMYBOOLTYPE_JUMP] == true)
		{
			if (m_move.x < 0.1f && m_move.x > -0.1 || m_move.z < 0.1f && m_move.z > -0.1)
			{
				m_bAction[ENEMYBOOLTYPE_LANDING] = true;//着地状態に
			}			m_bAction[ENEMYBOOLTYPE_JUMP] = false;//ジャンプできる状態に
		}
	}
	else
	{//空中にいるとき
		m_bAction[ENEMYBOOLTYPE_JUMP] = true;//ジャンプできない状態に
	}

	//CDebugProc::Print("m_pos: %.1f,%.1f, %.1f\n", m_pos.x, m_pos.y, m_pos.z);


	//以前の位置を今の位置に代入
	m_posold = m_pos;


	m_MotionstateOld = m_Motionstate;
}

//=============================================================================
// 関数名：敵シーンの描画処理
// 関数の概要：敵のマトリックス設定、各パーツの描画
//=============================================================================
void CEnemy::Draw(void)
{
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

	for (int nCntParts = 0; nCntParts < MAX_ENEMY_PARTS; nCntParts++)
	{
		//モデルの描画
		m_apModel[nCntParts]->Draw();
	}
}

//=============================================================================
// 関数名：敵のモーション
// 関数の概要：ブレンド設定や、モーション
//=============================================================================
void CEnemy::EnemyMotion(void)
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
				{// 敵のモーションが
				case MOTION_JUMP: // ジャンプモーションだったら
					m_nCounterKey--; // キーフレームを戻す
					break;

				case MOTION_LANDING: // 着地モーションだったら
					m_bAction[ENEMYBOOLTYPE_LANDING] = false;
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
		for (int nCntParts = 0; nCntParts < MAX_ENEMY_PARTS; nCntParts++)
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
	for (int nCntParts = 0; nCntParts < MAX_ENEMY_PARTS; nCntParts++)
	{// パーツ数分ループ
	 // 位置差分をフレーム数で割った値を加算していく
		m_PartsDefaultpos[nCntParts].x += m_Motionpos[nCntParts].x / m_nFrame;		// 位置の差分をフレーム数で割った値を加算していく
		m_PartsDefaultpos[nCntParts].y += m_Motionpos[nCntParts].y / m_nFrame;		// 位置の差分をフレーム数で割った値を加算していく
		m_PartsDefaultpos[nCntParts].z += m_Motionpos[nCntParts].z / m_nFrame;		// 位置の差分をフレーム数で割った値を加算していく
		//回転値の代入
		m_apModel[nCntParts]->SetPos(m_PartsDefaultpos[nCntParts]);


		// 回転差分をフレーム数で割った値を加算していく
		m_PartsDefaultrot[nCntParts].x += m_Motionrot[nCntParts].x / m_nFrame;		// 回転の差分をフレーム数で割った値を加算していく
		m_PartsDefaultrot[nCntParts].y += m_Motionrot[nCntParts].y / m_nFrame;		// 回転の差分をフレーム数で割った値を加算していく
		m_PartsDefaultrot[nCntParts].z += m_Motionrot[nCntParts].z / m_nFrame;		// 回転の差分をフレーム数で割った値を加算していく
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
// 関数名：敵のモーションステートの変更
// 関数の概要：敵の状態遷移
//=============================================================================
void CEnemy::EnemyMotionState(bool *bAction)
{
	if (bAction[ENEMYBOOLTYPE_LANDING] == true)
	{// ダメージ着地
		//m_Motionstate = MOTION_LANDING; // 着地モーションに設定
		m_Motionstate = MOTION_MOVE; // 着地モーションに設定
	}

	else if (bAction[ENEMYBOOLTYPE_JUMP] == true)
	{// 敵がジャンプ中なら
		//m_Motionstate = MOTION_JUMP; // ジャンプモーションに設定
		m_Motionstate = MOTION_MOVE; // ジャンプモーションに設定
	}
	else if (m_move.x > ENEMY_WALK_DEFAULT ||
		m_move.x < -ENEMY_WALK_DEFAULT ||
		m_move.z >  ENEMY_WALK_DEFAULT ||
		m_move.z < -ENEMY_WALK_DEFAULT)
	{// 敵が動いているとき
		m_Motionstate = MOTION_MOVE; // 移動状態にする
	}
	else
	{// 敵が動いてないとき
		m_Motionstate = MOTION_NEUTRAL; // ニュートラル状態にする
	}
}


//=============================================================================
// 関数名：角度の修正
// 関数の概要：大きければ、引く、小さければ、足す
//=============================================================================
void CEnemy::EnemyRotFixes(D3DXVECTOR3 *rot)
{
	// X軸の修正
	if (rot->x > D3DX_PI)
	{// +3.14より、超えてしまったら
		rot->x -= D3DX_PI * 2.0f;
	}
	else if (rot->x < -D3DX_PI)
	{// -3.14より、下回ったら
		rot->x += D3DX_PI * 2.0f;
	}
	// Y軸の修正
	if (rot->y > D3DX_PI)
	{// +3.14より、超えてしまったら
		rot->y -= D3DX_PI * 2.0f;
	}
	else if (rot->y < -D3DX_PI)
	{// -3.14より、下回ったら
		rot->y += D3DX_PI * 2.0f;
	}
	// Z軸の修正
	if (rot->z > D3DX_PI)
	{// +3.14より、超えてしまったら
		rot->z -= D3DX_PI * 2.0f;
	}
	else if (rot->z < -D3DX_PI)
	{// -3.14より、下回ったら
		rot->z += D3DX_PI * 2.0f;
	}
}
//=============================================================================
// 関数名：角度の修正
// 関数の概要：大きければ、引く、小さければ、足す
//=============================================================================
void CEnemy::EnemyRotFixes(float *rot)
{
	if (*rot > D3DX_PI)
	{// +3.14より、超えてしまったら
		*rot -= D3DX_PI * 2.0f;
	}
	else if (*rot < -D3DX_PI)
	{// -3.14より、下回ったら
		*rot += D3DX_PI * 2.0f;
	}

}

//=============================================================================
// 関数名：プレイヤーの探索
// 関数の概要：プレイヤーが半径に入ってきたらtrueを返す
//=============================================================================
bool CEnemy::FindPlayer(D3DXVECTOR3 *pos, float fRange)
{
	bool bfind = false;

	float fLength = powf((pos->x - m_pos.x)*(pos->x - m_pos.x) + (pos->z - m_pos.z)*(pos->z - m_pos.z), 0.5f);	//プレイヤーとの距離
	float fFind = powf(fRange, 2.0f);	//半径
	//CDebugProc::Print("fFind:%.1f\n", fFind);
	//CDebugProc::Print("fLength:%.1f\n", fLength);

	if (fLength < fFind)
	{//対象の頂点が範囲内だったら
		bfind = true;
		//CDebugProc::Print("がいこつにみつかった！\n");
	}
	else
	{
		bfind = false;
	}


	return bfind;
}


//=============================================================================
// 関数名：敵との当たり判定
// 関数の概要：プレイヤーが半径に入ってきたらtrueを返す
//=============================================================================
bool CEnemy::CollisionEnemy(D3DXVECTOR3 *pos, D3DXVECTOR3 *move, D3DXVECTOR3 *rot)
{
	bool bCol = false;

	float fLength = powf((pos->x - m_pos.x)*(pos->x - m_pos.x) + (pos->z - m_pos.z)*(pos->z - m_pos.z), 0.5f);	//プレイヤーとの距離
	float fRadius = powf(30.0f, 1.0f);	//半径
	//CDebugProc::Print("当たり判定の半径:%.1f\n", fRadius);
	//CDebugProc::Print("プレイヤーとの距離:%.1f\n", fLength);

	if (fLength < fRadius && pos->y <= m_pos.y + m_fHeight && pos->y >= m_pos.y)
	{//対象の頂点が範囲内だったら
		bCol = true;
		float fAttackAngle = atan2f(pos->x - m_pos.x,
			pos->z - m_pos.z);// 当たった角度を計算
		move->x = sinf((D3DX_PI *  fAttackAngle)) * 25.0f;
		move->y = 20.0f;
		move->z = cosf((D3DX_PI *  fAttackAngle)) * 25.0f;
	}
	else
	{
		bCol = false;

	}
	return bCol;
}


//=============================================================================
// 関数名：位置情報の取得
// 関数の概要：プレイヤーの位置情報を返す
//=============================================================================
D3DXVECTOR3 CEnemy::GetPos(void)
{
	return m_pos;
}
//=============================================================================
// 関数名：角度情報の取得
// 関数の概要：敵の角度情報を返す
//=============================================================================
D3DXVECTOR3 CEnemy::GetRot(void)
{
	return m_rot;
}

