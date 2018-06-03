////=============================================================================
////
//// シェーダ処理 [shader.cpp]
//// Author : GP12A295 25 人見友基
////
////=============================================================================
//#include "shader.h"
//#include "camera.h"
//
///* Debug */
//#ifdef _DEBUG
//#include "debugproc.h"
//#endif
//
////*****************************************************************************
//// マクロ定義
////*****************************************************************************
//
////*****************************************************************************
//// プロトタイプ宣言
////*****************************************************************************
//
////*****************************************************************************
//// グローバル変数
////*****************************************************************************
//
////=============================================================================
//// 更新処理
////=============================================================================
//void Shader::Update(void)
//{
//
//}
//
////=============================================================================
//// 描画処理
////=============================================================================
//void Shader::Draw(LPDIRECT3DTEXTURE9 pD3DTexture)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;	// スケール, 回転, 平行移動
//
//	CAMERA *camera = GetCamera(0);
//	D3DXMATRIX mtxView = GetMtxView();
//	D3DXMATRIX mtxProjection = GetMtxProjection();
//
//	// テクスチャをセット
//	pEffect->SetTexture("tex", pD3DTexture);
//	// シェーダーの開始、numPassには指定してあるテクニックに定義してあるpassの数が変える
//	pEffect->Begin(&numPassEffect, 0);
//
//	for (int i = 0; i < EFFECT_MAX; i++, effect++)
//	{
//		if (effect->bUse)
//		{
//			/******************** ワールド変換 ********************/
//			// ワールドマトリクスの初期化
//			D3DXMatrixIdentity(&g_mtxWorldEffect);
//
//			// ビルボード化
//			g_mtxWorldEffect._11 = mtxView._11;
//			g_mtxWorldEffect._12 = mtxView._21;
//			g_mtxWorldEffect._13 = mtxView._31;
//			g_mtxWorldEffect._21 = mtxView._12;
//			g_mtxWorldEffect._22 = mtxView._22;
//			g_mtxWorldEffect._23 = mtxView._32;
//			g_mtxWorldEffect._31 = mtxView._13;
//			g_mtxWorldEffect._32 = mtxView._23;
//			g_mtxWorldEffect._33 = mtxView._33;
//
//
//			// 【S】スケールを反映(Multiplyは行列計算)
//			D3DXMatrixScaling(&mtxScl, effect->sclEffect.x, effect->sclEffect.y, effect->sclEffect.z);
//			D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxScl);
//
//			//// 【R】回転を反映(YawPitchRollはy,x,z)
//			//D3DXMatrixRotationYawPitchRoll(&mtxRot, effect->rotEffect.y, effect->rotEffect.x, effect->rotEffect.z);
//			//D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &GetInvRotateMat(effect->posEffect));
//			////D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxRot);
//
//			// 【T】平行移動を反映(オブジェクトを配置している）
//			D3DXMatrixTranslation(&mtxTranslate, effect->posEffect.x, effect->posEffect.y, effect->posEffect.z);
//			D3DXMatrixMultiply(&g_mtxWorldEffect, &g_mtxWorldEffect, &mtxTranslate);
//
//			//// ワールドマトリクスの設定
//			//pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldEffect);
//
//			/******************** ビューポート変換 ********************/
//			// 頂点バッファをデバイスのデータストリームにバインド
//			pDevice->SetStreamSource(0, g_pD3DVtxBuffEffect, 0, sizeof(VERTEX_3D));
//
//			// 頂点フォーマットの設定
//			pDevice->SetFVF(FVF_VERTEX_3D);
//
//			// 必要なグローバル情報を与える
//			pEffect->SetMatrix("proj", &mtxProjection);
//			pEffect->SetMatrix("view", &mtxView);
//			pEffect->SetMatrix("world", &g_mtxWorldEffect);
//
//			//// テクスチャの設定
//			//pDevice->SetTexture(0, g_pD3DTextureEffect);
//
//
//			// パスを指定して開始
//			pEffect->BeginPass(0);
//
//
//			// ポリゴンの描画
//			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (i * 4), NUM_POLYGON);
//
//			// シェーダ終了
//			pEffect->EndPass();
//
//		}
//	}
//	pEffect->End();
//
//}
//
////=============================================================================
//// コンストラクタ処理
////=============================================================================
//Shader::Shader(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	const char* path = SHADER_SPRITE;
//	// ファイル( const char* path )からシェーダーファイルを読み込み読み込み
//	hrEffect = D3DXCreateEffectFromFile(pDevice, path, 0, 0, 0, 0, &pEffect, &errorEffect);
//	if (FAILED(hrEffect))
//	{
//		// エラー
//		//return S_FALSE;
//		return;
//	}
//	// 使用するテクニックを定義
//	hrEffect = pEffect->SetTechnique("Tec01");
//	if (FAILED(hrEffect)) {
//
//		// エラー
//		//return S_FALSE;
//		return;
//	}
//}
//
////=============================================================================
//// デストラクタ処理
////=============================================================================
//Shader::~Shader(void)
//{
//	if (pEffect != NULL)
//	{	// シェーダーを解放
//		pEffect->Release();
//	}
//}