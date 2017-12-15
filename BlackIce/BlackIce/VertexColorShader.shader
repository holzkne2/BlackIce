cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PixelInputType vert(VertexInputType i)
{
	PixelInputType o;

	i.position.w = 1.0f;

	o.position = mul(i.position, worldMatrix);
	o.position = mul(o.position, viewMatrix);
	o.position = mul(o.position, projectionMatrix);

	o.color = i.color;

	return o;
}

float4 frag(PixelInputType i) : SV_TARGET
{
	return i.color;
}