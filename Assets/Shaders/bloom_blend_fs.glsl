#version 330 core

in v2f
{
	vec2 uv;
} Input;

out vec3 fragColor;

uniform sampler2D image;
uniform sampler2D bloom;

void main()
{
    fragColor = texture(image, Input.uv).rgb + texture(bloom, Input.uv).rgb;
}