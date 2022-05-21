#version 400 core
out vec4 FragColor;

uniform vec4 customColor;
uniform vec4 lightPos;
uniform vec4 lightColor;

in vec3 Normal;
in vec3 FragPos;

void main() {    
    // vec4 -> vec3 conversion
    vec3 lp = vec3(lightPos);
    vec3 lc = vec3(lightColor);
    
    // ambient light
    float ambientStrength = 0;
    vec3 ambient = ambientStrength * lc;
    
    // normalize normal vertices 
    vec3 norm = normalize(Normal);
    
    // normalize to light position
    vec3 lightDir = normalize(lp - FragPos);
    
    // calculate diffusion impact 
    float diff = max(dot(norm, lightDir), 0);
    vec3 diffuse = diff * lc;
    
    // calculate result and return for vertice
    vec3 result = (ambient + diffuse) * customColor.xyz;
    FragColor = vec4(result, 1.0);
}
