//=============================================================================
//
// シェーダー処理 [Shader.fx]
// Author : GP12A295 25 人見友基
//
// 頂点シェーダーによるライティング・ピクセルシェーダー処理
//
// 参考：DirectX9での上位レベルシェーダー言語（HLSL）
// http://www001.upp.so-net.ne.jp/y_yutaka/labo/directx/shader_hlsl.html
//
//=============================================================================


//**********************************//
//	グローバル変数					//
//**********************************//

//ワールド*ビュー*投影変換の行列
float4x4 gWVPMatrix;

//アンビエント値
float4 gAmbient = { 0.2f,0.2f,0.2f,1.0f };

//基本色
float4 gBaseColor = { 1.0f, 0.1f, 0.2f, 1.0f };

//テクスチャ
texture gTexture0;

//----------------------//
//	頂点の入力情報		//
//----------------------//
struct VS_INPUT
{
	float4 vPosition		: POSITION;			//頂点座標
	float4 vNormal			: NORMAL;			//法線ベクトル
	float2 vTexCoords		: TEXCOORD0;		//テクスチャUV
	float4 vDiffuse         : COLOR0;			//デフューズ色
	float4 vSpecular        : COLOR1;			//スペキュラ色
};

//----------------------//
//	頂点の出力情報		//
//----------------------//
struct VS_OUTPUT
{
	float4 vPosition		: POSITION;			//頂点座標
	float4 vDiffuse			: COLOR0;			//デフューズ色
	float4 vSpecular		: COLOR1;			//スペキュラ色
	float2 vTexCoords		: TEXCOORD0;		//テクスチャUV
};

//----------------------//
//	ピクセルの入力情報	//
//----------------------//
struct PS_INPUT
{
	float4 vDiffuse			: COLOR0;			//デフューズ色
	float4 vSpecular		: COLOR1;			//スペキュラ色
	float2 vTexCoords		: TEXCOORD0;		//テクスチャUV
};

//----------------------//
//	ピクセルの出力情報	//
//----------------------//
struct PS_OUTPUT
{
	float4 vColor			: COLOR0;			//最終的な出力色
};


//**********************************************//
//	基本色のみを考慮した頂点・ピクセル計算		//
//**********************************************//

VS_OUTPUT VSBaseColor(VS_INPUT v)
{
	VS_OUTPUT o = (VS_OUTPUT)0;

	//座標変換
	o.vPosition = mul(v.vPosition, gWVPMatrix);

	//デフューズ色をそのまま格納
	o.vDiffuse = v.vDiffuse;

	//スペキュラ色をそのまま格納
	o.vSpecular = v.vSpecular;

	return o;
}

//注意！！ピクセルシェーダー内では
//maxなどの組み込み関数は使えない！！！

PS_OUTPUT PSBaseColor(PS_INPUT p)
{
	PS_OUTPUT o = (PS_OUTPUT)0;

	o.vColor = (p.vDiffuse + gAmbient) * gBaseColor + p.vSpecular;

	return o;
}

//**********************************************//
//	テクスチャを考慮した頂点・ピクセル計算		//
//**********************************************//

VS_OUTPUT VSTextureColor(const VS_INPUT v)
{
	VS_OUTPUT o = (VS_OUTPUT)0;

	//座標変換
	o.vPosition = mul(v.vPosition, gWVPMatrix);

	//デフューズ色をそのまま格納
	o.vDiffuse = v.vDiffuse;

	//スペキュラ色をそのまま格納
	o.vSpecular = v.vSpecular;

	//テクスチャ値
	o.vTexCoords = v.vTexCoords;

	return o;
}

//テクスチャの線形補間のためのサンプラー情報
sampler Sampler = sampler_state
{
	Texture = <gTexture0>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

PS_OUTPUT PSTextureColor(PS_INPUT p)
{
	PS_OUTPUT o = (PS_OUTPUT)0;
	float4 col;

	col = (p.vDiffuse + gAmbient);
	o.vColor = col * tex2D(Sampler, p.vTexCoords) + p.vSpecular;

	return o;
}

//**********************************************//
//	テクニックの記述							//
//**********************************************//

//----------------------//
//	基本色ののみ		//
//----------------------//
technique BaseColor
{
	pass P0
	{
		VertexShader = compile vs_2_0 VSBaseColor();
		PixelShader = compile ps_2_0 PSBaseColor();
	}
}

//----------------------//
//	テクスチャあり		//
//----------------------//
technique TextureColor
{
	pass P0
	{
		VertexShader = compile vs_2_0 VSTextureColor();
		PixelShader = compile ps_2_0 PSTextureColor();
	}
}
