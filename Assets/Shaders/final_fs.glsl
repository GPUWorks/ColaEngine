#version 330 core

in v2f
{
	vec2 uv;
} Input;

out vec3 fragColor;

uniform sampler2D final;

void main()
{
    fragColor = textureLod(final, Input.uv, 0).rgb;
    fragColor = fragColor / (fragColor + 1);
    fragColor = pow(fragColor, vec3(1.0 / 2.2));
}