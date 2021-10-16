#version 460 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

out vec4 FragColor;
  
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform Material material;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
    // Ambient
    vec3 ambient = material.ambient;
    
    // Specular
    // vec3 viewDir = normalize(viewPos - FragPos);
    // vec3 reflectDir = reflect(-lightDir, norm);  
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // vec3 specular = lightColor * (spec * material.specular);  

    // Diffuse
    vec3 lightPos = vec3(-10.0f, -10.0f, -10.0f);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(Normal, lightDir), 0.0f);
    vec3 diffuse = diff * material.diffuse;

    vec3 result = ambient + diffuse;

    FragColor = vec4(result, 1.0f) * texture(texture_diffuse1, TexCoord);
    // FragColor = texture(texture_diffuse1, TexCoord);
}