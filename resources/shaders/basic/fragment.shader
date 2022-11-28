#version 410 core

precision highp float;

in vec3 vertex_position;
in vec3 vertex_color;
in vec4 screen_position;
in float is_grid;
out vec3 color;

uniform vec3 u_ambient_light;
uniform sampler2D u_shadow_map;

float grid() {
  float grid_factor = (mod(vertex_position.x, 2) > 1 ? 1 : -1) *
  (mod(vertex_position.z, 2) > 1 ? 1 : -1);

  float depth = 1 / (screen_position.w * 0.1 + 1);
  grid_factor = grid_factor * depth + 0.5 * (1 - depth);

  return 1 - 0.2 * is_grid * grid_factor;
}

void main() {
  vec2 position = (screen_position.xy / screen_position.w + vec2(1, 1)) * 0.5;
  vec3 light = u_ambient_light + texture(u_shadow_map, position).xyz;

  color = vertex_color * light * grid();
}
