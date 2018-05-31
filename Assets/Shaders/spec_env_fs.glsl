#version 330 core

#define PI      3.141592654
#define TWO_PI  6.283185307
#define HALF_PI 1.570796327

in vec3 fragPos;

uniform float roughness;
uniform sampler2D skyTex;

out vec3 fragColor;

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
// Microfaced Distribution
float NDF_GGX(float roughness, float NdotH)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float d = (NdotH * a2 - NdotH) * NdotH + 1.0f;
	return a2 / (PI * d * d);
}

/************
* 计算采样UV *
************/
vec2 SampleVecToUV(vec3 v)
{
    return vec2(atan(v.z, v.x) * 0.1591 + 0.5, 1.0 - (asin(v.y) * 0.3183 + 0.5));
}

/***************************
* 片段着色器计算蒙特卡罗积分 *
***************************/
void main()
{
    vec3 N = normalize(fragPos);
    vec3 R = N;
    vec3 V = R;

    vec3 prefilteredColor = vec3(0.0);

    const uint sampleCount = 4096u;
    float totalWeight = 0.0;
    for(uint i = 0u; i < sampleCount; ++i) {
        vec2 Xi = Hammersley(i, sampleCount);
        vec3 H  = ImportanceSampleGGX(Xi, N, roughness);
        vec3 L  = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = clamp(dot(N, L), 0.0, 1.0);
        if(NdotL > 0.0)
        {
            // 基于roughness/pdf计算mipLevel以减少失真
            float NdotH = clamp(dot(N, H), 0.0, 1.0);
            float HdotV = clamp(dot(H, V), 0.0, 1.0);
            float D = NDF_GGX(roughness, NdotH);
            float pdf = D * NdotH / (4.0 * HdotV) + 0.0001; 
            float resolution = 512.0;
            float saTexel  = 4.0 * PI / (6.0 * resolution * resolution);
            float saSample = 1.0 / (float(sampleCount) * pdf + 0.0001);
            float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);
            
            prefilteredColor += textureLod(skyTex, SampleVecToUV(L), mipLevel).rgb * NdotL;
            //prefilteredColor += texture(skyTex, SampleVecToUV(L)).rgb * NdotL;
            totalWeight += NdotL;
        }
    }

    fragColor = prefilteredColor / totalWeight;
}