//=============================================================================
//
// 地面処理 [field.h]
// Author : 
//
//=============================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include "main.h"


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitField(void);
void UninitField(void);
void UpdateField(void);
void DrawField(void);

int hitCheck(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);

int hitCheckBill(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);

#endif
