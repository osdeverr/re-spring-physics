#version 410 core

in vec3 a_position;
in vec4 a_color;

uniform mat4 u_camera_matrix;

out vec3 vertex_position;
out vec4 vertex_color;

void main() {
  vec4 position = u_camera_matrix * vec4(a_position, 1.0);
  gl_Position = position;
  vertex_color = a_color;
  vertex_position = a_position;
}
