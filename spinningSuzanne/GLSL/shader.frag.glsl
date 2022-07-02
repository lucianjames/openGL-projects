#version 330 core
uniform vec3 lightPos;
out vec4 FragColor;
in vec3 normal;
in vec3 fragPos;
vec3 lightColor = vec3(1.0, 1.0, 1.0);
vec3 objectColor = vec3(0.1, 0.5, 0.6);
void main(){

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);

    FragColor = vec4(normal, 1.0); // Normal debug
}
