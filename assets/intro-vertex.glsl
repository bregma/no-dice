#version 300 es

in vec2 in_position;
in vec2 in_texcoord;

uniform mat4 mvp;

out vec2 fr_texcoord;

void main()
{
  gl_Position = mvp * vec4(in_position.xy, 0.0, 1.0);
  fr_texcoord = in_texcoord;
}

