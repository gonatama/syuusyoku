
/*****************************************************************
**physics.cpp
*****************************************************************/

#include "main.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "physics.h"


/***********************************
*�O���[�o���ϐ�
************************************/

float vel = 0;
//float evel = 0;
//float evel02 = 0;

bool g;

/*********************************
*�d��(player)
**********************************/
void Gravity(PLAYER *player)
{

	if (GetKeyboardTrigger(DIK_G))
	{
		if (g == false)
		{
			g = true;
		}
		else if (g == true)
		{
			g = false;
			vel = 0;
		}
	}
		if (g == true)
		{
			vel -= GRAVITY * UNIT_TIME;
			player->posPlayer.y += 0.5f * vel * UNIT_TIME;
		}
	
}
/*********************************
*�W�����v(player)
*********************************/
void Jump(PLAYER *player)
{
	player->air = true;
	vel += player->jumpspeed;

}


//void JumpAt(PLAYER *player)
//{
//	vel += PLAYER_SPEED * 8;
//}


/********************************
*����
*********************************/
bool Repelling(PLAYER *player)
{

	if ((vel < 200.0f) && (player->posPlayer.y <= 5.0f))
	{

		vel = vel * REPELLING;
		return true;
	}
	else
	{
		return false;
	}
}


///*********************************
//*�d��(enemy)
//**********************************/
//void Gravity_enemy(ENEMY *enemy)
//{
//	enemy->vel += GRAVITY * UNIT_TIME;
//	enemy->epos.y += 0.5f * enemy->vel * UNIT_TIME;
//
//}
//
///*********************************
//*�W�����v(enemy)
//*********************************/
//void Jump_enemy(ENEMY *enemy)
//{
//	enemy->air = true;
//	enemy->vel -= enemy->jumpspeed;
//}
//
///*********************************
//*�d��(enemy)
//**********************************/
//void Gravity_enemy02(ENEMY02 *enemy02)
//{
//
//	evel02 += GRAVITY * UNIT_TIME;
//	enemy02->epos.y += 0.5f * evel02 * UNIT_TIME;
//
//}
//
///*********************************
//*�W�����v(enemy)
//*********************************/
//void Jump_enemy02(ENEMY02 *enemy02)
//{
//	enemy02->air = true;
//	evel02 -= enemy02->jumpspeed;
//}
//
//
/************************************
*SetVel
* Vel��0�ɖ߂��B
*************************************/
void SetVel(void)
{
	vel = 0;
}

void SetG(void)
{
	g = false;
}
//void SetVel_Enemy(ENEMY *enemy)
//{
//	enemy->vel = 0;
//}
//
//void SetVel_Enemy02(void)
//{
//	evel02 = 0;
//}