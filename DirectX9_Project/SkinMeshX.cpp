//=============================================================================
//
// モデル処理 [SkinMeshX.cpp]
// Author : GP12A295 25 人見友基
//
// 参考：GESブログ
// http://ges.blog.shinobi.jp/directx/directx%209%E3%80%80%E3%82%B9%E3%82%AD%E3%83%B3%E3%83%A1%E3%83%83%E3%82%B7%E3%83%A5%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "SkinMeshX.h"

// デバッグ用
#ifdef _DEBUG
#include "debugproc.h"
#endif

//=============================================================================
// フレーム格納関数
//=============================================================================
HRESULT MY_HIERARCHY::CreateFrame(LPCTSTR Name, LPD3DXFRAME *ppNewFrame)
{
	HRESULT hr = S_OK;
	MYFRAME *pFrame;
	//新しいフレームアドレス格納用変数を初期化
	*ppNewFrame = NULL;
	//フレームの領域確保
	pFrame = new MYFRAME;
	//領域確保の失敗時の処理
	if (pFrame == NULL)
	{
		return E_OUTOFMEMORY;
	}
	//フレーム名格納用領域確保
	pFrame->Name = new TCHAR[lstrlen(Name) + 1];
	//領域確保の失敗時の処理
	if (!pFrame->Name)
	{
		return E_FAIL;
	}
	// strcpy(pFrame->Name,Name);
	//フレーム名格納
	strcpy_s(pFrame->Name, lstrlen(Name) + 1, Name);
	//行列の初期化
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);
	//追加：オフセット関係初期化
	pFrame->OffsetID = 0xFFFFFFFF;
	D3DXMatrixIdentity(&(pFrame->OffsetMat));
	//新規フレームのメッシュコンテナ初期化
	pFrame->pMeshContainer = NULL;
	//新規フレームの兄弟フレームアドレス格納用変数初期化
	pFrame->pFrameSibling = NULL;
	//新規フレームの子フレームアドレス格納用変数初期化
	pFrame->pFrameFirstChild = NULL;
	//外部の新規フレームアドレス格納用変数に、作成したフレームのアドレスを格納
	*ppNewFrame = pFrame;
	return S_OK;
}

//=============================================================================
// メッシュコンテナー作成関数
//=============================================================================
//HRESULT MY_HIERARCHY::CreateMeshContainer
//メッシュコンテナーを作成する
HRESULT MY_HIERARCHY::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA* pMeshData,
	CONST D3DXMATERIAL* pMaterials, CONST D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppMeshContainer)
{
	HRESULT hr;
	//ローカル生成用
	MYMESHCONTAINER *pMeshContainer = NULL;
	//メッシュの面の数を格納
	int iFacesAmount;
	//forループで使用
	int iMaterial;
	//一時的なDirectXデバイス取得用
	LPDIRECT3DDEVICE9 pDevice = NULL;
	//一時的なメッシュデータ格納用
	LPD3DXMESH pMesh = NULL;
	//メッシュコンテナ格納用変数初期化
	*ppMeshContainer = NULL;
	//ボーンの数格納用変数初期化
	DWORD dwBoneNum = 0;
	//pMeshに"外部引数の"メッシュアドレスを格納
	pMesh = pMeshData->pMesh;
	//メッシュコンテナ領域の動的確保
	pMeshContainer = new MYMESHCONTAINER;
	//領域確保失敗時
	if (pMeshContainer == NULL)
	{
		return E_OUTOFMEMORY;
	}
	//メッシュコンテナを初期化
	ZeroMemory(pMeshContainer, sizeof(MYMESHCONTAINER));
	//メッシュコンテナの名前格納用領域を動的確保
	pMeshContainer->Name = new TCHAR[lstrlen(Name) + 1];
	//失敗時の処理
	if (!pMeshContainer->Name)
	{
		return E_FAIL;
	}
	//確保した領域にメッシュコンテナ名を格納
	strcpy_s(pMeshContainer->Name, lstrlen(Name) + 1, Name);
	//DirectXデバイス取得
	pMesh->GetDevice(&pDevice);
	//メッシュの面の数を取得
	iFacesAmount = pMesh->GetNumFaces();


	// メッシュに法線がないので法線を追加
	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	// 柔軟な頂点フォーマット (FVF) コードを使ってメッシュのコピーを作成する
	hr = pMesh->CloneMeshFVF(
		pMesh->GetOptions(),
		pMesh->GetFVF() | D3DFVF_NORMAL,
		pDevice,
		&pMeshContainer->MeshData.pMesh); // ←ここにコピー
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	pMesh = pMeshContainer->MeshData.pMesh;
	// メッシュに含まれる各頂点の法線を計算して、設定する
	D3DXComputeNormals(pMesh, NULL);


	//// 法線の確認
	//if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
	//{
	//	// メッシュに法線がないので法線を追加
	//	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	//	// 柔軟な頂点フォーマット (FVF) コードを使ってメッシュのコピーを作成する
	//	hr = pMesh->CloneMeshFVF(
	//		pMesh->GetOptions(),
	//		pMesh->GetFVF() | D3DFVF_NORMAL,
	//		pDevice,
	//		&pMeshContainer->MeshData.pMesh); // ←ここにコピー
	//	if (FAILED(hr))
	//	{
	//		return E_FAIL;
	//	}
	//	pMesh = pMeshContainer->MeshData.pMesh;
	//	// メッシュに含まれる各頂点の法線を計算して、設定する
	//	D3DXComputeNormals(pMesh, NULL);
	//}
	//else
	//{
	//	pMeshContainer->MeshData.pMesh = pMesh;
	//	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	//	// 参照カウンタをインクリメント
	//	pMesh->AddRef();
	//}

	//- メッシュのマテリアル設定 -//
	//メッシュのマテリアル数を格納(最大で1つ)
	pMeshContainer->NumMaterials = max(1, NumMaterials);
	//メッシュコンテナの、マテリアルデータ格納領域を動的確保
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	//メッシュコンテナの、テクスチャデータ格納領域を動的確保
	pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	//メッシュコンテナの、面ごとに持つ3つの隣接性情報が格納されたDWORD型のアドレス格納用(ポインタ)変数
	pMeshContainer->pAdjacency = new DWORD[iFacesAmount * 3];
	//領域確保の失敗時の処理
	if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
	{
		return E_FAIL;
	}
	//外部引数の隣接性情報をメッシュコンテナに格納
	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * iFacesAmount * 3);
	//テクスチャデータ格納用領域を初期化(memsetを使用して0で中身を埋める)
	memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);
	//引数のマテリアル数が0じゃない場合
	if (NumMaterials > 0)
	{
		//外部引数のマテリアルデータアドレスをメッシュコンテナに格納
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);
		//マテリアル数分ループさせる
		for (iMaterial = 0; (DWORD)iMaterial < NumMaterials; iMaterial++)
		{
			//テクスチャのファイル名がNULLでなければ(テクスチャデータがあれば)
			if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
			{
				//////マテリアルカラーを0.5に設定
				//pMeshContainer->pMaterials[iMaterial].MatD3D.Diffuse.r = 1.0f;
				//pMeshContainer->pMaterials[iMaterial].MatD3D.Diffuse.g = 1.0f;
				//pMeshContainer->pMaterials[iMaterial].MatD3D.Diffuse.b = 1.0f;
				//pMeshContainer->pMaterials[iMaterial].MatD3D.Diffuse.a = 1.0f;

				//pMeshContainer->pMaterials[iMaterial].MatD3D.Emissive.r = 0.5f;
				//pMeshContainer->pMaterials[iMaterial].MatD3D.Emissive.g = 0.5f;
				//pMeshContainer->pMaterials[iMaterial].MatD3D.Emissive.b = 0.5f;
				//pMeshContainer->pMaterials[iMaterial].MatD3D.Emissive.a = 0.5f;


				////スペキュラも0.5に設定(上で設定したマテリアルカラーの0.5の設定をコピー)
				////pMeshContainer->pMaterials[iMaterial].MatD3D.Specular = pMeshContainer->pMaterials[iMaterial].MatD3D.Diffuse;
				////pMeshContainer->pMaterials[iMaterial].MatD3D.Emissive = pMeshContainer->pMaterials[iMaterial].MatD3D.Diffuse;
				//pMeshContainer->pMaterials[iMaterial].MatD3D.Ambient = pMeshContainer->pMaterials[iMaterial].MatD3D.Diffuse;

				//テクスチャのファイルパス保存用変数
				TCHAR strTexturePath[MAX_PATH];
				//テクスチャのファイルパスを保存(再読み込み時に必要)
				strcpy_s(strTexturePath, lstrlen(pMeshContainer->pMaterials[iMaterial].pTextureFilename) + 1, pMeshContainer->pMaterials[iMaterial].pTextureFilename);
				//テクスチャ情報の読み込み
				if (FAILED(D3DXCreateTextureFromFile(pDevice, strTexturePath,
					&pMeshContainer->ppTextures[iMaterial])))
				{
					//失敗時の処理
					//テクスチャファイル名格納用
					CHAR TexMeshPass[255];
					//追記
					//もしなければ、Graphフォルダを調べる
					//注）ファイル名の結合時に、必ず両方にファイル名がある事を確認してから
					//  strcpy_sとstrcat_sを使うようにする(この場合は、上にある 
					//    テクスチャのファイルがあり、さらにそのファイル名の長さが0でなければ の所のif文)。
					//  TexMeshPassに、Xファイルがある場所と同じディレクトリと、テクスチャのファイル名を
					//  結合したものを格納
					// strcpy_s( TexMeshPass, sizeof( TexMeshPass ) , "./../Source/Graph/" );
					strcpy_s(TexMeshPass, sizeof(TexMeshPass), "./Graph/");
					strcat_s(TexMeshPass, sizeof(TexMeshPass) - strlen(TexMeshPass) - strlen(strTexturePath) - 1, strTexturePath);
					//テクスチャ情報の読み込み
					if (FAILED(D3DXCreateTextureFromFile(pDevice, TexMeshPass,
						&pMeshContainer->ppTextures[iMaterial])))
					{
						pMeshContainer->ppTextures[iMaterial] = NULL;
					}
					//テクスチャのファイルパスをNULLにする
					pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
				}
			}
		}
	}
	else
	{
		//- マテリアルなしの場合 -//
		//テクスチャファイル名をNULLに
		pMeshContainer->pMaterials[0].pTextureFilename = NULL;
		//マテリアルデータ初期化(memsetを使用して中身を0で埋める)
		memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		//マテリアルカラーを0.5に設定
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		//スペキュラも0.5に設定(上で設定したマテリアルカラーの0.5の設定をコピー)
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
		//pMeshContainer->pMaterials[0].MatD3D.Emissive = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
		//pMeshContainer->pMaterials[0].MatD3D.Ambient = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}
	//メッシュ情報を格納(今回は通常メッシュと完全に分けているためすべてスキンメッシュ情報となる)
	pMeshContainer->pSkinInfo = pSkinInfo;
	//参照カウンタ
	pSkinInfo->AddRef();
	//ボーンの数を取得
	dwBoneNum = pSkinInfo->GetNumBones();
	//フレーム(ボーン)単位でのワールド行列格納用領域の動的確保
	pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[dwBoneNum];
	//ボーンの数だけループさせる
	for (DWORD i = 0; i < dwBoneNum; i++)
	{
		//角フレーム(ボーン)のオフセット行列を取得して格納
		memcpy(&pMeshContainer->pBoneOffsetMatrices[i],
			pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i), sizeof(D3DMATRIX));
	}
	//- 変換作業 -//
	//メッシュコンテナにオリジナルのpMesh情報を格納
	D3DVERTEXELEMENT9 Decl[MAX_FVF_DECL_SIZE];
	pMesh->GetDeclaration(&Decl[0]);
	pMesh->CloneMesh(pMesh->GetOptions(), &Decl[0], pDevice, &pMeshContainer->pOriMesh);
	//メッシュのタイプを定義
	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
	//- 固定パイプライン描画用に変換 -//
	//シェーダで描画する場合は別途変換が必要
	//頂点単位でのブレンドの重みとボーンの組み合わせテーブルを適応した新しいメッシュを返す。
	if (FAILED(pMeshContainer->pSkinInfo->ConvertToBlendedMesh(
		pMeshContainer->pOriMesh, //元のメッシュデータアドレス
		NULL, //オプション(現在は使われていないためNULLでいい)
		pMeshContainer->pAdjacency, //元のメッシュの隣接性情報
		NULL, //出力メッシュの隣接性情報
		NULL, //各面の新しいインデックス値格納用変数のアドレス
		NULL, //角頂点の新しいインデックス値格納用変数のアドレス
		&pMeshContainer->dwWeight, //ボーンの影響の一面当たりの最大数格納用変数のアドレス
		&pMeshContainer->dwBoneNum, //ボーンの組み合わせテーブルに含まれるボーン数格納用変数のアドレス
		&pMeshContainer->pBoneBuffer, //ボーンの組み合わせテーブルへのポインタ
		&pMeshContainer->MeshData.pMesh //出力されるメッシュアドレス格納用変数のアドレス(固定パイプライン用)
	)))
	{
		return E_FAIL;
	}
	//ローカルに生成したメッシュコンテナーを呼び出し側にコピーする
	*ppMeshContainer = pMeshContainer;
	//参照カウンタを増やしたので減らす
	SAFE_RELEASE(pDevice);
	return S_OK;
}

//=============================================================================
// フレーム破棄関数
//=============================================================================
//HRESULT MY_HIERARCHY::DestroyFrame(LPD3DXFRAME pFrameToFree) 
//フレームを破棄する
HRESULT MY_HIERARCHY::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	//2銃解放防止
	// if (pFrameToFree == NULL)return S_FALSE;
	SAFE_DELETE_ARRAY(pFrameToFree->Name);
	if (pFrameToFree->pFrameFirstChild)
	{
		DestroyFrame(pFrameToFree->pFrameFirstChild);
	}
	if (pFrameToFree->pFrameSibling)
	{
		DestroyFrame(pFrameToFree->pFrameSibling);
	}
	SAFE_DELETE(pFrameToFree);
	return S_OK;
}

//=============================================================================
// メッシュコンテナー破棄関数
//=============================================================================
//HRESULT MY_HIERARCHY::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
//メッシュコンテナーを破棄する
HRESULT MY_HIERARCHY::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	int iMaterial;
	MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;
	SAFE_DELETE_ARRAY(pMeshContainer->Name);
	SAFE_RELEASE(pMeshContainer->pSkinInfo);
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);
	SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrix);
	if (pMeshContainer->ppTextures != NULL)
	{
		for (iMaterial = 0; (DWORD)iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			//テクスチャ解放
			SAFE_RELEASE(pMeshContainer->ppTextures[iMaterial]);
		}
	}
	SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);
	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	SAFE_RELEASE(pMeshContainer->pOriMesh);
	if (pMeshContainer->pBoneBuffer != NULL)
	{
		SAFE_RELEASE(pMeshContainer->pBoneBuffer);
		SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);
	}
	SAFE_DELETE(pMeshContainer);
	pMeshContainerBase = NULL;
	return S_OK;
}

//=============================================================================
// 初期化関数
//=============================================================================
CSkinMesh::CSkinMesh() {
	//マテリアル変更フラグ
	m_MaterialFlg = FALSE;
	//マテリアルデータ
	ZeroMemory(&m_Material, sizeof(D3DMATERIAL9));
	//単位行列化
	D3DXMatrixIdentity(&(this->m_World));
	//アニメーション時間初期化
	m_AnimeTime = 0;
	//アニメーションスピード初期化
	m_AnimSpeed = SKIN_ANIME_SPEED; //固定
	//現在のアニメーショントラック初期化
	m_CurrentTrack = 0;
	//アニメーションデータトラック初期化
	//有効にする
	m_CurrentTrackDesc.Enable = TRUE;
	//影響度100%
	m_CurrentTrackDesc.Weight = 1;
	//開始位置初期化
	m_CurrentTrackDesc.Position = 0;
	//速度
	m_CurrentTrackDesc.Speed = 1;
}

//=============================================================================
// ボーン解放関数
//=============================================================================
VOID CSkinMesh::Release() {
	if (m_pFrameRoot != NULL) {
		//ボーンフレーム関係解放
		FreeAnim(m_pFrameRoot);
		//その他情報(テクスチャの参照データなど)の解放
		m_cHierarchy.DestroyFrame(m_pFrameRoot);
		m_pFrameRoot = NULL;
	}
	//アニメーションコントローラー解放
	SAFE_RELEASE(m_pAnimController);
	//すべてのフレーム参照変数の要素を削除
	m_FrameArray.clear();
	//メッシュコンテナありのフレーム参照変数の要素を削除
	m_IntoMeshFrameArray.clear();
}

//=============================================================================
// ボーン行列取得関数
//=============================================================================
HRESULT CSkinMesh::AllocateBoneMatrix(LPD3DXFRAME pFrameRoot, LPD3DXMESHCONTAINER pMeshContainerBase)
{
	MYFRAME *pFrame = NULL;
	DWORD dwBoneNum = 0;
	//メッシュコンテナの型をオリジナルの型として扱う
	//(メッシュコンテナ生成時にオリジナルの型として作っているので問題はないが、
	//基本ダウンキャストは危険なので多用は避けるべき)
	MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;
	//スキンメッシュでなければ
	if (pMeshContainer->pSkinInfo == NULL)
	{
		return S_OK;
	}
	//ボーンの数取得
	dwBoneNum = pMeshContainer->pSkinInfo->GetNumBones();
	//各ボーンのワールド行列格納用領域を確保
	SAFE_DELETE(pMeshContainer->ppBoneMatrix);
	pMeshContainer->ppBoneMatrix = new D3DXMATRIX*[dwBoneNum];
	//ボーンの数だけループ
	for (DWORD i = 0; i<dwBoneNum; i++)
	{
		//子フレーム(ボーン)のアドレスを検索してpFrameに格納
		pFrame = (MYFRAME*)D3DXFrameFind(pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(i));
		//子フレームがなければ処理を終了する
		if (pFrame == NULL)
		{
			return E_FAIL;
		}
		//各ボーンのワールド行列格納用変数に最終行列を格納
		pMeshContainer->ppBoneMatrix[i] = &pFrame->CombinedTransformationMatrix;
	}
	return S_OK;
}

//=============================================================================
// ボーン行列初期化関数
//=============================================================================
//HRESULT AllocateAllBoneMatrices( THING* pThing,LPD3DXFRAME pFrame )
HRESULT CSkinMesh::AllocateAllBoneMatrices(LPD3DXFRAME pFrameRoot, LPD3DXFRAME pFrame)
{
	//階層の走査(メモリを確保したメッシュコンテナ領域を探す処理)
	if (pFrame->pMeshContainer != NULL)
	{
		//ボーン行列の初期化処理
		if (FAILED(AllocateBoneMatrix(pFrameRoot, pFrame->pMeshContainer)))
		{
			return E_FAIL;
		}
	}
	//再起判断処理
	if (pFrame->pFrameSibling != NULL)
	{
		if (FAILED(AllocateAllBoneMatrices(pFrameRoot, pFrame->pFrameSibling)))
		{
			return E_FAIL;
		}
	}
	if (pFrame->pFrameFirstChild != NULL)
	{
		if (FAILED(AllocateAllBoneMatrices(pFrameRoot, pFrame->pFrameFirstChild)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

//=============================================================================
// メッシュレンダリング関数
//=============================================================================
//VOID RenderMeshContainer(LPDIRECT3DDEVICE9 pDevice,MYMESHCONTAINER* pMeshContainer, MYFRAME* pFrame)
//フレーム内のそれぞれのメッシュをレンダリングする
VOID CSkinMesh::RenderMeshContainer(LPDIRECT3DDEVICE9 pDevice, MYMESHCONTAINER* pMeshContainer, MYFRAME* pFrame, PRS prs)
{
	DWORD i, k;
	DWORD dwBlendMatrixNum;
	DWORD dwPrevBoneID;
	LPD3DXBONECOMBINATION pBoneCombination;
	UINT iMatrixIndex;
	D3DXMATRIX mStack;
	//スキンメッシュの描画
	if (pMeshContainer->pSkinInfo != NULL)
	{
		//ボーンテーブルからバッファの先頭アドレスを取得
		pBoneCombination = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneBuffer->GetBufferPointer());
		//dwPrevBoneIDにUINT_MAXの値(0xffffffff)を格納
		dwPrevBoneID = UINT_MAX;
		//スキニング計算
		for (i = 0; i < pMeshContainer->dwBoneNum; i++)
		{
			dwBlendMatrixNum = 0;
			//影響している行列数取得
			for (k = 0; k< pMeshContainer->dwWeight; k++)
			{
				//UINT_MAX(-1)
				if (pBoneCombination[i].BoneId[k] != UINT_MAX)
				{
					//現在影響を受けているボーンの数
					dwBlendMatrixNum = k;
				}
			}
			//ジオメトリブレンディングを使用するために行列の個数を指定
			pDevice->SetRenderState(D3DRS_VERTEXBLEND, dwBlendMatrixNum);
			//影響している行列の検索
			for (k = 0; k < pMeshContainer->dwWeight; k++)
			{
				//iMatrixIndexに1度の呼び出しで描画出来る各ボーンを識別する値を格納
				//( このBoneID配列の長さはメッシュの種類によって異なる
				//( インデックスなしであれば　=　頂点ごとの重み であり
				// インデックスありであれば　=　ボーン行列パレットのエントリ数)
				//現在のボーン(i番目)からみてk番目のボーンid
				iMatrixIndex = pBoneCombination[i].BoneId[k];
				//行列の情報があれば
				if (iMatrixIndex != UINT_MAX)
				{
					//mStackにオフセット行列*ボーン行列を格納
					mStack = pMeshContainer->pBoneOffsetMatrices[iMatrixIndex] * (*pMeshContainer->ppBoneMatrix[iMatrixIndex]);
					//行列スタックに格納

					D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

					// 【S】スケールを反映(Multiplyは行列計算)
					D3DXMatrixScaling(&mtxScl, prs.scl.x, prs.scl.y, prs.scl.z);
					D3DXMatrixMultiply(&mStack, &mStack, &mtxScl);

					// 【R】回転を反映(YawPitchRollはy,x,z)
					D3DXMatrixRotationYawPitchRoll(&mtxRot, prs.rot.y, prs.rot.x, prs.rot.z);
					D3DXMatrixMultiply(&mStack, &mStack, &mtxRot);

					// 【T】平行移動を反映(オブジェクトを配置している）
					D3DXMatrixTranslation(&mtxTranslate, prs.pos.x, prs.pos.y, prs.pos.z);
					D3DXMatrixMultiply(&mStack, &mStack, &mtxTranslate);

					pDevice->SetTransform(D3DTS_WORLDMATRIX(k), &mStack);
				}
			}
			D3DMATERIAL9 TmpMat = pMeshContainer->pMaterials[pBoneCombination[i].AttribId].MatD3D;
			TmpMat.Emissive.a = TmpMat.Diffuse.a = TmpMat.Ambient.a = 1.0f;
			pDevice->SetMaterial(&TmpMat);
			pDevice->SetTexture(0, pMeshContainer->ppTextures[pBoneCombination[i].AttribId]);
			//dwPrevBoneIDに属性テーブルの識別子を格納
			dwPrevBoneID = pBoneCombination[i].AttribId;
			//メッシュの描画
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
	}
	//通常メッシュの場合
	else
	{
		MessageBox(NULL, "スキンメッシュXファイルの描画に失敗しました。", NULL, MB_OK);
		exit(EOF);
	}
}

//=============================================================================
// フレームレンダリング関数
//=============================================================================
//VOID DrawFrame(LPDIRECT3DDEVICE9 pDevice,LPD3DXFRAME pFrameBase)
//フレームをレンダリングする。
VOID CSkinMesh::DrawFrame(LPDIRECT3DDEVICE9 pDevice, LPD3DXFRAME pFrameBase, PRS prs)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	MYMESHCONTAINER* pMeshContainer = (MYMESHCONTAINER*)pFrame->pMeshContainer;
	while (pMeshContainer != NULL)
	{
		//SHADER_KIND a = GetpShader()->GetShaderKind();
		////シェーダを使用しているのなら専用の描画関数に飛ばす
		//if( GetpShader() != NULL && GetpShader()->GetShaderKind() == SHADER_KIND_LAMBERT ){
		// ShaderDraw( pDevice, ControlNum, pMeshContainer, pFrame ); 
		//}else{
		RenderMeshContainer(pDevice, pMeshContainer, pFrame, prs);
		// }
		//次のメッシュコンテナへアクティブを移す
		pMeshContainer = (MYMESHCONTAINER*)pMeshContainer->pNextMeshContainer;
	}
	if (pFrame->pFrameSibling != NULL)
	{
		DrawFrame(pDevice, pFrame->pFrameSibling, prs);
	}
	if (pFrame->pFrameFirstChild != NULL)
	{
		DrawFrame(pDevice, pFrame->pFrameFirstChild, prs);
	}
}

//=============================================================================
// ワールド更新関数
//=============================================================================
//VOID UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
//フレーム内のメッシュ毎にワールド変換行列を更新する
VOID CSkinMesh::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	MYFRAME *pFrame = (MYFRAME*)pFrameBase;
	if (pParentMatrix != NULL)
	{
		//CombinedTransformationMatrixに最終行列を格納
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	}
	else
	{
		//CombinedTransformationMatrixに最終行列を格納
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	}
	if (pFrame->pFrameSibling != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}
	if (pFrame->pFrameFirstChild != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}
}

//=============================================================================
// フレーム解放関数
//=============================================================================
//全ての階層フレームを解放する
VOID CSkinMesh::FreeAnim(LPD3DXFRAME pFrame)
{
	if (pFrame->pMeshContainer != NULL) {
		m_cHierarchy.DestroyMeshContainer(pFrame->pMeshContainer);
		pFrame->pMeshContainer = NULL;
	}
	if (pFrame->pFrameSibling != NULL)
		FreeAnim(pFrame->pFrameSibling);
	if (pFrame->pFrameFirstChild != NULL)
		FreeAnim(pFrame->pFrameFirstChild);
}

//=============================================================================
// 初期化関数
//=============================================================================
HRESULT CSkinMesh::Init(LPDIRECT3DDEVICE9 lpD3DDevice, LPSTR pMeshPass) {
	CHAR TmpMeshPass[255];
	strcpy_s(TmpMeshPass, sizeof(TmpMeshPass) - 1, pMeshPass);
	// Xファイルからアニメーションメッシュを読み込み作成する
	if (FAILED(
		D3DXLoadMeshHierarchyFromX(TmpMeshPass, D3DXMESH_MANAGED, lpD3DDevice, &m_cHierarchy,
			NULL,
			&m_pFrameRoot,
			&m_pAnimController)))
	{
		MessageBox(NULL, "Xファイルの読み込みに失敗しました", TmpMeshPass, MB_OK);
		return E_FAIL;
	}
	//ボーン行列初期化
	AllocateAllBoneMatrices(m_pFrameRoot, m_pFrameRoot);
	//アニメーショントラックの取得
	for (DWORD i = 0; i<m_pAnimController->GetNumAnimationSets(); i++)
	{
		//アニメーション取得
		m_pAnimController->GetAnimationSet(i, &(m_pAnimSet[i]));
	}
	//すべてのフレーム参照変数の生成
	m_FrameArray.clear();
	m_IntoMeshFrameArray.clear();
	CreateFrameArray(m_pFrameRoot);
	//フレーム配列にオフセット情報作成
	for (DWORD i = 0; i<m_IntoMeshFrameArray.size(); i++) {
		MYMESHCONTAINER* pMyMeshContainer = (MYMESHCONTAINER*)m_IntoMeshFrameArray[i]->pMeshContainer;
		while (pMyMeshContainer) {
			//スキン情報
			if (pMyMeshContainer->pSkinInfo) {
				DWORD cBones = pMyMeshContainer->pSkinInfo->GetNumBones();
				for (DWORD iBone = 0; iBone<cBones; iBone++) {
					//フレーム内から同じ名前のフレームを検索
					for (DWORD Idx = 0; Idx<m_FrameArray.size(); Idx++) {
						if (strcmp(pMyMeshContainer->pSkinInfo->GetBoneName(iBone), m_FrameArray[Idx]->Name) == 0) {
							pMyMeshContainer->BoneFrameArray.push_back(m_FrameArray[Idx]);
							//Offset行列
							m_FrameArray[Idx]->OffsetMat = *(pMyMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone));
							m_FrameArray[Idx]->OffsetID = Idx;
							break;
						}
					}
				}
			}
			//次へ
			pMyMeshContainer = (MYMESHCONTAINER *)pMyMeshContainer->pNextMeshContainer;
		}
	}
	return S_OK;
}

//=============================================================================
// フレームポインタ格納処理関数
//=============================================================================
VOID CSkinMesh::CreateFrameArray(LPD3DXFRAME _pFrame) {
	if (_pFrame == NULL) { return; }
	//フレームアドレス格納
	MYFRAME* pMyFrame = (MYFRAME*)_pFrame;
	m_FrameArray.push_back(pMyFrame);
	//メッシュコンテナがある場合はIntoMeshFrameArrayにアドレスを格納
	if (pMyFrame->pMeshContainer != NULL) {
		m_IntoMeshFrameArray.push_back(pMyFrame);
	}
	//子フレーム検索
	if (pMyFrame->pFrameFirstChild != NULL) {
		CreateFrameArray(pMyFrame->pFrameFirstChild);
	}
	//兄弟フレーム検索
	if (pMyFrame->pFrameSibling != NULL) {
		CreateFrameArray(pMyFrame->pFrameSibling);
	}
}

//=============================================================================
// 更新処理
//=============================================================================
VOID CSkinMesh::Update(D3DXMATRIX _World) {
	//押しっぱなしによる連続切り替え防止
	static bool PushFlg = false; //ここでは仮でフラグを使用するが、本来はメンバ変数などにする
								 //アニメーション変更チェック
	if ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8000)) {
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
			if (PushFlg == false) {
				int Num = GetAnimTrack() - 1;
				if (Num < 0)Num = 0;
				ChangeAnim(Num);
			}
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			if (PushFlg == false) {
				int Num = GetAnimTrack() + 1;
				if ((DWORD)Num > m_pAnimController->GetNumAnimationSets())Num = m_pAnimController->GetNumAnimationSets();
				ChangeAnim(Num);
			}
		}
		PushFlg = true;
	}
	else {
		PushFlg = false;
	}
	//マトリックス行列反映
	m_World = _World;
	//アニメーション時間を更新
	m_AnimeTime++;
}

//=============================================================================
// スキンメッシュ描画関数
//=============================================================================
VOID CSkinMesh::Draw(LPDIRECT3DDEVICE9 lpD3DDevice, PRS prs) {
	//現在のアニメーション番号を適応
	m_pAnimController->SetTrackAnimationSet(0, m_pAnimSet[m_CurrentTrack]);
	//0(再生中の)トラックからトラックデスクをセットする
	m_pAnimController->SetTrackDesc(0, &(m_CurrentTrackDesc));
	//アニメーション時間データの更新
	m_pAnimController->AdvanceTime(m_AnimSpeed, NULL);
	//アニメーションデータを更新
	UpdateFrameMatrices(m_pFrameRoot, &m_World);
	//アニメーション描画
	DrawFrame(lpD3DDevice, m_pFrameRoot, prs);
	//0(再生中の)トラックから更新したトラックデスクを取得する
	m_pAnimController->GetTrackDesc(0, &m_CurrentTrackDesc);
}

//=============================================================================
// アニメーション変更関数
//=============================================================================
//オブジェクトのアニメーション変更( 変更するアニメーション番号 )
VOID CSkinMesh::ChangeAnim(DWORD _NewAnimNum) {
	//新規アニメーションに変更
	m_CurrentTrack = _NewAnimNum;
	//アニメーションタイムを初期化
	m_AnimeTime = 0;
	//アニメーションを最初の位置から再生させる
	m_pAnimController->GetTrackDesc(0, &m_CurrentTrackDesc);
	m_CurrentTrackDesc.Position = 0;
	m_pAnimController->SetTrackDesc(0, &m_CurrentTrackDesc);
}

//=============================================================================
// 対象ボーン検索関数
//=============================================================================
MYFRAME* CSkinMesh::SearchBoneFrame(LPSTR _BoneName, D3DXFRAME* _pFrame) {
	MYFRAME* pFrame = (MYFRAME*)_pFrame;
	if (strcmp(pFrame->Name, _BoneName) == 0) {
		return pFrame;
	}
	if (_pFrame->pFrameSibling != NULL)
	{
		pFrame = SearchBoneFrame(_BoneName, _pFrame->pFrameSibling);
		if (pFrame != NULL) {
			return pFrame;
		}
	}
	if (_pFrame->pFrameFirstChild != NULL)
	{
		pFrame = SearchBoneFrame(_BoneName, _pFrame->pFrameFirstChild);
		if (pFrame != NULL) {
			return pFrame;
		}
	}
	return NULL;
}

//=============================================================================
// ボーンのマトリクス取得関数
//=============================================================================
D3DXMATRIX CSkinMesh::GetBoneMatrix(LPSTR _BoneName) {
	MYFRAME* pFrame = SearchBoneFrame(_BoneName, m_pFrameRoot);
	//ボーンが見つかれば
	if (pFrame != NULL) {
		//ボーン行列を返す
		return pFrame->CombinedTransformationMatrix;
	}
	//ボーンが見つからなければ
	else {
		//単位行列を返す
		D3DXMATRIX TmpMatrix;
		D3DXMatrixIdentity(&TmpMatrix);
		return TmpMatrix;
	}
}

//=============================================================================
// ボーンのマトリクスポインタ取得関数
//=============================================================================
D3DXMATRIX* CSkinMesh::GetpBoneMatrix(LPSTR _BoneName) {
	/////////////////////////////////////
	//注意）RokDeBone用に設定(対象ボーンの一つ先の行列をとってくる)
	MYFRAME* pFrame = SearchBoneFrame(_BoneName, m_pFrameRoot);
	//ボーンが見つかれば
	if (pFrame != NULL) {
		return &pFrame->CombinedTransformationMatrix;
	}
	//ボーンが見つからなければ
	else {
		//NULLを返す
		return NULL;
	}
}
