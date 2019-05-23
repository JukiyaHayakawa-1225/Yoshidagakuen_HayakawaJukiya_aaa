//=============================================================================
//
// メッシュフィールドの処理 [meshField.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>


#include "meshField.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESH_SIZE (25.0f)
//#define MESH_VERTEX ((MESH_X + 1) * (MESH_Z + 1))	//頂点数
//#define MESH_NUMBER (((MESH_X * MESH_Z) * 2) + ((MESH_Z - 1) * 4))
//#define MESH_INDEX /*(MESH_X + 1) * (MESH_Z + 1) + (2 * (MESH_Z - 1)) + (MESH_X + 1) * (MESH_Z - 1)*/((((MESH_X + 1) * 2) * (MESH_Z)) + ((MESH_Z - 1) * 2))
//#define TXT_NAME "MeshHeight.txt"		//テキスト名
#define BIN_NAME "data/TEXT/MeshHeight.bin"		//バイナリ名
//*****************************************************************************
// グローバル変数
//*****************************************************************************
float g_fHeight[MESH_X + 1][MESH_Z + 1];			//高さ

//=============================================================================
// コンストラクタ
//=============================================================================
CMeshField::CMeshField(int nPrioriry, OBJTYPE objtype) : CScene(nPrioriry, objtype)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の初期設定
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向きの初期設定
	m_pTexture = NULL;							//テクスチャの初期化
	m_pVtxBuff = NULL;							//頂点バッファの初期化
	m_pIdxBuff = NULL;							//インデックスバッファの初期化
	m_nNumVartex = 0;							//頂点の総数を初期化
	m_nNumIdx = 0;								//インデックスの総数を初期化
	m_nNumPolygon = 0;							//ポリゴンの総数を初期化

	for (int nCntNor = 0; nCntNor < NUM_POLYGON; nCntNor++)
	{//ポリゴンの数だけ繰り返し
		m_aNor[nCntNor] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//法線を初期化
	}

}
//=============================================================================
// デストラクタ
//=============================================================================
CMeshField::~CMeshField()
{

}
//=============================================================================
// オブジェクトXの生成
//=============================================================================
CMeshField *CMeshField::Create(D3DXVECTOR3 pos)
{
	CMeshField *pMeshField = NULL;		//ポインタ

	if (pMeshField == NULL)
	{
		pMeshField = new CMeshField;	//動的確保
		pMeshField->m_pos = pos;		//位置の代入
		pMeshField->Init();				//初期化処理
	}
	return pMeshField;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMeshField::Init(void)
{
	m_pTexture = NULL;					//テクスチャの初期化
	m_pVtxBuff = NULL;					//頂点バッファを初期化
	m_pIdxBuff = NULL;					//インデックスバッファを初期化
	m_nNumVartex = 0;					//頂点の総数を初期化
	m_nNumIdx = 0;						//インデックスの総数を初期化
	m_nNumPolygon = 0;					//ポリゴンの総数を初期化

										//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\field002.jpg", &m_pTexture);

	//頂点数の計算
	m_nNumVartex = (MESH_X + 1) * (MESH_Z + 1);

	//インデックス数の計算
	m_nNumIdx = (MESH_X + 1) * (MESH_Z + 1)
		+ (2 * (MESH_Z - 1))
		+ (MESH_X + 1) * (MESH_Z - 1);

	//ポリゴン数を計算
	m_nNumPolygon = m_nNumIdx - 2;

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVartex,	// 確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,		//インデックスの情報を2バイトにする
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

						//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCntPolygon = 0;	//ポリゴンのカウンター

	for (int nCntVtxZ = 0; nCntVtxZ < MESH_Z + 1; nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < MESH_X + 1; nCntVtxX++)
		{
			//位置の初期化
			pVtx[(nCntVtxZ + nCntVtxX) + nCntPolygon].pos = D3DXVECTOR3((-MESH_SIZE + (nCntVtxX * MESH_SIZE)), 0.0f, (MESH_SIZE - (nCntVtxZ * MESH_SIZE)));
			//テクスチャの初期化
			pVtx[(nCntVtxZ + nCntVtxX) + nCntPolygon].tex = D3DXVECTOR2(1.0f * (nCntVtxX % (MESH_X + 1)), 1.0f * (nCntVtxZ));
			//色の初期化
			pVtx[(nCntVtxZ + nCntVtxX) + nCntPolygon].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			//法線の初期化
			pVtx[(nCntVtxZ + nCntVtxX) + nCntPolygon].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		}
		nCntPolygon += MESH_X;	//カウンターにX軸の総数加算
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//起伏の読み込み
	//LoadHeight();

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	nCntPolygon = 0;	//ポリゴンのカウンター初期化
	int nCntNor = 0;	//法線

	for (int nCntZ = 0; nCntZ < MESH_Z; nCntZ++)
	{//Zの数だけ繰り返し
		for (int nCntX = 0; nCntX < MESH_X; nCntX++)
		{//Xの数だけ繰り返し
			D3DXVECTOR3 *pPos0, *pPos1, *pPos2, *pPos3;	//位置のポインタ
			D3DXVECTOR3 vec0, vec1, vec2;				//ベクトル
			D3DXVECTOR3 nor;							//法線

			//位置の代入
			pPos0 = &pVtx[(nCntZ + nCntX + nCntPolygon)].pos;
			pPos1 = &pVtx[(nCntZ + nCntX + nCntPolygon) + MESH_X + 1].pos;
			pPos2 = &pVtx[(nCntZ + nCntX + nCntPolygon) + MESH_X + 2].pos;
			pPos3 = &pVtx[(nCntZ + nCntX + nCntPolygon) + 1].pos;

			//ベクトル情報取得
			vec0 = *pPos1 - *pPos0;
			vec1 = *pPos2 - *pPos0;
			vec2 = *pPos3 - *pPos0;

			D3DXVec3Cross(&nor, &vec1, &vec0);
			D3DXVec3Normalize(&nor, &nor);								//正規化する
			m_aNor[((nCntZ * 2) + (nCntX * 2) + nCntNor)] = nor;		//法線を保管

			D3DXVec3Cross(&nor, &vec2, &vec1);
			D3DXVec3Normalize(&nor, &nor);								//正規化する
			m_aNor[((nCntZ * 2) + (nCntX * 2) + nCntNor + 1)] = nor;	//法線を保管
		}
		nCntPolygon += MESH_X;			//ポリゴンのカウンターX軸の総数分加算
		nCntNor += (MESH_X * 2) - 2;	//法線のカウンター加算
	}
	for (int nCntZ = 0; nCntZ < MESH_Z + 1; nCntZ++)
	{//Zの数だけ繰り返し
		for (int nCntX = 0; nCntX < MESH_X + 1; nCntX++)
		{//Xの数だけ繰り返し
			if (nCntZ == 0)
			{//上の場合
				if (nCntX == 0)
				{//左端の場合
					pVtx[0].nor = (m_aNor[0] + m_aNor[1]) / 2;
				}
				else if (nCntX == MESH_X)
				{//右端の場合
					pVtx[MESH_X].nor = (m_aNor[MESH_X + (MESH_X - 1)]);
				}
				else
				{//それ以外の場合
					pVtx[nCntX].nor = (m_aNor[((nCntX * 2) - 1)]
						+ m_aNor[((nCntX * 2) - 1) + 1]
						+ m_aNor[((nCntX * 2) - 1) + 2]) / 3;
				}
			}
			else if (nCntZ == MESH_Z)
			{//下の場合
				if (nCntX == 0)
				{//左端の場合
					pVtx[MESH_Z * (MESH_X + 1)].nor = m_aNor[2 * (MESH_X * (MESH_Z - 1))];
				}
				else if (nCntX == MESH_X)
				{//右端の場合
					pVtx[(MESH_X * MESH_Z) + (MESH_X + MESH_Z)].nor =
						(m_aNor[(2 * (MESH_X * (MESH_Z - 1))) + (2 * (MESH_X - 1))] +
							m_aNor[((2 * (MESH_X * (MESH_Z - 1))) + (2 * (MESH_X - 1))) + 1]) / 2;
				}
				else
				{//それ以外の場合
					pVtx[(MESH_Z * (MESH_X + 1)) + nCntX].nor =
						(m_aNor[(MESH_Z - 1) * (MESH_X * 2) + (nCntX * 2) - 2] +
							m_aNor[((MESH_Z - 1) * (MESH_X * 2) + (nCntX * 2) - 2) + 1] +
							m_aNor[((MESH_Z - 1) * (MESH_X * 2) + (nCntX * 2) - 2) + 2]) / 3;
				}
			}
			else if (nCntZ > 0 && nCntZ < MESH_Z)
			{
				if (nCntX == 0)
				{//左端の場合
					pVtx[(MESH_X + 1) + (((MESH_X + 1) * nCntZ) - (MESH_X + 1))].nor = (m_aNor[(2 * MESH_X)*(nCntZ - 1)]
						+ m_aNor[(2 * MESH_X)*(nCntZ)]
						+ m_aNor[(2 * MESH_X)*(nCntZ)+1]) / 3;
				}
				else if (nCntX == MESH_X)
				{//右端の場合
					pVtx[((2 * MESH_X) + 1) + (((MESH_X + 1) * nCntZ) - (MESH_X + 1))].nor = (m_aNor[(MESH_X * nCntZ) * 2 - 1]
						+ m_aNor[(MESH_X * nCntZ) * 2 - 2]
						+ m_aNor[(MESH_X * (nCntZ + 1)) * 2 - 1]) / 3;
				}
				else
				{//それ以外の場合
					pVtx[(MESH_X + 2) + (nCntX - 1) + ((nCntZ * (MESH_X + 1)) - (MESH_X + 1))].nor =
						(m_aNor[((nCntX - 1) * 2) + ((nCntZ * (2 * MESH_X)) - (2 * MESH_X))] +
							m_aNor[((((nCntX - 1) * 2) + 1) + ((nCntZ * (2 * MESH_X)) - (2 * MESH_X)))] +
							m_aNor[((((nCntX - 1) * 2) + 2) + ((nCntZ * (2 * MESH_X)) - (2 * MESH_X)))] +
							m_aNor[((2 * MESH_X) + ((nCntZ * (2 * MESH_X)) - (2 * MESH_X))) + ((nCntX * 2) - 2)] +
							m_aNor[((2 * MESH_X) + ((nCntZ * (2 * MESH_X)) - (2 * MESH_X)) + 1) + ((nCntX * 2) - 2)] +
							m_aNor[((2 * MESH_X) + ((nCntZ * (2 * MESH_X)) - (2 * MESH_X)) + 2) + ((nCntX * 2) - 2)]) / 6;
				}
			}
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	WORD *pIdx;	//インデックスデータへのポインタ

				//インデックスバッファをロックし、インデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	int nCntMesh = 0;		//メッシュのカウンター

	for (int nCntVtxZ = 0; nCntVtxZ < MESH_Z; nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < MESH_X + 1; nCntVtxX++, nCntMesh++)
		{
			//インデックスの設定
			pIdx[0] = nCntMesh + MESH_X + 1;
			pIdx[1] = nCntMesh;
			pIdx += 2;						//ポインタを進める

			if (nCntVtxZ < MESH_Z - 1 && nCntVtxX == MESH_X)
			{
				pIdx[0] = nCntMesh;
				pIdx[1] = nCntMesh + (MESH_X + 1) + 1;
				pIdx += 2;					//ポインタを進める
			}
		}
	}

	//インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();

	LoadHeight();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMeshField::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	//インデックスの開放
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}
	//データの開放
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMeshField::Update(void)
{
	VERTEX_3D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCntPolygon = 0;		//ポリゴンのカウンター
	int nCntNor = 0;			//法線のカウンター

	for (int nCntZ = 0; nCntZ < MESH_Z; nCntZ++)
	{//Zの数だけ繰り返し
		for (int nCntX = 0; nCntX < MESH_X; nCntX++)
		{//Xの数だけ繰り返し
			D3DXVECTOR3 *pPos0, *pPos1, *pPos2, *pPos3;	//位置のポインタ
			D3DXVECTOR3 vec0, vec1, vec2;				//ベクトル
			D3DXVECTOR3 nor;							//法線

														//位置の代入
			pPos0 = &pVtx[(nCntZ + nCntX + nCntPolygon)].pos;
			pPos1 = &pVtx[(nCntZ + nCntX + nCntPolygon) + MESH_X + 1].pos;
			pPos2 = &pVtx[(nCntZ + nCntX + nCntPolygon) + MESH_X + 2].pos;
			pPos3 = &pVtx[(nCntZ + nCntX + nCntPolygon) + 1].pos;

			//ベクトル情報取得
			vec0 = *pPos1 - *pPos0;
			vec1 = *pPos2 - *pPos0;
			vec2 = *pPos3 - *pPos0;

			D3DXVec3Cross(&nor, &vec1, &vec0);
			D3DXVec3Normalize(&nor, &nor);								//正規化する
			m_aNor[((nCntZ * 2) + (nCntX * 2) + nCntNor)] = nor;		//法線を保管

			D3DXVec3Cross(&nor, &vec2, &vec1);
			D3DXVec3Normalize(&nor, &nor);								//正規化する
			m_aNor[((nCntZ * 2) + (nCntX * 2) + nCntNor + 1)] = nor;	//法線を保管
		}
		nCntPolygon += MESH_X;			//ポリゴンのカウンターX軸の総数分加算
		nCntNor += (MESH_X * 2) - 2;	//法線のカウンター加算
	}
	for (int nCntZ = 0; nCntZ < MESH_Z + 1; nCntZ++)
	{//Zの数だけ繰り返し
		for (int nCntX = 0; nCntX < MESH_X + 1; nCntX++)
		{//Xの数だけ繰り返し
			if (nCntZ == 0)
			{//上の場合
				if (nCntX == 0)
				{//左端の場合
					pVtx[0].nor = (m_aNor[0] + m_aNor[1]) / 2;
				}
				else if (nCntX == MESH_X)
				{//右端の場合
					pVtx[MESH_X].nor = (m_aNor[MESH_X + (MESH_X - 1)]);
				}
				else
				{//それ以外の場合
					pVtx[nCntX].nor = (m_aNor[((nCntX * 2) - 1)]
						+ m_aNor[((nCntX * 2) - 1) + 1]
						+ m_aNor[((nCntX * 2) - 1) + 2]) / 3;
				}
			}
			else if (nCntZ == MESH_Z)
			{//下の場合
				if (nCntX == 0)
				{//左端の場合
					pVtx[MESH_Z * (MESH_X + 1)].nor = m_aNor[2 * (MESH_X * (MESH_Z - 1))];
				}
				else if (nCntX == MESH_X)
				{//右端の場合
					pVtx[(MESH_X * MESH_Z) + (MESH_X + MESH_Z)].nor =
						(m_aNor[(2 * (MESH_X * (MESH_Z - 1))) + (2 * (MESH_X - 1))] +
							m_aNor[((2 * (MESH_X * (MESH_Z - 1))) + (2 * (MESH_X - 1))) + 1]) / 2;
				}
				else
				{//それ以外の場合
					pVtx[(MESH_Z * (MESH_X + 1)) + nCntX].nor =
						(m_aNor[(MESH_Z - 1) * (MESH_X * 2) + (nCntX * 2) - 2] +
							m_aNor[((MESH_Z - 1) * (MESH_X * 2) + (nCntX * 2) - 2) + 1] +
							m_aNor[((MESH_Z - 1) * (MESH_X * 2) + (nCntX * 2) - 2) + 2]) / 3;
				}
			}
			else if (nCntZ > 0 && nCntZ < MESH_Z)
			{
				if (nCntX == 0)
				{//左端の場合
					pVtx[(MESH_X + 1) + (((MESH_X + 1) * nCntZ) - (MESH_X + 1))].nor = (m_aNor[(2 * MESH_X)*(nCntZ - 1)]
						+ m_aNor[(2 * MESH_X)*(nCntZ)]
						+ m_aNor[(2 * MESH_X)*(nCntZ)+1]) / 3;
				}
				else if (nCntX == MESH_X)
				{//右端の場合
					pVtx[((2 * MESH_X) + 1) + (((MESH_X + 1) * nCntZ) - (MESH_X + 1))].nor = (m_aNor[(MESH_X * nCntZ) * 2 - 1]
						+ m_aNor[(MESH_X * nCntZ) * 2 - 2]
						+ m_aNor[(MESH_X * (nCntZ + 1)) * 2 - 1]) / 3;
				}
				else
				{//それ以外の場合
					pVtx[(MESH_X + 2) + (nCntX - 1) + ((nCntZ * (MESH_X + 1)) - (MESH_X + 1))].nor =
						(m_aNor[((nCntX - 1) * 2) + ((nCntZ * (2 * MESH_X)) - (2 * MESH_X))] +
							m_aNor[((((nCntX - 1) * 2) + 1) + ((nCntZ * (2 * MESH_X)) - (2 * MESH_X)))] +
							m_aNor[((((nCntX - 1) * 2) + 2) + ((nCntZ * (2 * MESH_X)) - (2 * MESH_X)))] +
							m_aNor[((2 * MESH_X) + ((nCntZ * (2 * MESH_X)) - (2 * MESH_X))) + ((nCntX * 2) - 2)] +
							m_aNor[((2 * MESH_X) + ((nCntZ * (2 * MESH_X)) - (2 * MESH_X)) + 1) + ((nCntX * 2) - 2)] +
							m_aNor[((2 * MESH_X) + ((nCntZ * (2 * MESH_X)) - (2 * MESH_X)) + 2) + ((nCntX * 2) - 2)]) / 6;
				}
			}
		}
	}

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void CMeshField::Draw(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

												// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y,
		m_rot.x,
		m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVartex, 0, m_nNumPolygon);
	/*pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 12);*/
}

//=============================================================================
// 高さの書き込み
//=============================================================================
void CMeshField::SaveHeight(void)
{
	//テキストでの書き込み
#if 0
	//ファイルの取得
	FILE *pFile;

	pFile = fopen(TXT_NAME, "w");

	if (pFile != NULL)
	{//ファイルが開けた場合
		VERTEX_3D*pVtx;	//頂点情報へのポインタ

						//頂点バッファをロックし、頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntZ = 0; nCntZ < MESH_Z; nCntZ++)
		{
			for (int nCntX = 0; nCntX < MESH_X; nCntX++)
			{
				fprintf(pFile, "%.1f\n", pVtx->pos.y);
				printf("\nセーブしました。\n");
				pVtx++;
			}
		}
		fclose(pFile);
		// 頂点バッファをアンロック
		m_pVtxBuff->Unlock();
	}
	else
	{//ファイルが開けなかった場合
		printf("ファイルを開くことができませんでした。\n");
	}
#endif

	//バイナリでの書き込み
#if 1
	//ファイルの取得
	FILE *pFile;

	VERTEX_3D*pVtx;	//頂点情報へのポインタ

					//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < MESH_Z + 1; nCntZ++)
	{
		for (int nCntX = 0; nCntX < MESH_X + 1; nCntX++)
		{
			g_fHeight[nCntX][nCntZ] = pVtx->pos.y;
			pVtx++;
		}
	}
	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	pFile = fopen(BIN_NAME, "wb");

	if (pFile != NULL)
	{//ファイルが開けた場合
		fwrite(&g_fHeight[0][0], sizeof(float), (MESH_X + 1) * (MESH_Z + 1), pFile);
		fclose(pFile);
	}
#endif
}

//=============================================================================
// 高さの読み込み
//=============================================================================
void CMeshField::LoadHeight(void)
{
	//テキストでの読み込み
#if 0
	//ファイルの取得
	FILE *pFile;

	pFile = fopen(TXT_NAME, "r");

	if (pFile != NULL)
	{//ファイルが開けた場合
		VERTEX_3D*pVtx;	//頂点情報へのポインタ

						//頂点バッファをロックし、頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntZ = 0; nCntZ < MESH_Z; nCntZ++)
		{
			for (int nCntX = 0; nCntX < MESH_X; nCntX++)
			{
				fscanf(pFile, "%f\n", &pVtx->pos.y);
				pVtx++;
			}
		}
		fclose(pFile);
		// 頂点バッファをアンロック
		m_pVtxBuff->Unlock();
	}
	else
	{//ファイルが開けなかった場合
		printf("ファイルを開くことができませんでした。\n");
	}
#endif

	//バイナリでの読み込み
#if 1
	//ファイルの取得
	FILE *pFile;

	pFile = fopen(BIN_NAME, "rb");

	if (pFile != NULL)
	{//ファイルが開けた場合
		fread(&g_fHeight[0][0], sizeof(float), (MESH_X + 1) * (MESH_Z + 1), pFile);
	}
	fclose(pFile);
	VERTEX_3D*pVtx;	//頂点情報へのポインタ

					//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < MESH_Z + 1; nCntZ++)
	{
		for (int nCntX = 0; nCntX < MESH_X + 1; nCntX++)
		{
			pVtx->pos.y = g_fHeight[nCntX][nCntZ];
			pVtx++;
		}
	}
	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
#endif
}

//=============================================================================
// 高さの取得
//=============================================================================
float CMeshField::GetHeight(D3DXVECTOR3 pos)
{
	bool bVtxX[MESH_X];	//X座標
	bool bVtxZ[MESH_Z];	//Y座標

	for (int nCntX = 0; nCntX < MESH_X; nCntX++)
	{//Xの数だけ繰り返し
		bVtxX[nCntX] = false;
	}
	for (int nCntZ = 0; nCntZ < MESH_Z; nCntZ++)
	{//Zの数だけ繰り返し
		bVtxZ[nCntZ] = false;
	}

	for (int nCntZ = 0; nCntZ < MESH_Z; nCntZ++)
	{//Zの数だけ繰り返し
		for (int nCntX = 0; nCntX < MESH_X; nCntX++)
		{//Xの数だけ繰り返し
			if (pos.x >= -MESH_SIZE + (nCntX * MESH_SIZE) && pos.x <= (nCntX * MESH_SIZE))
			{//左側２つのポリゴンの内側にいる場合
				bVtxX[nCntX] = true;
			}
			else
			{//右側２つのポリゴンの内側にいる場合
				bVtxX[nCntX] = false;
			}

			if (pos.z <= MESH_SIZE + (nCntZ * -MESH_SIZE) && pos.z >= MESH_SIZE + ((nCntZ + 1) * -MESH_SIZE))
			{//上の辺より下にいて下の辺より上にいる場合
				bVtxZ[nCntZ] = true;
			}
			else
			{//上の辺より上にいて下の辺より下にいる場合
				bVtxZ[nCntZ] = false;
			}
		}
	}

	int nCntPolygon = 0;							//ポリゴンのカウンター
	int nCntNor = 0;								//法線のカウンター
	D3DXVECTOR3 *pPos0, *pPos1, *pPos2, *pPos3;		//位置
	D3DXVECTOR3 vec0, vec1, vec2, vec3;				//ベクトル
	D3DXVECTOR3 nor0, nor1;							//法線
	float fData;									//データ格納

	VERTEX_3D*pVtx;	//頂点情報へのポインタ
	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntZ = 0; nCntZ < MESH_Z; nCntZ++)
	{//Zの数だけ繰り返し
		for (int nCntX = 0; nCntX < MESH_X; nCntX++)
		{//Xの数だけ繰り返し
			if (bVtxX[nCntX] == true && bVtxZ[nCntZ] == true)
			{
				//位置の代入
				pPos0 = &pVtx[(nCntZ + nCntX + nCntPolygon)].pos;
				pPos1 = &pVtx[(nCntZ + nCntX + nCntPolygon) + MESH_X + 1].pos;
				pPos2 = &pVtx[(nCntZ + nCntX + nCntPolygon) + MESH_X + 2].pos;
				pPos3 = &pVtx[(nCntZ + nCntX + nCntPolygon) + 1].pos;

				//ベクトル情報取得
				vec0 = *pPos1 - *pPos0;
				vec1 = *pPos2 - *pPos0;
				vec2 = *pPos3 - *pPos0;
				vec3 = pos - *pPos0;

				//２つのベクトルから法線を算出
				D3DXVec3Cross(&nor0, &vec0, &vec1);
				//正規化する
				D3DXVec3Normalize(&nor0, &nor0);

				//２つのベクトルから法線を算出
				D3DXVec3Cross(&nor1, &vec1, &vec2);
				//正規化する
				D3DXVec3Normalize(&nor1, &nor1);

				//２つの法線から平均値を求める
				fData = ((vec1.z * vec3.x) - (vec1.x * vec3.z));

				if (fData >= 0)
				{//左のポリゴンの内側にいる場合
					pos.y = ((nor0.x *(pos.x - pPos0->x) + nor0.z * (pos.z - pPos0->z)) / -nor0.y) + pPos0->y;
				}
				else
				{//右のポリゴンの内側にいる場合
					pos.y = ((nor1.x *(pos.x - pPos2->x) + nor1.z * (pos.z - pPos2->z)) / -nor1.y) + pPos2->y;
				}
			}
		}
		nCntPolygon += MESH_X;			//ポリゴンのカウンター加算
		nCntNor += (MESH_X * 2) - 2;	//法線のカウンター加算
	}
	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
	return pos.y;
}

//=============================================================================
// 高さの設置処理
//=============================================================================
void CMeshField::SetHeight(D3DXVECTOR3 pos, float fValue, float fRange)
{
	VERTEX_3D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < MESH_Z + 1; nCntZ++)
	{//Zの数だけ繰り返し
		for (int nCntX = 0; nCntX < MESH_X + 1; nCntX++)
		{//Xの数だけ繰り返し
		 //プレイヤーの位置と頂点の距離を計算
			float fLength = sqrtf((pos.x - pVtx->pos.x) * (pos.x - pVtx->pos.x)
				+ (pos.z - pVtx->pos.z) * (pos.z - pVtx->pos.z));

			if (fLength < fRange)
			{//対象の頂点が範囲内の場合
				float fHeight = cosf((D3DX_PI / 2) *  (fLength / fRange))  * fValue;
				pVtx->pos.y += fHeight;
			}
			pVtx++;
		}
	}
	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
