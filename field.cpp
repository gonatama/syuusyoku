//=============================================================================
//
// �n�ʏ��� [field.cpp]
// Author : 
//
//=============================================================================
#include "field.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_FIELD	"data/TEXTURE/field001.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����

#define FILED_MAX		(4)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureField = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffField = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

D3DXMATRIX				g_mtxWorldField;			// ���[���h�}�g���b�N�X
D3DXVECTOR3				g_posField;					// ���݂̈ʒu
D3DXVECTOR3				g_rotField;					// ���݂̌���

//=============================================================================
// ����������
//=============================================================================
HRESULT InitField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 
	HRESULT hr;

	// �ʒu�A�����̏����ݒ�
	g_posField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_FIELD,			// �t�@�C���̖��O
								&g_pD3DTextureField);	// �ǂݍ��ރ������[

	// ���_���̍쐬
	hr = MakeVertexField(pDevice);

	return hr;
}

//=============================================================================
// �I������
//=============================================================================
void UninitField(void)
{
	if(g_pD3DTextureField != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureField->Release();
		g_pD3DTextureField = NULL;
	}

	if(g_pD3DVtxBuffField != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffField->Release();
		g_pD3DVtxBuffField = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateField(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 
	D3DXMATRIX mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldField);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotField.y, g_rotField.x, g_rotField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_posField.x, g_posField.y, g_posField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldField);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pD3DVtxBuffField, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureField);


	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	for (int i = 0; i < FILED_MAX; i++)
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
			D3DUSAGE_WRITEONLY,			// ���_�o�b�t�@�̎g�p�@�@
			FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
			D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
			&g_pD3DVtxBuffField,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
			NULL)))						// NULL�ɐݒ�
		{
			return E_FAIL;
		}

		{//���_�o�b�t�@�̒��g�𖄂߂�
			VERTEX_3D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_pD3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(-200.0f*i, 0.0f, 200.0f*i);
			pVtx[1].vtx = D3DXVECTOR3(200.0f*i, 0.0f, 200.0f*i);
			pVtx[2].vtx = D3DXVECTOR3(-200.0f*i, 0.0f, -200.0f*i);
			pVtx[3].vtx = D3DXVECTOR3(200.0f*i, 0.0f, -200.0f*i);

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// ���ˌ��̐ݒ�
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// ���_�f�[�^���A�����b�N����
			g_pD3DVtxBuffField->Unlock();
		}
	}
	return S_OK;
}


//=============================================================================
// ����
//=============================================================================
float dotProduct(D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
{

	float ans = (vl->x*vr->x) + (vl->y*vr->y) + (vl->z*vr->z);


	return(ans);
}


//=============================================================================
// �O��
//=============================================================================
void crossProduct(D3DXVECTOR3 *ret, D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
{
	//#if 0

	ret->x = (vl->y * vr->z) - (vl->z * vr->y);
	ret->y = (vl->z * vr->x) - (vl->x * vr->z);
	ret->z = (vl->x * vr->y) - (vl->y * vr->x);
	//#else
		//�֐�
		//D3DXVECTOR3 ans = D3DXVec3Cross(ret, vl, vr);


	//#endif

}


//=============================================================================
// �|���S���Ɛ����Ƃ̓����蔻��
// p0  :�|���S���̒��_1
// p1  :�|���S���̒��_2
// p2  :�|���S���̒��_3
// pos0:�n�_�i�ړ��O�j
// pos1:�I�_�i�ړ���j
//=============================================================================
int hitCheck(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	D3DXVECTOR3		nor;		// �|���S���̖@��

	{	// �|���S���̊O�ς��Ƃ��Ė@�������߂�B�����Đ��K�����Ă����B
		D3DXVECTOR3		vec01 = p1 - p0;
		D3DXVECTOR3		vec02 = p2 - p0;
		crossProduct(&nor, &vec01, &vec02);
		D3DXVec3Normalize(&nor, &nor);


	}

	{	// ����(�O�p�`�|���S��)�Ɛ����̓��ςƂ��ďՓ˂��Ă���\���𒲂ׂ�
		// ���߂��@���ƃx�N�g���Q�i�����̗��[�ƃ|���S����̔C�ӂ̓_�j�̓��ςƂ��ďՓ˂��Ă���\���𒲂ׂ�
		D3DXVECTOR3		vec1 = pos0 - p0;
		D3DXVECTOR3		vec2 = pos1 - p0;
		float a = dotProduct(&vec1, &nor);
		float b = dotProduct(&vec2, &nor);
		if ((a * b) > 0)
		{
			// �������Ă���\���͖���
			return(0);
		}
	}


	{	// �|���S���Ɛ����̌�_�����߂�
		//D3DXVECTOR3		vec1 = pos0 - p0;
		//D3DXVECTOR3		vec2 = pos0 - p0;
		//float			d1;					// ����������߂�ׂ̓_�ƃ|���S���Ƃ̋���
		//float			d2;					// ����������߂�ׂ̓_�ƃ|���S���Ƃ̋���
		//float			a;					// ������
		//D3DXVECTOR3		vec3;
		//D3DXVECTOR3		p3;					// ��_


		D3DXVECTOR3		vec1 = pos0 - p0;
		D3DXVECTOR3		vec2 = pos1 - p0;
		float			d1 = fabs(dotProduct(&nor, &vec1));
		float			d2 = fabs(dotProduct(&nor, &vec2));
		float			a = d1 / (d1 + d2);
		D3DXVECTOR3		vec3 = (1 - a)*vec1 + a * vec2;
		D3DXVECTOR3		p3 = p0 + vec3;




		{	// ���߂���_���|���S���̒��ɂ��邩���ׂ�

			// �|���S���̊e�ӂ̃x�N�g��
			D3DXVECTOR3		v1 = p1 - p0;
			D3DXVECTOR3		v2 = p2 - p1;
			D3DXVECTOR3		v3 = p3 = p0;

			// �e���_�ƌ�_�Ƃ̃x�N�g��
			D3DXVECTOR3		v4 = p3 - p1;
			D3DXVECTOR3		v5 = p3 - p2;
			D3DXVECTOR3		v6 = p3 - p0;

			// �e�ӂƂ̊O�ςŖ@�������߂�
			D3DXVECTOR3		n1, n2, n3;
			crossProduct(&n1, &v1, &v4);
			crossProduct(&n2, &v2, &v5);
			crossProduct(&n3, &v3, &v6);



			// ���ꂼ��̃x�N�g���̌�������ςŃ`�F�b�N
			if (dotProduct(&n1, &nor) < 0) return(0);
			if (dotProduct(&n2, &nor) < 0) return(0);
			if (dotProduct(&n3, &nor) < 0) return(0);



		}
	}

	return(1);
}


//=============================================================================
// �r���{�[�h�Ƃ̓����蔻��
// pos0:�n�_�i�ړ��O�j
// pos1:�I�_�i�ړ���j
//=============================================================================
int hitCheckBill(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	int				ans = 0;	// ��悸�������Ă��Ȃ����Z�b�g���Ă���

	D3DXVECTOR3		pos[4];
	D3DXVECTOR3		nor;		// �|���S���̖@��

								// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	VERTEX_3D		*pVtx;
	g_pD3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < 1; i++, pVtx += 4)
	{	// �܂��́A�|���S���̒��_�����߂�

		// ���_���W�̐ݒ�
		pos[0] = pVtx[0].vtx;
		pos[1] = pVtx[1].vtx;
		pos[2] = pVtx[2].vtx;
		pos[3] = pVtx[3].vtx;

		// �������|���S���Ɛ����̓����蔻��
		ans = hitCheck(pos[0], pos[2], pos[3], pos0, pos1);
		if (ans != 0) break;

		// �E�㑤�|���S���Ɛ����̓����蔻��
		ans = hitCheck(pos[0], pos[3], pos[1], pos0, pos1);
		if (ans != 0) break;

	}

	// ���_�f�[�^���A�����b�N����
	g_pD3DVtxBuffField->Unlock();




	return(ans);

}
