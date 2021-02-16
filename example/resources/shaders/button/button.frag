#version 330 core
in vec2 pass_texture_coordinates;

uniform sampler2D Texture;

out vec4 FragmentColor;

void main(){
	FragmentColor = texture(Texture, pass_texture_coordinates);
}