//=============================================================================
//
// 地面処理 [field.cpp]
// Author : 
//
//=============================================================================
#include "field.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_FIELD	"data/TEXTURE/field001.jpg"	// 読み込むテクスチャファイル名

#define FILED_MAX		(4)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureField = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffField = NULL;	// 頂点バッファへのポインタ

D3DXMATRIX				g_mtxWorldField;			// ワールドマトリックス
D3DXVECTOR3				g_posField;					// 現在の位置
D3DXVECTOR3				g_rotField;					// 現在の向き

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 
	HRESULT hr;

	// 位置、向きの初期設定
	g_posField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
								TEXTURE_FIELD,			// ファイルの名前
								&g_pD3DTextureField);	// 読み込むメモリー

	// 頂点情報の作成
	hr = MakeVertexField(pDevice);

	return hr;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitField(void)
{
	if(g_pD3DTextureField != NULL)
	{// テクスチャの開放
		g_pD3DTextureField->Release();
		g_pD3DTextureField = NULL;
	}

	if(g_pD3DVtxBuffField != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffField->Release();
		g_pD3DVtxBuffField = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateField(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 
	D3DXMATRIX mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldField);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotField.y, g_rotField.x, g_rotField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_posField.x, g_posField.y, g_posField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldField);

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pD3DVtxBuffField, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pD3DTextureField);


	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	for (int i = 0; i < FILED_MAX; i++)
	{
		// オブジェクトの頂点バッファを生成
		if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
			FVF_VERTEX_3D,				// 使用する頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
			&g_pD3DVtxBuffField,		// 頂点バッファインターフェースへのポインタ
			NULL)))						// NULLに設定
		{
			return E_FAIL;
		}

		{//頂点バッファの中身を埋める
			VERTEX_3D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			g_pD3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-200.0f*i, 0.0f, 200.0f*i);
			pVtx[1].vtx = D3DXVECTOR3(200.0f*i, 0.0f, 200.0f*i);
			pVtx[2].vtx = D3DXVECTOR3(-200.0f*i, 0.0f, -200.0f*i);
			pVtx[3].vtx = D3DXVECTOR3(200.0f*i, 0.0f, -200.0f*i);

			// 法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// 頂点データをアンロックする
			g_pD3DVtxBuffField->Unlock();
		}
	}
	return S_OK;
}


//=============================================================================
// 内積
//=============================================================================
float dotProduct(D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
{

	float ans = (vl->x*vr->x) + (vl->y*vr->y) + (vl->z*vr->z);


	return(ans);
}


//=============================================================================
// 外積
//=============================================================================
void crossProduct(D3DXVECTOR3 *ret, D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
{
	//#if 0

	ret->x = (vl->y * vr->z) - (vl->z * vr->y);
	ret->y = (vl->z * vr->x) - (vl->x * vr->z);
	ret->z = (vl->x * vr->y) - (vl->y * vr->x);
	//#else
		//関数
		//D3DXVECTOR3 ans = D3DXVec3Cross(ret, vl, vr);


	//#endif

}


//=============================================================================
// ポリゴンと線分との当たり判定
// p0  :ポリゴンの頂点1
// p1  :ポリゴンの頂点2
// p2  :ポリゴンの頂点3
// pos0:始点（移動前）
// pos1:終点（移動後）
//=============================================================================
int hitCheck(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	D3DXVECTOR3		nor;		// ポリゴンの法線

	{	// ポリゴンの外積をとって法線を求める。そして正規化しておく。
		D3DXVECTOR3		vec01 = p1 - p0;
		D3DXVECTOR3		vec02 = p2 - p0;
		crossProduct(&nor, &vec01, &vec02);
		D3DXVec3Normalize(&nor, &nor);


	}

	{	// 平面(三角形ポリゴン)と線分の内積とって衝突している可能性を調べる
		// 求めた法線とベクトル２つ（線分の両端とポリゴン上の任意の点）の内積とって衝突している可能性を調べる
		D3DXVECTOR3		vec1 = pos0 - p0;
		D3DXVECTOR3		vec2 = pos1 - p0;
		float a = dotProduct(&vec1, &nor);
		float b = dotProduct(&vec2, &nor);
		if ((a * b) > 0)
		{
			// 当たっている可能性は無い
			return(0);
		}
	}


	{	// ポリゴンと線分の交点を求める
		//D3DXVECTOR3		vec1 = pos0 - p0;
		//D3DXVECTOR3		vec2 = pos0 - p0;
		//float			d1;					// 内分比を求める為の点とポリゴンとの距離
		//float			d2;					// 内分比を求める為の点とポリゴンとの距離
		//float			a;					// 内分比
		//D3DXVECTOR3		vec3;
		//D3DXVECTOR3		p3;					// 交点


		D3DXVECTOR3		vec1 = pos0 - p0;
		D3DXVECTOR3		vec2 = pos1 - p0;
		float			d1 = fabs(dotProduct(&nor, &vec1));
		float			d2 = fabs(dotProduct(&nor, &vec2));
		float			a = d1 / (d1 + d2);
		D3DXVECTOR3		vec3 = (1 - a)*vec1 + a * vec2;
		D3DXVECTOR3		p3 = p0 + vec3;




		{	// 求めた交点がポリゴンの中にあるか調べる

			// ポリゴンの各辺のベクトル
			D3DXVECTOR3		v1 = p1 - p0;
			D3DXVECTOR3		v2 = p2 - p1;
			D3DXVECTOR3		v3 = p3 = p0;

			// 各頂点と交点とのベクトル
			D3DXVECTOR3		v4 = p3 - p1;
			D3DXVECTOR3		v5 = p3 - p2;
			D3DXVECTOR3		v6 = p3 - p0;

			// 各辺との外積で法線を求める
			D3DXVECTOR3		n1, n2, n3;
			crossProduct(&n1, &v1, &v4);
			crossProduct(&n2, &v2, &v5);
			crossProduct(&n3, &v3, &v6);



			// それぞれのベクトルの向きを内積でチェック
			if (dotProduct(&n1, &nor) < 0) return(0);
			if (dotProduct(&n2, &nor) < 0) return(0);
			if (dotProduct(&n3, &nor) < 0) return(0);



		}
	}

	return(1);
}


//=============================================================================
// ビルボードとの当たり判定
// pos0:始点（移動前）
// pos1:終点（移動後）
//=============================================================================
int hitCheckBill(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	int				ans = 0;	// 一先ず当たっていないをセットしておく

	D3DXVECTOR3		pos[4];
	D3DXVECTOR3		nor;		// ポリゴンの法線

								// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	VERTEX_3D		*pVtx;
	g_pD3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < 1; i++, pVtx += 4)
	{	// まずは、ポリゴンの頂点を求める

		// 頂点座標の設定
		pos[0] = pVtx[0].vtx;
		pos[1] = pVtx[1].vtx;
		pos[2] = pVtx[2].vtx;
		pos[3] = pVtx[3].vtx;

		// 左下側ポリゴンと線分の当たり判定
		ans = hitCheck(pos[0], pos[2], pos[3], pos0, pos1);
		if (ans != 0) break;

		// 右上側ポリゴンと線分の当たり判定
		ans = hitCheck(pos[0], pos[3], pos[1], pos0, pos1);
		if (ans != 0) break;

	}

	// 頂点データをアンロックする
	g_pD3DVtxBuffField->Unlock();




	return(ans);

}
