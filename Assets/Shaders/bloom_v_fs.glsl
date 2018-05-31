#version 330 core

in v2f
{
	vec2 uv;
} Input;

out vec3 fragColor;

uniform sampler2D image;

int mipLevel = 4;

const float kernel[6] = float[]
(
    0.198596, 0.175713, 0.121703, 0.065984, 0.028002, 0.0093
);

void main()
{
    float offsetV = 1.0 / textureSize(image, mipLevel).y;
    // Convolution
    vec3 blurV = textureLod(image, Input.uv, mipLevel).rgb * kernel[0];
    for (int i = 1; i < 6; ++i) {
        vec2 offset = vec2(0.0, i * offsetV);
        blurV += textureLod(image, Input.uv - offset, mipLevel).rgb * kernel[i];
        blurV += textureLod(image, Input.uv + offset, mipLevel).rgb * kernel[i];
    }
    fragColor = blurV;
}