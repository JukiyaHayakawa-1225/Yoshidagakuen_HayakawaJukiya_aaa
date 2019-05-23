//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : Jukiya Hayakawa
// Editor : Yuto Kodama
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "scene.h"
#include "player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERA_MOVE (1.0f);

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// カメラコンストラクタ
//=============================================================================
CCamera::CCamera()
{
	m_pTargetObj = NULL;
}

//=============================================================================
// カメラデストラクタ
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// カメラの生成
//=============================================================================
CCamera *CCamera::Create()
{
	CCamera *pCamera = NULL;

	if (pCamera == NULL)
	{
		pCamera = new CCamera;
		pCamera->Init();
	}
	return pCamera;
}

//=============================================================================
// カメラの初期化処理
//=============================================================================
HRESULT CCamera::Init(void)
{
	m_posV = D3DXVECTOR3(0.0f, 100.0f, 150.0f);						//	視点
	m_posR = D3DXVECTOR3(0.0f, 40.0f, 0.0f);						//	注視点
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);							//	上方向ベクトル
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//	目的の視点
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//	目的の注視点
	m_rot.y = atan2f(m_posR.x - m_posV.x, m_posR.z - m_posV.z);		//	向き
																	//	m_rot.y = 0.0f;

	m_rotDest = m_rot;													//目的の向き
	m_rotDiff = 0.0f;
	m_fLength = sqrtf((m_posV.x - m_posR.x) * (m_posV.x - m_posR.x) + (m_posV.z - m_posR.z) * (m_posV.z - m_posR.z));	//距離
	return S_OK;
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
// カメラの更新処理
//=============================================================================
void CCamera::Update(void)
{
	CManager::MODE mode = CManager::GetMode();
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	switch (mode)
	{
	case CManager::MODE_TITLE:
		m_posV.z -= 1.0f;
		m_posR.z -= 1.0f;
		break;
	case CManager::MODE_GAME:
		//追従
		TargetTracking();

		if (pInputKeyboard->GetPress(DIK_Z) == true)
		{// キーボードの[Z]キーが押された
			m_rot.y += 0.025f;
			if (m_rot.y >= D3DX_PI)
			{
				m_rot.y = -D3DX_PI;
			}
			//m_posV.x = m_posR.x - sinf(m_rot.y) * m_fLength;
			//m_posV.z = m_posR.z - cosf(m_rot.y) *m_fLength;
		}
		if (pInputKeyboard->GetPress(DIK_C) == true)
		{// キーボードの[C]キーが押された
			m_rot.y -= 0.025f;
			if (m_rot.y <= -D3DX_PI)
			{
				m_rot.y = D3DX_PI;
			}
			//m_posV.x = m_posR.x - sinf(m_rot.y) * m_fLength;
			//m_posV.z = m_posR.z - cosf(m_rot.y) * m_fLength;
		}
		break;
	}

	//#ifdef _DEBUG
	//	//注視点の表示
	//	CDebugProc::Print(1, "視点の移動    : x[%.1f],y[%.1f],z[%.1f]\n", m_posV.x, m_posV.y, m_posV.z);
	//	//視点の表示
	//	CDebugProc::Print(1, "注視点の移動  : x[%.1f],y[%.1f],z[%.1f]\n", m_posR.x, m_posR.y, m_posR.z);
	//#endif // _DEBUG
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void CCamera::SetCamera(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),						//画角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	//画面比率
		10.0f,										//手前
		1000.0f);									//奥行き	変更中元1000

													// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=============================================================================
// カメラのターゲット設定
//=============================================================================
void CCamera::SetTarget(CScene* pTarget)
{
	m_pTargetObj = pTarget;
}

//=============================================================================
// カメラの向き取得
//=============================================================================
D3DXVECTOR3 CCamera::GetRot(void)
{
	return m_rot;
}

//=============================================================================
// カメラの追従処理
//=============================================================================
void CCamera::TargetTracking(void)
{
	D3DXVECTOR3 TargetPos;
	if (m_pTargetObj != NULL)
	{
		switch (m_pTargetObj->GetObjType())
		{
		case CScene::OBJTYPE_PLAYER:
			CPlayer* pPlayer = NULL;
			pPlayer = (CPlayer*)m_pTargetObj;
			if (pPlayer != NULL)
			{
				TargetPos = pPlayer->GetPosition();
			}

			break;
		}

		//移動に追従
		m_posRDest = TargetPos + D3DXVECTOR3(0.0f, 30.0f, 0.0f);	//見づらかったらここに値を足す
		m_posR.x += (m_posRDest.x - m_posR.x) * 0.6f;
		m_posR.y += (m_posRDest.y - m_posR.y) * 0.6f;
		m_posR.z += (m_posRDest.z - m_posR.z) * 0.6f;

		m_posVDest.x = m_posRDest.x + sinf(m_rot.y) * m_fLength;
		m_posVDest.y = m_posV.y;
		m_posVDest.z = m_posRDest.z + cosf(m_rot.y) * m_fLength;

		m_posV += (m_posVDest - m_posV) * 0.1f;

		CDebugProc::Print(1, "m_posV = (%f,%f,%f)", m_posV.x, m_posV.y, m_posV.z);
	}
}