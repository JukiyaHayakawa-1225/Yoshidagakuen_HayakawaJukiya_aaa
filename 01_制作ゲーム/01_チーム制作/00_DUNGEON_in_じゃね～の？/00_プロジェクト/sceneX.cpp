//=============================================================================
//
// シーンX処理[3Dポリゴン] [sceneX.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene3D.h"
#include "manager.h"
#include "input.h"
#include "sceneX.h"
#include "scene2D.h"
#include "debugproc.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************


//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CSceneX::CSceneX(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{
}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CSceneX::~CSceneX()
{
}

//=============================================================================
// 関数名：Xファイルシーンの生成
// 関数の概要：Xファイルシーンを生成する
//=============================================================================
CSceneX *CSceneX::Create(D3DXVECTOR3 pos)
{
	CSceneX  *pSceneX;
	pSceneX = new CSceneX;//シーンクラスの生成
	if (pSceneX != NULL)
	{
		pSceneX->m_pos = pos;
		pSceneX->Init();

	}
	return pSceneX;
}


//=============================================================================
// 関数名：Xファイルシーンの初期化処理
// 関数の概要：Xファイル情報の設定
//=============================================================================
HRESULT CSceneX::Init(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATERIAL *pMat;								// マテリアルデータへのポインタ

	// Xファイルの読み込み
	D3DXLoadMeshFromX(m_pAddressModel,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuff,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuff->GetBufferPointer();

	//m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumMat];		// マテリアル分容量を増やす

	//for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	//{// マテリアルの数分ループ
	//	if (pMat[nCntMat].pTextureFilename != NULL)
	//	{// テクスチャのあるとき
	//		D3DXCreateTextureFromFile(pDevice,
	//			pMat[nCntMat].pTextureFilename,					// 読み込むテクスチャ名
	//			&m_pTexture[nCntMat]);
	//	}
	//	else
	//	{// テクスチャがないとき
	//		m_pTexture[nCntMat] = NULL;							// NULLに設定する
	//	}
	//}

	// 変数宣言
	int nNumVtx;		// 頂点数
	DWORD sizeFVF;		// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;		// 頂点バッファへのポインタ

	// 最小値と最大値を初期化
	m_VtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);		// 最小値を初期化
	m_VtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);	// 最大値を初期化

																												// 頂点数を取得
	nNumVtx = m_pMesh->GetNumVertices();

	// 頂点フォーマットを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// 頂点バッファをロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// プレイヤーの最小値と最大値を比較
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{// 頂点数分ループ
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;								// 頂点座標の代入

																				// 最小値と比較
		if (m_VtxMin.x > vtx.x)
		{// 今持っている最小値のxよりも小さい値だったら
			m_VtxMin.x = vtx.x;						// 最小値のxを代入
		}
		if (m_VtxMin.y > vtx.y)
		{// 今持っている最小値のyよりも小さい値だったら
			m_VtxMin.y = vtx.y;						// 最小値のyを代入
		}
		if (m_VtxMin.z > vtx.z)
		{// 今持っている最小値のyよりも小さい値だったら
			m_VtxMin.z = vtx.z;						// 最小値のzを代入
		}

		// 最大値と比較
		if (m_VtxMax.x < vtx.x)
		{// 今持っている最大値のxよりも大きい値だったら
			m_VtxMax.x = vtx.x;						// 最大値のxを代入
		}
		if (m_VtxMax.y < vtx.y)
		{// 今持っている最大値のxよりも大きい値だったら
			m_VtxMax.y = vtx.y;						// 最大値のyを代入
		}
		if (m_VtxMax.z < vtx.z)
		{// 今持っている最大値のxよりも大きい値だったら
			m_VtxMax.z = vtx.z;						// 最大値のzを代入
		}

		pVtxBuff += sizeFVF;					// ポインタをサイズ分進める
	}

	// 頂点バッファをアンロック
	m_pMesh->UnlockVertexBuffer();



	return S_OK;
}

//=============================================================================
// 関数名：Xファイルシーンの終了処理
// 関数の概要：メッシュ、マテリアル、自身の破棄
//=============================================================================
void CSceneX::Uninit(void)
{

	// マテリアルの開放
	if (m_pBuff != NULL)
	{
		m_pBuff->Release();
		m_pBuff = NULL;
	}

	// メッシュの開放
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	if (m_nNumMat != NULL)
	{
		m_nNumMat = NULL;
	}

	//オブジェクト(自分自身)の破棄
	Release();
}

//=============================================================================
// 関数名：Xファイルシーンの更新処理
// 関数の概要：--
//=============================================================================
void CSceneX::Update(void)
{

}

//=============================================================================
// 関数名：Xファイルシーンの描画処理
// 関数の概要：--
//=============================================================================
void CSceneX::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);



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

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuff->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		//テクニックのせってい
		pDevice->SetTexture(0, m_pTexture);
		// モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// 関数名：Xファイル名の割り当て処理
// 関数の概要：ファイル名を代入する
//=============================================================================
void CSceneX::BindFileName(char *cFileName)
{
	m_pAddressModel = cFileName;
}

//=============================================================================
// 関数名：頂点バッファの割り当て
// 関数の概要：頂点バッファを代入する
//=============================================================================
void CSceneX::BindBuff(LPD3DXBUFFER Buff)
{
	m_pBuff = Buff;
}

//=============================================================================
// 関数名：メッシュ情報の割り当て
// 関数の概要：メッシュ情報を代入する
//=============================================================================
void CSceneX::BindMesh(LPD3DXMESH Mesh)
{
	m_pMesh = Mesh;
}

//=============================================================================
// 関数名：マテリアル情報の数割り当て
// 関数の概要：マテリアル情報を代入する
//=============================================================================
void CSceneX::BindNumMat(DWORD NumMat)
{
	m_nNumMat = NumMat;
}

//=============================================================================
// 関数名：位置情報の代入
// 関数の概要：位置情報を代入する
//=============================================================================
void CSceneX::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}


//=============================================================================
// 関数名：角度情報の代入
// 関数の概要：角度情報を代入する
//=============================================================================
void CSceneX::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}


//=============================================================================
// 関数名：最大頂点座標の取得
// 関数の概要：最大頂点座標を返す
//=============================================================================
D3DXVECTOR3 CSceneX::GetVtxMax(void)
{
	return m_VtxMax;
}

//=============================================================================
// 関数名：最小頂点座標の取得
// 関数の概要：最小頂点座標を返す
//=============================================================================
D3DXVECTOR3 CSceneX::GetVtxMin(void)
{
	return m_VtxMin;
}

//=============================================================================
// 関数名：テクスチャの割り当て
// 関数の概要：テクスチャを割り当てる
//=============================================================================
void CSceneX::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;
}
