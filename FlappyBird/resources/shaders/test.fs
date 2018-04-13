#version 330 core 

out vec4 fragColor;
in vec2 texCoords;

uniform vec3 color;
uniform sampler2D textureSampler;

void main()
{
  fragColor = vec4(texture(textureSampler, texCoords).rgb, 1.0) * vec4(color, 1.0);
}