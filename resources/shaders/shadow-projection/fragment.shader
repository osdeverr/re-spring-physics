#version 410 core

out float depth;
in float vertex_depth;

void main() {
  depth = gl_FragCoord.z;
}
