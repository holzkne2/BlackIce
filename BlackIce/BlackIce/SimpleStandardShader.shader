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
	float4 lightDirection;
	float4 cameraPosition;
	//float3 padding;
};

static const float PI = 3.14159265359;

float3 fresnelSchlick(float cosTheta, float3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(float3 N, float3 H, float roughness)
{
	float a = roughness*roughness;
	float a2 = a*a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH*NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / max(denom, 0.001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}
float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

float G1V(float dotNV, float k)
{
	return 1.0f / (dotNV*(1.0f - k) + k);
}

float2 LightingFuncGGX_REF(float3 N, float3 V, float3 L, float roughness, float F0)
{
	float alpha = roughness*roughness;

	float3 H = normalize(V + L);

		float dotNL = saturate(dot(N, L));
	float dotNV = saturate(dot(N, V));
	float dotNH = saturate(dot(N, H));
	float dotLH = saturate(dot(L, H));

	float F, D, vis;

	// D
	float alphaSqr = alpha*alpha;
	float pi = 3.14159f;
	float denom = dotNH * dotNH *(alphaSqr - 1.0) + 1.0f;
	D = alphaSqr / (pi * denom * denom);

	// F
	float dotLH5 = pow(1.0f - dotLH, 5);
	F = F0 + (1.0 - F0)*(dotLH5);

	// V
	float k = alpha / 2.0f;
	vis = G1V(dotNL, k)*G1V(dotNV, k);

	float specular = dotNL * D * F * vis;
	return specular;
}

float4 frag(PixelInputType i) : SV_TARGET
{
	float3 albedo = float3(0.55, 0.55, 0.55);
	albedo = pow(mainTexture.Sample(SampleType, i.uv).rgb, 2.2);
	float3 normal = i.normal;
	float metallic = 1.0;
	float roughness = 0.5;
	roughness = max(roughness, 0.002);

	float3 N = normalize(i.normal);
	float3 V = -normalize(cameraPosition.xyz - i.worldPos);

	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
	// of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)   
	float3 F0 = float3(0.04, 0.04, 0.04);
	F0 = lerp(F0, albedo, metallic);

	//{
	//	float3 L = normalize(-lightDirection.xyz);
	//		float lf = LightingFuncGGX_REF(N, V, L, roughness, F0);
	//	float3 light = (float3(lf, lf, lf) + albedo / PI) * max(dot(N, L), 0.0) * lightColor.rgb;
	//		float3 finalColor = light + float3(0.03, 0.03, 0.03) * albedo;
	//		finalColor = finalColor / (finalColor + float3(1.0, 1.0, 1.0));
	//	finalColor = pow(finalColor, 1 / 2.2);
	//	return float4(finalColor, 1.0);
	//}

	float3 Lo = float3(0, 0, 0);
	{
		float3 L = normalize(-lightDirection.xyz);
		float3 H = normalize(V + L);
		float3 radiance = lightColor.rgb;

		// cook-torrance brdf
		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);
		float3 F = fresnelSchlick(saturate(dot(H, V)), F0);

		float3 kS = F;
		float3 kD = float3(1.0, 1.0, 1.0) - kS;
		kD *= 1.0 - metallic;

		float3 nominator = NDF * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		float3 specular = nominator / max(denominator, 0.001);

			//return float4(saturate(dot(V, H)), 0, 0, 1.0);
			//return float4(nominator, 1.0);

		// add to outgoing radiance Lo
		float NdotL = max(dot(N, L), 0.0);
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}
	float3 ambient = float3(0.03, 0.03, 0.03) * albedo;
	float3 finalColor = ambient + Lo;

	// HDR tonemapping
	finalColor = finalColor / (finalColor + float3(1.0, 1.0, 1.0));
	return float4(pow(finalColor, 1 / 2.2), 1.0);
}