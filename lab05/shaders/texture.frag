#version 400
in vec2 uv;
out vec4 fragColor;

in vec3 WS_position; // world-space position
in vec3 WS_normal;   // world-space normal

uniform sampler2D textureSampler;

void main() {
    vec4 terrainColor = texture(textureSampler, uv);
    vec3 WS_toLight = normalize(vec3(10.0) - WS_position);
    fragColor = 0.3 * vec4(vec3(0.3 + 0.7 * max(0.0, dot(normalize(WS_normal), WS_toLight))), 0.f) + terrainColor;
}
