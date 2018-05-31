#version 330 core

in v2f
{
	vec2 uv;
} Input;

uniform sampler2D image;
uniform sampler2D coc;

out vec3 fragColor;

const float kernel[6] = float[]
(
    0.198596, 0.175713, 0.121703, 0.065984, 0.028002, 0.0093
);

void main()
{
    float offsetV = 1.0 / textureSize(image, 0).y * texture(coc, Input.uv).r;
    vec3 blurV = texture(image, Input.uv).rgb * kernel[0];
    for (int i = 1; i < 6; ++i) {
		vec2 offset = vec2(0.0, i * offsetV);
        blurV += texture(image, Input.uv - offset).rgb * kernel[i];
        blurV += texture(image, Input.uv + offset).rgb * kernel[i];
    }
    fragColor = blurV;
}