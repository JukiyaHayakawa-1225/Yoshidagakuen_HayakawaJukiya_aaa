//=============================================================================
//
// シーンX処理[3Dポリゴン] [scene3D.cpp]
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
#include "model.h"
#include "scene2D.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_CHAR (256)
#define GRAVITY_NUM (0.7f)
#define MODEL_FILENAME ("data/TEXT/model_data.txt")

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
char CModel::m_acFileName[MAX_PARTS][256] = {};		// 読み込むモデルの名前
//LPDIRECT3DTEXTURE9	*CModel::m_apTexture[MAX_TEXTURE] = {};		// テクスチャへのポインタ
LPD3DXMESH CModel::m_apMesh[MAX_PARTS] = {};	//メッシュ情報へのポインタ
LPD3DXBUFFER CModel::m_apBuffMat[MAX_PARTS] = {};	//マテリアル情報へのポインタ
DWORD CModel::m_aNumMat[MAX_PARTS] = {};	//マテリアルの情報の数


//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CModel::CModel()
{

}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CModel::~CModel()
{
}


//=============================================================================
// 関数名：プレイヤーのパーツ読み込み
// 関数の概要：パーツを読み込む
//=============================================================================
void CModel::Load(void)
{

	FILE *pFile = fopen(MODEL_FILENAME, "r");		// 読み込むテキストファイルのアドレス

													// 読み込むための変数宣言
	char acLine[MAX_CHAR];							// 1行分読み込む変数
	char acData[MAX_CHAR];							// 1文を読み込む変数
	int nCntModel = 0;								// 読み込むモデルの数
	int nCntTexture = 0;							// 読み込むテクスチャ数
	int nIndex = 0;									// 使用するモデルのタイプ
	int nParent = 0;								//親の数字
	int nMaxParts = 0;								//パーツ数
	D3DXVECTOR3 pos;								// 位置を入れる変数
	D3DXVECTOR3 rot;								// 向きを入れる変数
	int nCntParts = 0;								//パーツのカウント

	if (pFile != NULL)
	{// ヌルチェック

		while (fgets(&acLine[0], MAX_CHAR, pFile) != NULL)
		{
			sscanf(&acLine[0], "%s", &acData);				// 文をデータにコピー

			if (strcmp(acData, "NUM_MODEL") == 0)
			{// "NUM_MODEL"の文字列があったら
				sscanf(&acLine[0], "%s %s %d", &acData[0], &acData[0], &nMaxParts);	// モデルの数を取得
			}
			else if (strcmp(acData, "MODEL_FILENAME") == 0)
			{// "MODEL_FILENAME"の文字列があったら
				sscanf(&acLine[0], "%s %s %s", &acData[0], &acData[0], &m_acFileName[nCntModel][0]);	// モデルの数を取得
				nCntModel++;		// モデル数を加算
			}
		}
		fclose(pFile);								// 開いたファイルを閉じる
	}
}

//=============================================================================
// 関数名：プレイヤーのパーツの破棄
// 関数の概要：パーツを読み込む
//=============================================================================
void CModel::Unload(void)
{
	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
	{
		m_acFileName[nCntModel][0] = NULL;
	}
}


//=============================================================================
// 関数名：Xファイルシーンの生成
// 関数の概要：Xファイルシーンを生成する
//=============================================================================
CModel *CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumParts, MODELTYPE type)
{
	CModel  *pModel;
	pModel = new CModel;//シーンクラスの生成
	if (pModel != NULL)
	{
		pModel->m_nNumParts = nNumParts;
		pModel->m_type = type;
		pModel->Init();
		pModel->m_pos = pos;
		pModel->m_rot = rot;

	}
	return pModel;
}

//=============================================================================
// 関数名：Xファイルシーンの初期化処理
// 関数の概要：Xファイル情報の設定
//=============================================================================
HRESULT CModel::Init(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();
	//D3DXMATERIAL *pMat;										// マテリアルデータへのポインタ


	// Xファイルの読み込み
	D3DXLoadMeshFromX(m_acFileName[m_nNumParts],
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_apBuffMat[m_nNumParts],
		NULL,
		&m_aNumMat[m_nNumParts],
		&m_apMesh[m_nNumParts]);

	//if (m_apMesh[m_nNumParts] != NULL)
	//{
	//	// マテリアルデータへのポインタを取得
	//	pMat = (D3DXMATERIAL*)m_apBuffMat[m_nNumParts]->GetBufferPointer();

	//	m_apTexture[m_type] = new LPDIRECT3DTEXTURE9[m_aNumMat[m_nNumParts]];		// マテリアル分容量を増やす
	//	for (int nCntMat = 0; nCntMat < (int)m_aNumMat[m_nNumParts]; nCntMat++)
	//	{// マテリアルの数分ループ
	//		if (pMat[nCntMat].pTextureFilename != NULL)
	//		{// テクスチャのアドレスが存在していたら
	//			D3DXCreateTextureFromFile(pDevice,		// 3Dデバイスのアドレス
	//				pMat[nCntMat].pTextureFilename,		// 読み込むテクスチャのファイルのアドレス
	//				m_apTexture[m_type]);						// テクスチャのポインタにアドレスに設定する
	//		}
	//		else
	//		{// テクスチャのアドレスがないなら
	//			m_apTexture[nCntMat] = NULL;							// NULLに設定する
	//		}
	//	}
	//}


	return S_OK;
}

//=============================================================================
// 関数名：Xファイルシーンの終了処理
// 関数の概要：メッシュ、マテリアル、自身の破棄
//=============================================================================
void CModel::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
	{


		// テクスチャの開放
		if (m_pTexture != NULL)
		{
			m_pTexture->Release();
			m_pTexture = NULL;
		}



		// メッシュの開放
		if (m_apMesh[nCntModel] != NULL)
		{
			m_apMesh[nCntModel]->Release();
			m_apMesh[nCntModel] = NULL;
		}

		// マテリアルの開放
		if (m_apBuffMat[nCntModel] != NULL)
		{
			m_apBuffMat[nCntModel]->Release();
			m_apBuffMat[nCntModel] = NULL;
		}

		if (m_aNumMat[nCntModel] != NULL)
		{
			m_aNumMat[nCntModel] = NULL;
		}
	}

}

//=============================================================================
// 関数名：Xファイルシーンの更新処理
// 関数の概要：--
//=============================================================================
void CModel::Update(void)
{
}


//=============================================================================
// 関数名：Xファイルシーンの描画処理
// 関数の概要：--
//=============================================================================
void CModel::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();


	//親のマトリックス
	D3DXMATRIX mtxParent;


	//計算用マトリックス
	D3DXMATRIX mtxrot, mtxtrans;

	D3DMATERIAL9 matDef;	//現在のマテリアル保存用
	D3DXMATERIAL *pMat;		//マテリアルへのポインタ

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


	//親のマトリックスと掛け合わせる
	if (m_pParent != NULL)
	{
		mtxParent = m_pParent->GetMatrix();
	}
	else
	{
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	if (m_apMesh[m_nNumParts] != NULL)
	{
		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)m_apBuffMat[m_nNumParts]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_aNumMat[m_nNumParts]; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				pDevice->SetTexture(0, m_pTexture);

			// モデル(パーツ)の描画
			m_apMesh[m_nNumParts]->DrawSubset(nCntMat);
		}
	}
	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

}

//=============================================================================
// 関数名：位置情報の代入
// 関数の概要：位置情報を代入する
//=============================================================================
void CModel::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}


//=============================================================================
// 関数名：角度情報の代入
// 関数の概要：角度情報を代入する
//=============================================================================
void CModel::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// 関数名：ワールドマトリックスの代入
// 関数の概要：ワールドマトリックス情報を代入する
//=============================================================================
void CModel::SetMtx(D3DXMATRIX mtx)
{
	m_mtxWorld = m_mtxWorld;
}

//=============================================================================
// 関数名：ワールドマトリックスの取得
// 関数の概要：ワールドマトリックス情報を取得する
//=============================================================================
D3DXMATRIX CModel::GetMatrix(void)
{
	return m_mtxWorld;
}

//=============================================================================
// 関数名：親のモデル
// 関数の概要：親のモデルの設定
//=============================================================================
void CModel::SetParent(CModel *pModel)
{
	m_pParent = pModel;
}


//=============================================================================
// 関数名：テクスチャの割り当て
// 関数の概要：テクスチャを割り当てる
//=============================================================================
void CModel::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;
}
