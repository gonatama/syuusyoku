//=============================================================================
//
// �p�[�e�B�N������ [particle.cpp]
// Author : 
//
//=============================================================================
#include "player.h"
#include "particle2.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include <time.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_PARTICLE	"data/TEXTURE/shadow000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_PARTICLE_END	 "data/TEXTURE/namaegg.png"	// �ǂݍ��ރe�N�X�`���t�@�C����"data/TEXTURE/shadow000.jpg"
#define	PARTICLE_SIZE_X		(50.0f)							// �r���{�[�h�̕�
#define	PARTICLE_SIZE_Y		(50.0f)							// �r���{�[�h�̍���
#define	VALUE_MOVE_PARTICLE	(0.8f)							// �ړ����x

#define	MAX_PARTICLE2			(32)						// �r���{�[�h�ő吔
#define	PARTICLE_ROTATE	(D3DX_PI * 0.02f)								// ��]��
#define	DISP_SHADOW				// �e�̕\��
//#undef DISP_SHADOW

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ��]
	D3DXVECTOR3 scale;		// �X�P�[��
	D3DXVECTOR3 move;		// �ړ���
	D3DXCOLOR col;			// �F
	//float Alpha;
	float fSizeX;			// ��
	float fSizeY;			// ����
	int nIdxShadow;			// �eID
	int nLife;				// ����
	bool bUse;				// �g�p���Ă��邩�ǂ���
} PARTICLE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexParticle2(LPDIRECT3DDEVICE9 pDevice);
void SetVertexParticle2(int nIdxParticle2, float fSizeX, float fSizeY);
void SetColorParticle2(int nIdxParticle2, D3DXCOLOR col);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureParticle2 = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9		g_pD3DTextureParticle2_end = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffParticle2 = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

D3DXMATRIX				g_mtxWorldParticle2;				// ���[���h�}�g���b�N�X

PARTICLE				g_aParticle2[MAX_PARTICLE2];		// �p�[�e�B�N�����[�N
D3DXVECTOR3				g_posBase2;						// �r���{�[�h�����ʒu
float					g_fWidthBase2 = 2.0f;			// ��̕�
float					g_fHeightBase2 = 10.0f;			// ��̍���
bool					g_bPause2 = false;				// �|�[�YON/OFF

int						rot;
static	float				Alpha;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitParticle2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̍쐬
	MakeVertexParticle2(pDevice);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PARTICLE,			// �t�@�C���̖��O
		&g_pD3DTextureParticle2);	// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PARTICLE_END,			// �t�@�C���̖��O
		&g_pD3DTextureParticle2_end);	// �ǂݍ��ރ������[


	for (int nCntParticle2 = 0; nCntParticle2 < MAX_PARTICLE2; nCntParticle2++)
	{
		g_aParticle2[nCntParticle2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle2[nCntParticle2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle2[nCntParticle2].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aParticle2[nCntParticle2].move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aParticle2[nCntParticle2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle2[nCntParticle2].fSizeX = PARTICLE_SIZE_X;
		g_aParticle2[nCntParticle2].fSizeY = PARTICLE_SIZE_Y;
		g_aParticle2[nCntParticle2].nIdxShadow = -1;
		g_aParticle2[nCntParticle2].nLife = 0;
		//g_aParticle2[nCntParticle2].Alpha = 0;
		g_aParticle2[nCntParticle2].bUse = false;
	}

	g_posBase2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitParticle2(void)
{
	if (g_pD3DTextureParticle2 != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureParticle2->Release();
		g_pD3DTextureParticle2 = NULL;
	}
	if (g_pD3DTextureParticle2_end != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureParticle2_end->Release();
		g_pD3DTextureParticle2_end = NULL;
	}

	if (g_pD3DVtxBuffParticle2 != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffParticle2->Release();
		g_pD3DVtxBuffParticle2 = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateParticle2(void)
{
	D3DXVECTOR3 rotCamera;
	PLAYER *player = GetPlayer(0);
	// �J�����̉�]���擾
	rotCamera = GetRotCamera();

	
	//if (GetKeyboardPress(DIK_LEFT))
	//{
	//	if (GetKeyboardPress(DIK_UP))
	//	{// ���O�ړ�
	//		g_posBase2.x -= sinf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PARTICLE;
	//		g_posBase2.z -= cosf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PARTICLE;
	//	}
	//	else if (GetKeyboardPress(DIK_DOWN))
	//	{// ����ړ�
	//		g_posBase2.x -= sinf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PARTICLE;
	//		g_posBase2.z -= cosf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PARTICLE;
	//	}
	//	else
	//	{// ���ړ�
	//		g_posBase2.x -= sinf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PARTICLE;
	//		g_posBase2.z -= cosf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PARTICLE;
	//	}
	//}
	//else if (GetKeyboardPress(DIK_RIGHT))
	//{
	//	if (GetKeyboardPress(DIK_UP))
	//	{// �E�O�ړ�
	//		g_posBase2.x -= sinf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PARTICLE;
	//		g_posBase2.z -= cosf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PARTICLE;
	//	}
	//	else if (GetKeyboardPress(DIK_DOWN))
	//	{// �E��ړ�
	//		g_posBase2.x -= sinf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PARTICLE;
	//		g_posBase2.z -= cosf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PARTICLE;
	//	}
	//	else
	//	{// �E�ړ�
	//		g_posBase2.x -= sinf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PARTICLE;
	//		g_posBase2.z -= cosf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PARTICLE;
	//	}
	//}
	//else if (GetKeyboardPress(DIK_UP))
	//{// �O�ړ�
	//	g_posBase2.x -= sinf(D3DX_PI + rotCamera.y) * VALUE_MOVE_PARTICLE;
	//	g_posBase2.z -= cosf(D3DX_PI + rotCamera.y) * VALUE_MOVE_PARTICLE;
	//}
	//else if (GetKeyboardPress(DIK_DOWN))
	//{// ��ړ�
	//	g_posBase2.x -= sinf(rotCamera.y) * VALUE_MOVE_PARTICLE;
	//	g_posBase2.z -= cosf(rotCamera.y) * VALUE_MOVE_PARTICLE;
	//}

	//if (GetKeyboardPress(DIK_1))
	//{
	//	g_fWidthBase2 -= 0.1f;
	//	if (g_fWidthBase2 < 2.0f)
	//	{
	//		g_fWidthBase2 = 2.0f;
	//	}
	//}
	//if (GetKeyboardPress(DIK_2))
	//{
	//	g_fWidthBase2 += 0.1f;
	//	if (g_fWidthBase2 > 10.0f)
	//	{
	//		g_fWidthBase2 = 10.0f;
	//	}
	//}
	//if (GetKeyboardPress(DIK_3))
	//{
	//	g_fHeightBase2 -= 0.1f;
	//	if (g_fHeightBase2 < 5.0f)
	//	{
	//		g_fHeightBase2 = 5.0f;
	//	}
	//}
	//if (GetKeyboardPress(DIK_4))
	//{
	//	g_fHeightBase2 += 0.1f;
	//	if (g_fHeightBase2 > 15.0f)
	//	{
	//		g_fHeightBase2 = 15.0f;
	//	}
	//}

	//if (GetKeyboardTrigger(DIK_P))
	//{
	//	g_bPause2 = g_bPause2 ? false : true;
	//}



	if (g_bPause2 == false)
	{
		for (int nCntParticle2 = 0; nCntParticle2 < MAX_PARTICLE2; nCntParticle2++)
		{
			if (g_aParticle2[nCntParticle2].bUse)
			{// �g�p��
				//if (g_aParticle2[nCntParticle2].bUse == false)
				//{
				//	g_aParticle2[nCntParticle2].bUse = true;
				//	/*g_aParticle2[nCntParticle2].*/Alpha = 100;

				//}
				//if (g_aParticle2[nCntParticle2].bUse == true)
				//{
				//	/*g_aParticle2[nCntParticle2].*/Alpha--;
				//}






				//g_aParticle2[nCntParticle2].pos.x += g_aParticle2[nCntParticle2].move.x;
				//g_aParticle2[nCntParticle2].pos.z += g_aParticle2[nCntParticle2].move.z;

				//if (g_aParticle2[nCntParticle2].pos.x <= g_aParticle2[nCntParticle2].fSizeX*(-1))
				//{// ���n����
				//	g_aParticle2[nCntParticle2].pos.x = g_aParticle2[nCntParticle2].fSizeX*(-1);
				//	g_aParticle2[nCntParticle2].move.x = -g_aParticle2[nCntParticle2].move.x * 1.5f;
				//}
				//g_aParticle2[nCntParticle2].pos.z += 3.0f* sinf(D3DXToRadian(rot));
				//*VALUE_MOVE_PARTICLE*(rot % 45);
				//g_aParticle2[nCntParticle2].pos.x += sinf(D3DXToRadian(VALUE_MOVE_PARTICLE*(rot % 45)));
				//g_aParticle2[nCntParticle2].pos.z += sinf(D3DXToRadian(rot % 30))*VALUE_MOVE_PARTICLE*(g_aParticle2[nCntParticle2].nLife % 60);

				//g_aParticle2[nCntParticle2].pos.y += sinf(D3DXToRadian(rot))*VALUE_MOVE_PARTICLE*(g_aParticle2[nCntParticle2].nLife % 15);

				//// �ɉ�����
				//{
				//	g_aParticle2[nCntParticle2].pos.x += 3.0f* sinf(D3DXToRadian(rot));

					//rot += 45;
				rot += 45;
				//	g_aParticle2[nCntParticle2].pos.y += g_aParticle2[nCntParticle2].move.y / 8;


				//	if (g_aParticle2[nCntParticle2].pos.x <= g_aParticle2[nCntParticle2].fSizeX)
				//	{// ���n����
				//		g_aParticle2[nCntParticle2].pos.x = g_aParticle2[nCntParticle2].fSizeX*(-1);
				//		g_aParticle2[nCntParticle2].move.x = -g_aParticle2[nCntParticle2].move.x;
				//	}
				//}
				//if (g_aParticle2[nCntParticle2].pos.y >= g_aParticle2[nCntParticle2].fSizeY*10)
				//{// ���n����
				//	g_aParticle2[nCntParticle2].pos.y = g_aParticle2[nCntParticle2].fSizeY*10;
				//	g_aParticle2[nCntParticle2].move.y = -g_aParticle2[nCntParticle2].move.y * 0.05f;
				//}


				//g_aParticle2[nCntParticle2].move.x += (0.0f - g_aParticle2[nCntParticle2].move.x) * 0.015f;
				//g_aParticle2[nCntParticle2].move.y -= 0.25f;
				//g_aParticle2[nCntParticle2].move.z += (0.0f - g_aParticle2[nCntParticle2].move.z) * 0.015f;



#ifdef DISP_SHADOW
				if (g_aParticle2[nCntParticle2].nIdxShadow != -1)
				{// �e�g�p��
					float colA;

					// �e�̈ʒu�ݒ�
					SetPositionShadow(g_aParticle2[nCntParticle2].nIdxShadow, D3DXVECTOR3(g_aParticle2[nCntParticle2].pos.x, 0.1f, g_aParticle2[nCntParticle2].pos.z));

					// �e�̃��l�ݒ�
					if (g_aParticle2[nCntParticle2].col.a > 0.0f)
					{
						colA = (300.0f - (g_aParticle2[nCntParticle2].pos.y - 9.0f)) / (300.0f / g_aParticle2[nCntParticle2].col.a);
						if (colA < 0.0f)
						{
							colA = 0.0f;
						}
					}
					else
					{
						colA = 0.0f;
					}

					// �e�̐F�̐ݒ�
					SetColorShadow(g_aParticle2[nCntParticle2].nIdxShadow, D3DXCOLOR(0.15f, 0.15f, 0.15f, colA));
				}
#endif

				g_aParticle2[nCntParticle2].nLife--;
				if(g_aParticle2[nCntParticle2].nLife <= 0)
				{
					g_aParticle2[nCntParticle2].bUse = false;
					ReleaseShadow(g_aParticle2[nCntParticle2].nIdxShadow);
					g_aParticle2[nCntParticle2].nIdxShadow = -1;
				}
				else
				{
					if(g_aParticle2[nCntParticle2].nLife <= 80)
					{
						g_aParticle2[nCntParticle2].col.r = 0.60f - (float)(80 - g_aParticle2[nCntParticle2].nLife) / 8.0f * 0.06f + 0.2f;
						g_aParticle2[nCntParticle2].col.g = 0.70f - (float)(80 - g_aParticle2[nCntParticle2].nLife) / 8.0f * 0.07f;
						g_aParticle2[nCntParticle2].col.b = 0.05f;
					}

					if(g_aParticle2[nCntParticle2].nLife <= 30)
					{
						// ���l�ݒ�
						g_aParticle2[nCntParticle2].col.a -= 0.15f;
						if(g_aParticle2[nCntParticle2].col.a < 0.0f)
						{
							g_aParticle2[nCntParticle2].col.a = 0.0f;
						}
					}

				//if (/*g_aParticle2[nCntParticle2].*/Alpha == 0)
				//{
				//	g_aParticle2[nCntParticle2].bUse = false;
				//}
						// �F�̐ݒ�
						SetColorParticle2(nCntParticle2, g_aParticle2[nCntParticle2].col);

				}
			}
		}

		// �p�[�e�B�N������
//		if((rand() % 2) == 0)
		{
			D3DXVECTOR3 pos;
			D3DXVECTOR3 move;
			float fAngle, fLength;
			int nLife;
			float fSize;

			pos = g_posBase2;

			fAngle = (float)(rand() % 628 - 314) / 100.0f;
			fLength = rand() % (int)(g_fWidthBase2 * 20) / 100.0f - g_fWidthBase2;
			move.x = sinf(fAngle) * fLength;
			move.y = rand() % 300 / 100.0f + g_fHeightBase2;
			move.z = cosf(fAngle) * fLength;

			nLife = rand() % 10 + 100;

			fSize = (float)(rand() % 30 + 20);

			//pos.y = fSize / 2;
			
			// �r���{�[�h�̐ݒ�
			SetParticle2(player->posPlayer, move, D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f), fSize, fSize, nLife);
		}

	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawParticle2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	// ���C�e�B���O�𖳌���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �p�[�e�B�N�����ۂ��ݒ������

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);	// ���� = �]����(DEST) - �]����(SRC)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);


	// Z��r�Ȃ�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, Alpha);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);





	for (int nCntParticle2 = 0; nCntParticle2 < MAX_PARTICLE2; nCntParticle2++)
	{
		if (g_aParticle2[nCntParticle2].bUse)
		{

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldParticle2);

			// �r���[�}�g���b�N�X���擾
			mtxView = GetMtxView();


			g_mtxWorldParticle2._11 = mtxView._11;
			g_mtxWorldParticle2._12 = mtxView._21;
			g_mtxWorldParticle2._13 = mtxView._31;
			g_mtxWorldParticle2._21 = mtxView._12;
			g_mtxWorldParticle2._22 = mtxView._22;
			g_mtxWorldParticle2._23 = mtxView._32;
			g_mtxWorldParticle2._31 = mtxView._13;
			g_mtxWorldParticle2._32 = mtxView._23;
			g_mtxWorldParticle2._33 = mtxView._33;

			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScale, g_aParticle2[nCntParticle2].scale.x, g_aParticle2[nCntParticle2].scale.y, g_aParticle2[nCntParticle2].scale.z);
			D3DXMatrixMultiply(&g_mtxWorldParticle2, &g_mtxWorldParticle2, &mtxScale);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTranslate, g_aParticle2[nCntParticle2].pos.x, g_aParticle2[nCntParticle2].pos.y, g_aParticle2[nCntParticle2].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldParticle2, &g_mtxWorldParticle2, &mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldParticle2);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pD3DVtxBuffParticle2, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);


			if (g_aParticle2[nCntParticle2].bUse == true)
			{
				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pD3DTextureParticle2);
			}
			//else if (g_aParticle2[nCntParticle2].nLife <= 0)
			//{

			//	// �e�N�X�`���̐ݒ�
			//	pDevice->SetTexture(0, g_pD3DTextureParticle2_end);

			//}


			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntParticle2 * 4), NUM_POLYGON);
		}
	}

	// ���C�e�B���O��L����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �ʏ�u�����h�ɖ߂�
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexParticle2(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_PARTICLE2,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&g_pD3DVtxBuffParticle2,					// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL)))										// NULL�ɐݒ�
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffParticle2->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntParticle2 = 0; nCntParticle2 < MAX_PARTICLE2; nCntParticle2++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);

			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffParticle2->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexParticle2(int nIdxParticle2, float fSizeX, float fSizeY)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffParticle2->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxParticle2 * 4);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffParticle2->Unlock();
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorParticle2(int nIdxParticle2, D3DXCOLOR col)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffParticle2->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxParticle2 * 4);

		// ���_���W�̐ݒ�
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = col;

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffParticle2->Unlock();
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
int SetParticle2(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSizeX, float fSizeY, int nLife)
{
	int nIdxParticle2 = -1;

	for (int nCntParticle2 = 0; nCntParticle2 < MAX_PARTICLE2; nCntParticle2++)
	{
		if (!g_aParticle2[nCntParticle2].bUse)
		{
			g_aParticle2[nCntParticle2].pos = pos;
			g_aParticle2[nCntParticle2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aParticle2[nCntParticle2].scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_aParticle2[nCntParticle2].move = move;
			g_aParticle2[nCntParticle2].col = col;
			g_aParticle2[nCntParticle2].fSizeX = fSizeX;
			g_aParticle2[nCntParticle2].fSizeY = fSizeY;
			g_aParticle2[nCntParticle2].nLife = nLife;

			g_aParticle2[nCntParticle2].bUse = true;

			// ���_���W�̐ݒ�
			SetVertexParticle2(nCntParticle2, fSizeX, fSizeY);

			nIdxParticle2 = nCntParticle2;

#ifdef DISP_SHADOW
			// �e�̐ݒ�
			g_aParticle2[nCntParticle2].nIdxShadow = CreateShadow(D3DXVECTOR3(pos.x, 0.1f, pos.z), fSizeX, fSizeY);		// �e�̐ݒ�
			if (g_aParticle2[nCntParticle2].nIdxShadow != -1)
			{
				SetColorShadow(g_aParticle2[nCntParticle2].nIdxShadow, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			}
#endif

			break;
		}
	}

	return nIdxParticle2;
}
