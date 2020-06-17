#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

//uniform sampler2D texture_diffuse1;
uniform vec3 obj_color;

void main()
{    
    //FragColor = texture(texture_diffuse1, TexCoords);
    FragColor = vec4(obj_color.xyz, 1);
}