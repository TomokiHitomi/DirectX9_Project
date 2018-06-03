//=============================================================================
//
// エフェクト処理 [effect.cpp]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#include "camera.h"
#include "effect.h"
#include "input.h"
#include "main.h"
#include "calculate.h"

// デバッグ用
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice);
void InitStatusEffect(LPDIRECT3DDEVICE9 pDevice, int nEffect);
void SetVtxEffect(int nWk, float fSizeX, float fSizeY);
void SetDiffuseEffect(int nWk, D3DXCOLOR col);
void SetTexEffect(int nWk, int nTex);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureEffect = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffEffect = NULL;	// 頂点バッファへのポインタ

EFFECT					effectWk[EFFECT_MAX];

D3DXMATRIX				g_mtxWorldEffect;

// シェーダー関連
ID3DXBuffer *errorEffect = 0;
ID3DXEffect *effectshader;
UINT numPassEffect;
HRESULT hrEffect;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEffect(int nType)
{
	EFFECT *effect = &effectWk[0];
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	const char* path = SHADER_EFFECT;
	// ファイル( const char* path )からシェーダーファイルを読み込み読み込み
	hrEffect = D3DXCreateEffectFromFile(pDevice, path, 0, 0, 0, 0, &effectshader, &errorEffect);
	if (FAILED(hrEffect))
	{
		// エラー
		return S_FALSE;
	}
	// 使用するテクニックを定義
	hrEffect = effectshader->SetTechnique("Tec01");
	if (FAILED(hrEffect)) {

		// エラー
		return S_FALSE;
	}


	if (nType == 0)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			TEXTURE_EFFECT,
			&g_pD3DTextureEffect);
	}

	for (int i = 0; i < EFFECT_MAX; i++, effect++)
	{
		// 位置・回転・スケールの設定
		effect->posEffect = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		effect->rotEffect = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		effect->sclEffect = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		InitStatusEffect(pDevice, i);

	}

	// 頂点情報の作成（ローカル座標の設定）
	MakeVertexEffect(pDevice);
	return S_OK;
}

//=============================================================================
// 再利用処理
//=============================================================================
void InitStatusEffect(LPDIRECT3DDEVICE9 pDevice, int nEffect)
{
	EFFECT *effect = &effectWk[nEffect];

	effect->bUse = false;
	effect->vec2Size = D3DXVECTOR2(EFFECT_SIZE_X, EFFECT_SIZE_Y);
	effect->colorEffect = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	effect->nTex = 0;
	effect->fSizeChange = 0.0f;
	effect->fAlphaChange = 0.0f;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect(void)
{
	// シェーダーを解放
	effectshader->Release();

	if (g_pD3DTextureEffect != NULL)
	{// テクスチャの開放
		g_pD3DTextureEffect->Release();
		g_pD3DTextureEffect = NULL;
	}

	if (g_pD3DVtxBuffEffect != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffEffect->Release();
		g_pD3DVtxBuffEffect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EFFECT *effect = &effectWk[0];
	CAMERA *camera = GetCamera(0);

	// エフェクト設置
	if (GetKeyboardPress(DIK_SPACE))
	{
		SetEffect(0,
			D3DXVECTOR2(25, 25),
			SetColorPallet(COLOR_PALLET_MAGENTA),
			camera->posCameraAt,
			0.0001f,
			0.0001f);
	}

#ifdef _DEBUG
	PrintDebugProc("【 EFFECT 】\n");
	int nEffectCount = 0;
#endif

	for (int i = 0; i < EFFECT_MAX; i++, effect++)
	{
		if (effect->bUse)
		{
#ifdef _DEBUG
			// 使用エフェクト数をカウント
			nEffectCount++;
#endif
			effect->vec2Size.x -= effect->fSizeChange;
			effect->vec2Size.y -= effect->fSizeChange;
			effect->colorEffect.a -= effect->fAlphaChange;

			SetVtxEffect(i, effect->vec2Size.x, effect->vec2Size.y);
			SetDiffuseEffect(i, effect->colorEffect);
			SetTexEffect(i, effect->nTex);

			if (effect->vec2Size.x < 0 || effect->colorEffect.a < 0)
			{
				InitStatusEffect(pDevice, i);
			}
		}
	}

#ifdef _DEBUG
	PrintDebugProc("EffectMax:%d\n", nEffectCount);
	PrintDebugProc("\n");
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// スケール, 回転, 平行移動

	EFFECT *effect = &effectWk[0];
	int *pCameraMode = GetCameraMode();
	CAMERA *camera = GetCamera(*pCameraMode);

	D3DXMATRIX mtxView = GetMtxView();
	D3DXMATRIX mtxProjection = GetMtxProjection();

	//// αテストを有効に
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, EFFECT_ALPHA);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// 加算合成に設定
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);			// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	////pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);			// Z比較なし

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// テクスチャをセット
	effectshader->SetTexture("tex", g_pD3DTextureEffect);
	// シェーダーの開始、numPassには指定してあるテクニックに定義してあるpassの数が変える
	effectshader->Begin(&numPassEffect, 0);

	for (int i = 0; i < EFFECT_MAX; i++, effect++)
	{
		if (effect->bUse)
		{
			/******************** ワールド変換 ********************/
			// ワールドマトリクスの初期化
			D3DXMatrixIdentity(&g_mtxWorldEffect);

			// ビルボード化
			g_mtxWorldEffect._11 = mtxView._11;
			g_mtxWorldEffect._12 = mtxView._21;
			g_mtxWorldEffect._13 = mtxView._31;
			g_mtxWorldEffect._21 = mtxView._12;
			g_mtxWorldEffect._22 = mtxView._22;
			g_mtxWorldEffect._23 = mtxView._32;
			g_mtxWorldEffect._31 = mtxView._13;
			g_mtxWorldEffect._32 = mtxView._23;
			g_mtxWorldEffect._33 = mtxView._33;


			// 【S】スケールを反映(Multiplyは行列計算)
			D3DXMatrixScaling(&mtxScl, effect->sclEffect.x, effect->sclEffect.y, effect->sclEffect.z);
			D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxScl);

			//// 【R】回転を反映(YawPitchRollはy,x,z)
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, effect->rotEffect.y, effect->rotEffect.x, effect->rotEffect.z);
			//D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &GetInvRotateMat(effect->posEffect));
			////D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxRot);

			// 【T】平行移動を反映(オブジェクトを配置している）
			D3DXMatrixTranslation(&mtxTranslate, effect->posEffect.x, effect->posEffect.y, effect->posEffect.z);
			D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxTranslate);

			//// ワールドマトリクスの設定
			//pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEffect);

			/******************** ビューポート変換 ********************/
			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pD3DVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// 必要なグローバル情報を与える
			effectshader->SetMatrix("proj", &mtxProjection);
			effectshader->SetMatrix("view", &mtxView);
			effectshader->SetMatrix("world", &g_mtxWorldEffect);

			//// テクスチャの設定
			//pDevice->SetTexture(0, g_pD3DTextureEffect);


			// パスを指定して開始
			effectshader->BeginPass(0);


			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * 4), NUM_POLYGON);

			// シェーダ終了
			effectshader->EndPass();

		}
	}
	effectshader->End();

	// 通常ブレンドに戻す
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//// αテストを無効に
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	// ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * EFFECT_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffEffect,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		for (int i = 0; i < EFFECT_MAX; i++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-EFFECT_SIZE_X, -EFFECT_SIZE_Y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(EFFECT_SIZE_X, -EFFECT_SIZE_Y, 0.0f);

			// 法線ベクトルの設定
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
			int x = 0 % TEXTURE_PATTERN_DIVIDE_X_EFFECT;
			int y = 0 / TEXTURE_PATTERN_DIVIDE_X_EFFECT;
			float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_EFFECT;
			float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_EFFECT;
			pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
			pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
			pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
			pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
		}
		// 頂点データをアンロックする
		g_pD3DVtxBuffEffect->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVtxEffect(int nWk, float fSizeX, float fSizeY)
{
	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nWk * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fSizeX, -fSizeY, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fSizeX, fSizeY, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fSizeX, -fSizeY, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fSizeX, fSizeY, 0.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffEffect->Unlock();
	}
}

//=============================================================================
// 反射光の設定関数
//=============================================================================
void SetDiffuseEffect(int nWk, D3DXCOLOR col)
{
	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nWk * 4);

		// 頂点座標の設定
		pVtx[0].diffuse =
			pVtx[1].diffuse =
			pVtx[2].diffuse =
			pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		g_pD3DVtxBuffEffect->Unlock();
	}
}

//=============================================================================
// テクスチャ座標の設定関数
//=============================================================================
void SetTexEffect(int nWk, int nTex)
{
	{// 頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nWk * 4);

		// テクスチャ座標の設定
		int x = nTex % TEXTURE_PATTERN_DIVIDE_X_EFFECT;
		int y = nTex / TEXTURE_PATTERN_DIVIDE_X_EFFECT;
		float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_EFFECT;
		float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_EFFECT;
		pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

		// 頂点データをアンロックする
		g_pD3DVtxBuffEffect->Unlock();
	}
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
//void SetTextureEffect(int no, int cntPattern)
//{
//	EFFECT *effect = &effectWk[no];
//
//	// テクスチャ座標の設定
//	int x = cntPattern % TEXTURE_PATTERN_DIVIDE_X_EFFECT;
//	int y = cntPattern / TEXTURE_PATTERN_DIVIDE_X_EFFECT;
//	float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_EFFECT;
//	float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_EFFECT;
//	effect->vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
//	effect->vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
//	effect->vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
//	effect->vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
//}

//=============================================================================
// 対象に設置
//=============================================================================
void SetEffect(int nTex, D3DXVECTOR2 vec2Size, D3DXCOLOR color, D3DXVECTOR3 vecPos, float fSizeChange, float fAlphaChange)
{
	EFFECT *effect = &effectWk[0];

	// 未使用を探す
	for (int i = 0; i < EFFECT_MAX; i++, effect++)
	{
		if (!effect->bUse)
		{
			effect->bUse = true;
			effect->posEffect = vecPos;
			effect->colorEffect = color;
			effect->colorEffect.a = 1.0f;
			effect->vec2Size = vec2Size;
			effect->fSizeChange = fSizeChange;
			effect->fAlphaChange = fAlphaChange;
			effect->nTex = nTex;

			SetVtxEffect(i, effect->vec2Size.x, effect->vec2Size.y);
			SetDiffuseEffect(i, effect->colorEffect);
			SetTexEffect(i, effect->nTex);
			return;
		}
	}
}