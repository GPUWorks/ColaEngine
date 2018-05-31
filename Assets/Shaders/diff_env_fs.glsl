#version 330 core

#define PI      3.141592654
#define TWO_PI  6.283185307
#define HALF_PI 1.570796327

in vec3 fragPos;

uniform sampler2D skyTex;

out vec3 fragColor;

vec2 SampleVecToUV(vec3 v)
{
    return vec2(atan(v.z, v.x) * 0.1591 + 0.5, 1.0 - (asin(v.y) * 0.3183 + 0.5));
}

void main()
{
    vec3 N = normalize(fragPos);
    vec3 value = vec3(0.0);
    // tangent
    vec3 up    = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, N));
    up = cross(N, right);

    int n1n2 = 0;
    float step = 0.05;
    for (float phi = 0.0; phi < TWO_PI; phi += step) {
        for (float theta = 0.0; theta < HALF_PI; theta += step) {
            vec3 sampleVec = cos(phi) * right + sin(phi) * up;
            sampleVec = cos(theta) * N + sin(theta) * sampleVec;
            value += textureLod(skyTex, SampleVecToUV(sampleVec), 0).rgb * cos(theta) * sin(theta);
            ++n1n2;
        }
    }
    value = PI / float(n1n2) * value;
    fragColor = value;
}