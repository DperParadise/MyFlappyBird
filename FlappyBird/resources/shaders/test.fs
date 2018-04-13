#version 330 core 

in vec2 texCoords;
out vec4 fragColor;
uniform vec3 color;
uniform sampler2D texSampler;

void main()
{
  fragColor = texture(texSampler, texCoords) * vec4(color, 1.0);
}