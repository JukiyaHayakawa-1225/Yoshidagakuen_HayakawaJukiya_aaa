//=============================================================================
//
// カメラ処理 [camera.h]
// Author :
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// カメラのクラス構造体
//*****************************************************************************
class CCamera
{
public:
	CCamera();	//コンストラクタ
	~CCamera();	//デストラクタ
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void SetCamera(void);	//描画処理
	void OverView(void);	//角度調整
	D3DXMATRIX GetmtxView(void);	//ビューマトリックスの取得
	D3DXVECTOR3 GetRot(void);		//カメラの角度情報の取得
private:
	static CManager *m_pManager;//マネージャ
	D3DXVECTOR3 m_posVDest;
	D3DXVECTOR3 m_posRDest;
	D3DXVECTOR3 m_posV;		//視点
	D3DXVECTOR3 m_posR;		//注視点
	D3DXVECTOR3 m_vecU;		//上方向ベクトル
	D3DXVECTOR3 m_rot;		//角度情報
	D3DXMATRIX m_mtxProjection;//プロジェクションマトリックス
	D3DXMATRIX m_mtxView;		//ビューマトリックス
	float m_eyepoint;		//視点
	float m_fAngle;		//角度情報
	float m_fLength;	//長さ
	float m_fHeight;	//高さ
};

#endif
