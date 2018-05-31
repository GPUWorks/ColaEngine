#version 330 core

in v2f
{
    vec3 pos;
    mat3 tbn;
    vec2 uv;
} Input;

uniform sampler2D albedoTex;
uniform sampler2D normalTex;
uniform sampler2D roughnessTex;
uniform sampler2D metallicTex;
uniform sampler2D emissiveTex;

uniform vec3 viewPos;

layout (location = 0) out vec4 PosDepth;
layout (location = 1) out vec4 NormalAO;
layout (location = 2) out vec4 AlbedoRoughness;
layout (location = 3) out vec4 EmissiveMetallic;

void main()
{
    PosDepth.rgb = Input.pos;
    PosDepth.a = distance(Input.pos, viewPos);

	vec3 tangentSpaceNormal = texture(normalTex, Input.uv).rgb;
	tangentSpaceNormal = normalize(tangentSpaceNormal * 2.0 - 1.0);
    NormalAO.rgb = normalize(Input.tbn * tangentSpaceNormal);
    NormalAO.a = 1.0;

    AlbedoRoughness.rgb = texture(albedoTex, Input.uv).rgb;
    AlbedoRoughness.a = texture(roughnessTex, Input.uv).r;

    EmissiveMetallic.rgb = texture(emissiveTex, Input.uv).rgb;
    EmissiveMetallic.a = texture(metallicTex, Input.uv).r;
}