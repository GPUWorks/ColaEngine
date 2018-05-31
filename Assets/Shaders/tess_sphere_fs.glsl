#version 400 core

in t2f
{
    vec3 pos;
    vec3 normal;
} Input;

uniform vec3 albedo;
uniform float roughness;
uniform vec3 emissive;
uniform float metallic;

uniform vec3 viewPos;

layout (location = 0) out vec4 PosDepth;
layout (location = 1) out vec4 NormalAO;
layout (location = 2) out vec4 AlbedoRoughness;
layout (location = 3) out vec4 EmissiveMetallic;

void main()
{
    PosDepth.rgb = Input.pos;
    PosDepth.a = distance(Input.pos, viewPos);

    NormalAO.rgb = Input.normal;
    NormalAO.a = 1.0;

    AlbedoRoughness.rgb = albedo;
    AlbedoRoughness.a = roughness;

    EmissiveMetallic.rgb = emissive;
    EmissiveMetallic.a = metallic;
}