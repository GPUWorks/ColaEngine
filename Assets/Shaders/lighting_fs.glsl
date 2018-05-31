#version 330 core

in v2f
{
	vec2 uv;
} Input;

out vec3 fragColor;

uniform sampler2D PosDepth;
uniform sampler2D NormalAO;
uniform sampler2D AlbedoRoughness;
uniform sampler2D EmissiveMetallic;

uniform samplerCube diffEnvMap;
uniform samplerCube specEnvMap;
uniform sampler2D   brdfLutTex;

uniform vec3 viewPos;
int mipCount = 5;

vec3 IBL(vec3 N, vec3 V, vec3 diffColor, vec3 specColor, float roughness)
{
	// diffuse
	vec3 diffuseLight = texture(diffEnvMap, N).rgb;
	vec3 diffuse = diffuseLight * diffColor;
	// specular
	float NdotV = dot(N, V);
	NdotV = clamp(abs(NdotV) + 1e-5, 0.0, 1.0);
	vec2 brdf = texture(brdfLutTex, vec2(NdotV, roughness)).rg;
	vec3 R = reflect(-V, N);
	vec3 specularLight = textureLod(specEnvMap, R, roughness * (mipCount - 1)).rgb;
	vec3 specular = specularLight * (specColor * brdf.x + brdf.y);

	return diffuse + specular;
}

void main()
{
    vec4 posD = texture(PosDepth, Input.uv);
    if (posD.a == 0.0) discard;

	vec3 N = texture(NormalAO, Input.uv).rgb;
	vec3 V = normalize(viewPos - posD.rgb);

	vec4 albedoR = texture(AlbedoRoughness, Input.uv);
	vec4 emissiveM = texture(EmissiveMetallic, Input.uv);

	vec3 f0 = vec3(0.04, 0.04, 0.04);// 非金属物体菲涅尔系数
	vec3 diffColor = albedoR.rgb * (1.0 - f0);// 漫反射辐射照度
	diffColor *= 1.0 - emissiveM.a;
	vec3 specColor = mix(f0, albedoR.rgb, emissiveM.a);

	vec3 finalColor = vec3(0.0, 0.0, 0.0);
	finalColor += IBL(N, V, diffColor, specColor, albedoR.a);

    fragColor = finalColor;
}