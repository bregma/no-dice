#version 300 es

precision highp float;

in vec2 fr_texcoord;

uniform sampler2D alphas;

out vec4 fragColor;

void main()
{
  vec4 text_color = vec4(1.0, 0.9, 0.3, 1.0);
  vec4 mask = vec4(1.0, 1.0, 1.0, texture(alphas, fr_texcoord).r);
  fragColor = text_color * mask;
}
