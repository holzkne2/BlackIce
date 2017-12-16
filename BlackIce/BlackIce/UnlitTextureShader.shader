cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 uv : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

PixelInputType vert(VertexInputType i)
{
	PixelInputType o;

	i.position.w = 1.0f;

	o.position = mul(i.position, worldMatrix);
	o.position = mul(o.position, viewMatrix);
	o.position = mul(o.position, projectionMatrix);

	o.uv = i.uv;

	return o;
}

Texture2D mainTexture;
SamplerState SampleType;

float4 frag(PixelInputType i) : SV_TARGET
{
	return mainTexture.Sample(SampleType, i.uv);
}