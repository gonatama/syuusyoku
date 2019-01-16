//=============================================================================
//
// �J����
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define	VIEW_ANGLE			(D3DXToRadian(45.0f))	// ����p
#define	VIEW_NEAR_Z			(10.0f)					// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z			(10000.0f)				// �r���[���ʂ�FarZ�l

#define	VALUE_MOVE_CAMERA			(5.0f/2)				// �ړ���
#define VALUE_ROTATE		(D3DX_PI * 0.02f)		// ��]��


typedef struct {
	D3DXVECTOR3		posCameraP;				// �J�����̎��_
	D3DXVECTOR3		posCameraR;				// �J�����̒����_
	D3DXVECTOR3		posCameraU;				// �J�����̏����
	D3DXMATRIX		mtxView;				// �r���[�}�g���b�N�X
	D3DXMATRIX		mtxProjection;			// �v���W�F�N�V�����}�g���b�N�X

	D3DXVECTOR3		rotCameraP;				// �J�����̎��_
	float			cam_len;				// ���_���王�_�܂ł̋���
} CAMERA_WORK;


//*****************************************************************************
// �v���g�^�C�v�錾
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
