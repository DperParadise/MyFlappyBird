#version 330 core 

out vec4 fragColor;
in vec2 texCoords;

uniform sampler2D textureSampler;

void main()
{
  fragColor = vec4(texture(textureSampler, texCoords).rgb, 1.0);
}