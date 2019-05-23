//=============================================================================
//
// オブジェクトの配置の処理 [object.cpp]
// Author : 岸田怜(Kishida Rei)
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "sceneX.h"
#include "input.h"
#include "debugproc.h"
#include "itemcounter.h"
#include "meshfield.h"
#include "game.h"
#include "fade.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_FILE_NAME			("data/TEXT/object_data.txt")		// Xファイルのアドレス
#define MAX_CHAR (256)	// 文字数の最大数
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
char *CObject::m_pModelName[MAXMODEL_TYPE] = {};
int	 CObject::m_nMaxModelNum = 0;
LPDIRECT3DTEXTURE9 CObject::m_apTexture[MAX_TEXTURE_OBJ] = {}; //共有テクスチャのポインタ

//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CObject::CObject(int nPriority, OBJTYPE type) : CSceneX(nPriority, type)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置の初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置の移動量の初期化
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向きの初期化
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 目的の向きを初期化
}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CObject::~CObject()
{

}


//=============================================================================
// 関数名：使用するテクスチャの読み込み
// 関数の概要：--
//=============================================================================
HRESULT CObject::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/coin_UV.jpg", &m_apTexture[0]);		// コイン
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/key02.jpg", &m_apTexture[1]);			// カギ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wood01.png", &m_apTexture[2]);			// 木
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/sky222.png", &m_apTexture[3]);			// 空
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cloud.png", &m_apTexture[4]);			// 雲
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/skyObj_UI.png", &m_apTexture[5]);		// 雲
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/D_UV222_.png", &m_apTexture[6]);		// ダンジョン
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UV_DOOR.jpg", &m_apTexture[7]);		// ドア
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/skyObj_UI.png", &m_apTexture[8]);		// 雲


	return S_OK;

}

//=============================================================================
// 関数名：使用したテクスチャの破棄
// 関数の概要：--
//=============================================================================
void CObject::Unload(void)
{
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE_OBJ; nCntTexture++)
	{
		m_apTexture[nCntTexture]->Release();
		m_apTexture[nCntTexture] = NULL;
	}
}


//=============================================================================
// 関数名：モデルの生成
// 関数の概要：モデルを生成する
//=============================================================================
CObject *CObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int type)
{
	CObject  *pModel;
	pModel = new CObject;//シーンクラスの生成
	if (pModel != NULL)
	{
		pModel->m_nTypeModel = type;
		pModel->m_pos = pos;
		pModel->m_rot = rot;
		pModel->m_fHeight = 100.0f;
		//初期化
		pModel->Init();
	}
	return pModel;
}

//=============================================================================
// 関数名：モデルの初期化処理
// 関数の概要：モデルの初期化
//=============================================================================
HRESULT CObject::Init(void)
{
	//位置情報の代入
	SetPos(m_pos);

	//角度情報の代入
	SetRot(m_rot);

	//ファイル名の割り当て
	BindFileName(m_pModelName[m_nTypeModel]);

	//初期化処理
	CSceneX::Init();

	//テクスチャの割り当て
	BindTexture(m_apTexture[m_nTypeModel]);

	m_vtxMax = GetVtxMax();		//最大頂点座標の取得
	m_vtxMin = GetVtxMin();		//最小頂点座標の取得

	return S_OK;
}

//=============================================================================
// 関数名：モデルの終了処理
// 関数の概要：モデルの終了
//=============================================================================
void CObject::Uninit(void)
{
	CSceneX::Uninit();
}

//=============================================================================
// 関数名：モデルの更新
// 関数の概要：--
//=============================================================================
void CObject::Update(void)
{
	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//メッシュフィールド取得
	CMeshField *pMeshField;
	pMeshField = CGame::GetSceneMeshField();


	switch (m_nTypeModel)
	{
	case OBJECTTYPE_COIN:	// コイン
		m_rot.y += 0.05f;
		//m_pos.y = pMeshField->GetHeight(m_pos);
		break;
	case OBJECTTYPE_KEY:	// カギ
		m_rot.y += 0.05f;
		//m_pos.y = pMeshField->GetHeight(m_pos);
		break;
	case OBJECTTYPE_WOOD:	// 木
		break;
	case OBJECTTYPE_SKY:	// 空
		m_rot.x -= 0.0006f;
		break;
	case OBJECTTYPE_CLOUD:	// 雲
		m_rot.y += 0.001f;
		break;
	case OBJECTTYPE_GOAL:	// ゴール
		break;
	}
	//角度情報の代入
	SetRot(m_rot);

	//位置情報の代入
	SetPos(m_pos);
}

//=============================================================================
// 関数名：モデルの描画処理
// 関数の概要：階層構造で描画
//=============================================================================
void CObject::Draw(void)
{
	//モデルの描画
	CSceneX::Draw();
}




//=============================================================================
// 関数名：モデルの情報の読み込み処理
// 関数の概要：テキストファイルからモデル情報を読み込む
//=============================================================================
void CObject::LoadModelText(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();
	//D3DXMATERIAL *pMat;		//マテリアルへのポインタ

	FILE *pFile = fopen(MODEL_FILE_NAME, "r");		// 読み込むテキストファイルのアドレス

	// 読み込むための変数宣言
	char acLine[MAX_CHAR];							// 1行分読み込む変数
	char acData[MAX_CHAR];							// 1文を読み込む変数
	char cFileName[MAXMODEL_TYPE][MAX_CHAR];		// 読み込む
	int nCntModel = 0;								// 読み込むモデルの数
	int nCntTexture = 0;							// 読み込むテクスチャ数
	int nTypeModel;									// 使用するモデルのタイプ
	int nMaxModel = 0;								// モデルの最大数
	D3DXVECTOR3 pos;								// 位置を入れる変数
	D3DXVECTOR3 rot;								// 向きを入れる変数

	if (pFile != NULL)
	{// ヌルチェック
		while (fgets(&acLine[0], MAX_CHAR, pFile) != NULL)
		{
			sscanf(&acLine[0], "%s", &acData);				// 文をデータにコピー

			if (strcmp(acData, "NUM_MODEL") == 0)
			{// "NUM_MODEL"の文字列があったら
				sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nMaxModel);	// モデルの数を取得
			}
			if (strcmp(acData, "MODEL_FILENAME") == 0)
			{// "MODEL_FILENAME"の文字列があったら
				sscanf(&acLine[0], "%s %s %s", &acData[0], &acData[0], &cFileName[nCntModel][0]);	// モデルの数を取得
				m_pModelName[nCntModel] = &cFileName[nCntModel][0];//ファイル名の代入
				nCntModel++;		// モデル数を加算
			}
			if (strcmp(acData, "MODELSET") == 0)
			{// "MODELSET"の文字列があったら

				while (strcmp(acData, "END_MODELSET") != 0)
				{// "END_MODELSET"がまで情報を取得し続ける
					fgets(&acLine[0], MAXCHAR, pFile);					// 行を取得
					sscanf(&acLine[0], "%s", &acData);					// データをコピー

					if (strcmp(acData, "TYPE") == 0)
					{// "TYPE"の文字列があったら
						sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nTypeModel);					// 使用するモデルの番号を取得
					}
					else if (strcmp(acData, "POS") == 0)
					{// "POS"の文字列があったら
						sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &pos.x, &pos.y, &pos.z);	// 位置を取得
					}
					else if (strcmp(acData, "ROT") == 0)
					{// "ROT"の文字列があったら
						sscanf(&acLine[0], "%s %s %f %f %f", &acData[0], &acData[0], &rot.x, &rot.y, &rot.z);	// 向きを取得
						//テキストの角度情報通りに入れる計算
						rot.x = (D3DX_PI * 2 / 360.0f * rot.x);
						rot.y = (D3DX_PI * 2 / 360.0f * rot.y);
						rot.z = (D3DX_PI * 2 / 360.0f * rot.z);
					}
				}
				//モデルの生成
				Create(pos, rot, nTypeModel);
			}
		}
		fclose(pFile);	// 開いたファイルを閉じる
	}
}

//=============================================================================
// 関数名：モデルとの当たり判定処理
// 関数の概要：めりこんだら戻す処理
//=============================================================================
bool CObject::CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fRadius)
{
	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();


	//フェードの取得
	CFade::FADE pFade;
	pFade = CFade::GetFade();

	int nKey;
	nKey = CItemCounter::GetKey();

	bool bRand = false;			// 着地したかどうか
	//----------------------------------------------------------
	// 矩形での当たり判定
	//----------------------------------------------------------
	if (m_pos.y + m_vtxMax.y > pPos->y + fRadius &&
		m_pos.y + m_vtxMin.y <= pPos->y + fRadius)
	{// プレイヤーがオブジェクトの範囲内なら

	 // ブロックの当たり判定
		if (m_pos.x + m_vtxMin.x < pPos->x + fRadius &&
			m_pos.x + m_vtxMax.x > pPos->x - fRadius)
		{// オブジェクトがオブジェクトのZ軸の範囲内に入ったら

			if (m_pos.z + m_vtxMin.z >= pPosOld->z + fRadius &&
				m_pos.z + m_vtxMin.z < pPos->z + fRadius)
			{// 手前からの判定
				if (m_nTypeModel == OBJECTTYPE_COIN)
				{//コインにふれたとき
					CItemCounter::AddCoin(1);
					Uninit();
				}
				else if (m_nTypeModel == OBJECTTYPE_KEY)
				{//カギにふれたとき
					pSound->PlaySound(CSound::SOUND_LABEL_SE_PICKUP);
					CItemCounter::AddKey(1);
					Uninit();
				}
				else if (m_nTypeModel == OBJECTTYPE_GOAL)
				{//ドアにふれたとき
					if (nKey >= 6)
					{
						if (pFade == CFade::FADE_NONE)
						{
							CFade::SetFade(CManager::MODE_RESULT);//リザルトへ移行
						}
					}
				}
				else
				{
					pPos->z = m_pos.z + m_vtxMin.z - fRadius;			// オブジェクトの位置を戻す
				}
			}
			else if (m_pos.z + m_vtxMax.z <= pPosOld->z - fRadius &&
				m_pos.z + m_vtxMax.z > pPos->z - fRadius)
			{// 奥からの判定
				if (m_nTypeModel == OBJECTTYPE_COIN)
				{//コインにふれたとき
					CItemCounter::AddCoin(1);
					Uninit();
				}
				else if (m_nTypeModel == OBJECTTYPE_KEY)
				{//カギにふれたとき
					pSound->PlaySound(CSound::SOUND_LABEL_SE_PICKUP);
					CItemCounter::AddKey(1);
					Uninit();
				}
				else if (m_nTypeModel == OBJECTTYPE_GOAL)
				{//ドアにふれたとき
					if (nKey >= 6)
					{
						if (pFade == CFade::FADE_NONE)
						{
							CFade::SetFade(CManager::MODE_RESULT);//リザルトへ移行
						}
					}
				}
				else
				{
					pPos->z = m_pos.z + m_vtxMax.z + fRadius;			// オブジェクトの位置を戻す
				}
			}

		}
		if (m_pos.z + m_vtxMin.z < pPos->z + fRadius &&
			m_pos.z + m_vtxMax.z > pPos->z - fRadius)
		{// Z軸がオブジェクトの範囲内だったら
			if (m_pos.x + m_vtxMin.x >= pPosOld->x + fRadius &&
				m_pos.x + m_vtxMin.x < pPos->x + fRadius)
			{// 左からの判定
				if (m_nTypeModel == OBJECTTYPE_COIN)
				{//コインにふれたとき
					CItemCounter::AddCoin(1);
					Uninit();
				}
				else if (m_nTypeModel == OBJECTTYPE_KEY)
				{//カギにふれたとき
					pSound->PlaySound(CSound::SOUND_LABEL_SE_PICKUP);
					CItemCounter::AddKey(1);
					Uninit();
				}
				else if (m_nTypeModel == OBJECTTYPE_GOAL)
				{//ドアにふれたとき
					if (nKey >= 6)
					{
						if (pFade == CFade::FADE_NONE)
						{
							CFade::SetFade(CManager::MODE_RESULT);//リザルトへ移行
						}
					}
				}
				else
				{
					pPos->x = m_pos.x + m_vtxMin.x - fRadius;			// オブジェクトの位置を戻す
				}
			}
			else if (m_pos.x + m_vtxMax.x <= pPosOld->x - fRadius &&
				m_pos.x + m_vtxMax.x > pPos->x - fRadius)
			{// 右からの判定
				if (m_nTypeModel == OBJECTTYPE_COIN)
				{//コインにふれたとき
					CItemCounter::AddCoin(1);
					Uninit();
				}
				else if (m_nTypeModel == OBJECTTYPE_KEY)
				{//カギにふれたとき
					pSound->PlaySound(CSound::SOUND_LABEL_SE_PICKUP);
					CItemCounter::AddKey(1);
					Uninit();
				}
				else if (m_nTypeModel == OBJECTTYPE_GOAL)
				{//ドアにふれたとき
					if (nKey >= 6)
					{
						if (pFade == CFade::FADE_NONE)
						{
							CFade::SetFade(CManager::MODE_RESULT);//リザルトへ移行
						}
					}
				}
				else
				{
					pPos->x = m_pos.x + m_vtxMax.x + fRadius;			// オブジェクトの位置を戻す
				}
			}
		}
	}
	if (m_pos.x + m_vtxMin.x < pPos->x + fRadius &&
		m_pos.x + m_vtxMax.x > pPos->x - fRadius &&
		m_pos.z + m_vtxMin.z < pPos->z + fRadius &&
		m_pos.z + m_vtxMax.z > pPos->z - fRadius)
	{// オブジェクトの範囲内だったら
		if (m_pos.y + m_vtxMax.y <= pPosOld->y &&
			m_pos.y + m_vtxMax.y > pPos->y)
		{// 上からの判定
			if (m_nTypeModel == OBJECTTYPE_COIN)
			{//コインにふれたとき
				CItemCounter::AddCoin(1);
				Uninit();
			}
			else if (m_nTypeModel == OBJECTTYPE_GOAL)
			{//ドアにふれたとき
				if (nKey >= 6)
				{
					if (pFade == CFade::FADE_NONE)
					{
						CFade::SetFade(CManager::MODE_RESULT);//リザルトへ移行
					}
				}
			}
			else if (m_nTypeModel == OBJECTTYPE_KEY)
			{//カギにふれたとき
				pSound->PlaySound(CSound::SOUND_LABEL_SE_PICKUP);
				CItemCounter::AddKey(1);
				Uninit();
			}
			pPos->y = m_pos.y + m_vtxMax.y;					// オブジェクトの位置を戻す
			pMove->y = 0.0f;								// 落下速度を0.0に戻す
			bRand = true;									// 着地した判定にする
		}
	}

	return bRand;
}


//=============================================================================
// 関数名：ゴールとの当たり判定
// 関数の概要：プレイヤーが半径に入ってきたらtrueを返す
//=============================================================================
bool CObject::CollisionGoal(D3DXVECTOR3 *pos, D3DXVECTOR3 *move, D3DXVECTOR3 *rot)
{
	bool bCol = false;

	float fLength = powf((pos->x - m_pos.x)*(pos->x - m_pos.x) + (pos->z - m_pos.z)*(pos->z - m_pos.z), 0.5f);	//プレイヤーとの距離
	float fRadius = powf(30.0f, 1.0f);	//半径

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
