#version 460 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
};

out vec4 FragColor;
  
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform Material material;
uniform vec3 viewPos;
uniform DirectionalLight[8] directionalLights;
uniform int directionalLightsCount;
uniform PointLight[16] pointLights;
uniform int pointLightsCount;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient  = material.ambient;
    vec3 diffuse  = diff * light.color * light.intensity;
    vec3 specular = spec * material.specular;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - FragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (1.0f + 0.09f * distance + 0.032f * (distance * distance));    
    // combine results
    vec3 ambient  = material.ambient;
    vec3 diffuse  = diff * light.color * light.intensity;
    vec3 specular = spec * material.specular;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Directional lighting
    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    for(int i = 0; i < directionalLightsCount; i++){
        result += CalcDirLight(directionalLights[i], norm, viewDir);
    }
    for(int i = 0; i < pointLightsCount; i++){
        result += CalcPointLight(pointLights[i], norm, viewDir);
    }
    FragColor = vec4(vec3(texture(texture_diffuse1, TexCoord)) * result, 1.0f);
}