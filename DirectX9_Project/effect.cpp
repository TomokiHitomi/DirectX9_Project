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
void SetInstEffect(int nWk, D3DXVECTOR3 pos, D3DXCOLOR color);
void SetVtxEffect(int nWk, float fSizeX, float fSizeY);
void SetDiffuseEffect(int nWk, D3DXCOLOR col);
void SetTexEffect(int nWk, int nTex);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9				g_pD3DTextureEffect = NULL;	// テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9			g_pD3DVtxBuffEffect = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9			g_pD3DInstBuffEffect = NULL;// インスタンシングバッファへのポインタ
LPDIRECT3DINDEXBUFFER9			g_pD3DIdxBuffEffect = NULL;

LPDIRECT3DVERTEXDECLARATION9	g_pD3DDeclEffect = NULL;

EFFECT					effectWk[EFFECT_MAX];
//VERTEX_PLANE			vtxPlane[NUM_VERTEX];

D3DXMATRIX				g_mtxWorldEffect;

// シェーダー関連
LPD3DXBUFFER errorEffect = NULL;
LPD3DXEFFECT effectshader = NULL;
UINT numPassEffect;
HRESULT hrEffect;

int g_nEffectColor = 0;
int g_nEffectCount = 0;
float g_fEffectMove = 0.0f;

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
	effect->fSize = 1.0f;
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

	if (g_pD3DInstBuffEffect != NULL)
	{// インスタンシングバッファの開放
		g_pD3DInstBuffEffect->Release();
		g_pD3DInstBuffEffect = NULL;
	}

	if (g_pD3DIdxBuffEffect != NULL)
	{// インデックスバッファの開放
		g_pD3DIdxBuffEffect->Release();
		g_pD3DIdxBuffEffect = NULL;
	}

	if (g_pD3DDeclEffect != NULL)
	{// 頂点宣言の開放
		g_pD3DDeclEffect->Release();
		g_pD3DDeclEffect = NULL;
	}

	if (errorEffect != NULL)
	{// シェーダデータバッファの開放
		errorEffect->Release();
		errorEffect = NULL;
	}

	if (effectshader != NULL)
	{// シェーダの開放
		effectshader->Release();
		effectshader = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEffect(void)
{
#ifdef _DEBUG
	PrintDebugProc("【 EFFECT 】\n");
	int nEffectCount = 0;
	DWORD  start = GetTickCount();
	static double s_dClockTime;
#endif
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EFFECT *effect = &effectWk[0];
	CAMERA *camera = GetCamera(0);

	g_fEffectMove += EFFECT_MOVE_SPEED;

	long ModUseZ = GetMobUseZ();
	if (ModUseZ != 0)
	{
		if (ModUseZ > 0)
		{
			g_nEffectColor++;
			if (g_nEffectColor >= COLOR_PALLET_MAX)
			{
				g_nEffectColor = 0;
			}
		}
		else
		{
			g_nEffectColor--;
			if (g_nEffectColor < 0)
			{
				g_nEffectColor = COLOR_PALLET_MAX - 1;
			}
		}
	}	



	// エフェクト設置
	if (IsMobUseLeftPressed())
	{
		for (unsigned int i = 0; i < EFFECT_SET; i++)
		{
			SetEffect(0,
				D3DXVECTOR2(10, 10), SetColorPallet(g_nEffectColor),
				camera->posCameraAt + D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				0.0001f, 0.0001f);
		}
	}

	for (int i = 0; i < EFFECT_MAX; i++, effect++)
	{
		if (effect->bUse)
		{
#ifdef _DEBUG
			// 使用エフェクト数をカウント
			nEffectCount++;
#endif
			//effect->fSize += 0.5f;
			//if (effect->fSize > 100.0f)
			//{
			//	InitStatusEffect(pDevice, i);
			//}
			//effect->vec2Size.x -= effect->fSizeChange;
			//effect->vec2Size.y -= effect->fSizeChange;
			//effect->colorEffect.a -= effect->fAlphaChange;

			//SetVtxEffect(i, effect->vec2Size.x, effect->vec2Size.y);
			//SetDiffuseEffect(i, effect->colorEffect);
			//SetTexEffect(i, effect->nTex);

			//if (effect->vec2Size.x < 0 || effect->colorEffect.a < 0)
			//{
			//	InitStatusEffect(pDevice, i);
			//}
		}
	}

#ifdef _DEBUG
	PrintDebugProc("EffectMax:%d\n", nEffectCount);
	DWORD end = GetTickCount();
	double dClockTemp = (double)(end - start) / 1000;

	if (dClockTemp > s_dClockTime)
	{
		s_dClockTime = dClockTemp;
	}
	PrintDebugProc("clockTime[%f]\n", s_dClockTime);
	PrintDebugProc("\n");

#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect(void)
{
#ifdef _DEBUG
	PrintDebugProc("【 EFFECT DRAW 】\n");
	DWORD  start = GetTickCount();
	static double s_dClockTime;
#endif
	if (g_nEffectCount > 0)
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
		//////pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);			// Z比較なし

		//// ラインティングを無効にする
		//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

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

		// インスタンス宣言
		pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | g_nEffectCount);
		pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);

		// 頂点とインデックスを設定
		pDevice->SetVertexDeclaration(g_pD3DDeclEffect);
		pDevice->SetStreamSource(0, g_pD3DVtxBuffEffect, 0, sizeof(VERTEX_PLANE));	// 頂点バッファ
		pDevice->SetStreamSource(1, g_pD3DInstBuffEffect, 0, sizeof(INSTANCING_PLANE));	// 座標バッファ
		pDevice->SetIndices(g_pD3DIdxBuffEffect);									// インデックスバッファ

		// テクニックを設定
		effectshader->SetTechnique("Tec01");

		// シェーダーの開始、passNumには指定してあるテクニックに定義してあるpassの数が変える
		UINT passNum = 0;
		effectshader->Begin(&passNum, 0);

		// パスを指定して開始
		effectshader->BeginPass(0);

		// テクスチャをセット
		effectshader->SetTexture("tex", g_pD3DTextureEffect);

		// 必要な行列情報をセット
		effectshader->SetMatrix("proj", &mtxProjection);
		effectshader->SetMatrix("view", &mtxView);
		effectshader->SetMatrix("world", &g_mtxWorldEffect);

		// 時間カウンターをセット
		effectshader->SetFloat("moveTime", g_fEffectMove);

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

		// シェーダーパスの終了
		effectshader->EndPass();
		// シェーダー終了
		effectshader->End();

		// インスタンス宣言を標準に戻す
		pDevice->SetStreamSourceFreq(0, 1);
		pDevice->SetStreamSourceFreq(1, 1);

		// 通常ブレンドに戻す
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		////// αテストを無効に
		////pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


		//// ラインティングを有効にする
		//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
#ifdef _DEBUG
	DWORD end = GetTickCount();
	double dClockTemp = (double)(end - start) / 1000;
	
	if (dClockTemp > s_dClockTime)
	{
		s_dClockTime = dClockTemp;
	}
	PrintDebugProc("clockTime[%f]\n", s_dClockTime);
	PrintDebugProc("\n");
#endif
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	/***** 頂点バッファ設定 *****/

	// 頂点バッファ生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_PLANE) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		0,									// 頂点バッファの使用法　
		0,									// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DVtxBuffEffect,				// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_PLANE *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR4(-EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f, 1.0f);
		pVtx[1].vtx = D3DXVECTOR4(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f, 1.0f);
		pVtx[2].vtx = D3DXVECTOR4(-EFFECT_SIZE_X, -EFFECT_SIZE_Y, 0.0f, 1.0f);
		pVtx[3].vtx = D3DXVECTOR4(EFFECT_SIZE_X, -EFFECT_SIZE_Y, 0.0f, 1.0f);

		// テクスチャ座標の設定
		int x = 0 % TEXTURE_PATTERN_DIVIDE_X_EFFECT;
		int y = 0 / TEXTURE_PATTERN_DIVIDE_X_EFFECT;
		float sizeX = 1.0f / TEXTURE_PATTERN_DIVIDE_X_EFFECT;
		float sizeY = 1.0f / TEXTURE_PATTERN_DIVIDE_Y_EFFECT;

		pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

		// 頂点データをアンロックする
		g_pD3DVtxBuffEffect->Unlock();
	}


	/***** インスタンシング用の座標バッファ設定 *****/

	// 座標バッファ生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(INSTANCING_PLANE) * EFFECT_MAX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		0,									// 頂点バッファの使用法　
		0,									// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DInstBuffEffect,				// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}

	{// インスタンシングバッファの中身を埋める
		INSTANCING_PLANE *pInst;
		

		// 座標データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DInstBuffEffect->Lock(0, 0, (void**)&pInst, 0);

		// 座標の設定
		for (unsigned int i = 0; i < EFFECT_MAX; i++, pInst++)
		{
			pInst->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			//pInst->diffuse = SetColorPalletRandom();
			pInst->diffuse = SetColorPallet(COLOR_PALLET_LIGHTBLUE);
			pInst->vec = RandVector();
			pInst->move = 0.0f;
		}

		// 座標データをアンロックする
		g_pD3DInstBuffEffect->Unlock();
	}

	/***** インデックスバッファ設定 *****/

	WORD wIndex[6] = { 0, 1, 2, 2, 1, 3 };

	// 座標バッファ生成
	if (FAILED(pDevice->CreateIndexBuffer(
		sizeof(wIndex),						// 頂点データ用に確保するバッファサイズ(バイト単位)
		0,									// 頂点バッファの使用法　
		D3DFMT_INDEX16,						// 使用する頂点フォーマット
		D3DPOOL_MANAGED,					// リソースのバッファを保持するメモリクラスを指定
		&g_pD3DIdxBuffEffect,				// 頂点バッファインターフェースへのポインタ
		NULL)))								// NULLに設定
	{
		return E_FAIL;
	}

	{// 座標バッファの中身を埋める
		WORD *pIdx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DIdxBuffEffect->Lock(0, 0, (void**)&pIdx, 0);

		// 頂点座標の設定
		for (unsigned int i = 0; i < 6; i++, pIdx++)
		{
			*pIdx = wIndex[i];
		}

		// 頂点データをアンロックする
		g_pD3DIdxBuffEffect->Unlock();
	}

	D3DVERTEXELEMENT9 declElems[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },		// VTX
		{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },		// UV
		{ 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },		// ワールド位置
		{ 1, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },		// 頂点カラー
		{ 1, 16, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },		// 移動ベクトル
		{ 1, 28, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },	// カウンター
		D3DDECL_END()
	};

	pDevice->CreateVertexDeclaration(declElems, &g_pD3DDeclEffect);

	return S_OK;
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetInstEffect(int nWk, D3DXVECTOR3 pos, D3DXCOLOR color)
{
	{// 座標バッファの中身を埋める
		INSTANCING_PLANE *pInst;

		// 座標データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DInstBuffEffect->Lock(0, 0, (void**)&pInst, 0);

		pInst += nWk;

		// 座標の設定
		pInst->pos = pos;
		//pInst->diffuse = color;
		pInst->diffuse = SetColorPalletRandom();
		//pInst->vec = RandVector();
		pInst->move = g_fEffectMove;

		// 座標データをアンロックする
		g_pD3DInstBuffEffect->Unlock();
	}
}

//=============================================================================
// 頂点座標の設定関数
//=============================================================================
void SetVtxEffect(int nWk, float fSizeX, float fSizeY)
{
	{// 頂点バッファの中身を埋める
		VERTEX_NOLIGHT *pVtx;

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
		VERTEX_NOLIGHT *pVtx;

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
		VERTEX_NOLIGHT *pVtx;

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
			//effect->posEffect = vecPos;
			//effect->colorEffect = color;
			//effect->colorEffect.a = 1.0f;
			//effect->vec2Size = vec2Size;
			//effect->fSizeChange = fSizeChange;
			//effect->fAlphaChange = fAlphaChange;
			//effect->nTex = nTex;
			g_nEffectCount++;

			SetInstEffect(i, vecPos, color);

			//SetVtxEffect(i, effect->vec2Size.x, effect->vec2Size.y);
			//SetDiffuseEffect(i, effect->colorEffect);
			//SetTexEffect(i, effect->nTex);
			return;
		}
	}
}