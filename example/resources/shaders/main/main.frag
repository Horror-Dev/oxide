#version 330 core

out vec4 FragmentColor;

in vec3 pass_color;

void main(){
	FragmentColor = vec4(pass_color, 1.0f);
}