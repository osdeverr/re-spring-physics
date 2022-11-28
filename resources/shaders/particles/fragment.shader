#version 410 core

precision highp float;

in vec3 vertex_position;
in vec4 vertex_color;
out vec4 color;

void main() {
//  vec2 position = (screen_position.xy / screen_position.w + vec2(1, 1)) * 0.5;
//  vec3 light = u_ambient_light + texture(u_shadow_map, position).xyz;

  color = vertex_color;
}
