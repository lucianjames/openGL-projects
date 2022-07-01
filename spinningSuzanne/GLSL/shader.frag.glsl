#version 330 core
uniform vec3 lightPos;
out vec4 FragColor;
in vec3 normal;
in vec3 fragPos;
void main(){
    FragColor = vec4(1.0, 0.2, 0.1, 1.0);
}
