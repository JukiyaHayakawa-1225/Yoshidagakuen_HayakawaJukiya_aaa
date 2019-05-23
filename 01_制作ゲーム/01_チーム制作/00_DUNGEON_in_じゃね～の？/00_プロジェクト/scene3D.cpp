//=============================================================================
//
// シーン2D処理 [scene3D.cpp]
// Author : Kodama Yuto
//
//=============================================================================
#include "scene3D.h"
#include "Manager.h"
#include "player.h"
#include "renderer.h"
//==================================================================
// 静的メンバ変数
//==================================================================
static bool g_DeleteFlag = true;		//ブロック消すフラグ(テスト)

//==================================================================
// コンストラクタ&デストラクタ
//==================================================================
CScene3D::CScene3D(int priority, OBJTYPE type) : CScene(priority, type)
{
	m_pTexture = NULL;
}
CScene3D::~CScene3D()
{

}

//==================================================================
// テクスチャの適用
//==================================================================
void CScene3D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//==================================================================
// 生成
//==================================================================
CScene3D* CScene3D::Create(void)
{
	CScene3D* pScene3D = NULL;

	pScene3D = new CScene3D(4);

	if (pScene3D != NULL)
	{
		pScene3D->Init();
	}

	return pScene3D;
}


//==================================================================
// ポリゴンの初期化処理
//==================================================================
HRESULT CScene3D::Init()
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//変数宣言
	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	m_fSize = 200.0f;
	pVtx[0].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * m_fSize, 0.0f, cosf(D3DX_PI * 0.75f) *m_fSize);
	pVtx[1].pos = D3DXVECTOR3(sinf(D3DX_PI * -0.75f) *m_fSize, 0.0f, cosf(D3DX_PI * -0.75f) *m_fSize);
	pVtx[2].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.25f) *m_fSize, 0.0f, cosf(D3DX_PI * 0.25f) *m_fSize);
	pVtx[3].pos = D3DXVECTOR3(sinf(D3DX_PI * -0.25f) *m_fSize, 0.0f, cosf(D3DX_PI * -0.25f) *m_fSize);

	D3DXVECTOR3 Vec[4];

	//nor[0]
	Vec[0] = pVtx[1].pos - pVtx[0].pos;
	Vec[1] = pVtx[2].pos - pVtx[0].pos;

	D3DXVec3Cross(&pVtx[0].nor, &Vec[0], &Vec[1]);
	//nor[1]
	Vec[0] = pVtx[2].pos - pVtx[1].pos;
	Vec[1] = pVtx[0].pos - pVtx[1].pos;
	D3DXVec3Cross(&pVtx[1].nor, &Vec[0], &Vec[1]);
	//nor[2]
	Vec[0] = pVtx[1].pos - pVtx[2].pos;
	Vec[1] = pVtx[3].pos - pVtx[2].pos;
	D3DXVec3Cross(&pVtx[2].nor, &Vec[0], &Vec[1]);

	//nor[3]
	Vec[0] = pVtx[2].pos - pVtx[3].pos;
	Vec[1] = pVtx[1].pos - pVtx[3].pos;
	D3DXVec3Cross(&pVtx[3].nor, &Vec[0], &Vec[1]);

	//正規化
	D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);
	D3DXVec3Normalize(&pVtx[1].nor, &pVtx[1].nor);
	D3DXVec3Normalize(&pVtx[2].nor, &pVtx[2].nor);
	D3DXVec3Normalize(&pVtx[3].nor, &pVtx[3].nor);

	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==================================================================
// ポリゴンの終了処理
//==================================================================
void CScene3D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture = NULL;
	}

	Release();
}

//==================================================================
// ポリゴンの更新処理
//==================================================================
void CScene3D::Update(void)
{

}
//==================================================================
// ポリゴンの描画処理
//==================================================================
void CScene3D::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y,
		m_rot.x,
		m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x,
		m_pos.y,
		m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));//頂点バッファをデバイスのデータストリームに設定

	pDevice->SetFVF(FVF_VERTEX_3D);//頂点フォーマットの設定

								   // テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);//テクスチャの設定

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//プリミティブの種類					//プリミティブ == ポリゴン
		0,					   //開始する頂点のインデックス(基本０)
		2);		   	       //プリミティブの数
}

//==================================================================
// 各種設定処理
//==================================================================
void CScene3D::SetPosition(D3DXVECTOR3 pos)
{//座標のみ設定
	m_pos = pos;
}

void CScene3D::SetRotation(D3DXVECTOR3 rot)
{//角度のみ設定
	m_rot = rot;
}

void CScene3D::SetColor(D3DXCOLOR col)
{
	m_Color = col;

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

						//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = m_Color;
	pVtx[1].col = m_Color;
	pVtx[2].col = m_Color;
	pVtx[3].col = m_Color;

	m_pVtxBuff->Unlock();
}
void CScene3D::SetSize(float fSize)
{
	m_fSize = fSize;

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

						//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * m_fSize, 0.0f, cosf(D3DX_PI * 0.75f) *m_fSize);
	pVtx[1].pos = D3DXVECTOR3(sinf(D3DX_PI * -0.75f) *m_fSize, 0.0f, cosf(D3DX_PI * -0.75f) *m_fSize);
	pVtx[2].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.25f) *m_fSize, 0.0f, cosf(D3DX_PI * 0.25f) *m_fSize);
	pVtx[3].pos = D3DXVECTOR3(sinf(D3DX_PI * -0.25f) *m_fSize, 0.0f, cosf(D3DX_PI * -0.25f) *m_fSize);

	m_pVtxBuff->Unlock();

}
