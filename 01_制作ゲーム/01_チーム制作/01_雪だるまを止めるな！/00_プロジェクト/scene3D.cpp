//=============================================================================
//
// オブジェクト3D処理 [scene3D.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "scene3D.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME01 "data\\TEXTURE\\grass.jpg"			//テクスチャ名
//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// ポリゴンのコンストラクタ
//=============================================================================
CScene3D::CScene3D(int nPrioriry,OBJTYPE objtype) : CScene(nPrioriry,objtype)
{
	m_pTexture = NULL;							// テクスチャへのポインタ
	m_pVtxBuff = NULL;							// 頂点バッファへのポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_fSizeX = 50.0f;							// X座標の大きさ
	m_fSizeZ = 50.0f;							// Z座標の大きさ
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
//=============================================================================
// ポリゴンのコンストラクタ
//=============================================================================
CScene3D::~CScene3D()
{

}

//=============================================================================
// ポリゴンの生成
//=============================================================================
CScene3D *CScene3D::Create(void)
{
	CScene3D *pScene3D = NULL;		//オブジェクトのポインタ

	if (pScene3D == NULL)
	{
		pScene3D = new CScene3D;	//動的確保
		if (pScene3D != NULL)
		{
			pScene3D->Init();		//初期化処理
		}
	}
	return pScene3D;
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScene3D::Init(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,	// 確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fSizeX, m_pos.y, m_pos.z + m_fSizeZ);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fSizeX, m_pos.y, m_pos.z + m_fSizeZ);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x -m_fSizeX, m_pos.y, m_pos.z - m_fSizeZ);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fSizeX, m_pos.y, m_pos.z - m_fSizeZ);

	//法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 頂点カラー
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScene3D::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScene3D::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CScene3D::Draw(void)
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

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
//=============================================================================
// 位置の移動処理
//=============================================================================
D3DXVECTOR3 CScene3D::GetPos(void)
{
	return m_pos;
}
//=============================================================================
// Xサイズの取得処理
//=============================================================================
float CScene3D::GetSizeX(void)
{
	return m_fSizeX;
}
//=============================================================================
// Zサイズの取得処理
//=============================================================================
float CScene3D::GetSizeZ(void)
{
	return m_fSizeZ;
}
//=============================================================================
// 法線の取得処理
//=============================================================================
D3DXVECTOR3 CScene3D::GetNor(void)
{
	return m_dData;
}

//=============================================================================
// 高さの取得
//=============================================================================
float CScene3D::GetHeight(D3DXVECTOR3 pos,bool bR)
{
	D3DXVECTOR3 vecA, vecC;
	VERTEX_3D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (bR == true)
	{
		pos.y = ((pVtx[0].nor.x * (pos.x - pVtx[0].pos.x)+ pVtx[0].nor.z * (pos.z - pVtx[0].pos.z)) / -pVtx[0].nor.y) + pVtx[0].pos.y;
	}
	else if (bR == false)
	{
		pos.y = ((pVtx[3].nor.x * (pos.x - pVtx[3].pos.x) + pVtx[3].nor.z * (pos.z - pVtx[3].pos.z)) / -pVtx[3].nor.y) + pVtx[3].pos.y;
	}
	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
	return pos.y;
}
//=============================================================================
// 位置の設置処理
//=============================================================================
void CScene3D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// テクスチャの設置処理
//=============================================================================
void CScene3D::BindTexture(LPDIRECT3DTEXTURE9 texture)
{
	m_pTexture = texture;
}
//=============================================================================
// Xサイズの設置処理
//=============================================================================
void CScene3D::SetSizeX(float sizeX)
{
	m_fSizeX = sizeX;
}

//=============================================================================
// Zサイズの設置処理
//=============================================================================
void CScene3D::SetSizeZ(float sizeZ)
{
	m_fSizeZ = sizeZ;
}
