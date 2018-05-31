#version 330 core

in vec3 fragPos;

uniform sampler2D skyTex;

out vec3 fragColor;

vec2 SampleVecToUV(vec3 v)
{
    return vec2(atan(v.z, v.x) * 0.1591 + 0.5, 1.0 - (asin(v.y) * 0.3183 + 0.5));
}

void main()
{
    fragColor = textureLod(skyTex, SampleVecToUV(normalize(fragPos)), 0).rgb;
}