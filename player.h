//=============================================================================
//
// モデル処理 [player.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

#define	PLAYER_MAX			(1)


typedef struct
{

	D3DXVECTOR3			posPlayer;				// モデルの位置
	D3DXVECTOR3			rotPlayer;				// モデルの向き(回転)
	D3DXVECTOR3			sclPlayer;				// モデルの大きさ(スケール)
	D3DXVECTOR3			movePlayer;				// 移動量
	D3DXVECTOR3			rotDestPlayer;			// 目的の向き
	//D3DXVECTOR3			osPlayer;					// 現在の位置

	float				jumpspeed = 100.0f;
	bool				air;
	bool				hit;
}PLAYER;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);


void DrawPlayer_shaft_r(void);
void DrawPlayer_shaft_l(void);
void DrawPlayer_wheel_r(void);
void DrawPlayer_wheel_l(void);
void DrawPlayer_enjin_r(void);
void DrawPlayer_enjin_l(void);


PLAYER	*GetPlayer(int pno);
//D3DXVECTOR3	*GetPlayer(void);

D3DXMATRIX *GetPlayerMatrix(void);

D3DXVECTOR3 GetPositionPlayer(void);

//D3DXMATRIX *D3DXMatrixRotationQuaternion(D3DXMATRIX *pOut, CONST D3DXQUATERNION *pQ);

//int hitCheckPlayer(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);


#endif
