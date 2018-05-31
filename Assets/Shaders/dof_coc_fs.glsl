#version 330 core

in v2f
{
	vec2 uv;
} Input;

uniform sampler2D PosDepth;

uniform vec2 foregroundRange;
uniform vec2 backgroundRange;

out float coc;

bool inRange(float value, vec2 range)
{
    return value > range.x && value < range.y;
}

void main()
{
    float fragDistance = texture(PosDepth, Input.uv).a;

    coc = 0.0;
    if (fragDistance < foregroundRange.x) {
        coc = 1.0;
    }
    else if (inRange(fragDistance, foregroundRange)) {
        float k = (foregroundRange.y - fragDistance) / (foregroundRange.y - foregroundRange.x);
        coc = mix(0.0, 1.0, k);
    }
    else if (inRange(fragDistance, backgroundRange)) {
        float k = (backgroundRange.y - fragDistance) / (backgroundRange.y - backgroundRange.x);
        coc = mix(0.5, 0.0, k);
    }
    else if (fragDistance > backgroundRange.y) {
        coc = 0.5;
    }
}