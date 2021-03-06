#ifndef PHYSICS_H_
#define PHYSICS_H_


// マクロ定義


#define GRAVITY			(32.0f)				//重力加速度
#define UNIT_TIME		(1/20.0f)			//単位時間(1/20[sec]
#define REPELLING		(-0.8f)				//反発係数
#define	RATE_REGIST		(0.075f)			// 抵抗係数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************



/*******************************************************************************
*player
********************************************************************************/
void Jump(PLAYER *player);
void Gravity(PLAYER *player);
//void JumpAt(PLAYER *player);
bool Repelling(PLAYER *player);
/********************************************************************************
*enemy
*********************************************************************************/
//
//void Jump_enemy(ENEMY *enemy);
//void Gravity_enemy(ENEMY *enemy);




/**********************************************************************************
*グローバル変数
***********************************************************************************/

void SetVel(void);
void SetG(void);

#endif




#pragma once
