#version 330 core

in vec2 pass_textCoords;

out vec4 FragmentColor;

uniform sampler2D texture0;

void main(){
	FragmentColor = texture(texture0, pass_textCoords);
}