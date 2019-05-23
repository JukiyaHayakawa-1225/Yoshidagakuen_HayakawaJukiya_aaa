//=============================================================================
//
// メッシュシリンダー処理 [meshcylinder.cpp]
// Author : 岸田怜(Kishida Rei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene3D.h"
#include "manager.h"
#include "input.h"
#include "scene_meshcylinder.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHCYLINDER_RADIUS (2000.0f)
#define MESHCYLINDER_HEIGHT (250.0f)

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int CMeshCylinder::m_nSquareNum = 0;	//四角面の数
int CMeshCylinder::m_nVecNum = 0;	//四角面の数
//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：クリアな値を代入
//=============================================================================
CMeshCylinder::CMeshCylinder(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{

}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：処理の最後に呼び出される
//=============================================================================
CMeshCylinder::~CMeshCylinder()
{
}

//=============================================================================
// 関数名：メッシュシリンダーの生成処理
// 関数の概要：メッシュシリンダーを生成する
//=============================================================================
CMeshCylinder * CMeshCylinder::Create(D3DXVECTOR3 pos)
{
	CMeshCylinder  *pSceneMeshfield;
	pSceneMeshfield = new CMeshCylinder;//シーンクラスの生成
	if (pSceneMeshfield != NULL)
	{
		pSceneMeshfield->m_pos = pos;
		pSceneMeshfield->m_nMesh_H = 30;
		pSceneMeshfield->m_nMesh_V = 1;
		pSceneMeshfield->Init();

	}
	return pSceneMeshfield;
}

//=============================================================================
// 関数名：メッシュシリンダーの初期化処理
// 関数の概要：頂点数とポリゴン数に注意
//=============================================================================
HRESULT CMeshCylinder::Init(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//頂点数
	m_nVertexNum = (m_nMesh_H + 1) * (m_nMesh_V + 1);
	//インデックス数
	m_nIndexNum = (m_nMesh_H * m_nMesh_V) * 2 + (4 * (m_nMesh_V - 1)) + 2;
	//ポリゴン数
	m_nPolygonNum = (m_nMesh_H * m_nMesh_V) * 2 + (4 * (m_nMesh_V - 1));
	//四角の面の数
	m_nSquareNum = (m_nMesh_H * m_nMesh_V);
	//囲うベクトルの数
	m_nVecNum = 6 * m_nSquareNum;

	MakeVertex(pDevice);
	MakeIndex(pDevice);

	return S_OK;
}

//=============================================================================
// 関数名：メッシュシリンダーの終了処理
// 関数の概要：テクスチャ、頂点バッファの破棄
//=============================================================================
void CMeshCylinder::Uninit(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//インデックスバッファの解放
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	Release();
}

//=============================================================================
// 関数名：メッシュシリンダーの描画処理
// 関数の概要：--
//=============================================================================
void CMeshCylinder::Update(void)
{

	m_rot.y += 0.0001f;

}

//=============================================================================
// 関数名：メッシュシリンダーの描画
// 関数の概要：頂点数とポリゴン数に注意
//=============================================================================
void CMeshCylinder::Draw(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxrot, mtxtrans;//マトリックス情報

	//アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 220);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);


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

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	//pDevice->SetFVF(D3DXGetFVFVertexSize);
	pDevice->SetTexture(0, m_pTexture);


	// メッシュシリンダーの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nIndexNum, 0, m_nPolygonNum);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}


//=============================================================================
// 関数名：頂点情報の作成
// 関数の概要：とりあえずは頂点を置くだけ
//=============================================================================
void CMeshCylinder::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/mountain000.png", &m_pTexture);


	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nVertexNum,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);



	// 頂点情報の設定
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMeshCylinder_V = 0; nCntMeshCylinder_V < m_nMesh_V + 1; nCntMeshCylinder_V++)
	{
		for (int nCntMeshCylinder_H = 0; nCntMeshCylinder_H < m_nMesh_H + 1; nCntMeshCylinder_H++)
		{
			pVtx[0].pos.x = sinf(-D3DX_PI * (nCntMeshCylinder_H * (1.0f / (m_nMesh_H / 2)))) * MESHCYLINDER_RADIUS;
			pVtx[0].pos.y = MESHCYLINDER_HEIGHT * nCntMeshCylinder_V;
			pVtx[0].pos.z = cosf(-D3DX_PI * (nCntMeshCylinder_H * (1.0f / (m_nMesh_H / 2)))) * MESHCYLINDER_RADIUS;
			//法線の設定
			pVtx[0].nor = D3DXVECTOR3(sinf(-D3DX_PI * (0.25f * nCntMeshCylinder_H)), 0.0f, cosf(D3DX_PI * (0.25f * nCntMeshCylinder_H)));
			D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

			//頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(nCntMeshCylinder_H * (1.0f / (m_nMesh_H / 2)), nCntMeshCylinder_V * -1.0f);


			pVtx += 1;
		}
	}
	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

}
//=============================================================================
// 関数名：インデックス情報の作成
// 関数の概要：インデックスでポリゴンを生成する
//=============================================================================
void CMeshCylinder::MakeIndex(LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの作成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nIndexNum,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD *pIdx;

	int nCntIndex = 0;	//インデックスのカウント


	//インデックスバッファをロック
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	for (int nCntIndex_V = 0; nCntIndex_V < m_nMesh_V; nCntIndex_V++)
	{
		for (int nCntIndex_H = 0; nCntIndex_H <= m_nMesh_H; nCntIndex_H++, nCntIndex++)
		{
			pIdx[0] = (m_nMesh_H + 1) + nCntIndex;
			pIdx[1] = nCntIndex;

			pIdx += 2;

			if (nCntIndex == m_nMesh_H && nCntIndex_V <= m_nMesh_V)
			{
				pIdx[0] = nCntIndex;
				pIdx[1] = (m_nMesh_H + 1) + nCntIndex + 1;

				pIdx += 2;
			}
		}
	}
	//頂点バッファをアンロック
	m_pIdxBuff->Unlock();
}


