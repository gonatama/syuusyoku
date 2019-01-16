#ifndef PHYSICS_H_
#define PHYSICS_H_


// �}�N����`


#define GRAVITY			(32.0f)				//�d�͉����x
#define UNIT_TIME		(1/20.0f)			//�P�ʎ���(1/20[sec]
#define REPELLING		(-0.8f)				//�����W��
#define	RATE_REGIST		(0.075f)			// ��R�W��

//*****************************************************************************
// �v���g�^�C�v�錾
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
*�O���[�o���ϐ�
***********************************************************************************/

void SetVel(void);
void SetG(void);

#endif




#pragma once