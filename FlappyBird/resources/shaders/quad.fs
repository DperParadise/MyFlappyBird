#version 330 core 

out vec4 fragColor;
in vec2 texCoords;

uniform sampler2D textureSampler;
uniform float alpha;

void main()
{
  vec4 color = texture(textureSampler, texCoords);
  fragColor = vec4(color.rgb, color.a * alpha);
}