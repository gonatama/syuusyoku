#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

typedef struct
{

	D3DXVECTOR3			g_posEnemy;				// モデルの位置
	D3DXVECTOR3			g_rotEnemy;				// モデルの向き(回転)
	D3DXVECTOR3			g_sclEnemy;				// モデルの大きさ(スケール)
	D3DXVECTOR3			g_moveEnemy;				// 移動量
	D3DXVECTOR3			g_rotDestEnemy;				// 目的の向き


}ENEMY;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);




ENEMY	*GetEnemy(int pno);


#endif
