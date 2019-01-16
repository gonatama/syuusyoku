//=============================================================================
//
// パーティクル処理 [particle.cpp]
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
// マクロ定義
//*****************************************************************************
#define	TEXTURE_PARTICLE	"data/TEXTURE/shadow000.jpg"	// 読み込むテクスチャファイル名
#define	TEXTURE_PARTICLE_END	 "data/TEXTURE/namaegg.png"	// 読み込むテクスチャファイル名"data/TEXTURE/shadow000.jpg"
#define	PARTICLE_SIZE_X		(50.0f)							// ビルボードの幅
#define	PARTICLE_SIZE_Y		(50.0f)							// ビルボードの高さ
#define	VALUE_MOVE_PARTICLE	(0.8f)							// 移動速度

#define	MAX_PARTICLE2			(32)						// ビルボード最大数
#define	PARTICLE_ROTATE	(D3DX_PI * 0.02f)								// 回転量
#define	DISP_SHADOW				// 影の表示
//#undef DISP_SHADOW

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 回転
	D3DXVECTOR3 scale;		// スケール
	D3DXVECTOR3 move;		// 移動量
	D3DXCOLOR col;			// 色
	//float Alpha;
	float fSizeX;			// 幅
	float fSizeY;			// 高さ
	int nIdxShadow;			// 影ID
	int nLife;				// 寿命
	bool bUse;				// 使用しているかどうか
} PARTICLE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexParticle2(LPDIRECT3DDEVICE9 pDevice);
void SetVertexParticle2(int nIdxParticle2, float fSizeX, float fSizeY);
void SetColorParticle2(int nIdxParticle2, D3DXCOLOR col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureParticle2 = NULL;	// テクスチャへのポインタ
LPDIRECT3DTEXTURE9		g_pD3DTextureParticle2_end = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffParticle2 = NULL;	// 頂点バッファインターフェースへのポインタ

D3DXMATRIX				g_mtxWorldParticle2;				// ワールドマトリックス

PARTICLE				g_aParticle2[MAX_PARTICLE2];		// パーティクルワーク
D3DXVECTOR3				g_posBase2;						// ビルボード発生位置
float					g_fWidthBase2 = 2.0f;			// 基準の幅
float					g_fHeightBase2 = 10.0f;			// 基準の高さ
bool					g_bPause2 = false;				// ポーズON/OFF

int						rot;
static	float				Alpha;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitParticle2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexParticle2(pDevice);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_PARTICLE,			// ファイルの名前
		&g_pD3DTextureParticle2);	// 読み込むメモリー

	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
		TEXTURE_PARTICLE_END,			// ファイルの名前
		&g_pD3DTextureParticle2_end);	// 読み込むメモリー


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
// 終了処理
//=============================================================================
void UninitParticle2(void)
{
	if (g_pD3DTextureParticle2 != NULL)
	{// テクスチャの開放
		g_pD3DTextureParticle2->Release();
		g_pD3DTextureParticle2 = NULL;
	}
	if (g_pD3DTextureParticle2_end != NULL)
	{// テクスチャの開放
		g_pD3DTextureParticle2_end->Release();
		g_pD3DTextureParticle2_end = NULL;
	}

	if (g_pD3DVtxBuffParticle2 != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffParticle2->Release();
		g_pD3DVtxBuffParticle2 = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateParticle2(void)
{
	D3DXVECTOR3 rotCamera;
	PLAYER *player = GetPlayer(0);
	// カメラの回転を取得
	rotCamera = GetRotCamera();

	
	//if (GetKeyboardPress(DIK_LEFT))
	//{
	//	if (GetKeyboardPress(DIK_UP))
	//	{// 左前移動
	//		g_posBase2.x -= sinf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PARTICLE;
	//		g_posBase2.z -= cosf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PARTICLE;
	//	}
	//	else if (GetKeyboardPress(DIK_DOWN))
	//	{// 左後移動
	//		g_posBase2.x -= sinf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PARTICLE;
	//		g_posBase2.z -= cosf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PARTICLE;
	//	}
	//	else
	//	{// 左移動
	//		g_posBase2.x -= sinf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PARTICLE;
	//		g_posBase2.z -= cosf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PARTICLE;
	//	}
	//}
	//else if (GetKeyboardPress(DIK_RIGHT))
	//{
	//	if (GetKeyboardPress(DIK_UP))
	//	{// 右前移動
	//		g_posBase2.x -= sinf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PARTICLE;
	//		g_posBase2.z -= cosf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PARTICLE;
	//	}
	//	else if (GetKeyboardPress(DIK_DOWN))
	//	{// 右後移動
	//		g_posBase2.x -= sinf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PARTICLE;
	//		g_posBase2.z -= cosf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PARTICLE;
	//	}
	//	else
	//	{// 右移動
	//		g_posBase2.x -= sinf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PARTICLE;
	//		g_posBase2.z -= cosf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PARTICLE;
	//	}
	//}
	//else if (GetKeyboardPress(DIK_UP))
	//{// 前移動
	//	g_posBase2.x -= sinf(D3DX_PI + rotCamera.y) * VALUE_MOVE_PARTICLE;
	//	g_posBase2.z -= cosf(D3DX_PI + rotCamera.y) * VALUE_MOVE_PARTICLE;
	//}
	//else if (GetKeyboardPress(DIK_DOWN))
	//{// 後移動
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
			{// 使用中
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
				//{// 着地した
				//	g_aParticle2[nCntParticle2].pos.x = g_aParticle2[nCntParticle2].fSizeX*(-1);
				//	g_aParticle2[nCntParticle2].move.x = -g_aParticle2[nCntParticle2].move.x * 1.5f;
				//}
				//g_aParticle2[nCntParticle2].pos.z += 3.0f* sinf(D3DXToRadian(rot));
				//*VALUE_MOVE_PARTICLE*(rot % 45);
				//g_aParticle2[nCntParticle2].pos.x += sinf(D3DXToRadian(VALUE_MOVE_PARTICLE*(rot % 45)));
				//g_aParticle2[nCntParticle2].pos.z += sinf(D3DXToRadian(rot % 30))*VALUE_MOVE_PARTICLE*(g_aParticle2[nCntParticle2].nLife % 60);

				//g_aParticle2[nCntParticle2].pos.y += sinf(D3DXToRadian(rot))*VALUE_MOVE_PARTICLE*(g_aParticle2[nCntParticle2].nLife % 15);

				//// 硝煙風味
				//{
				//	g_aParticle2[nCntParticle2].pos.x += 3.0f* sinf(D3DXToRadian(rot));

					//rot += 45;
				rot += 45;
				//	g_aParticle2[nCntParticle2].pos.y += g_aParticle2[nCntParticle2].move.y / 8;


				//	if (g_aParticle2[nCntParticle2].pos.x <= g_aParticle2[nCntParticle2].fSizeX)
				//	{// 着地した
				//		g_aParticle2[nCntParticle2].pos.x = g_aParticle2[nCntParticle2].fSizeX*(-1);
				//		g_aParticle2[nCntParticle2].move.x = -g_aParticle2[nCntParticle2].move.x;
				//	}
				//}
				//if (g_aParticle2[nCntParticle2].pos.y >= g_aParticle2[nCntParticle2].fSizeY*10)
				//{// 着地した
				//	g_aParticle2[nCntParticle2].pos.y = g_aParticle2[nCntParticle2].fSizeY*10;
				//	g_aParticle2[nCntParticle2].move.y = -g_aParticle2[nCntParticle2].move.y * 0.05f;
				//}


				//g_aParticle2[nCntParticle2].move.x += (0.0f - g_aParticle2[nCntParticle2].move.x) * 0.015f;
				//g_aParticle2[nCntParticle2].move.y -= 0.25f;
				//g_aParticle2[nCntParticle2].move.z += (0.0f - g_aParticle2[nCntParticle2].move.z) * 0.015f;



#ifdef DISP_SHADOW
				if (g_aParticle2[nCntParticle2].nIdxShadow != -1)
				{// 影使用中
					float colA;

					// 影の位置設定
					SetPositionShadow(g_aParticle2[nCntParticle2].nIdxShadow, D3DXVECTOR3(g_aParticle2[nCntParticle2].pos.x, 0.1f, g_aParticle2[nCntParticle2].pos.z));

					// 影のα値設定
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

					// 影の色の設定
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
						// α値設定
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
						// 色の設定
						SetColorParticle2(nCntParticle2, g_aParticle2[nCntParticle2].col);

				}
			}
		}

		// パーティクル発生
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
			
			// ビルボードの設定
			SetParticle2(player->posPlayer, move, D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f), fSize, fSize, nLife);
		}

	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawParticle2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	// ライティングを無効に
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// パーティクルっぽく設定をする

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);	// 結果 = 転送先(DEST) - 転送元(SRC)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);


	// Z比較なし
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, Alpha);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);





	for (int nCntParticle2 = 0; nCntParticle2 < MAX_PARTICLE2; nCntParticle2++)
	{
		if (g_aParticle2[nCntParticle2].bUse)
		{

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_mtxWorldParticle2);

			// ビューマトリックスを取得
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

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, g_aParticle2[nCntParticle2].scale.x, g_aParticle2[nCntParticle2].scale.y, g_aParticle2[nCntParticle2].scale.z);
			D3DXMatrixMultiply(&g_mtxWorldParticle2, &g_mtxWorldParticle2, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_aParticle2[nCntParticle2].pos.x, g_aParticle2[nCntParticle2].pos.y, g_aParticle2[nCntParticle2].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldParticle2, &g_mtxWorldParticle2, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldParticle2);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pD3DVtxBuffParticle2, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);


			if (g_aParticle2[nCntParticle2].bUse == true)
			{
				// テクスチャの設定
				pDevice->SetTexture(0, g_pD3DTextureParticle2);
			}
			//else if (g_aParticle2[nCntParticle2].nLife <= 0)
			//{

			//	// テクスチャの設定
			//	pDevice->SetTexture(0, g_pD3DTextureParticle2_end);

			//}


			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntParticle2 * 4), NUM_POLYGON);
		}
	}

	// ライティングを有効に
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// 通常ブレンドに戻す
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexParticle2(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_PARTICLE2,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
		FVF_VERTEX_3D,								// 使用する頂点フォーマット
		D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffParticle2,					// 頂点バッファインターフェースへのポインタ
		NULL)))										// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffParticle2->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntParticle2 = 0; nCntParticle2 < MAX_PARTICLE2; nCntParticle2++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);

			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// 頂点データをアンロックする
		g_pD3DVtxBuffParticle2->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexParticle2(int nIdxParticle2, float fSizeX, float fSizeY)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffParticle2->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxParticle2 * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX / 2, fSizeY / 2, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX / 2, -fSizeY / 2, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX / 2, fSizeY / 2, 0.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffParticle2->Unlock();
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorParticle2(int nIdxParticle2, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffParticle2->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxParticle2 * 4);

		// 頂点座標の設定
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		g_pD3DVtxBuffParticle2->Unlock();
	}
}

//=============================================================================
// 頂点情報の作成
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

			// 頂点座標の設定
			SetVertexParticle2(nCntParticle2, fSizeX, fSizeY);

			nIdxParticle2 = nCntParticle2;

#ifdef DISP_SHADOW
			// 影の設定
			g_aParticle2[nCntParticle2].nIdxShadow = CreateShadow(D3DXVECTOR3(pos.x, 0.1f, pos.z), fSizeX, fSizeY);		// 影の設定
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
