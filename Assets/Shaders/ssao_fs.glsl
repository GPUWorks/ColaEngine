#version 330 core

#define NUM_SAMPLE 64

in v2f
{
	vec2 uv;
} Input;

out vec3 fragColor;

uniform sampler2D lightingColor;
uniform sampler2D PosDepth;
uniform sampler2D NormalAO;
uniform sampler2D offset;

uniform vec3 samples[NUM_SAMPLE];
uniform mat4 vp;
uniform vec3 viewPos;
uniform float radius;

void main()
{
    vec2 scale = textureSize(lightingColor, 0) / 4.0;
    vec3 normal = normalize(texture(NormalAO, Input.uv).rgb);
    vec3 random = texture(offset, Input.uv * scale).rgb;

    vec3 tangent = normalize(random - normal * dot(random, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 tbn = mat3(tangent, bitangent, normal);

    vec3 fragPos = texture(PosDepth, Input.uv).rgb;
    float occlusion = 0.0;
    for (int i = 0; i < NUM_SAMPLE; ++i)
    {
        // 计算采样点世界坐标
        vec3 sample = tbn * samples[i];
        sample = fragPos + sample * radius;
        // 采样点变换到屏幕空间
        vec4 samplePos = vec4(sample, 1.0);
        samplePos = vp * samplePos;
        samplePos.xyz /= samplePos.w;
        samplePos.xyz = samplePos.xyz * 0.5 + 0.5;
        // 计算贡献
        float fragDistance = texture(PosDepth, samplePos.xy).w;
        float sampleDistance = distance(sample, viewPos);
        occlusion += (fragDistance <= sampleDistance ? 1.0 : 0.0) * (abs(sampleDistance - fragDistance) < radius ? 1.0 : 0.0);
    }
    occlusion = 1.0 - (occlusion / NUM_SAMPLE);

    fragColor = texture(lightingColor, Input.uv).rgb * occlusion;
}