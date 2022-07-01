#version 330 core
uniform vec3 lightPos;
out vec4 FragColor;
in vec3 normal;
in vec3 fragPos;
void main(){
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(0.5, 0.5, 0.5);
    vec3 result = diffuse + vec3(0.5, 0.5, 0.5);
    FragColor = vec4(result, 1.0);
}
