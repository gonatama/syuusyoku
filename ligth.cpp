#include "ligth.h"
#include "input.h"

//マクロ
// ライトの数
D3DLIGHT9 Light[NUM_LIGHT];		// ライトのワーク


//=============================================================================
// ライトの初期化処理
//=============================================================================
HRESULT InitLight(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXVECTOR3 vecDir;

	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&Light[0], sizeof(D3DLIGHT9));

	// ライトのタイプの設定
	Light[0].Type = D3DLIGHT_DIRECTIONAL;

	// 拡散光
	Light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 環境光
	Light[0].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(0.20f, -0.60f, 0.80f);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light[0].Direction, &vecDir);

	// ライトをレンダリングパイプラインに設定
	Device->SetLight(0, &Light[0]);

	// ライトの設定
	Device->LightEnable(0, TRUE);


	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&Light[1], sizeof(D3DLIGHT9));

	// ライトのタイプの設定
	Light[1].Type = D3DLIGHT_DIRECTIONAL;

	// 拡散光
	Light[1].Diffuse = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

	// 環境光
	Light[1].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(-0.20f, 1.00f, -0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light[1].Direction, &vecDir);

	// ライトをレンダリングパイプラインに設定
	Device->SetLight(1, &Light[1]);

	// ライトの設定
	Device->LightEnable(1, TRUE);

	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&Light[2], sizeof(D3DLIGHT9));

	// ライトのタイプの設定
	Light[2].Type = D3DLIGHT_DIRECTIONAL;

	// 拡散光
	Light[2].Diffuse = D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f);

	// 環境光
	Light[2].Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(0.80f, 0.10f, 0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light[2].Direction, &vecDir);

	// ライトをレンダリングパイプラインに設定
	Device->SetLight(2, &Light[2]);

	// ライトの設定
	Device->LightEnable(2, TRUE);


	// ライティングモード
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	return S_OK;
}

//=============================================================================
// ライトの終了処理
//=============================================================================
void UninitLigth(void)
{

}
//=============================================================================
// ライトの更新処理
//=============================================================================
void UpdateLigth(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

}
