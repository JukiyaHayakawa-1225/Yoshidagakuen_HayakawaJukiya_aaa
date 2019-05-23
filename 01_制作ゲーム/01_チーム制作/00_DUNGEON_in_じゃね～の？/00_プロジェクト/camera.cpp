//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#include "camera.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "player.h"
#include "game.h"
#include "inputx.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define CAMERA_MOVE (3.5f)
#define ROT_CAMERASPEED (0.04f)
#define ROT_SPEED (0.1f)

#define CAMERA_ANGLE (12.0f)
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CManager *CCamera::m_pManager = NULL;
//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0);

	m_fLength = 0.0f;
	m_fHeight = m_posV.y - m_posR.y;

	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0);
	m_pManager = NULL;
}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CCamera::~CCamera()
{
}

//=============================================================================
// 関数名：カメラの初期化処理
// 関数の概要：カメラの位置や、距離を設定する。
//=============================================================================
HRESULT CCamera::Init(void)
{
	//カメラの位置情報の初期化
	m_posV = D3DXVECTOR3(450.0f, 100.0f, -800.0f);
	m_posR = D3DXVECTOR3(450.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0);

	m_eyepoint = 0.0f;
	m_fLength =150.0f;		// 注視点からの距離
	m_fHeight = m_posV.y - m_posR.y;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0);	// 角度情報


	return S_OK;
}

//=============================================================================
// 関数名：カメラの終了
// 関数の概要：
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
// 関数名：カメラの更新
// 関数の概要：各種キーボード操作
//=============================================================================
void CCamera::Update(void)
{

#if 1
	CInputX::InputJoyState JoyState = CInputX::GetJoystate();
	bool bConnect = CInputX::GetbConnect();			//ジョイパッドの接続状態

													//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);


	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	if (mode == CManager::MODE_TITLE || mode == CManager::MODE_RESULT || mode == CManager::MODE_RANKING)
	{
		m_posV = D3DXVECTOR3(450.0f, 100.0f, -800.0f);
		m_posR = D3DXVECTOR3(450.0f, 0.0f, 0.0f);
		m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0);

	}
	if (mode == CManager::MODE_GAME)
	{

		//===============================ぴったり追従==================================//
		//プレイヤーの取得
		CPlayer *pPlayer;
		pPlayer = CGame::GetPlayer();
		D3DXVECTOR3 playerpos = pPlayer->GetPos();
		D3DXVECTOR3 playerrot = pPlayer->GetRot();
		D3DXVECTOR3 playermove = pPlayer->GetMove();


		m_posVDest.x = playerpos.x + sinf(m_rot.y) * -m_fLength;
		m_posVDest.y = (playerpos.y  + 60.0f) + m_eyepoint;
		m_posVDest.z = playerpos.z + cosf(m_rot.y) * -m_fLength;

		m_posRDest.x = playerpos.x - sinf(playerrot.y) * 1.0f;
		m_posRDest.y = (playerpos.y + 50.0f) ;
		m_posRDest.z = playerpos.z - cosf(playerrot.y) * 1.0f;

		m_posV.x += (m_posVDest.x - m_posV.x) * 0.4f;
		m_posV.y += (m_posVDest.y - m_posV.y) * 0.4f;
		m_posV.z += (m_posVDest.z - m_posV.z) * 0.4f;

		m_posR.x += (m_posRDest.x - m_posR.x) * 0.4f;
		m_posR.y +=	(m_posRDest.y - m_posR.y) * 0.4f;
		m_posR.z += (m_posRDest.z - m_posR.z) * 0.4f;

		if (pInputKeyboard->GetKeyboardPress(DIK_LEFT) == true || (state.Gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && bConnect == true))
		{
			m_rot.y -= ROT_CAMERASPEED;

			m_posV.x = m_posR.x - sinf(m_rot.y) * m_fLength;
			m_posV.z = m_posR.z - cosf(m_rot.y) * m_fLength;
		}
		if (pInputKeyboard->GetKeyboardPress(DIK_RIGHT) == true || (state.Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && bConnect == true))
		{
			m_rot.y += ROT_CAMERASPEED;

			m_posV.x = m_posR.x - sinf(m_rot.y) * m_fLength;
			m_posV.z = m_posR.z - cosf(m_rot.y) * m_fLength;
		}

		if (pInputKeyboard->GetKeyboardPress(DIK_DOWN) == true || (state.Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && bConnect == true))
		{
			m_eyepoint += 2.1f;
			if (m_eyepoint >= 70.0f)
			{
				m_eyepoint = 70.0f;
			}

		}
		if (pInputKeyboard->GetKeyboardPress(DIK_UP) == true || (state.Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && bConnect == true))
		{
			m_eyepoint -= 2.1f;


			if (m_eyepoint <= -32.0f)
			{
				m_eyepoint = -32.0f;
			}

		}


		if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB && bConnect == true))
		{//カメラをプレイヤーの回転に合わせる
			if (JoyState == CInputX::INPUT_JOYSTATE_NOTPUSH)
			{
				m_rot.y = playerrot.y + (-1.0f * D3DX_PI);
				m_posV = D3DXVECTOR3(0.0f, 100.0f + playerpos.y, -150.0f);
			}
			JoyState = CInputX::INPUT_JOYSTATE_PUSH;

		}
	//============================================================================//
	}

	//カメラの位置情報
	//CDebugProc::Print("PosV(X,Y,Z):%.1f, %.1f, %.1f\n", m_posV.x, m_posV.y, m_posV.z);
	//CDebugProc::Print("Rot(X,Y,Z):%.1f, %.1f, %.1f\n", m_rot.x, m_rot.y, m_rot.z);
	//CDebugProc::Print("PosR(X,Y,Z):%.1f, %.1f, %.1f\n", m_posR.x, m_posR.y, m_posR.z);
	//CDebugProc::Print("eyepoint: %.1f\n", m_eyepoint);



	OverView();
#endif

	if (JoyState == CInputX::INPUT_JOYSTATE_PUSH)
	{
		JoyState = CInputX::INPUT_JOYSTATE_NONE;
	}
	else if (JoyState == CInputX::INPUT_JOYSTATE_NONE)
	{
		JoyState = CInputX::INPUT_JOYSTATE_NOTPUSH;
	}


}

//=============================================================================
// 関数名：カメラの設定処理
// 関数の概要：カメラを設置する
//=============================================================================
void CCamera::SetCamera(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();


	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		4000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&m_mtxView,//ビューマトリックス
		&m_posV,//視点
		&m_posR,//注視点
		&m_vecU);//上方向ベクトル

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//=============================================================================
// 関数名：カメラの調整
// 関数の概要：カメラの角度が一周過ぎたときの調整
//=============================================================================
void CCamera::OverView()
{
	if (m_rot.y > D3DX_PI)
	{

		m_rot.y -= D3DX_PI * 2;
	}

	if (m_rot.y < -D3DX_PI)
	{

		m_rot.y += D3DX_PI * 2;
	}


	if (m_rot.z > D3DX_PI)
	{

		m_rot.z -= D3DX_PI * 2;
	}

	if (m_rot.z < -D3DX_PI)
	{

		m_rot.z += D3DX_PI * 2;
	}

	if (m_rot.x > D3DX_PI)
	{

		m_rot.x -= D3DX_PI * 2;
	}

	if (m_rot.x < -D3DX_PI)
	{

		m_rot.x += D3DX_PI * 2;
	}


}

//=============================================================================
// 関数名：カメラのマトリックスの取得
// 関数の概要：カメラのマトリックスを返す
//=============================================================================
D3DXMATRIX CCamera::GetmtxView(void)
{
	return m_mtxView;
}

D3DXVECTOR3 CCamera::GetRot(void)
{
	return m_rot;
}

