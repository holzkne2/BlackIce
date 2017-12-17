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
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL;
	float3 worldPos : TEXCOORD1;
};

PixelInputType vert(VertexInputType i)
{
	PixelInputType o;

	i.position.w = 1.0f;

	o.position = mul(i.position, worldMatrix);
	o.worldPos = o.position;
	o.position = mul(o.position, viewMatrix);
	o.position = mul(o.position, projectionMatrix);

	o.uv = i.uv;

	// Uniform Scaling only
	o.normal = mul(i.normal, (float3x3)worldMatrix);
	o.normal = normalize(o.normal);

	return o;
}

Texture2D mainTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
	float4 lightColor;
	float3 lightDirection;
	float padding;
};

cbuffer Camera
{
	float3 cameraPosition;
	float padding1;
};

float4 frag(PixelInputType i) : SV_TARGET
{
	float3 albedo = pow(mainTexture.Sample(SampleType, i.uv).rgb, 2.2);

	float ndotl = saturate(dot(i.normal, -lightDirection));

	float3 finalColor = albedo * ndotl * lightColor.rgb + float3(0.01, 0.01, 0.01);

	return float4(pow(finalColor, 1 / 2.2), 1.0);
}