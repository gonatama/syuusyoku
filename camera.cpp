//=============================================================================
//
// カメラ
//
//=============================================================================
#define	_CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "camera.h"
#include "input.h"
#include "player.h"
#include "billboard.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
CAMERA_WORK		cwk[1];

int				g_debug = 1;

//=============================================================================
// カメラの初期化
//=============================================================================
void InitCamera(void)
{
	//cwk->posCameraP = D3DXVECTOR3(0.0f, 500.0f, -250.0f);
	//cwk->posCameraR = D3DXVECTOR3(0.0f, 0.0f, 250.0f);
	cwk->posCameraP = D3DXVECTOR3(0.0f, 0.0f, 200.0f);
	cwk->posCameraR = D3DXVECTOR3(0.0f, 0.0f, 400.0f);
	cwk->posCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// カメラ視点の回転角度初期化
	cwk->rotCameraP = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 注視点と視点の距離
	cwk->cam_len = 150.0f;
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// カメラ視点移動処理
//=============================================================================

// カメラの位置 = カメラの注視点 + 注視点からの相対的位置
// カメラの位置 = カメラの注視点 + (注視点からの角度 * 視点までの距離)

void calc_posCameraP(D3DXVECTOR3 pno)
{
	D3DXVECTOR3 vec(0.0f, 0.25f, -1.0f);		// 正規化された注視点からの向き
	D3DXMATRIX	mtx;

	cwk->posCameraR = pno;

	// カメラの相対的な回転角度を行列に変換する
	D3DXMatrixRotationYawPitchRoll(&mtx, cwk->rotCameraP.y, cwk->rotCameraP.x, cwk->rotCameraP.z);

	// 正規化された点に回転行列を掛ける
	D3DXVec3TransformCoord(&vec, &vec, &mtx);

	// カメラの位置 = カメラの注視点 + (注視点からの角度 * 視点までの距離)
	cwk->posCameraP = cwk->posCameraR + (vec * cwk->cam_len);


}


//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
	PLAYER *player = GetPlayer(0);

	if (GetKeyboardPress(DIK_Q))
	{// Y軸左回転
		cwk->rotCameraP.y -= VALUE_ROTATE;
		if (cwk->rotCameraP.y > D3DX_PI)
		{
			cwk->rotCameraP.y -= D3DX_PI * 2.0f;
		}
		if (cwk->rotCameraP.y < -D3DX_PI)
		{
			cwk->rotCameraP.y += D3DX_PI * 2.0f;
		}
	}

	if (GetKeyboardPress(DIK_E))
	{// Y軸右回転
		cwk->rotCameraP.y += VALUE_ROTATE;
		if (cwk->rotCameraP.y > D3DX_PI)
		{
			cwk->rotCameraP.y -= D3DX_PI * 2.0f;
		}
		if (cwk->rotCameraP.y < -D3DX_PI)
		{
			cwk->rotCameraP.y += D3DX_PI * 2.0f;
		}
	}

	//if(GetKeyboardPress(DIK_C))
	//{// X軸左回転
	//	cwk->rotCameraP.x -= VALUE_ROTATE;
	//	if(cwk->rotCameraP.x > D3DX_PI)
	//	{
	//		cwk->rotCameraP.x -= D3DX_PI * 2.0f;
	//	}
	//	if(cwk->rotCameraP.x < -D3DX_PI)
	//	{
	//		cwk->rotCameraP.x += D3DX_PI * 2.0f;
	//	}
	//}

	//if(GetKeyboardPress(DIK_V))
	//{// x軸右回転
	//	cwk->rotCameraP.x += VALUE_ROTATE;
	//	if(cwk->rotCameraP.x > D3DX_PI)
	//	{
	//		cwk->rotCameraP.x -= D3DX_PI * 2.0f;
	//	}
	//	if(cwk->rotCameraP.x < -D3DX_PI)
	//	{
	//		cwk->rotCameraP.x += D3DX_PI * 2.0f;
	//	}
	//}

	if (GetKeyboardPress(DIK_Z))
	{
		// 左移動
		cwk->cam_len -= VALUE_MOVE_CAMERA;
	}
	else if (GetKeyboardPress(DIK_C))
	{
		// 右移動
		cwk->cam_len += VALUE_MOVE_CAMERA;
	}

	calc_posCameraP(player->posPlayer);
	
// デバッグ表示のＯＮ/ＯＦＦ
	if (GetKeyboardTrigger(DIK_P))
	{
		g_debug = (g_debug + 1) % 2;
	}
}

//=============================================================================
// カメラの描画処理
//=============================================================================
void DrawCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&cwk->mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&cwk->mtxView,
		&cwk->posCameraP,		// カメラの視点
		&cwk->posCameraR,		// カメラの注視点
		&cwk->posCameraU);		// カメラの上方向

// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &cwk->mtxView);


	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&cwk->mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&cwk->mtxProjection,
		VIEW_ANGLE,										// 視野角
		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT),	// アスペクト比
		VIEW_NEAR_Z,									// ビュー平面のNearZ値
		VIEW_FAR_Z);									// ビュー平面のFarZ値

// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &cwk->mtxProjection);
}

//=============================================================================
// カメラワークのアドレスを取得
//=============================================================================

CAMERA_WORK *get_camerawk(void)
{
	return(cwk);
}


//=============================================================================
// ビルボードのデバッグ表示
//=============================================================================
int DrawDebugCamera(LPD3DXFONT font, RECT rect)
{
	char str[256];

	if (g_debug != 1) return(rect.top);

	rect.top += 16;

	// 座標表示
	sprintf(str, "カメラ注視座標：%.4f %.4f %.4f", cwk->posCameraR.x, cwk->posCameraR.y, cwk->posCameraR.z);
	font->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	rect.top += 16;

	// 座標表示
	sprintf(str, "カメラ視点座標：%.4f %.4f %.4f", cwk->posCameraP.x, cwk->posCameraP.y, cwk->posCameraP.z);
	font->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	rect.top += 16;

	// 座標表示
	sprintf(str, "カメラ視点回転：%.4f %.4f %.4f ZXCV", cwk->rotCameraP.x, cwk->rotCameraP.y, cwk->rotCameraP.z);
	font->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	rect.top += 16;

	// 距離表示
	sprintf(str, "距離：%.4f BN", cwk->cam_len);
	font->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	rect.top += 16;

	return(rect.top);

}



//=============================================================================
// カメラの向きの取得
//=============================================================================
D3DXVECTOR3 GetRotCamera(void)
{
	return cwk->posCameraR;
}


////=============================================================================
////
//// カメラ処理 [camera.cpp]
//// Author : 
////
////=============================================================================
//#include "camera.h"
//#include "player.h"
//#include "input.h"
//#include "debugproc.h"
//
////*****************************************************************************
//// マクロ定義
////*****************************************************************************
//#define	VIEW_ANGLE			(D3DXToRadian(45.0f))	// 視野角
//#define	VIEW_ASPECT			((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比
//#define	VIEW_NEAR_Z			(10.0f)					// ビュー平面のNearZ値
//#define	VIEW_FAR_Z			(1000.0f)				// ビュー平面のFarZ値
//#define	VALUE_MOVE_CAMERA	(2.0f)					// カメラの移動量
//#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)		// カメラの回転量
//
//#define	INTERVAL_CAMERA_R	(12.5f)					// モデルの視線の先までの距離
//#define	RATE_CHASE_CAMERA_P	(0.35f)					// カメラの視点への補正係数
//#define	RATE_CHASE_CAMERA_R	(0.20f)					// カメラの注視点への補正係数
//
//#define	CHASE_HEIGHT_P		(100.0f)				// 追跡時の視点の高さ
//#define	CHASE_HEIGHT_R		(10.0f)					// 追跡時の注視点の高さ
//
////*****************************************************************************
//// プロトタイプ宣言
////*****************************************************************************
//
//
////*****************************************************************************
//// グローバル変数
////*****************************************************************************
//D3DXVECTOR3		posCameraP;				// カメラの視点
//D3DXVECTOR3		posCameraR;				// カメラの注視点
//D3DXVECTOR3		posCameraU;				// カメラの上方向
//D3DXVECTOR3		posCameraPDest;			// カメラの視点の目的位置
//D3DXVECTOR3		posCameraRDest;			// カメラの注視点の目的位置
//D3DXVECTOR3		rotCamera;				// カメラの回転
//float			LengthIntervalCamera;	// カメラの視点と注視点の距離
//D3DXMATRIX		mtxView;					// ビューマトリックス
//D3DXMATRIX		mtxProjection;			// プロジェクションマトリックス
//
////=============================================================================
//// カメラの初期化
////=============================================================================
//HRESULT InitCamera(void)
//{
//	posCameraP = D3DXVECTOR3(0.0f, 100.0f, -200.0f);
//	posCameraR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	posCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//	posCameraPDest = D3DXVECTOR3(0.0f, 100.0f, -200.0f);
//	posCameraRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//
//	float vx,vz;
//	vx = posCameraP.x - posCameraR.x;
//	vz = posCameraP.z - posCameraR.z;
//	LengthIntervalCamera = sqrtf(vx * vx + vz * vz);
//
//	return S_OK;
//}
//
////=============================================================================
//// カメラの終了処理
////=============================================================================
//void UninitCamera(void)
//{
//
//
//}
//
////=============================================================================
//// カメラの更新処理
////=============================================================================
//void UpdateCamera(void)
//{
//	if(GetKeyboardPress(DIK_A))
//	{
//		if(GetKeyboardPress(DIK_W))
//		{// 左前移動
//			posCameraP.x -= cosf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
//			posCameraP.z += sinf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
//		}
//		else if(GetKeyboardPress(DIK_S))
//		{// 左後移動
//			posCameraP.x -= cosf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
//			posCameraP.z += sinf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
//		}
//		else
//		{// 左移動
//			posCameraP.x -= cosf(rotCamera.y) * VALUE_MOVE_CAMERA;
//			posCameraP.z += sinf(rotCamera.y) * VALUE_MOVE_CAMERA;
//		}
//
//		posCameraR.x = posCameraP.x + sinf(rotCamera.y) * LengthIntervalCamera;
//		posCameraR.z = posCameraP.z + cosf(rotCamera.y) * LengthIntervalCamera;
//	}
//	else if(GetKeyboardPress(DIK_D))
//	{
//		if(GetKeyboardPress(DIK_W))
//		{// 右前移動
//			posCameraP.x += cosf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
//			posCameraP.z -= sinf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
//		}
//		else if(GetKeyboardPress(DIK_S))
//		{// 右後移動
//			posCameraP.x += cosf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
//			posCameraP.z -= sinf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
//		}
//		else
//		{// 右移動
//			posCameraP.x += cosf(rotCamera.y) * VALUE_MOVE_CAMERA;
//			posCameraP.z -= sinf(rotCamera.y) * VALUE_MOVE_CAMERA;
//		}
//
//		posCameraR.x = posCameraP.x + sinf(rotCamera.y) * LengthIntervalCamera;
//		posCameraR.z = posCameraP.z + cosf(rotCamera.y) * LengthIntervalCamera;
//	}
//	else if(GetKeyboardPress(DIK_W))
//	{// 前移動
//		posCameraP.x += sinf(rotCamera.y) * VALUE_MOVE_CAMERA;
//		posCameraP.z += cosf(rotCamera.y) * VALUE_MOVE_CAMERA;
//
//		posCameraR.x = posCameraP.x + sinf(rotCamera.y) * LengthIntervalCamera;
//		posCameraR.z = posCameraP.z + cosf(rotCamera.y) * LengthIntervalCamera;
//	}
//	else if(GetKeyboardPress(DIK_S))
//	{// 後移動
//		posCameraP.x -= sinf(rotCamera.y) * VALUE_MOVE_CAMERA;
//		posCameraP.z -= cosf(rotCamera.y) * VALUE_MOVE_CAMERA;
//
//		posCameraR.x = posCameraP.x + sinf(rotCamera.y) * LengthIntervalCamera;
//		posCameraR.z = posCameraP.z + cosf(rotCamera.y) * LengthIntervalCamera;
//	}
//
//	if(GetKeyboardPress(DIK_Z))
//	{// 視点旋回「左」
//		rotCamera.y += VALUE_ROTATE_CAMERA;
//		if(rotCamera.y > D3DX_PI)
//		{
//			rotCamera.y -= D3DX_PI * 2.0f;
//		}
//
//		posCameraP.x = posCameraR.x - sinf(rotCamera.y) * LengthIntervalCamera;
//		posCameraP.z = posCameraR.z - cosf(rotCamera.y) * LengthIntervalCamera;
//	}
//	if(GetKeyboardPress(DIK_C))
//	{// 視点旋回「右」
//		rotCamera.y -= VALUE_ROTATE_CAMERA;
//		if(rotCamera.y < -D3DX_PI)
//		{
//			rotCamera.y += D3DX_PI * 2.0f;
//		}
//
//		posCameraP.x = posCameraR.x - sinf(rotCamera.y) * LengthIntervalCamera;
//		posCameraP.z = posCameraR.z - cosf(rotCamera.y) * LengthIntervalCamera;
//	}
//	if(GetKeyboardPress(DIK_Y))
//	{// 視点移動「上」
//		posCameraP.y += VALUE_MOVE_CAMERA;
//	}
//	if(GetKeyboardPress(DIK_N))
//	{// 視点移動「下」
//		posCameraP.y -= VALUE_MOVE_CAMERA;
//	}
//
//	if(GetKeyboardPress(DIK_Q))
//	{// 注視点旋回「左」
//		rotCamera.y -= VALUE_ROTATE_CAMERA;
//		if(rotCamera.y < -D3DX_PI)
//		{
//			rotCamera.y += D3DX_PI*2.0f;
//		}
//
//		posCameraR.x = posCameraP.x + sinf(rotCamera.y) * LengthIntervalCamera;
//		posCameraR.z = posCameraP.z + cosf(rotCamera.y) * LengthIntervalCamera;
//	}
//	if(GetKeyboardPress(DIK_E))
//	{// 注視点旋回「右」
//		rotCamera.y += VALUE_ROTATE_CAMERA;
//		if(rotCamera.y > D3DX_PI)
//		{
//			rotCamera.y -= D3DX_PI*2.0f;
//		}
//
//		posCameraR.x = posCameraP.x + sinf(rotCamera.y) * LengthIntervalCamera;
//		posCameraR.z = posCameraP.z + cosf(rotCamera.y) * LengthIntervalCamera;
//	}
//	if(GetKeyboardPress(DIK_T))
//	{// 注視点移動「上」
//		posCameraR.y += VALUE_MOVE_CAMERA;
//	}
//	if(GetKeyboardPress(DIK_B))
//	{// 注視点移動「下」
//		posCameraR.y -= VALUE_MOVE_CAMERA;
//	}
//
//	PrintDebugProc("[カメラの視点  ：(%f : %f : %f)]\n", posCameraP.x, posCameraP.y, posCameraP.z);
//	PrintDebugProc("[カメラの注視点：(%f : %f : %f)]\n", posCameraR.x, posCameraR.y, posCameraR.z);
//	PrintDebugProc("[カメラの向き  ：(%f)]\n", rotCamera.y);
//	PrintDebugProc("\n");
//
//	PrintDebugProc("*** 視点操作 ***\n");
//	PrintDebugProc("前移動 : W\n");
//	PrintDebugProc("後移動 : S\n");
//	PrintDebugProc("左移動 : A\n");
//	PrintDebugProc("右移動 : D\n");
//	PrintDebugProc("上移動 : Y\n");
//	PrintDebugProc("下移動 : N\n");
//	PrintDebugProc("左旋回 : Z\n");
//	PrintDebugProc("右旋回 : C\n");
//	PrintDebugProc("\n");
//
//	PrintDebugProc("*** 注視点操作 ***\n");
//	PrintDebugProc("上移動 : T\n");
//	PrintDebugProc("下移動 : B\n");
//	PrintDebugProc("左旋回 : Q\n");
//	PrintDebugProc("右旋回 : E\n");
//	PrintDebugProc("\n");
//}
//
////=============================================================================
//// カメラの設定処理
////=============================================================================
//void SetCamera(void)
//{
//	LPDIRECT3DDEVICE9 Device = GetDevice();
//	PLAYER *player = GetPlayer(0);
//	// ビューマトリックスの初期化
//	D3DXMatrixIdentity(&cwk->mtxView);
//
//	// ビューマトリックスの作成
//	D3DXMatrixLookAtLH(&cwk->mtxView,
//						&posCameraP,		// カメラの視点
//						&player->posPlayer,		// カメラの注視点
//						&posCameraU);		// カメラの上方向
//	// ビューマトリックスの設定
//	Device->SetTransform(D3DTS_VIEW, &cwk->mtxView);
//
//
//	// プロジェクションマトリックスの初期化
//	D3DXMatrixIdentity(&mtxProjection);
//
//	// プロジェクションマトリックスの作成
//	D3DXMatrixPerspectiveFovLH(&mtxProjection,
//								VIEW_ANGLE,			// 視野角
//								VIEW_ASPECT,		// アスペクト比
//								VIEW_NEAR_Z,		// ビュー平面のNearZ値
//								VIEW_FAR_Z);		// ビュー平面のFarZ値
//
//	// プロジェクションマトリックスの設定
//	Device->SetTransform(D3DTS_PROJECTION, &mtxProjection);
//}

//=============================================================================
// ビューマトリックスの取得
//=============================================================================
D3DXMATRIX GetMtxView(void)
{
	return cwk->mtxView;
}

////=============================================================================
//// カメラワークのアドレスを取得
////=============================================================================
//
//CAMERA_WORK *get_camerawk(void)
//{
//	return(cwk);
//}
