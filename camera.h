//=============================================================================
//
// カメラ
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define	VIEW_ANGLE			(D3DXToRadian(45.0f))	// 視野角
#define	VIEW_NEAR_Z			(10.0f)					// ビュー平面のNearZ値
#define	VIEW_FAR_Z			(10000.0f)				// ビュー平面のFarZ値

#define	VALUE_MOVE_CAMERA			(5.0f/2)				// 移動量
#define VALUE_ROTATE		(D3DX_PI * 0.02f)		// 回転量


typedef struct {
	D3DXVECTOR3		posCameraP;				// カメラの視点
	D3DXVECTOR3		posCameraR;				// カメラの注視点
	D3DXVECTOR3		posCameraU;				// カメラの上方向
	D3DXMATRIX		mtxView;				// ビューマトリックス
	D3DXMATRIX		mtxProjection;			// プロジェクションマトリックス

	D3DXVECTOR3		rotCameraP;				// カメラの視点
	float			cam_len;				// 中点から視点までの距離
} CAMERA_WORK;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void DrawCamera(void);

void calc_posCameraP(D3DXVECTOR3 pno);
CAMERA_WORK *get_camerawk(void);
int DrawDebugCamera(LPD3DXFONT font, RECT rect);

D3DXMATRIX GetMtxView(void);

D3DXVECTOR3 GetRotCamera(void);

#endif
