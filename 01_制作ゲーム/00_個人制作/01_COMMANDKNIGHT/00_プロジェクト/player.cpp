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
#include "command.h"
#include "enemy.h"
#include "loadfilefunction.h"
#include "status_number.h"
#include "ui.h"
#include "particle.h"
#include "meshorbit.h"
#include "command.h"
#include "icecircle.h"
#include "icicle.h"

#define MOTION_TXT_NAME "data\\TEXT\\motion.txt"

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

//=============================================================================
// プレイヤーのコンストラクタ
//=============================================================================
CPlayer::CPlayer() : CScene(1, OBJTYPE_NONE)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//位置の初期値
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//過去の位置の初期値
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向きの初期値
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動量初期値
	m_bJump = false;								//ジャンプしていない状態
	m_bAction = false;								//行動していない状態
	m_motionType = MOTION_NEUTRAL;					//モーションを初期化
	m_nCountFrame = 0;								//カウントの初期値
	m_nLife = 100;									//体力初期値
	m_nSkill = 40;									//とくぎ初期値
	m_nMaxLife = 100;								//体力最大値初期化
	m_nMaxSkill = 40;								//とくぎの最大値初期化
	m_state = PLAYER_STATE_NOMAL;
	m_useItem = USE_ITEM0;
	m_nNumDeathBird = 0;
	m_nNumDeathFrog = 0;
	m_pMeshOrbit = NULL;
	for (int nCntSelect = 0; nCntSelect < MAX_SELECT; nCntSelect++)
	{
		m_aSkillLevel[nCntSelect] = 1;
		m_aCntLevel[nCntSelect] = 0;
		m_aItemQuantity[nCntSelect] = 1;
		m_aCutSkill[nCntSelect] = 1;
	}
	for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
	{//モデルの数だけ繰り返し
		m_apModel[nCntModel] = NULL;
	}
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{//モデルの数だけ繰り返し
		m_aMotionAttack[nCntMotion].nAttackEndFrame = 0;
		m_aMotionAttack[nCntMotion].nAttackStartFrame = 0;
		m_aMotionAttack[nCntMotion].nNumFrame = 0;
	}
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
	//モーション情報を読み込み
	LoadMotion();

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の初期化
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向きの初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量初期値
	m_bJump = false;							//ジャンプしていない状態
	m_nKey = 0;									//現在のキーを初期化
	m_nNumKey = 2;								//キーの総数を初期化
	m_nCountFrame = 0;							//カウントフレームを初期化
	SetObjType(OBJTYPE_PLAYER);					//オブジェクトの種類の設置
	//m_pMeshOrbit = CMeshOrbit::Create();
	m_state = PLAYER_STATE_NOMAL;
	m_nCntDamage = 0;
	m_nCounter = 0;
	for (int nCntSelect = 0; nCntSelect < MAX_SELECT; nCntSelect++)
	{
		m_aSkillLevel[nCntSelect] = 1;
		m_aCntLevel[nCntSelect] = 0;
		m_aItemQuantity[nCntSelect] = 3;
	}
	m_nNumDeathBird = 0;
	m_nNumDeathFrog = 0;

	return S_OK;
}

//=============================================================================
// プレイヤーの終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
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

	//モーションの更新処理
	UpdateMotion();

	//現在のモーションを過去のモーション情報に代入
	m_OldMotion = m_motionType;

	switch (m_state)
	{
	case PLAYER_STATE_NOMAL:
		break;
	case PLAYER_STATE_DAMAGE:
		m_nCntDamage++;
		if (m_nCntDamage >= 100)
		{
			m_nCntDamage = 0;
			m_state = PLAYER_STATE_NOMAL;
		}
		break;
	}

	//移動処理
	Move();

	//当たり判定処理
	Collision();

	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	if (pInputKeyboard->GetTrigger(DIK_M) == true)
	{
		for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
		{
			m_fSpin += 100.0f;
			D3DXVECTOR3 movePar(0.0f, 0.0f, 0.0f);
				movePar.x = sinf(D3DX_PI * m_fSpin) * (rand() % 10 * 0.3f);
				movePar.y = cosf(D3DX_PI * m_fSpin) * (rand() % 10 * -0.3f);
				movePar.z = cosf(D3DX_PI * m_fSpin) * (rand() % 10 * -0.5f);
				CParticle::Create(m_pos, movePar, D3DXCOLOR(rand() % 10 * 0.15f, rand() % 10 * 0.15f, rand() % 10 * 0.15f, 1.0f), rand() % 10 + 30.0f);
		}
	}

#ifdef _DEBUG
	CDebugProc::Print(1, "プレイヤーの位置  : x[%.1f],y[%.1f],z[%.1f]\n", m_pos.x, m_pos.y, m_pos.z);
	CDebugProc::Print(1, "プレイヤーの体力  : %d]\n",m_nLife);
	CDebugProc::Print(1, "プレイヤーのとくぎ : %d]\n", m_nSkill);
	CDebugProc::Print(1, "モーションOLD %d\n", m_OldMotion);

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

	D3DXMATRIX mtxRot, mtxTrans;					//計算用マトリックス

	if (m_pMeshOrbit != NULL)
	{
		//オービットの設置処理
		m_pMeshOrbit->SetMtxParent(&m_apModel[15]->GetMtxWorld());
	}
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

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

	if (m_motionType == MOTION_SKILL0)
	{
		for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
		{//モデルの数だけ繰り返し
		 //テクスチャ無効
		 //pDevice->SetTexture(0, NULL);
		 // モデル(パーツ)の描画
		 //m_pMesh[nCntModel]->DrawSubset(0);
		 //描画処理
			m_apModel[nCntModel]->Draw();
		}
	}
	else
	{
		for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL - 1; nCntModel++)
		{//モデルの数だけ繰り返し
		 //テクスチャ無効
		 //pDevice->SetTexture(0, NULL);
		 // モデル(パーツ)の描画
		 //m_pMesh[nCntModel]->DrawSubset(0);
		 //描画処理
			m_apModel[nCntModel]->Draw();
		}
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


	if (m_motionType == MOTION_SKILL0)
	{
		for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL; nCntModel++)
		{
			if (m_state == PLAYER_STATE_DAMAGE)
			{// 押されていない状態の場合
				if (m_nCntDamage % 5 == 0)
				{// カウンターが一定の値になったら
				 //モデルの描画
					m_apModel[nCntModel]->Draw();
				}
			}
			else
			{
				//モデルの描画
				m_apModel[nCntModel]->Draw();
			}
		}
	}
	else
	{
		for (int nCntModel = 0; nCntModel < MAX_PLAYER_MODEL - 1; nCntModel++)
		{
			if (m_state == PLAYER_STATE_DAMAGE)
			{// 押されていない状態の場合
				if (m_nCntDamage % 5 == 0)
				{// カウンターが一定の値になったら
				 //モデルの描画
					m_apModel[nCntModel]->Draw();
				}
			}
			else
			{
				//モデルの描画
				m_apModel[nCntModel]->Draw();
			}
		}
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

	if (m_OldMotion != m_motionType)
	{//前回のモーションを違う場合
		m_nKey = 0;			//現在のキーを初期化
		m_nCountFrame = 0;	//カウントフレームを初期化
		m_nCntNumFrame = 0;
	}
	m_nCntNumFrame++;
	if (m_nCntNumFrame >= m_aMotionAttack[m_motionType].nAttackStartFrame &&
		m_nCntNumFrame <= m_aMotionAttack[m_motionType].nAttackEndFrame)
	{
		switch (m_motionType)
		{
		case MOTION_COMBO0:
			m_bAction = true;
			break;
		case MOTION_COMBO1:
			m_bAction = true;
			break;
		case MOTION_SKILL0:
			if (m_bAction == false)
			{
				m_bAction = true;
				SwitchIceSize();
			}
			break;
		case MOTION_SKILL1:
			AttackSkill1();
			break;
		case MOTION_SKILL2:
			SwitchWeaponSize();
			break;
		case MOTION_ITEM:
			UseItem();
			break;
		}
		//m_apModel[15]->SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	}
	else
	{
		m_bAction = false;
		m_apModel[15]->SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	}

	if (m_state != PLAYER_STATE_DEATH)
	{

		for (int nCntModel = 0; nCntModel < g_nNumModel; nCntModel++)
		{//モデルの数だけ繰り返し
			if (m_apModel[nCntModel] != NULL)
			{//モデルがある場合
				pKey = &m_apMotionInfo[m_motionType].aKeyInfo[m_nKey].aKey[nCntModel];
				pKeyNext = &m_apMotionInfo[m_motionType].aKeyInfo[(m_nKey + 1) % /*2*/m_apMotionInfo[m_motionType].nNumKey].aKey[nCntModel];

				//位置座標X
				fRateMotion = (float)(m_nCountFrame) / (float)m_apMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame;	// 相対値の取得
				fDiffMotion = pKeyNext->fPosX - pKey->fPosX;														// 差分の取得
				pos.x = pKey->fPosX + (fDiffMotion * fRateMotion);

				//位置座標Y
				fDiffMotion = pKeyNext->fPosY - pKey->fPosY;								// 差分の取得
				pos.y = pKey->fPosY + (fDiffMotion * fRateMotion);

				//位置座標Z
				fDiffMotion = pKeyNext->fPosZ - pKey->fPosZ;								// 差分の取得
				pos.z = pKey->fPosZ + (fDiffMotion * fRateMotion);

				//絶対値処理
				D3DXVECTOR3 rotPlus, rotMinus;
				rotPlus.x = pKeyNext->fRotX + pKey->fRotX;
				rotPlus.y = pKeyNext->fRotY + pKey->fRotY;
				rotPlus.z = pKeyNext->fRotZ + pKey->fRotZ;
				rotMinus.x = pKeyNext->fRotX - pKey->fRotX;
				rotMinus.y = pKeyNext->fRotY - pKey->fRotY;
				rotMinus.z = pKeyNext->fRotZ - pKey->fRotZ;
				rotPlus.x = abs(rotPlus.x);
				rotPlus.y = abs(rotPlus.y);
				rotPlus.z = abs(rotPlus.z);
				rotMinus.x = abs(rotMinus.x);
				rotMinus.y = abs(rotMinus.y);
				rotMinus.z = abs(rotMinus.z);

				if (nCntModel == 0)
				{
					if (rotPlus.x < rotMinus.x)
					{
						//向き座標X
						fDiffMotion = pKeyNext->fRotX + pKey->fRotX;								// 差分の取得
						rot.x = pKey->fRotX + (fDiffMotion * fRateMotion);
					}
					else
					{
						//向き座標X
						fDiffMotion = pKeyNext->fRotX - pKey->fRotX;								// 差分の取得
						rot.x = pKey->fRotX + (fDiffMotion * fRateMotion);
					}

					if (rotPlus.y < rotMinus.y)
					{
						//向き座標Y
						fDiffMotion = pKeyNext->fRotY + pKey->fRotY;								// 差分の取得
						rot.y = pKey->fRotY + (fDiffMotion * fRateMotion);

					}
					else
					{
						//向き座標Y
						fDiffMotion = pKeyNext->fRotY - pKey->fRotY;								// 差分の取得
						rot.y = pKey->fRotY + (fDiffMotion * fRateMotion);
					}

					if (rotPlus.z < rotMinus.z)
					{
						//向き座標Z
						fDiffMotion = pKeyNext->fRotZ + pKey->fRotZ;								// 差分の取得
						rot.z = pKey->fRotZ + (fDiffMotion * fRateMotion);
					}
					else
					{
						//向き座標Z
						fDiffMotion = pKeyNext->fRotZ - pKey->fRotZ;								// 差分の取得
						rot.z = pKey->fRotZ + (fDiffMotion * fRateMotion);
					}
				}
				else
				{
					//向き座標X
					fDiffMotion = pKeyNext->fRotX - pKey->fRotX;								// 差分の取得
					rot.x = pKey->fRotX + (fDiffMotion * fRateMotion);

					//向き座標Y
					fDiffMotion = pKeyNext->fRotY - pKey->fRotY;								// 差分の取得
					rot.y = pKey->fRotY + (fDiffMotion * fRateMotion);

					//向き座標Z
					fDiffMotion = pKeyNext->fRotZ - pKey->fRotZ;								// 差分の取得
					rot.z = pKey->fRotZ + (fDiffMotion * fRateMotion);

				}
				//向きの設置処理
				m_apModel[nCntModel]->SetRot(rot);
				m_apModel[nCntModel]->SetPos(D3DXVECTOR3(m_offsetPos[nCntModel].x + pos.x, m_offsetPos[nCntModel].y + pos.y, m_offsetPos[nCntModel].z + pos.z));
				//位置の設置処理
				//m_apModel[nCntModel]->SetPos(pos);

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
			if (m_motionType == MOTION_SKILL0 && m_nKey == 5)
			{
				AttackSkill0();
			}
			else if (m_motionType == MOTION_SKILL1)
			{
				AttackSkill1();
			}
			else if (m_motionType == MOTION_SKILL2)
			{
				if (m_nKey >= 5 && m_nKey <= 6)
				{
					AttackSkill2();
				}
			}
			if (m_nKey == m_apMotionInfo[m_motionType].nNumKey - 1 && m_apMotionInfo[m_motionType].bLoop == false)
			{//現在のキーが対象のキー総数に達しかつループしない場合
				if (m_motionType == MOTION_JUMP && m_bJump == true)
				{//
					m_nKey = m_apMotionInfo[m_motionType].nNumKey - 1;
				}
				//if (m_motionType == MOTION_PANCH || m_motionType == MOTION_LANDING)
				else if (m_motionType != MOTION_MOVE || m_motionType != MOTION_NEUTRAL)
				{

					if (m_motionType == MOTION_DEATH)
					{
						m_nKey = m_apMotionInfo[m_motionType].nNumKey - 1;
						SetState(PLAYER_STATE_DEATH);
					}
					else
					{

						//現在のキーを初期化
						m_nKey = 0;
						//コマンドの取得
						CCommand *pCommand;
						pCommand = CGame::GetCommad();
						//ニュートラルに切り替え
						//m_motionType = MOTION_NEUTRAL;
						if (m_bJump == false)
						{
							SetCountLevel(m_motionType);
							//if (m_motionType == MOTION_COMBO1)
							//{
							//	pCommand->SetCommandState(pCommand->COMMAND_SKILL);
							//}
							//else
							{
								//コマンドを実行していない状態にする
								pCommand->SetCommandState(pCommand->COMMAND_END);
							}
						}
						m_bAction = false;
					}
				}
			}
			//カウントフレームを初期化
			m_nCountFrame = 0;
		}
	}

#ifdef _DEBUG
	CDebugProc::Print(1, "プレイヤー移動[W,A,S,D]\n");
	CDebugProc::Print(1, "Key[%d / %d]\n", m_nKey, m_apMotionInfo[m_motionType].nNumKey);
	CDebugProc::Print(1, "Frame[%d / %d]\n", m_nCountFrame, m_apMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame);
	CDebugProc::Print(1, "モーション[%d]\n", m_motionType);
	CDebugProc::Print(1, "カウンター総数[%d]\n",m_aMotionAttack[m_motionType].nNumFrame);

	switch (m_apMotionInfo[m_motionType].bLoop)
	{
	case false:
		CDebugProc::Print(1,"ループしない\n");
		break;
	case true:
		CDebugProc::Print(1, "ループする\n");
		break;
	}
	switch (m_bJump)
	{
	case false:
	CDebugProc::Print(1, "ジャンプしていない\n");
		break;
	case true:
		CDebugProc::Print(1, "ジャンプしている\n");
		break;
	}
	switch (m_bAction)
	{
	case false:
		CDebugProc::Print(1, "当たり判定OFF\n");

		break;
	case true:
		CDebugProc::Print(1, "当たり判定ON\n");
		break;
	}
#endif
}
//=============================================================================
// プレイヤーの行動状態を取得
//=============================================================================
bool CPlayer::GetAction(void)
{
	return m_bAction;
}

//=============================================================================
// プレイヤーの位置取得処理
//=============================================================================
D3DXVECTOR3 CPlayer::GetPos(void)
{
	return m_pos;
}
//=============================================================================
// プレイヤーの移動量取得処理
//=============================================================================
D3DXVECTOR3 CPlayer::GetMove(void)
{
	return m_move;
}

//=============================================================================
// プレイヤーの向き取得処理
//=============================================================================
D3DXVECTOR3 CPlayer::GetRot(void)
{
	return m_rot;
}

//=============================================================================
// プレイヤーの体力取得処理
//=============================================================================
int CPlayer::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
// プレイヤーのとくぎ取得処理
//=============================================================================
int CPlayer::GetSkill(void)
{
	return m_nSkill;
}

//=============================================================================
// プレイヤーの体力最大値取得処理
//=============================================================================
int CPlayer::GetMaxLife(void)
{
	return m_nMaxLife;
}

//=============================================================================
// プレイヤーのとくぎ最大値取得処理
//=============================================================================
int CPlayer::GetMaxSkill(void)
{
	return m_nMaxSkill;
}

//=============================================================================
// プレイヤーの行動状態の設置処理
//=============================================================================
void CPlayer::SetAction(bool action)
{
	m_bAction = action;
}

//=============================================================================
// プレイヤーの体力設置処理
//=============================================================================
void CPlayer::SetLife(int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
// プレイヤーのとくぎ設置処理
//=============================================================================
void CPlayer::SetSkill(int nSkill)
{
	m_nSkill = nSkill;
}

//=============================================================================
// プレイヤーの体力最大値設置処理
//=============================================================================
void CPlayer::SetMaxLife(int nMaxLife)
{
	m_nMaxLife = nMaxLife;
}

//=============================================================================
// プレイヤーの状態設置処理
//=============================================================================
void CPlayer::SetState(PLAYER_STATE state)
{
	m_state = state;
}

//=============================================================================
// プレイヤーの移動量の設置処理
//=============================================================================
void CPlayer::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//=============================================================================
// プレイヤーのとくぎ量加算処理
//=============================================================================
void CPlayer::AddSkillLevel(int nSelect,int nValue)
{
	m_aSkillLevel[nSelect] += nValue;
}

//=============================================================================
// プレイヤーのどうぐ量加算処理
//=============================================================================
void CPlayer::AddItemQuantity(int nSelect, int nValue)
{
	m_aItemQuantity[nSelect] += nValue;
}

//=============================================================================
// プレイヤーのとくぎ量取得処理
//=============================================================================
int CPlayer::GetSkillLevel(int nSelect)
{
	return m_aSkillLevel[nSelect];
}

//=============================================================================
// プレイヤーのどうぐ量取得処理
//=============================================================================
int CPlayer::GetItemQuantity(int nSelect)
{
	return m_aItemQuantity[nSelect];
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
//　モーション種類の取得
//=============================================================================
CPlayer::MOTION_TYPE CPlayer::GetMotion(void)
{
	return m_motionType;
}
CPlayer::PLAYER_STATE CPlayer::GetState(void)
{
	return m_state;
}
//=============================================================================
// プレイヤーの移動処理
//=============================================================================
void CPlayer::Move(void)
{
	float fDiffAngle;

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

	//コマンドの取得
	CCommand *pCommand;
	pCommand = CGame::GetCommad();

	//ステータス数字の取得
	CStatus_Number *pStatus_Number;
	pStatus_Number = CGame::GetStatusNumber();

	CUI_LifeGauge *pUI_Life;
	pUI_Life = CGame::GetUI_LifeGauge();

	CUI_SkillGauge *pUI_Skill;
	pUI_Skill = CGame::GetUI_SkillGauge();

	//if (pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_UP) == true ||
	//	pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_DOWN) == true ||
	//	pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LEFT) == true ||
	//	pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_RIGHT) == true)
	//{
	//	//移動処理
	//	m_move.x -= sinf(pInputJoypad->GetRadian()) * 2.0f;
	//	m_move.z += cosf(pInputJoypad->GetRadian()) * 2.0f;
	//}


	//if (pInputKeyboard->GetTrigger(DIK_7) == true)
	//{
	//	m_nLife -= 1;
	//	if (m_nLife <= 0)
	//	{
	//		m_nLife = 0;
	//	}
	//	pUI_Life->CutLifeGauge(-1);
	//	pStatus_Number->AddStatus_Number(m_nLife,0);
	//	//pStatus_Number->AddStatus_Number(m_nLife, 0);
	//}
	//if (pInputKeyboard->GetTrigger(DIK_8) == true)
	//{
	//	m_nSkill -= 1;
	//	if (m_nSkill <= 0)
	//	{
	//		m_nSkill = 0;
	//	}
	//	pUI_Skill->CutSkillGauge(-1);
	//	pStatus_Number->AddStatus_Number(m_nSkill, 1);
	//}
	//if (pInputKeyboard->GetTrigger(DIK_9) == true)
	//{
	//	m_nMaxLife = 150;
	//	m_nLife = m_nMaxLife;

	//	pUI_Life->ResetLifeGauge(m_nLife);
	//	//pUI_Skill->CutSkillGauge(-1);

	//	pStatus_Number->AddStatus_Number(m_nLife, 0);
	//}
	if (m_state != PLAYER_STATE_DEATH)
	{
		if (pCommand != NULL)
		{
			if (pCommand->GetCommandState() == pCommand->COMMAND_NONE || pCommand->GetCommandState() == pCommand->COMMAND_END /*|| pCommand->GetCommandState() == pCommand->COMMAND_SKILL*/)
			{//コマンドが実行されていない場合

				if (pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LSTICK_UP) == true ||
					pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LSTICK_DOWN) == true ||
					pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LSTICK_LEFT) == true ||
					pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_LSTICK_RIGHT) == true)
				{
					if (m_bJump == false)
					{//ジャンプをしていない場合
					 //移動モーションに切り替え
						m_motionType = MOTION_MOVE;
					}
					//移動処理
					m_move.x -= sinf(cameraRot.y - pInputJoypad->GetLeftAxiz()) * 2.0f;
					m_move.z -= cosf(cameraRot.y - pInputJoypad->GetLeftAxiz()) * 2.0f;
					//fDiffAngle = (cameraRot.y - pInputJoypad->GetLeftAxiz() );

					float fDest = (cameraRot.y - pInputJoypad->GetLeftAxiz());

					fDiffAngle = fDest - m_rot.y;
					if (fDiffAngle > D3DX_PI)
					{
						fDiffAngle -= D3DX_PI * 2.0f;
					}
					if (fDiffAngle < -D3DX_PI)
					{
						fDiffAngle += D3DX_PI * 2.0f;
					}
					m_rot.y += fDiffAngle * 0.5f;
					if (m_rot.y > D3DX_PI)
					{
						m_rot.y -= D3DX_PI * 2.0f;
					}
					if (m_rot.y < -D3DX_PI)
					{
						m_rot.y += D3DX_PI * 2.0f;
					}
				}
				else
				{
					m_motionType = MOTION_NEUTRAL;
				}


				if (pInputKeyboard->GetPress(DIK_A) == true)
				{// キーボードの[A]キーが押された（左移動）
				 //移動モーションにする
					if (m_bJump == false)
					{//ジャンプをしていない場合
					 //移動モーションに切り替え
						m_motionType = MOTION_MOVE;
					}
					if (pInputKeyboard->GetPress(DIK_W) == true)
					{// キーボードの[W]キーが押された（左上移動）
						m_pos.x += -sinf(cameraRot.y + D3DX_PI * 0.75f) * PLAYER_MOVE;
						m_pos.z += -cosf(cameraRot.y + D3DX_PI * 0.75f) * PLAYER_MOVE;
						//rot.y = cameraRot.y + D3DX_PI * 0.75f;
						fDiffAngle = (cameraRot.y + D3DX_PI * 0.75f) - m_rot.y;
						if (fDiffAngle > D3DX_PI)
						{
							fDiffAngle -= D3DX_PI * 2.0f;
						}
						if (fDiffAngle < -D3DX_PI)
						{
							fDiffAngle += D3DX_PI * 2.0f;
						}
						m_rot.y += fDiffAngle * 0.5f;
						if (m_rot.y > D3DX_PI)
						{
							m_rot.y -= D3DX_PI * 2.0f;
						}
						if (m_rot.y < -D3DX_PI)
						{
							m_rot.y += D3DX_PI * 2.0f;
						}
					}
					else if (pInputKeyboard->GetPress(DIK_S) == true)
					{// キーボードの[S]キーが押された（左下移動）
						m_pos.x += -sinf(cameraRot.y + D3DX_PI * 0.25f) * PLAYER_MOVE;
						m_pos.z += -cosf(cameraRot.y + D3DX_PI * 0.25f) * PLAYER_MOVE;
						//rot.y = cameraRot.y + D3DX_PI * 0.25f;
						fDiffAngle = (cameraRot.y + D3DX_PI * 0.25f) - m_rot.y;
						if (fDiffAngle > D3DX_PI)
						{
							fDiffAngle -= D3DX_PI * 2.0f;
						}
						if (fDiffAngle < -D3DX_PI)
						{
							fDiffAngle += D3DX_PI * 2.0f;
						}
						m_rot.y += fDiffAngle * 0.5f;
						if (m_rot.y > D3DX_PI)
						{
							m_rot.y -= D3DX_PI * 2.0f;
						}
						if (m_rot.y < -D3DX_PI)
						{
							m_rot.y += D3DX_PI * 2.0f;
						}
					}
					else
					{// キーボードの[A]キーが押された（左移動）
						m_pos.x += -sinf(cameraRot.y + D3DX_PI / 2) * PLAYER_MOVE;
						m_pos.z += -cosf(cameraRot.y + D3DX_PI / 2) * PLAYER_MOVE;
						//rot.y = cameraRot.y + D3DX_PI / 2;
						fDiffAngle = (cameraRot.y + D3DX_PI / 2) - m_rot.y;
						if (fDiffAngle > D3DX_PI)
						{
							fDiffAngle -= D3DX_PI * 2.0f;
						}
						if (fDiffAngle < -D3DX_PI)
						{
							fDiffAngle += D3DX_PI * 2.0f;
						}
						m_rot.y += fDiffAngle * 0.5f;
						if (m_rot.y > D3DX_PI)
						{
							m_rot.y -= D3DX_PI * 2.0f;
						}
						if (m_rot.y < -D3DX_PI)
						{
							m_rot.y += D3DX_PI * 2.0f;
						}
					}
				}
				else if (pInputKeyboard->GetPress(DIK_D) == true)
				{// キーボードの[D]キーが押された（右移動）
					if (m_bJump == false)
					{//ジャンプをしていない場合
					 //移動モーションに切り替え
						m_motionType = MOTION_MOVE;
					}

					if (pInputKeyboard->GetPress(DIK_W) == true)
					{// キーボードの[W]キーが押された（右上移動）
						m_pos.x += -sinf(cameraRot.y + D3DX_PI * -0.75f) * PLAYER_MOVE;
						m_pos.z += -cosf(cameraRot.y + D3DX_PI * -0.75f) * PLAYER_MOVE;
						//rot.y = cameraRot.y + D3DX_PI * -0.75f;
						fDiffAngle = (cameraRot.y + D3DX_PI * -0.75f) - m_rot.y;
						if (fDiffAngle > D3DX_PI)
						{
							fDiffAngle -= D3DX_PI * 2.0f;
						}
						if (fDiffAngle < -D3DX_PI)
						{
							fDiffAngle += D3DX_PI * 2.0f;
						}
						m_rot.y += fDiffAngle * 0.5f;
						if (m_rot.y > D3DX_PI)
						{
							m_rot.y -= D3DX_PI * 2.0f;
						}
						if (m_rot.y < -D3DX_PI)
						{
							m_rot.y += D3DX_PI * 2.0f;
						}
					}
					else if (pInputKeyboard->GetPress(DIK_S) == true)
					{// キーボードの[S]キーが押された（右下移動）
						m_pos.x += -sinf(cameraRot.y + D3DX_PI * -0.25f) * PLAYER_MOVE;
						m_pos.z += -cosf(cameraRot.y + D3DX_PI * -0.25f) * PLAYER_MOVE;
						//rot.y = cameraRot.y + D3DX_PI * -0.25f;
						fDiffAngle = (cameraRot.y + D3DX_PI * -0.25f) - m_rot.y;
						if (fDiffAngle > D3DX_PI)
						{
							fDiffAngle -= D3DX_PI * 2.0f;
						}
						if (fDiffAngle < -D3DX_PI)
						{
							fDiffAngle += D3DX_PI * 2.0f;
						}
						m_rot.y += fDiffAngle * 0.5f;
						if (m_rot.y > D3DX_PI)
						{
							m_rot.y -= D3DX_PI * 2.0f;
						}
						if (m_rot.y < -D3DX_PI)
						{
							m_rot.y += D3DX_PI * 2.0f;
						}
					}
					else
					{// キーボードの[D]キーが押された（右移動）
						m_pos.x += -sinf(cameraRot.y + D3DX_PI / -2) * PLAYER_MOVE;
						m_pos.z += -cosf(cameraRot.y + D3DX_PI / -2) * PLAYER_MOVE;
						//rot.y = cameraRot.y + D3DX_PI / -2;
						fDiffAngle = (cameraRot.y + D3DX_PI / -2) - m_rot.y;
						if (fDiffAngle > D3DX_PI)
						{
							fDiffAngle -= D3DX_PI * 2.0f;
						}
						if (fDiffAngle < -D3DX_PI)
						{
							fDiffAngle += D3DX_PI * 2.0f;
						}
						m_rot.y += fDiffAngle * 0.5f;
						if (m_rot.y > D3DX_PI)
						{
							m_rot.y -= D3DX_PI * 2.0f;
						}
						if (m_rot.y < -D3DX_PI)
						{
							m_rot.y += D3DX_PI * 2.0f;
						}
					}
				}
				else if (pInputKeyboard->GetPress(DIK_W) == true)
				{//キーボードの[W]キーが押された（前移動）
					if (m_bJump == false)
					{//ジャンプをしていない場合
					 //移動モーションに切り替え
						m_motionType = MOTION_MOVE;
					}
					m_pos.x -= sinf(cameraRot.y + D3DX_PI * 1)*PLAYER_MOVE;
					m_pos.z -= cosf(cameraRot.y + D3DX_PI * 1)*PLAYER_MOVE;
					//	rot.y = cameraRot.y + D3DX_PI * 1;
					fDiffAngle = (cameraRot.y + D3DX_PI) - m_rot.y;
					if (fDiffAngle > D3DX_PI)
					{
						fDiffAngle -= D3DX_PI * 2.0f;
					}
					if (fDiffAngle < -D3DX_PI)
					{
						fDiffAngle += D3DX_PI * 2.0f;
					}
					m_rot.y += fDiffAngle * 0.5f;
					if (m_rot.y > D3DX_PI)
					{
						m_rot.y -= D3DX_PI * 2.0f;
					}
					if (m_rot.y < -D3DX_PI)
					{
						m_rot.y += D3DX_PI * 2.0f;
					}
				}
				else if (pInputKeyboard->GetPress(DIK_S) == true)
				{//キーボードの[S]キーが押された（後移動）
					if (m_bJump == false)
					{//ジャンプをしていない場合
					 //移動モーションに切り替え
						m_motionType = MOTION_MOVE;
					}
					m_pos.x += -sinf(cameraRot.y + D3DX_PI * 0)*PLAYER_MOVE;
					m_pos.z += -cosf(cameraRot.y + D3DX_PI * 0)*PLAYER_MOVE;
					//rot.y = cameraRot.y + D3DX_PI * -0;
					fDiffAngle = (cameraRot.y + D3DX_PI * 0) - m_rot.y;
					if (fDiffAngle > D3DX_PI)
					{
						fDiffAngle -= D3DX_PI * 2.0f;
					}
					if (fDiffAngle < -D3DX_PI)
					{
						fDiffAngle += D3DX_PI * 2.0f;
					}
					m_rot.y += fDiffAngle * 0.5f;
					if (m_rot.y > D3DX_PI)
					{
						m_rot.y -= D3DX_PI * 2.0f;
					}
					if (m_rot.y < -D3DX_PI)
					{
						m_rot.y += D3DX_PI * 2.0f;
					}
				}
				else
				{
					if (m_bJump == false)
					{//ジャンプをしていない場合
					 //ニュートラルモーションに切り替え
						//m_motionType = MOTION_NEUTRAL;
					}
				}
				//重力
				m_move.y -= cosf(D3DX_PI * 0) * 0.5f;

				//移動加算処理
				//m_pos.y += m_move.y;

				m_pos += m_move;

				//慣性処理
				m_move.x += (0.0f - m_move.x) * 0.8f;
				m_move.z += (0.0f - m_move.z) * 0.8f;
				if (m_bJump == false)
				{//ジャンプしていない場合
				 //地面の高さ取得位置代入
					m_pos.y = CGame::GetMeshField()->GetHeight(m_pos);
					//移動量初期化
					m_move.y = 0.0f;
					if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
					{//Spaceキーが押された場合
					 //ジャンプモーションに切り替え
						//m_motionType = MOTION_JUMP;
						////現在のキーを初期化
						//m_nKey = 0;
						////移動量を加算
						//m_move.y += 8.0f;
						////ジャンプしている状態にする
						//m_bJump = true;
					}
				}
				else
				{//ジャンプしている場合
					if (m_pos.y <= CGame::GetMeshField()->GetHeight(m_pos))
					{//地面についている場合
					 //ジャンプしていない状態にする
						m_bJump = false;
						//ニュートラルモーションにする
						m_motionType = MOTION_NEUTRAL;
						//移動量を初期化
						m_move.y = 0.0f;
					}
				}
			}
		}
	}
	if (m_pos.y <= 0.0f)
	{
		//m_pos.y = 0.0f;
	}
	CDebugProc::Print(1, "移動量ｙ %f\n", m_move.y);
}

//=============================================================================
// プレイヤーの当たり判定処理
//=============================================================================
void CPlayer::Collision(void)
{
	int nattack = 0;
	float fRange = 30.0f;
	float fWeaponRange = 15.0f;
	float fLength = 0.0f;
	float fHit = 0.0f;
	// 配置物と地面へのポインタ取得する
	CScene *pScene = NULL;               // オブジェクトのポインタ
	CScene *pSceneNext = NULL;           // 次のオブジェクトのポインタ
	CObject *pObject = NULL;             // 配置物のポインタ
	D3DXVECTOR3 Offset = D3DXVECTOR3(0.0f,25.0f,0.0f);
	D3DXVec3TransformCoord(&Offset, &Offset, &m_apModel[15]->GetMtxWorld());
	fHit = sqrtf((fRange + fWeaponRange) * (fRange + fWeaponRange));

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULLになるまで繰り返す
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_OBJECT)
			{// 配置物だったら
				pObject = (CObject*)pScene;
				if (pObject != NULL)
				{// 配置物が取得できた
					if (pObject->Collision(&m_pos, &m_posOld, &m_move) == true)
					{// 配置物の上に乗っている
						m_bJump = false;
					}
				}
			}
			if (pScene->GetObjType() == OBJTYPE_ENEMY ||
				pScene->GetObjType() == OBJTYPE_BIRD ||
				pScene->GetObjType() == OBJTYPE_FROG)
			{
				CEnemy *pEnemy = (CEnemy*)pScene;
				if (pEnemy != NULL)
				{// 配置物が取得できた
					if (m_bAction == true)
					{

						D3DXVECTOR3 EnemeyPos, EnemyMove;
						EnemeyPos = pEnemy->GetPos();
						EnemyMove = pEnemy->GetMove();
						fLength = sqrtf((Offset.x - EnemeyPos.x) * (Offset.x - EnemeyPos.x) + (Offset.y - EnemeyPos.y) * (Offset.y - EnemeyPos.y) + (Offset.z - EnemeyPos.z) * (Offset.z - EnemeyPos.z));

						if (fHit >= fLength)
						{
							int nDamage = rand() % 1 + 3;
							pEnemy->HitDamage(nDamage);
							float fAttackAngle = atan2f(EnemeyPos.x - m_pos.x, EnemeyPos.z - m_pos.z);// 当たった角度を計算
							EnemyMove.x = sinf((D3DX_PI *  fAttackAngle)) * -25.0f;
							EnemyMove.z = cosf((D3DX_PI *  fAttackAngle)) * 25.0f;
							if (pEnemy->GetObjType() == OBJTYPE_BIRD)
							{
								pEnemy->SetMove(EnemyMove);
							}
							m_nSkill += 1;
							if (m_nSkill >= 40)
							{
								m_nSkill = 40;
							}
							nattack = 1;
							m_bAction = false;
						}

					}
				}
			}
			// 次のオブジェクトへのポインタを取得
			pScene = pSceneNext;
		}
	}
	CDebugProc::Print(1, "当たっているかどうか%d\n", nattack);
}

//=============================================================================
// プレイヤーのモーションカウンター設置取得
//=============================================================================
void CPlayer::SetCountLevel(MOTION_TYPE type)
{
	//とくぎのレベルカウンター加算処理
	switch (type)
	{
	case MOTION_SKILL0:
		m_aCntLevel[0]++;
		break;
	case MOTION_SKILL1:
		m_aCntLevel[1]++;
		break;
	case MOTION_SKILL2:
		m_aCntLevel[2]++;
		break;
	}

	for (int nCntLevel = 0; nCntLevel < MAX_SELECT; nCntLevel++)
	{
		switch (m_aCntLevel[nCntLevel])
		{
		case 1:
			m_aSkillLevel[nCntLevel] = 2;
			m_aCutSkill[nCntLevel] = 2;
			break;
		case 2:
			m_aSkillLevel[nCntLevel] = 3;
			m_aCutSkill[nCntLevel] = 3;
			break;
		case 4:
			m_aSkillLevel[nCntLevel] = 4;
			m_aCutSkill[nCntLevel] = 3;
			break;
		case 6:
			m_aSkillLevel[nCntLevel] = 5;
			m_aCutSkill[nCntLevel] = 4;
			break;
		case 8:
			m_aSkillLevel[nCntLevel] = 6;
			m_aCutSkill[nCntLevel] = 4;
			break;
		case 10:
			m_aSkillLevel[nCntLevel] = 7;
			m_aCutSkill[nCntLevel] = 5;
			break;
		case 12:
			m_aSkillLevel[nCntLevel] = 8;
			m_aCutSkill[nCntLevel] = 5;
			break;
		case 14:
			m_aSkillLevel[nCntLevel] = 9;
			m_aCutSkill[nCntLevel] = 6;

			break;
		case 16:
			m_aSkillLevel[nCntLevel] = 10;
			m_aCutSkill[nCntLevel] = 6;
			break;
		}
	}
}
//=============================================================================
// プレイヤーの武器大きさ切り替え処理
//=============================================================================
void CPlayer::SwitchWeaponSize(void)
{
	float fData = (float)m_aSkillLevel[2] / 5.0f;
	m_apModel[15]->SetSize(D3DXVECTOR3(1.0f + fData, 1.0f + fData, 1.0f + fData));

	for (int nCntParticle = 0; nCntParticle < 5 * fData; nCntParticle++)
	{
		m_fSpin += 50.0f;
		float fAngle = (rand() % 628 * 0.01f) - D3DX_PI;
		D3DXVECTOR3 movePar(0.0f, 0.0f, 0.0f);
		movePar.x = sinf(fAngle) * (rand() % 10 * 0.75f);
		movePar.y = (rand() % 10 * 0.75f);
		movePar.z = cosf(fAngle) * (rand() % 10 * 0.75f);
		CDamageParticle::Create(D3DXVECTOR3(m_apModel[15]->GetMtxWorld()._41, m_apModel[15]->GetMtxWorld()._42, m_apModel[15]->GetMtxWorld()._43), movePar, D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f), rand() % 10 + 10.0f, 25);
	}
}

//=============================================================================
// プレイヤーの武器大きさ切り替え処理
//=============================================================================
void CPlayer::SwitchIceSize(void)
{
	float fData = (float)m_aSkillLevel[0] / 5.0f;
	if (m_aSkillLevel[0] >= 0 && m_aSkillLevel[0] <= 4)
	{
		CIceCircle::Create(D3DXVECTOR3(m_pos.x, 2.0f, m_pos.z));
		float fAngle = D3DX_PI;	//円周率
		float fRadius = 40.0f;	//半径
		D3DXVECTOR3 ParPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//パーティクルの位置
		if (fAngle > D3DX_PI)
		{
			fAngle -= -D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{
			fAngle += D3DX_PI * 2.0f;
		}

		for (int nCntRad = 0; nCntRad < 3; nCntRad++)
		{
			for (int nCntPar = 0; nCntPar < 10 * fData; nCntPar++)
			{
				ParPos = D3DXVECTOR3(sinf(fAngle) * fRadius, 0.0f, cosf(fAngle) * fRadius);

				CSkill2Particle::Create(D3DXVECTOR3(ParPos.x + m_pos.x, 0.0f, ParPos.z + m_pos.z),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					rand() % 10 + 5.0f,
					50);

				fAngle += 15.0f;
			}
			fRadius += 30.0f;
		}
	}
	else if (m_aSkillLevel[0] >= 5  && m_aSkillLevel[1] <= 10)
	{
		CIcicle::Create(m_pos, CIcicle::ICICLE_RIGIHT);
		CIcicle::Create(m_pos, CIcicle::ICICLE_LEFT);

		CIceCircle::Create(D3DXVECTOR3(m_pos.x, 2.0f, m_pos.z));
		float fAngle = D3DX_PI;	//円周率
		float fRadius = 40.0f;	//半径
		D3DXVECTOR3 ParPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//パーティクルの位置
		if (fAngle > D3DX_PI)
		{
			fAngle -= -D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{
			fAngle += D3DX_PI * 2.0f;
		}

		for (int nCntRad = 0; nCntRad < 3; nCntRad++)
		{
			for (int nCntPar = 0; nCntPar < 5 * fData; nCntPar++)
			{
				ParPos = D3DXVECTOR3(sinf(fAngle) * fRadius, 0.0f, cosf(fAngle) * fRadius);

				CSnowParticle::Create(D3DXVECTOR3(ParPos.x + m_pos.x, 150.0f, ParPos.z + m_pos.z),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					rand() % 10 + 5.0f,
					150);

				fAngle += 10.0f;
			}
			fRadius += 30.0f;
		}
	}
}
//=============================================================================
// プレイヤーのダメージ処理
//=============================================================================
void CPlayer::HitDamage(int nDamage)
{
	CCommand::COMMAND_STATE state;
	state = CGame::GetCommad()->GetCommandState();
	if (m_state != PLAYER_STATE_DAMAGE &&state != CCommand::COMMAND_PROCESS)
	{
		//ステータス数字の取得
		CStatus_Number *pStatus_Number;
		pStatus_Number = CGame::GetStatusNumber();

		CUI_LifeGauge *pUI_Life;
		pUI_Life = CGame::GetUI_LifeGauge();

		CUI_SkillGauge *pUI_Skill;
		pUI_Skill = CGame::GetUI_SkillGauge();
		m_nLife -= nDamage;
		pUI_Life->CutLifeGauge(-1);
		pStatus_Number->AddStatus_Number(m_nLife, 0);

		if (m_nLife <= 0)
		{
			m_nLife = 0;
			SetMotion(MOTION_DEATH);
			//Uninit();
		}
		else
		{
			////サウンドの取得
			//CSound *pSound;
			//pSound = CManager::GetSound();
			//pSound->PlaySound(CSound::SOUND_LABEL_SE_DAMAGE);
			m_state = PLAYER_STATE_DAMAGE;
			//for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
			//{
			//	m_fSpin += 50.0f;
			//	D3DXVECTOR3 movePar(0.0f, 0.0f, 0.0f);
			//	movePar.x = sinf(D3DX_PI * m_fSpin) * (rand() % 10 * 0.3f);
			//	movePar.y = cosf(D3DX_PI * m_fSpin) * (rand() % 10 * -0.3f);
			//	movePar.z = cosf(D3DX_PI * m_fSpin) * (rand() % 10 * -0.3f);
			//	CParticle::Create(m_pos, movePar, D3DXCOLOR(rand() % 10 * 1.0f, rand() % 10 * 0.8f, rand() % 10 * 0.15f, 1.0f), rand() % 10 + 30.0f);
			//}
		}
		//サウンドの取得
		CSound *pSound;
		pSound = CManager::GetSound();
		pSound->PlaySound(CSound::SOUND_LABEL_SE_DAMAGE);
		for (int nCntParticle = 0; nCntParticle < 15; nCntParticle++)
		{
			m_fSpin += 50.0f;
			float fAngle = (rand() % 628 * 0.01f) - D3DX_PI;
			D3DXVECTOR3 movePar(0.0f, 0.0f, 0.0f);
			movePar.x = sinf(fAngle) * (rand() % 10 * 0.75f);
			movePar.y = (rand() % 10 * 0.75f);
			movePar.z = cosf(fAngle) * (rand() % 10 * 0.75f);
			/*float fAngle = (rand() % 628 * 0.01f) - D3DX_PI;
			movePar.x = sinf(fAngle) * ((rand() % 5) + 0.5f);
			movePar.y = ((rand() % 10) + 5.0f);
			movePar.z = cosf(fAngle) * ((rand() % 5) + 0.5f);*/

			CDamageParticle::Create(m_pos, movePar, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), rand() % 10 + 10.0f, 25);
		}
	}

}

//=============================================================================
// プレイヤーのどうぐ設置処理
//=============================================================================
void CPlayer::SetUseItem(USE_ITEM item)
{
	m_useItem = item;
}

//=============================================================================
// プレイヤーのどうぐ使用処理
//=============================================================================
void CPlayer::UseItem(void)
{
	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_SE_CURE);

	for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
	{
		m_fSpin += 50.0f;
		float fAngle = (rand() % 628 * 0.01f) - D3DX_PI;
		D3DXVECTOR3 movePar(0.0f, 0.0f, 0.0f);
		movePar.x = sinf(fAngle) * (rand() % 10 * 0.75f);
		movePar.y = (rand() % 10 * 0.75f);
		movePar.z = cosf(fAngle) * (rand() % 10 * 0.75f);
		CDamageParticle::Create(D3DXVECTOR3(m_apModel[8]->GetMtxWorld()._41, m_apModel[8]->GetMtxWorld()._42, m_apModel[8]->GetMtxWorld()._43), movePar, D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f), rand() % 3 + 5.0f, 25);
	}

	switch (m_useItem)
	{
	case USE_ITEM0:
		m_nLife += 50;
		if (m_nLife >= 100)
		{
			m_nLife = 100;
		}
		break;
	case USE_ITEM1:
		m_nSkill += 20;
		if (m_nSkill >= 40)
		{
			m_nSkill = 40;
		}
		break;
	case USE_ITEM2:
		m_nLife += 50;
		m_nSkill += 20;

		if (m_nLife >= 100)
		{
			m_nLife = 100;
		}
		if (m_nSkill >= 40)
		{
			m_nSkill = 40;
		}
		break;
	}
}
//=============================================================================
// プレイヤーのスキル減産量取得処理
//=============================================================================
int CPlayer::GetCutSkill(int nSkill)
{
	return m_aCutSkill[nSkill];
}

//=============================================================================
// プレイヤーのダメージ処理
//=============================================================================
void CPlayer::AttackSkill0(void)
{
	// 配置物と地面へのポインタ取得する
	CScene *pScene = NULL;               // オブジェクトのポインタ
	CScene *pSceneNext = NULL;           // 次のオブジェクトのポインタ
	CObject *pObject = NULL;             // 配置物のポインタ


	//CIceCircle::Create(D3DXVECTOR3(m_pos.x, 2.0f, m_pos.z));
	float fAngle = D3DX_PI;	//円周率
	float fRadius = 10.0f;	//半径
	D3DXVECTOR3 ParPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//パーティクルの位置

	for (int nCntParticle = 0; nCntParticle < 30; nCntParticle++)
	{
		m_fSpin += 50.0f;
		float fAngle = (rand() % 628 * 0.01f) - D3DX_PI;
		D3DXVECTOR3 movePar(0.0f, 0.0f, 0.0f);
		movePar.x = sinf(fAngle) * (rand() % 10 * 1.0f);
		movePar.y = (rand() % 30 * 0.75f);
		movePar.z = cosf(fAngle) * (rand() % 10 * 1.0f);
		CFountainParticle::Create(m_pos, movePar, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), rand() % 10 + 10.0f, 25);
	}
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULLになるまで繰り返す
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_ENEMY ||
				pScene->GetObjType() == OBJTYPE_BIRD ||
				pScene->GetObjType() == OBJTYPE_FROG)
			{
				CEnemy *pEnemy = (CEnemy*)pScene;

				if (pEnemy != NULL)
				{// 配置物が取得できた
					D3DXVECTOR3 EnemeyPos, EnemyMove;
					EnemeyPos = pEnemy->GetPos();
					EnemyMove = pEnemy->GetMove();

					float fLength = powf((EnemeyPos.x - m_pos.x)*(EnemeyPos.x - m_pos.x) + (EnemeyPos.z - m_pos.z)*(EnemeyPos.z - m_pos.z), 0.5f);	//プレイヤーとの距離
					float fHit = powf(12.0f, 2.0f);	//半径

					if (fHit > fLength)
					{
						int nData = (int)m_aSkillLevel[0];
						nData = rand() % 2 + nData;
						pEnemy->HitDamage(nData);
						float fAttackAngle = atan2f(EnemeyPos.x - m_pos.x, EnemeyPos.z - m_pos.z);// 当たった角度を計算
						EnemyMove.x = sinf((D3DX_PI *  fAttackAngle)) * 25.0f;
						EnemyMove.z = cosf((D3DX_PI *  fAttackAngle)) * 25.0f;
						if (pEnemy->GetObjType() == OBJTYPE_BIRD)
						{
							pEnemy->SetMove(EnemyMove);
						}
					}

				}
			}
			// 次のオブジェクトへのポインタを取得
			pScene = pSceneNext;

		}
	}
}

//=============================================================================
// プレイヤーのダメージ処理
//=============================================================================
void CPlayer::AttackSkill1(void)
{
	// 配置物と地面へのポインタ取得する
	CScene *pScene = NULL;               // オブジェクトのポインタ
	CScene *pSceneNext = NULL;           // 次のオブジェクトのポインタ
	CObject *pObject = NULL;             // 配置物のポインタ

	float fData = (float)m_aSkillLevel[1] / 5.0f;

	for (int nCntParticle = 0; nCntParticle < 5 * fData; nCntParticle++)
	{
		m_fSpin += 50.0f;
		float fAngle = (rand() % 628 * 0.01f) - D3DX_PI;
		D3DXVECTOR3 movePar(0.0f, 0.0f, 0.0f);
		movePar.x = sinf(fAngle) * (rand() % 10 * 0.75f);
		movePar.y = (rand() % 10 * 0.0f);
		movePar.z = cosf(fAngle) * (rand() % 10 * 0.75f);
		CDamageParticle::Create(D3DXVECTOR3(m_apModel[15]->GetMtxWorld()._41, m_apModel[15]->GetMtxWorld()._42, m_apModel[15]->GetMtxWorld()._43), movePar, D3DXCOLOR(rand() % 10 * 0.5f, rand() % 10 * 0.5f, 1.0f, 1.0f), rand() % 3 + 15.0f, 25);
	}

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULLになるまで繰り返す
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_ENEMY ||
				pScene->GetObjType() == OBJTYPE_BIRD ||
				pScene->GetObjType() == OBJTYPE_FROG)
			{
				CEnemy *pEnemy = (CEnemy*)pScene;

				if (pEnemy != NULL)
				{// 配置物が取得できた
					D3DXVECTOR3 EnemeyPos, EnemyMove;
					EnemeyPos = pEnemy->GetPos();
					EnemyMove = pEnemy->GetMove();
					float fLength = powf((EnemeyPos.x - m_pos.x)*(EnemeyPos.x - m_pos.x) + (EnemeyPos.z - m_pos.z)*(EnemeyPos.z - m_pos.z), 0.5f);	//プレイヤーとの距離
					float fHit = powf(8.0f, 2.0f);	//半径

					if (fHit > fLength)
					{
						int nData = (int)m_aSkillLevel[1];
						nData = rand() % 3 + nData;
						pEnemy->HitDamage(nData);
						float fAttackAngle = atan2f(EnemeyPos.x - m_pos.x, EnemeyPos.z - m_pos.z);// 当たった角度を計算
						EnemyMove.x = sinf((D3DX_PI *  fAttackAngle)) * 25.0f;
						EnemyMove.z = cosf((D3DX_PI *  fAttackAngle)) * 25.0f;
						if (pEnemy->GetObjType() == OBJTYPE_BIRD)
						{
							pEnemy->SetMove(EnemyMove);
						}
					}

				}
			}
			// 次のオブジェクトへのポインタを取得
			pScene = pSceneNext;

		}
	}
}

//=============================================================================
// プレイヤーのダメージ処理
//=============================================================================
void CPlayer::AttackSkill2(void)
{
	// 配置物と地面へのポインタ取得する
	CScene *pScene = NULL;               // オブジェクトのポインタ
	CScene *pSceneNext = NULL;           // 次のオブジェクトのポインタ
	CObject *pObject = NULL;             // 配置物のポインタ

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULLになるまで繰り返す
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_ENEMY ||
				pScene->GetObjType() == OBJTYPE_BIRD ||
				pScene->GetObjType() == OBJTYPE_FROG)
			{
				CEnemy *pEnemy = (CEnemy*)pScene;

				if (pEnemy != NULL)
				{// 配置物が取得できた
					D3DXVECTOR3 EnemeyPos, EnemyMove;
					EnemeyPos = pEnemy->GetPos();
					EnemyMove = pEnemy->GetMove();
					float fLength = powf((EnemeyPos.x - m_pos.x)*(EnemeyPos.x - m_pos.x) + (EnemeyPos.z - m_pos.z)*(EnemeyPos.z - m_pos.z), 0.75f);	//プレイヤーとの距離
					float fHit = powf(6.0f, 4.0f);	//半径

					if (fHit > fLength)
					{
						int nData = (int)m_aSkillLevel[2];
						nData = 7;
						pEnemy->HitDamage(nData);
						float fAttackAngle = atan2f(EnemeyPos.x - m_pos.x, EnemeyPos.z - m_pos.z);// 当たった角度を計算
						EnemyMove.x = sinf((D3DX_PI *  fAttackAngle)) * 25.0f;
						EnemyMove.z = cosf((D3DX_PI *  fAttackAngle)) * 25.0f;
						if (pEnemy->GetObjType() == OBJTYPE_BIRD)
						{
							pEnemy->SetMove(EnemyMove);
						}

					}

				}
			}
			// 次のオブジェクトへのポインタを取得
			pScene = pSceneNext;

		}
	}
}
//=============================================================================
// 鳥敵の死亡数加算処理
//=============================================================================
void CPlayer::AddDeathBird(int nValue)
{
	m_nNumDeathBird += nValue;
}

//=============================================================================
// かえる敵の死亡数加算処理
//=============================================================================
void CPlayer::AddDeathFrog(int nValue)
{
	m_nNumDeathFrog += nValue;
}

//=============================================================================
// 鳥敵の死亡数加算処理
//=============================================================================
int CPlayer::GetDeathBird(void)
{
	return m_nNumDeathBird;
}

//=============================================================================
// かえる敵の死亡数取得処理
//=============================================================================
int CPlayer::GetDeathFrog(void)
{
	return m_nNumDeathFrog;
}

//=============================================================================
// 攻撃の当たり判定処理
//=============================================================================
bool CPlayer::HitAttack(void)
{
	int nattack = 0;
	float fRange = 30.0f;
	float fWeaponRange = 15.0f;
	float fLength = 0.0f;
	float fHit = 0.0f;
	bool bHit = false;

	// 配置物と地面へのポインタ取得する
	CScene *pScene = NULL;               // オブジェクトのポインタ
	CScene *pSceneNext = NULL;           // 次のオブジェクトのポインタ
	CObject *pObject = NULL;             // 配置物のポインタ
	D3DXVECTOR3 Offset = D3DXVECTOR3(0.0f,25.0f,0.0f);
	D3DXVec3TransformCoord(&Offset, &Offset, &m_apModel[15]->GetMtxWorld());
	fHit = sqrtf((fRange + fWeaponRange) * (fRange + fWeaponRange));

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULLになるまで繰り返す
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_ENEMY ||
				pScene->GetObjType() == OBJTYPE_BIRD ||
				pScene->GetObjType() == OBJTYPE_FROG)
			{
				CEnemy *pEnemy = (CEnemy*)pScene;
				if (pEnemy != NULL)
				{// 配置物が取得できた
					if (m_bAction == true)
					{

						D3DXVECTOR3 EnemeyPos, EnemyMove;
						EnemeyPos = pEnemy->GetPos();
						EnemyMove = pEnemy->GetMove();
						fLength = sqrtf((Offset.x - EnemeyPos.x) * (Offset.x - EnemeyPos.x) + (Offset.y - EnemeyPos.y) * (Offset.y - EnemeyPos.y) + (Offset.z - EnemeyPos.z) * (Offset.z - EnemeyPos.z));

						if (fHit >= fLength)
						{
							int nDamage = rand() % 1 + 3;
							pEnemy->HitDamage(nDamage);
							float fAttackAngle = atan2f(EnemeyPos.x - m_pos.x, EnemeyPos.z - m_pos.z);// 当たった角度を計算
							EnemyMove.x = sinf((D3DX_PI *  fAttackAngle)) * -25.0f;
							EnemyMove.z = cosf((D3DX_PI *  fAttackAngle)) * 25.0f;
							if (pEnemy->GetObjType() == OBJTYPE_BIRD)
							{
								pEnemy->SetMove(EnemyMove);
								bHit = true;
							}
							m_nSkill += 1;
							if (m_nSkill >= 40)
							{
								m_nSkill = 40;
							}
							nattack = 1;
							m_bAction = false;
						}

					}
				}
			}
			// 次のオブジェクトへのポインタを取得
			pScene = pSceneNext;
		}
	}

	return bHit;
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
	int nCntAttack = 0;
	FILE *pFile;	//ファイルのポインタ

	pFile = fopen(MOTION_TXT_NAME, "r");	//テキスト読み込み
	if (pFile != NULL)
	{//ファイルが開けた場合
		char *pStrCur;						//文字列の先頭へのポインタ
		char aLine[256];					//文字列の読み込み用
		char aStr[256];						//文字列抜き出し用
		int nIdx;

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
												m_apModel[nIdx]->SetParent(NULL);
												//m_offsetPos[nIdx] = m_apModel[nIdx]->GetPos();
											}
											else if (m_nIndexParent[nCntParts] > -1)
											{
												m_apModel[nIdx] = CModel::Create(D3DXVECTOR3(m_pos.x + m_aKeyOffset[nIdx].fPosX, m_pos.y + m_aKeyOffset[nIdx].fPosY, m_pos.z + m_aKeyOffset[nIdx].fPosZ));
												m_apModel[nIdx]->BindX(m_pMesh[nIdx], m_pBuffMat[nIdx], m_nNumMat[nIdx]);
												m_apModel[nIdx]->SetParent(m_apModel[m_nIndexParent[nIdx]]);
												//m_offsetPos[nIdx] = m_apModel[nIdx]->GetPos();
											}
											m_offsetPos[nIdx] = m_apModel[nIdx]->GetPos();
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
					else if (memcmp(&aStr[0], TEXT_COLLISION, strlen(TEXT_COLLISION)) == 0)
					{
						pStrCur += strlen(TEXT_COLLISION);								//頭出し
						pStrCur = CLoadFileFunction::GetLineTop(pStrCur);				//空白消去
						strcpy(&aStr[0], pStrCur);										//文字列を取り出す
						int nWord = 0;

						m_aMotionAttack[nCntMotion].nAttackStartFrame = atoi(&aStr[0]);	//X座標の代入

						nWord = CLoadFileFunction::PopString(pStrCur, &aStr[0]);		//文字数カウント

						pStrCur += nWord;												//カウントされた分頭出し
						strcpy(&aStr[0], pStrCur);										//文字列を取り出す
						m_aMotionAttack[nCntMotion].nAttackEndFrame = atoi(&aStr[0]);	//X座標の代入
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
												m_aMotionAttack[nCntMotion].nNumFrame += atoi(&aStr[0]);
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
						nCntAttack++;
					}
					else if (memcmp(&aStr[0], TEXT_END_MOTIONSET, strlen(TEXT_END_MOTIONSET)) == 0)
					{//モーションの設置終了を示す行の場合
						break;
					}
				}
			}
		}
	}
	fclose(pFile);	//ファイルを閉じる
}