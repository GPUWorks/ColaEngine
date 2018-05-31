#version 330 core

in v2f
{
	vec2 uv;
} Input;

out vec3 fragColor;

uniform sampler2D image;

void main()
{
    fragColor = textureLod(image, Input.uv, 0).rgb;
    float brightness = dot(fragColor, vec3(0.2126, 0.7152, 0.0722));
    if (brightness < 1.0) fragColor = vec3(0);
}