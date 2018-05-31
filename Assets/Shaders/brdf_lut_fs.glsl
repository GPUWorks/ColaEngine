#version 330 core

#define PI 3.141592654

in vec2 uv;

out vec2 fragColor;

/************
* 低差异序列 *
************/
float RadicalInverse_VdC(uint bits) 
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
// 返回N个样本中的第i个样本
vec2 Hammersley(uint i, uint N)
{
    return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}

/***************
* GGX重要性采样 *
***************/
vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
    float a = roughness * roughness;
    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
    // 球坐标系到直角坐标系
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;
    // 切线空间到世界空间
    vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent   = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);
    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}

// Geometric Occlusion
float Vis_SmithJointApprox(float roughness, float NdotV, float NdotL)
{
	float a = pow(roughness, 2);
	float Vis_SmithV = NdotL * ( NdotV * ( 1.0f - a ) + a );
	float Vis_SmithL = NdotV * ( NdotL * ( 1.0f - a ) + a );
	return 0.5f / (Vis_SmithV + Vis_SmithL);
}

// Geometric Occlusion
float GeometrySmith(float roughness, float NdotV, float NdotL)
{
    float a = roughness;
    float k = (a * a) / 2.0;
    float SmithV = NdotV / (NdotV * (1.0 - k) + k);
    float SmithL = NdotL / (NdotL * (1.0 - k) + k);
    return SmithV * SmithL;
}

/***********
* 计算BRDF *
***********/
vec2 ComputeBRDF(float NdotV, float roughness)
{
    vec3 V;
    V.x = sqrt(1.0 - NdotV * NdotV);
    V.y = 0.0;
    V.z = NdotV;

    float A = 0.0;
    float B = 0.0;

    vec3 N = vec3(0.0, 0.0, 1.0);

    const uint sampleCount = 1024u;
    for(uint i = 0u; i < sampleCount; ++i)
    {
        vec2 Xi = Hammersley(i, sampleCount);
        vec3 H  = ImportanceSampleGGX(Xi, N, roughness);
        vec3 L  = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = clamp(L.z, 0.0, 1.0);
        float NdotH = clamp(H.z, 0.0, 1.0);
        float VdotH = clamp(dot(V, H), 0.0, 1.0);

        if(NdotL > 0.0)
        {
            //float G_Vis = Vis_SmithJointApprox(roughness, NdotV, NdotL);
            float G = GeometrySmith(roughness, NdotV, NdotL);
            float G_Vis = (G * VdotH) / (NdotH * NdotV);
            float Fc = pow(1.0 - VdotH, 5.0);

            A += (1.0 - Fc) * G_Vis;
            B += Fc * G_Vis;
        }
    }

    return vec2(A, B) / sampleCount;
}

void main()
{
    fragColor = ComputeBRDF(uv.x, uv.y);
}