#version 330 core

#define NR_POINT_LIGHTS 4

out vec4 FragColor;

vec4 gray = vec4(0.0f,0.0f,0.0f,0.0f);
uniform int numberOfPointLights;
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct DirectionalLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectionalLight dirLight;

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform SpotLight spotLight;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

uniform bool hasSpotLight;
uniform bool hasPointLights;
uniform bool hasDirLight;


vec4 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec4 ambient = vec4(light.ambient, 1.0f) * vec4(texture(material.diffuse, TexCoords));

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec4 diffuse = vec4(light.diffuse, 1.0f) * diff * vec4(texture(material.diffuse, TexCoords));

    vec3 reflectDir = reflect(-lightDir, normal);
    //need to add material shininess
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec4 specular = vec4(light.specular, 1.0f) * spec * vec4(texture(material.specular, TexCoords));

    float distance = length(light.position - fragPos);
    float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient+diffuse+specular);
}

vec4 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec4 ambient = vec4(light.ambient, 1.0f) * vec4(texture(material.diffuse, TexCoords));

    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec4 diffuse = vec4(light.diffuse, 1.0f) * diff * vec4(texture(material.diffuse, TexCoords));

    vec3 reflectDir = reflect(-lightDir, normal);
    //need to add material shininess
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec4 specular = vec4(light.specular, 1.0f) * spec * vec4(texture(material.specular, TexCoords));


    return (ambient+diffuse+specular);
}

vec4 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);

    //need to add material shininess
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec4 ambient = vec4(light.ambient, 1.0f) * vec4(texture(material.diffuse, TexCoords));
    vec4 diffuse = vec4(light.diffuse, 1.0f) * diff * vec4(texture(material.diffuse, TexCoords));
    vec4 specular = vec4(light.specular, 1.0f) * spec * vec4(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    
    return (ambient + diffuse + specular);

}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec4 result = gray;


    if(hasPointLights)
    {
    for(int i = 0; i < numberOfPointLights; i++)
        {
            result += calcPointLight(pointLights[i], norm, FragPos, viewDir);
        }
    }

    if(hasSpotLight)
    {
        result += calcSpotLight(spotLight, norm, FragPos, viewDir);
    }

    if(hasDirLight)
    {
        result += calcDirLight(dirLight, norm, viewDir);
    }

    FragColor = vec4(result);
}