#version 330 core
layout(location = 0) out vec4 vFragColor;

uniform vec4 obj_color;

void main()
{
    vFragColor = obj_color;
}