#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec2 uv;
layout (location = 5) in vec4 weights;
layout (location = 6) in ivec4 boneIDs;

uniform mat4 model;
uniform mat4 mvp;
uniform mat4 boneTrans[64];

out v2f
{
    vec3 pos;
    mat3 tbn;
    vec2 uv;
} Output;

void main()
{
    mat4 finalModel = model;
    if (weights.x == 0.0f) {
        gl_Position = mvp * vec4(pos, 1.0f);
    }
    else {
        mat4 boneTrans =
        boneTrans[boneIDs.x] * weights.x +
        boneTrans[boneIDs.y] * weights.y +
        boneTrans[boneIDs.z] * weights.z +
        boneTrans[boneIDs.w] * weights.w;

        finalModel *= boneTrans;
        gl_Position = mvp * boneTrans * vec4(pos, 1.0f);
    }
    Output.pos = (finalModel * vec4(pos, 1.0)).xyz;
    Output.uv = uv;

    mat3 normalTrans = mat3(transpose(inverse(finalModel)));
    vec3 T = normalize(normalTrans * tangent);
	vec3 B = normalize(normalTrans * bitangent);
	vec3 N = normalize(normalTrans * normal);
    Output.tbn = mat3(T, B, N);
}