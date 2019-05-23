//=============================================================================
//
// プレイヤーの処理 [player.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "player.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "debugproc.h"
#include "scene3D.h"
#include "meshfield.h"
#include "model.h"
#include "game.h"
#include "object.h"
#include "loadfilefunction.h"
#include "objBillboard.h"
#include "particle.h"

#define MOTION_TXT_NAME "data\\TEXT\\motion.txt"
#define PLAYER_APPEAR (500)
#define PLAYER_SPEED (30.0f)
#define QUATANION_ROT (0.05f)
//=============================================================================
// グローバル変数
//=============================================================================
int g_nNumModel = 2;			//モデルの総数
char g_aFilenameModel[MAX_PLAYER_MODEL][256];	//

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPD3DXMESH			CPlayer::m_pMesh[MAX_PLAYER_MODEL] = {};
LPD3DXBUFFER		CPlayer::m_pBuffMat[MAX_PLAYER_MODEL] = {};
DWORD				CPlayer::m_nNumMat[MAX_PLAYER_MODEL] = {};
LPDIRECT3DTEXTURE9	CPlayer::m_pTexture = NULL;
//=============================================================================
// プレイヤーのコンストラクタ
//=============================================================================
CPlayer::CPlayer() : CScene(1,OBJTYPE_NONE)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//位置の初期値
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//過去の位置の初期値
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向きの初期値
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動量初期値
	m_bJump = false;								//ジャンプしていない状態
	m_motionType = MOTION_NEUTRAL;					//モーションを初期化
	m_nCountFrame = 0;								//フレーム数を初期化
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
	{//モデルの数だけ繰り返し
		m_apModel[nCntModel] = NULL;
	}
	D3DXMatrixIdentity(&m_mtxRot);					// ワールドマトリックスの初期化
	D3DXQuaternionIdentity(&m_quat);				//クオータニオンを初期化
	m_vecAxis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//ベクトルを初期化
	m_fValueRot = 0.0f;								//向きを初期化
	m_state = PLAYER_STATE_START;					//プレイヤーを始める状態にする
	m_bHitFlag = false;
	m_nAppearTimer = 0;

}
//=============================================================================
// プレイヤーのデストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// オブジェクトのテクスチャ読み込み
//=============================================================================
HRESULT CPlayer::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\earth000.jpg",
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// オブジェクトのテクスチャ破棄
//=============================================================================
void CPlayer::Unload(void)
{
	for (int nCntMesh = 0; nCntMesh < MAX_PLAYER_MODEL; nCntMesh++)
	{//モデルの数だけ繰り返し
		// メッシュの開放
		if (m_pMesh[nCntMesh] != NULL)
		{
			m_pMesh[nCntMesh]->Release();
			m_pMesh[nCntMesh] = NULL;
		}
	}

	for (int nCntMat = 0; nCntMat < MAX_PLAYER_MODEL; nCntMat++)
	{//モデルの数だけ繰り返し
		// マテリアルの開放
		if (m_pBuffMat[nCntMat] != NULL)
		{
			m_pBuffMat[nCntMat]->Release();
			m_pBuffMat[nCntMat] = NULL;
		}
	}
}

//=============================================================================
//プレイヤーの生成
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos)
{
	CPlayer *pPlayer = NULL;					//プレイヤーのポインタ
	if (pPlayer == NULL)
	{
		pPlayer = new CPlayer;					//シーンの動的確保
		pPlayer->Init();						//初期化処理
		pPlayer->m_pos = pos;
	}
	return pPlayer;					//値を返す
}

//=============================================================================
// プレイヤーの初期化処理
//=============================================================================
HRESULT CPlayer::Init(void)
{
	SetObjType(CScene::OBJTYPE_PLAYER);
	//モーション情報を読み込み
	LoadMotion();
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_VtxMax = D3DXVECTOR3(30.0f*m_scale.x, 30.0f*m_scale.y, 30.0f*m_scale.z);	//頂点座標の最大値初期化
	m_VtxMin = D3DXVECTOR3(-30.0f*m_scale.x, -30.0f*m_scale.y, -30.0f*m_scale.z);	//頂点座標の最小値初期化

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//位置の初期化
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向きの初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動量初期値
	m_bJump = false;								//ジャンプしていない状態
	m_nKey = 0;										//現在のキーを初期化
	m_nNumKey = 2;									//キーの総数を初期化
	m_nCountFrame = 0;								//カウントフレームを初期化
	return S_OK;
}

//=============================================================================
// プレイヤーの終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	for (int nCntModel = 0 ; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
	{//モデルの数だけ繰り返し
		if (m_apModel[nCntModel] != NULL)
		{
			//終了処理
			m_apModel[nCntModel]->Uninit();
			delete m_apModel[nCntModel];
			m_apModel[nCntModel] = NULL;
		}
	}

	Release();
}

//=============================================================================
// プレイヤーの更新処理
//=============================================================================
void CPlayer::Update(void)
{
	//現在の位置情報を過去の位置に代入
	m_posOld = m_pos;

	//現在のモーションを過去のモーション情報に代入
	m_OldMotion = m_motionType;

	//移動処理
	Move();

	//当たり判定処理
	Collision();

	//モーションの更新処理
	//UpdateMotion();

	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

#ifdef _DEBUG
	CDebugProc::Print(1, "プレイヤーの位置  : x[%.1f],y[%.1f],z[%.1f]\n", m_pos.x, m_pos.y, m_pos.z);
	CDebugProc::Print(1, "プレイヤーの移動量  : %f",m_move.y);

#endif
}

//=============================================================================
// プレイヤーの描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans,mtxScale;					//計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//
	D3DXQuaternionRotationAxis(&m_quat, &m_vecAxis, m_fValueRot);

	//
	D3DXMatrixRotationQuaternion(&mtxRot, &m_quat);

	// 回転を反映
	D3DXMatrixMultiply(&mtxRot, &m_mtxRot, &mtxRot);

	//保存
	m_mtxRot = mtxRot;

	//
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

	//// 回転を反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot,
	//	m_rot.y, m_rot.x, m_rot.z);

	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 大きさの反映
	D3DXMatrixScaling(&mtxScale,
		m_scale.x, m_scale.y, m_scale.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);


	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	D3DXMATRIX	mtxShadow;		//シャドウマトリックス
	D3DXPLANE	planeField;		//平面情報
	D3DXVECTOR4 vecLight;		//ライト情報
	D3DXVECTOR3	pos, normal;	//位置、法線

								//減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP,
		D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND,
		D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,
		D3DBLEND_ONE);

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//シャドウマトリックスを初期化
	D3DXMatrixIdentity(&mtxShadow);

	//ライトの設定
	vecLight = D3DXVECTOR4(0.25f, 0.87f, -0.44f, 0.0f);

	//位置の設定
	pos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//法線の設定
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//位置と法線から平面を作成する
	D3DXPlaneFromPointNormal(&planeField, &pos, &normal);

	//平面の射影
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);

	//ワールドマトリックスと掛け合わせる
	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
	{//モデルの数だけ繰り返し
	 //テクスチャ無効
		pDevice->SetTexture(0, NULL);
		// モデル(パーツ)の描画
		m_pMesh[nCntModel]->DrawSubset(0);
		//描画処理
		//m_apModel[nCntModel]->Draw();
	}

	//元の設定に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP,
		D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND,
		D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,
		D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
	{
		//モデルの描画
		m_apModel[nCntModel]->Draw();
	}
}

//=============================================================================
// プレイヤーのモーション更新処理
//=============================================================================
void CPlayer::UpdateMotion(void)
{
	KEY *pKey, *pKeyNext;		// キーの情報
	float fRateMotion;			// 相対値
	float fDiffMotion;			// 差分
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き

	/*if (m_apMotionInfo[m_motionType].nNumKey <= m_nKey)
	{
		m_nKey = 0;
		m_nCountFrame = 0;
	}*/

	if (m_OldMotion != m_motionType)
	{//前回のモーションを違う場合
		m_nKey = 0;			//現在のキーを初期化
		m_nCountFrame = 0;	//カウントフレームを初期化
	}
	for (int nCntModel = 0; nCntModel < g_nNumModel; nCntModel++)
	{//モデルの数だけ繰り返し
		if (m_apModel[nCntModel] != NULL)
		{//モデルがある場合
			pKey = &m_apMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[nCntModel];
			pKeyNext = &m_apMotionInfo[m_motionType].aKeyInfo[(m_nKey + 1) % /*2*/m_apMotionInfo[m_motionType].nNumKey].aKey[nCntModel];

			//位置座標X
			fRateMotion = (float)(m_nCountFrame) / (float)m_apMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame;	// 相対値の取得
			fDiffMotion = pKeyNext->fPosX - pKey->fPosX;														// 差分の取得
			pos.x = m_apModel[nCntModel]->GetPos().x +  pKey->fPosX + (fDiffMotion * fRateMotion);

			//位置座標Y
			fDiffMotion = pKeyNext->fPosY - pKey->fPosY;								// 差分の取得
			pos.y = m_apModel[nCntModel]->GetPos().y + pKey->fPosY + (fDiffMotion * fRateMotion);

			//位置座標Z
			fDiffMotion = pKeyNext->fPosZ - pKey->fPosZ;								// 差分の取得
			pos.z = m_apModel[nCntModel]->GetPos().z + pKey->fPosZ + (fDiffMotion * fRateMotion);

			//向き座標X
			fDiffMotion = pKeyNext->fRotX - pKey->fRotX;								// 差分の取得
			rot.x = pKey->fRotX + (fDiffMotion * fRateMotion);

			//向き座標Y
			fDiffMotion = pKeyNext->fRotY - pKey->fRotY;								// 差分の取得
			rot.y = pKey->fRotY + (fDiffMotion * fRateMotion);

			//向き座標Z
			fDiffMotion = pKeyNext->fRotZ - pKey->fRotZ;								// 差分の取得
			rot.z = pKey->fRotZ + (fDiffMotion * fRateMotion);

			//位置の設置処理
			//m_apModel[nCntModel]->SetPos(pos);
			//向きの設置処理
			m_apModel[nCntModel]->SetRot(rot);

		}
	}

		if (m_nKey <= m_apMotionInfo[m_motionType].nNumKey - 1)
		{//現在のキーが対象のキーの総数より小さい場合カウントフレームを加算
			m_nCountFrame++;
		}
		if (m_nCountFrame >= m_apMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame)
		{//現在のフレーム数が対象のキーフレーム数以上の場合
			//m_nKey = (m_nKey + (m_apMotionInfo[m_motionType].nNumKey - 1)) % m_apMotionInfo[m_motionType].nNumKey;
			//現在のキーの切り替え
			m_nKey = (m_nKey + 1) % m_apMotionInfo[m_motionType].nNumKey;
			if (m_nKey == m_apMotionInfo[m_motionType].nNumKey - 1 && m_apMotionInfo[m_motionType].bLoop == false)
			{//現在のキーが対象のキー総数に達しかつループしない場合

				if (m_motionType == MOTION_JUMP && m_bJump == true)
				{//
					//m_nKey = m_apMotionInfo[m_motionType].nNumKey - 1;
				}

			}
			//カウントフレームを初期化
			m_nCountFrame = 0;
		}
#if 0
	switch (m_apMotionInfo[m_motionType].bLoop)
	{
	case true:
		m_nCountFrame++;	//カウントフレームの加算

		if (m_nCountFrame >= m_apMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame)
		{//現在のフレーム数が対象のキーフレーム数以上の場合
			if (m_nKey == m_apMotionInfo[m_motionType].nNumKey - 1)
			{//現在のキーが対象のキー総数分に達した場合０番目のキー番号に戻す

				m_nKey = 0;
				//m_nKey = m_apMotionInfo[m_motionType].nNumKey - 1;
			}
			else
			{//キーの加算
				m_nKey++;
			}
			//カウントフレーム初期化
			m_nCountFrame = 0;
		}
		break;
	case false:
		if (m_nKey <= m_apMotionInfo[m_motionType].nNumKey - 1)
		{//現在のキーが対象のキーの総数より小さい場合カウントフレームを加算
			m_nCountFrame++;
		}
		if (m_nCountFrame >= m_apMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame)
		{//現在のフレーム数が対象のキーフレーム数以上の場合
			if (m_apMotionInfo[m_motionType].nNumKey - 1 > m_nKey)
			{//現在のキーが対象のキーの総数より小さい場合キーを加算
				m_nKey++;

				m_nCountFrame = 0;
			}
			else
			{
				//コマンドの取得
				CCommand *pCommand;
				pCommand = CGame::GetCommad();
				//ニュートラルに切り替え
				m_motionType = MOTION_NEUTRAL;
				//コマンドを実行していない状態にする
				pCommand->SetCommandState(pCommand->COMMAND_NONE);
				m_nCountFrame = 0;

			}
		}
		break;
	}
#endif
#ifdef _DEBUG
	for (int nCntRot = 0; nCntRot < MAX_PLAYER_MODEL; nCntRot++)
	{//パーツの数だけ繰り返し
		//位置と向きを表示
		//CDebugProc::Print(1, "Pos[%d]  : x[%.1f],y[%.1f],z[%.1f]\n", nCntRot, m_apModel[nCntRot]->GetPos().x, m_apModel[nCntRot]->GetPos().y, m_apModel[nCntRot]->GetPos().z);
		//CDebugProc::Print(1, "Rot[%d]  : x[%.1f],y[%.1f],z[%.1f]\n", nCntRot, m_apModel[nCntRot]->GetRot().x, m_apModel[nCntRot]->GetRot().y, m_apModel[nCntRot]->GetRot().z);

	}
	CDebugProc::Print(1, "プレイヤー移動[W,A,S,D]\n");
	CDebugProc::Print(1, "Key[%d / %d]\n",m_nKey,m_apMotionInfo[m_motionType].nNumKey);
	CDebugProc::Print(1, "Frame[%d / %d]\n",m_nCountFrame, m_apMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame);
	CDebugProc::Print(1, "モーション[%d]\n", m_motionType);
#endif
}
//=============================================================================
// プレイヤーの移動処理
//=============================================================================
void CPlayer::Move(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	//ジョイパッドの取得
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetJoypad();

	//カメラの取得
	CCamera *pCamera;
	pCamera = CManager::GetCamera();
	//カメラの向き取得
	D3DXVECTOR3 cameraRot = pCamera->GetRot();

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	//前のフレームで当たっていたら
	if (m_bHitFlag == true)
	{
		//CParticle::Create(m_pos, m_rot, 50, CParticle::PARTICLE_TYPE1);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_DAMAGE);

		m_nAppearTimer = PLAYER_APPEAR;
		m_pos.z -= 100.0f;
		m_bHitFlag = false;
	}

	if (m_state == PLAYER_STATE_START)
	{//プレイヤーが動ける状態の場合
		//常に前進
		m_vecAxis = D3DXVECTOR3(1.0f, 0.0f, 0.0f);			//軸の設定
		m_fValueRot = D3DX_PI * QUATANION_ROT;				//回転量
		float fAngle = (2 * D3DX_PI * PLAYER_SPEED);		//円周
		m_pos.z += (m_fValueRot / (2 * D3DX_PI)) * fAngle;	//位置加算

		//if (pInputKeyboard->GetPress(DIK_W) == true)
		//{
		//	m_vecAxis = D3DXVECTOR3(1.0f, 0.0f, 0.0f);			//軸の設定
		//	m_fValueRot = D3DX_PI * QUATANION_ROT;				//回転量
		//	float fAngle = (2 * D3DX_PI * PLAYER_SPEED);		//円周
		//	m_pos.z += (m_fValueRot / (2 * D3DX_PI)) * fAngle;	//位置加算
		//}
		if (pInputKeyboard->GetPress(DIK_A) == true
			||pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LSTICK_LEFT) == true)
		{//左移動
			m_vecAxis = D3DXVECTOR3(0.0f, 0.0f, 1.0f);			//軸の設定
			m_fValueRot = D3DX_PI * QUATANION_ROT;				//回転量
			float fAngle = (2 * D3DX_PI * PLAYER_SPEED);		//円周
			m_pos.x -= (m_fValueRot / (2 * D3DX_PI)) * fAngle;	//位置加算
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true
			|| pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LSTICK_RIGHT) == true)
		{//右移動
			m_vecAxis = D3DXVECTOR3(0.0f, 0.0f, -1.0f);			//軸の設定
			m_fValueRot = D3DX_PI * QUATANION_ROT;				//回転量
			float fAngle = (2 * D3DX_PI * PLAYER_SPEED);		//円周
			m_pos.x += (m_fValueRot / (2 * D3DX_PI)) * fAngle;	//位置加算
		}

		//if (pInputKeyboard->GetTrigger(DIK_J) == true
		//	|| pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_LB) == true)
		//{//プレイヤー縮小
		//	if (m_scale.x > 0.5f && m_scale.y > 0.5f && m_scale.z > 0.5f)
		//	{//大きさが0.5より大きい場合
		//		pSound->PlaySound(CSound::SOUND_LABEL_SE_SYUKUSYOU);

		//		m_scale = D3DXVECTOR3(m_scale.x - 0.5f, m_scale.y - 0.5f, m_scale.z - 0.5f);
		//	}
		//}
		//if (pInputKeyboard->GetTrigger(DIK_K) == true
		//	|| pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_RB) == true)
		//{//プレイヤー拡大
		//	if (m_scale.x < 2.0f && m_scale.y < 2.0f && m_scale.z < 2.0f)
		//	{//大きさが2.0より小さい場合
		//		pSound->PlaySound(CSound::SOUND_LABEL_SE_KAKUDAI);

		//		m_scale = D3DXVECTOR3(m_scale.x + 0.5f, m_scale.y + 0.5f, m_scale.z + 0.5f);
		//	}
		//}
	}
	CDebugProc::Print(1, "大きさ x:%f y:%f z:%f\n", m_scale.x, m_scale.y, m_scale.z);

	if (m_bJump == false)
	{//ジャンプしていない場合
		//移動量初期化
		m_move.y = 0.0f;
		if (pInputKeyboard->GetTrigger(DIK_SPACE) == true
			|| pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_A) == true)
		{//Spaceキーが押された場合
			pSound->PlaySound(CSound::SOUND_LABEL_SE_JUMP);

			//ジャンプモーションに切り替え
			m_motionType = MOTION_JUMP;
			//現在のキーを初期化
			m_nKey = 0;
			//移動量を加算
			m_move.y -= (cosf(D3DX_PI * 1.0f) * 8.0f);				//ジャンプしている状態にする
			m_bJump = true;
		}
	}

	if (m_pos.x <= -75.0f)
	{
		m_pos.x = -75.0f;
	}
	else if (m_pos.x >= 80.0f)
	{
		m_pos.x = 80.0f;
	}
	if (m_pos.z >= 5900.0f)
	{//プレイヤーがゴール地点に到達した場合
		m_pos.z = 5900.0f;			//位置固定
		m_state = PLAYER_STATE_GOAL;//ゴールした状態にする
		m_fValueRot = 0.0f;			//初期化
	}
	//重力
	m_move.y -= cosf(D3DX_PI * 0) * 0.5f;

	//移動加算処理
	m_pos.y += m_move.y;

}

//=============================================================================
// プレイヤーの当たり判定処理
//=============================================================================
void CPlayer::Collision(void)
{
	// 配置物と地面へのポインタ取得する
	CScene *pScene = NULL;               // オブジェクトのポインタ
	CScene *pSceneNext = NULL;           // 次のオブジェクトのポインタ
	CObject *pObject = NULL;             // 配置物のポインタ
	CMeshField *pMeshField = NULL;		// メッシュフィールドのポインタ
	CItemBillboard *pItemBillboard = NULL;	//ビルボードのポインタ

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULLになるまで繰り返す
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_OBJECT)
			{// 配置物の場合
					pObject = (CObject*)pScene;
				if (pObject != NULL)
				{// 配置物が取得できた
					if (m_nAppearTimer <= 0)
					{
						if (pObject->Collision(&m_pos, &m_posOld, &m_move,
							&(D3DXVECTOR3(m_VtxMin.x * m_scale.x, m_VtxMin.y * m_scale.y, m_VtxMin.z * m_scale.z)),
							&(D3DXVECTOR3(m_VtxMax.x * m_scale.x, m_VtxMax.y * m_scale.y, m_VtxMax.z * m_scale.z)),
							&m_bHitFlag) == true)
						{// 配置物の上に乗っている
							m_bJump = false;
						}
					}
					else
					{
						m_nAppearTimer--;
					}
				}
			}
			else if (pScene->GetObjType() == OBJTYPE_MESHFIELD)
			{//メッシュフィールドの場合
				//pMeshField = (CMeshField*)pScene;

				////フィールドの位置情報
				//float fMeshHeight = pMeshField->GetHeight(m_pos);

				//if (m_bJump == false)
				//{//ジャンプしていない場合
				// 	if (m_pos.y <= fMeshHeight + (10.0f * m_scale.y))
				//	{//プレイヤーがメッシュフィールドより低い位置の場合
				//		m_move.y = 0.0f;								//y軸の移動量を0にする
				//		m_pos.y = fMeshHeight + (10.0f * m_scale.y);							//メッシュフィールドと同じ高さにする
				//		m_bJump = false;								//ジャンプしていない状態にする
				//	}
				//}
				//else
				//{//ジャンプしている場合
				//	if (m_pos.y <= fMeshHeight + (10.0f * m_scale.y))
				//	{// プレイヤーのy座標が地面よりも低いとき
				//	 // ジャンプしていない状態にする
				//		m_bJump = false;
				//	}
				//}
			}
			else if (pScene->GetObjType() == OBJTYPE_ITEM_BILLBOARD)
			{
				pItemBillboard = (CItemBillboard*)pScene;
				pItemBillboard->Collision(m_pos);

			}
			// 次のオブジェクトへのポインタを取得
			pScene = pSceneNext;
		}
	}
	if (m_pos.y <= 0.0f + (10.0f * m_scale.y))
	{
		m_move.y = 0.0f;								//y軸の移動量を0にする
		m_pos.y = 0.0f + (10.0f * m_scale.y);			//メッシュフィールドと同じ高さにする
		m_bJump = false;								//ジャンプしていない状態にする
	}
}

//=============================================================================
// プレイヤーのモーション読み込み
//=============================================================================
void CPlayer::LoadMotion(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	FILE *pFile;	//ファイルのポインタ

	pFile = fopen(MOTION_TXT_NAME, "r");	//テキスト読み込み
	if (pFile != NULL)
	{//ファイルが開けた場合
		char *pStrCur;						//文字列の先頭へのポインタ
		char aLine[256];					//文字列の読み込み用
		char aStr[256];						//文字列抜き出し用
		int nIdx;

		while (1)
		{//モデルファイルを読み込むまで繰り返し
			pStrCur = CLoadFileFunction::ReadLine(pFile, &aLine[0]);
			int nNum = CLoadFileFunction::PopString(pStrCur, &aStr[0]);
			strcpy(&aStr[0], pStrCur);
			aStr[nNum - 1] = '\0';
			if (memcmp(&aStr[0], TEXT_SCRIPT, strlen(TEXT_SCRIPT)) == 0)
			{
				//************************************************************************
				// モデルの総数&ファイルの読み込み
				//************************************************************************
				while (1)
				{//モデルファイルを読み込むまで繰り返し
					pStrCur = CLoadFileFunction::ReadLine(pFile, &aLine[0]);
					strcpy(&aStr[0], pStrCur);				//文字列を取り出す

					if (memcmp(&aStr[0], TEXT_NUM_MODEL, strlen(TEXT_NUM_MODEL)) == 0)
					{//テキストがモデルの総数を示す行の場合
						pStrCur += strlen(TEXT_NUM_MODEL);	//頭出し
						strcpy(&aStr[0], pStrCur);

						g_nNumModel = atoi(&aStr[0]);	//モデルの総数に代入


						for (int nCntModel = 0; nCntModel < g_nNumModel; nCntModel++)
						{
							while (1)
							{
								pStrCur = CLoadFileFunction::ReadLine(pFile, &aLine[0]);
								strcpy(&aStr[0], pStrCur);				//文字列を取り出す
								if (memcmp(&aStr[0], TEXT_FILENAME_MODEL, strlen(TEXT_FILENAME_MODEL)) == 0)
								{//テキストがモデルの名前を示す行の場合
									pStrCur += strlen(TEXT_FILENAME_MODEL);					//頭出し
									strcpy(&aStr[0], pStrCur);
									int nNum = CLoadFileFunction::PopString(pStrCur, &aStr[0]);
									strcpy(&aStr[0], pStrCur);
									aStr[nNum - 1] = '\0';

									strcpy(&g_aFilenameModel[nCntModel][0], aStr);	//モデルの名前を取得

																					// Xファイルの読み込み
									D3DXLoadMeshFromX(&g_aFilenameModel[nCntModel][0],
										D3DXMESH_SYSTEMMEM,
										pDevice,
										NULL,
										&m_pBuffMat[nCntModel],
										NULL,
										&m_nNumMat[nCntModel],
										&m_pMesh[nCntModel]);

									break;
								}
							}
						}
						break;		//抜け出す
					}
				}

				while (1)
				{//モデルの情報を読み取るまで繰り返し
					pStrCur = CLoadFileFunction::ReadLine(pFile, &aLine[0]);
					strcpy(&aStr[0], pStrCur);				//文字列を取り出す

					//************************************************************************
					// モデルの情報読み込み
					//************************************************************************
					if (memcmp(&aStr[0], TEXT_CHARASET, strlen(TEXT_CHARASET)) == 0)
					{//テキストがモデル設置を示す行の場合
						while (1)
						{
							pStrCur = CLoadFileFunction::ReadLine(pFile, pStrCur);
							strcpy(&aStr[0], pStrCur);				//文字列を取り出す
							if (memcmp(&aStr[0], TEXT_NUM_PARTS, strlen(TEXT_NUM_PARTS)) == 0)
							{
								pStrCur += strlen(TEXT_NUM_PARTS);
								pStrCur = CLoadFileFunction::GetLineTop(pStrCur);
								strcpy(&aStr[0], pStrCur);			//文字列を取り出す
								m_nNumParts = atoi(&aStr[0]);

								for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
								{
									pStrCur = CLoadFileFunction::ReadLine(pFile, &aLine[0]);
									strcpy(&aStr[0], pStrCur);				//文字列を取り出す

									while (1)
									{
										if (memcmp(&aStr[0], TEXT_PARTSSET, strlen(TEXT_PARTSSET)) == 0)
										{//パーツ情報の設置を示す行の場合
											while (1)
											{
												pStrCur = CLoadFileFunction::ReadLine(pFile, &aLine[0]);
												strcpy(&aStr[0], pStrCur);				//文字列を取り出す

												if (memcmp(&aStr[0], TEXT_INDEX, strlen(TEXT_INDEX)) == 0)
												{//テキストがインデックスを示す行の場合
													pStrCur += strlen(TEXT_INDEX);
													pStrCur = CLoadFileFunction::GetLineTop(pStrCur);
													strcpy(&aStr[0], pStrCur);			//文字列を取り出す
													nIdx = atoi(&aStr[0]);
													m_nIndexParent[nIdx];
												}
												else if (memcmp(&aStr[0], TEXT_PARENT, strlen(TEXT_PARENT)) == 0)
												{//テキストが親子関係を示す行の場合
													pStrCur += strlen(TEXT_PARENT);
													pStrCur = CLoadFileFunction::GetLineTop(pStrCur);
													strcpy(&aStr[0], pStrCur);			//文字列を取り出す
													m_nIndexParent[nIdx] = atoi(&aStr[0]);
												}
												else if (memcmp(&aStr[0], TEXT_POS, strlen(TEXT_POS)) == 0)
												{//テキストが位置を示す行の場合
													int nWord = 0;
													pStrCur += strlen(TEXT_POS);
													//nWord = PopString(pStrCur, &aStr[0]);
													//pStrCur += nWord;
													strcpy(&aStr[0], pStrCur);			//文字列を取り出す
													m_aKeyOffset[nCntParts].fPosX = (float)atof(&aStr[0]);

													nWord = CLoadFileFunction::PopString(pStrCur, &aStr[0]);
													pStrCur += nWord;
													strcpy(&aStr[0], pStrCur);			//文字列を取り出す
													m_aKeyOffset[nCntParts].fPosY = (float)atof(&aStr[0]);

													nWord = CLoadFileFunction::PopString(pStrCur, &aStr[0]);
													pStrCur += nWord;
													strcpy(&aStr[0], pStrCur);			//文字列を取り出す
													m_aKeyOffset[nCntParts].fPosZ = (float)atof(&aStr[0]);
												}
												else if (memcmp(&aStr[0], TEXT_ROT, strlen(TEXT_ROT)) == 0)
												{//テキストが位置を示す行の場合
													int nWord = 0;
													pStrCur += strlen(TEXT_ROT);
													//nWord = PopString(pStrCur, &aStr[0]);
													//pStrCur += nWord;
													strcpy(&aStr[0], pStrCur);			//文字列を取り出す
													m_aKeyOffset[nCntParts].fRotX = (float)atof(&aStr[0]);

													nWord = CLoadFileFunction::PopString(pStrCur, &aStr[0]);
													pStrCur += nWord;
													strcpy(&aStr[0], pStrCur);			//文字列を取り出す
													m_aKeyOffset[nCntParts].fRotY = (float)atof(&aStr[0]);

													nWord = CLoadFileFunction::PopString(pStrCur, &aStr[0]);
													pStrCur += nWord;
													strcpy(&aStr[0], pStrCur);			//文字列を取り出す
													m_aKeyOffset[nCntParts].fRotZ = (float)atof(&aStr[0]);
												}
												else if (memcmp(&aStr[0], TEXT_END_PARTSSET, strlen(TEXT_END_PARTSSET)) == 0)
												{//テキストがパーツの設置終了を示す行の場合
													if (m_nIndexParent[nCntParts] == -1)
													{
														m_apModel[nIdx] = CModel::Create(D3DXVECTOR3(m_pos.x + m_aKeyOffset[nIdx].fPosX, m_pos.y + m_aKeyOffset[nIdx].fPosY, m_pos.z + m_aKeyOffset[nIdx].fPosZ));
														m_apModel[nIdx]->BindX(m_pMesh[nIdx], m_pBuffMat[nIdx], m_nNumMat[nIdx]);
														m_apModel[nIdx]->BindTexture(m_pTexture);
														m_apModel[nIdx]->SetParent(NULL);
													}
													else if (m_nIndexParent[nCntParts] > -1)
													{
														m_apModel[nIdx] = CModel::Create(D3DXVECTOR3(m_pos.x + m_aKeyOffset[nIdx].fPosX, m_pos.y + m_aKeyOffset[nIdx].fPosY, m_pos.z + m_aKeyOffset[nIdx].fPosZ));
														m_apModel[nIdx]->BindX(m_pMesh[nIdx], m_pBuffMat[nIdx], m_nNumMat[nIdx]);
														m_apModel[nIdx]->SetParent(m_apModel[m_nIndexParent[nIdx]]);
													}

													break;
												}
											}
											break;
										}
									}
								}
							}
							else if (memcmp(&aStr[0], TEXT_END_CHARASET, strlen(TEXT_END_CHARASET)) == 0)
							{//テキストがモデルの設置終了を示す行の場合
								break;
							}
						}
						break;
					}
				}

				//************************************************************************
				// モーションの読み込み
				//************************************************************************
				for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
				{
					pStrCur = CLoadFileFunction::ReadLine(pFile, &aLine[0]);
					strcpy(&aStr[0], pStrCur);				//文字列を取り出す
					if (memcmp(&aStr[0], TEXT_MOTIONSET, strlen(TEXT_MOTIONSET)) == 0)
					{//モーションの設置を示す行の場合
						while (1)
						{
							pStrCur = CLoadFileFunction::ReadLine(pFile, pStrCur);				//行の読み込み
							strcpy(&aStr[0], pStrCur);						//文字列を取り出す
							if (memcmp(&aStr[0], TEXT_LOOP, strlen(TEXT_LOOP)) == 0)
							{//ループを示す行の場合
								pStrCur += strlen(TEXT_LOOP);				//頭出し
								pStrCur = CLoadFileFunction::GetLineTop(pStrCur);				//空白消去
								int nNum = CLoadFileFunction::PopString(pStrCur, &aStr[0]);	//文字数のカウント
								strcpy(&aStr[0], pStrCur);
								aStr[nNum - 1] = '\0';

								if (atoi(&aStr[0]) == 0)
								{//０が記入されていた場合ループしない
									m_apMotionInfo[nCntMotion].bLoop = false;
								}
								else if (atoi(&aStr[0]) == 1)
								{//１が記入されていた場合ループする
									m_apMotionInfo[nCntMotion].bLoop = true;
								}
							}
							else if (memcmp(&aStr[0], TEXT_NUM_KEY, strlen(TEXT_NUM_KEY)) == 0)
							{//キーの総数を示す行の場合
								pStrCur += strlen(TEXT_NUM_KEY);			//頭出し
								pStrCur = CLoadFileFunction::GetLineTop(pStrCur);				//空白消去
								strcpy(&aStr[0], pStrCur);					//文字列を取り出す

								m_apMotionInfo[nCntMotion].nNumKey = atoi(&aStr[0]);//キーの総数を代入

								for (int nCntKey = 0; nCntKey < m_apMotionInfo[nCntMotion].nNumKey; nCntKey++)
								{//キーの総数分繰り返し
									while (1)
									{
										pStrCur = CLoadFileFunction::ReadLine(pFile, &aLine[0]);	//行の読み込み
										strcpy(&aStr[0], pStrCur);				//文字列を取り出す
										if (memcmp(&aStr[0], TEXT_KEYSET, strlen(TEXT_KEYSET)) == 0)
										{//キーの設置開始を示す行の場合
											for (int nCntModel = 0; nCntModel < g_nNumModel; nCntModel++)
											{
												while (1)
												{
													pStrCur = CLoadFileFunction::ReadLine(pFile, &aLine[0]);
													strcpy(&aStr[0], pStrCur);				//文字列を取り出す

													if (memcmp(&aStr[0], TEXT_FRAME, strlen(TEXT_FRAME)) == 0)
													{//フレーム数を示す行の場合
														pStrCur += strlen(TEXT_FRAME);		//頭出し
														pStrCur = CLoadFileFunction::GetLineTop(pStrCur);		//行の読み込み
														strcpy(&aStr[0], pStrCur);			//文字列を取り出す
														m_apMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame = atoi(&aStr[0]);	//フレームの代入
													}
													else if (memcmp(&aStr[0], TEXT_KEY, strlen(TEXT_KEY)) == 0)
													{//キーの開始を示す行の場合

														while (1)
														{
															pStrCur = CLoadFileFunction::ReadLine(pFile, &aLine[0]);	//行の読み込み
															strcpy(&aStr[0], pStrCur);				//文字列を取り出す

															if (memcmp(&aStr[0], TEXT_POS, strlen(TEXT_POS)) == 0)
															{//位置の情報を示す行の場合
																int nWord = 0;
																pStrCur += strlen(TEXT_POS);	//頭出し

																strcpy(&aStr[0], pStrCur);			//文字列を取り出す
																m_apMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].fPosX = (float)atof(&aStr[0]);	//X座標の代入

																nWord = CLoadFileFunction::PopString(pStrCur, &aStr[0]);	//文字数カウント
																pStrCur += nWord;						//カウントされた分頭出し
																strcpy(&aStr[0], pStrCur);				//文字列を取り出す
																m_apMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].fPosY = (float)atof(&aStr[0]);	//Y座標の代入

																nWord = CLoadFileFunction::PopString(pStrCur, &aStr[0]);	//文字数カウント
																pStrCur += nWord;						//カウントされた分頭出し
																strcpy(&aStr[0], pStrCur);				//文字列を取り出す
																m_apMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].fPosZ = (float)atof(&aStr[0]);	//Z座標の代入
															}
															else if (memcmp(&aStr[0], TEXT_ROT, strlen(TEXT_ROT)) == 0)
															{//向きの情報を示す行の場合
																int nWord = 0;
																pStrCur += strlen(TEXT_ROT);		//頭出し

																strcpy(&aStr[0], pStrCur);			//文字列を取り出す
																m_apMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].fRotX = (float)atof(&aStr[0]);	//X座標の代入

																nWord = CLoadFileFunction::PopString(pStrCur, &aStr[0]);	//文字数カウント
																pStrCur += nWord;						//カウントされた分頭出し
																strcpy(&aStr[0], pStrCur);				//文字列を取り出す
																m_apMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].fRotY = (float)atof(&aStr[0]);	//Y座標の代入

																nWord = CLoadFileFunction::PopString(pStrCur, &aStr[0]);	//文字数カウント
																pStrCur += nWord;						//カウントされた分頭出し
																strcpy(&aStr[0], pStrCur);				//文字列を取り出す
																m_apMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].fRotZ = (float)atof(&aStr[0]);	//Z座標の代入
															}
															else if (memcmp(&aStr[0], TEXT_END_KEY, strlen(TEXT_END_KEY)) == 0)
															{//キーの終了を示す行の場合
																break;
															}
														}
														break;
													}

												}
											}
										}
										else if (memcmp(&aStr[0], TEXT_END_KEYSET, strlen(TEXT_END_KEYSET)) == 0)
										{//キーの設置終了を示す行の場合
											break;
										}
									}
								}
							}
							else if (memcmp(&aStr[0], TEXT_END_MOTIONSET, strlen(TEXT_END_MOTIONSET)) == 0)
							{//モーションの設置終了を示す行の場合
								break;
							}
						}
					}
				}
			}
			else if (memcmp(&aStr[0], TEXT_END_SCRIPT, strlen(TEXT_END_SCRIPT)) == 0)
			{
				break;
			}
		}
	}
	fclose(pFile);	//ファイルを閉じる
}

//=============================================================================
// モーションの設置処理
//=============================================================================
void CPlayer::SetMotion(MOTION_TYPE type)
{
	m_motionType = type;
}

//=============================================================================
//　ジャンプをしているかを取得
//=============================================================================
bool CPlayer::GetJump(void)
{
	return m_bJump;
}

//=============================================================================
//　ジャンプをしているかを取得
//=============================================================================
CPlayer::PLAYER_STATE CPlayer::GetState(void)
{
	return m_state;
}
