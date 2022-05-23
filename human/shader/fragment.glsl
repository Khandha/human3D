#version 400 core
out vec4 FragColor;

uniform vec4 customColor;
uniform vec4 lightPosA;
uniform vec4 lightPosB;
uniform vec4 lightColorA;
uniform vec4 lightColorB;
uniform vec4 viewPos;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture_nl;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

vec3 calculateLight(vec3 normal, vec3 lightPos, vec3 viewPos, vec3 lightColor, float specularStrength, vec3 ambient, int spec_impact)
{
    // normalize to light position
    vec3 lightDir = normalize(lightPos - FragPos);
    
    // calculate diffusion impact
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor  * vec3(texture(texture1, TexCoords));
    
    // normalize to light position
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // get reflect direction
    vec3 reflectDir = reflect(-lightDir, normal);
    
    // calculate specular impact
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), spec_impact);
    
    // calculate specular color
    vec3 specular = specularStrength * spec * vec3(texture(texture2, TexCoords));
    
    return (ambient + diffuse + specular);
}

void main() {    
    // vec4 -> vec3 conversion
    vec3 lpa = vec3(lightPosA);
    vec3 lpb = vec3(lightPosB);
    vec3 lca = vec3(lightColorA);
    vec3 lcb = vec3(lightColorB);
    vec3 vp = vec3(viewPos);
    
    // ambient light
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lca * lcb * texture(texture1, TexCoords).rgb;
    float specularStrength = 0.4;
  
    // normalize normal vertices 
    vec3 norm = normalize(Normal * texture(texture_nl, TexCoords).rgb);

    // calculate result and return for vertice
    
    vec3 result = calculateLight(norm, lpa, vp, lca, specularStrength, ambient, 200) 
        + 0.6 * calculateLight(norm, lpb, vp, lcb, specularStrength, ambient, 8);
    
    FragColor = vec4(result, 1.0);
    
//    FragColor = texture(texture1, TexCoords);
}
