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
    float offsetH = 1.0 / textureSize(image, 0).x * texture(coc, Input.uv).r;
    vec3 blurH = texture(image, Input.uv).rgb * kernel[0];
    for (int i = 1; i < 6; ++i) {
		vec2 offset = vec2(i * offsetH, 0.0);
        blurH += texture(image, Input.uv - offset).rgb * kernel[i];
        blurH += texture(image, Input.uv + offset).rgb * kernel[i];
    }
    fragColor = blurH;
}