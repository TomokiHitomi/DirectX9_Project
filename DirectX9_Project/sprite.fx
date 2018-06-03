//=============================================================================
//
// Sprite処理 [sprite.fx]
// Author : GP12A295 25 人見友基
//
//=============================================================================
float4x4 world;		// ワールドマトリクス
float4x4 view;		// ビューマトリクス
float4x4 proj;		// プロジェクションマトリクス

texture tex;		// 使用するテクスチャ
sampler smp = sampler_state {
	texture = <tex>;
};

 
//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct VS_IN		// 頂点シェーダの引数
{
	float3 pos : POSITION;
	float4 col : COLOR0;
	float2 uv : TEXCOORD0;
};

struct VS_OUT		// 頂点シェーダの戻り値かつピクセルシェーダーの引数
{
	float4 pos : POSITION;
	float4 col : COLOR0;
	float2 uv : TEXCOORD0;
};

//=============================================================================
// 頂点シェーダ
//=============================================================================
VS_OUT vs_main( VS_IN In )
{
	VS_OUT Out = (VS_OUT)0;
	float4x4 mat;

	mat = mul(mul(world, view), proj);
	Out.pos = mul( float4(In.pos, 1.0f), mat );
	Out.uv  = In.uv;
	Out.col = In.col;
	return Out;
}

//=============================================================================
// ピクセルシェーダ
//=============================================================================
float4 ps_nomal(VS_OUT In) : COLOR0
{
	return tex2D( smp, In.uv ) * In.col;
}
technique Tec01		// テクスチャ描画
{
	pass p0
	{
		VertexShader = compile vs_2_0 vs_main();
		PixelShader = compile ps_2_0 ps_nomal();
	}
}