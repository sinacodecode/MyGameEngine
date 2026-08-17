#version 330 core
out vec4 FragColor;

struct Material{
sampler2D diffuse;
sampler2D specular;
float specPow;
};

struct dirLight{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//struct spotLight{
//    vec3 position;
//    vec3 direction;
//    float cutoff;
//    float outercutoff;
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;

//    float constant;
//    float linear;
//    float quadratic;

//};

struct pointLight{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

};

#define NR_POINT_LIGHTS 4

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material;
uniform dirLight dirlight;
uniform pointLight pointlight[NR_POINT_LIGHTS];
//uniform spotLight spotlight;
uniform vec3 viewPos; 

vec3 directionalLighting(dirLight light, vec3 normal, vec3 viewDir);
vec3 pointLighting(pointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
//vec3 spotLighting();

void main()
{
 
        vec3 norm = normalize(Normal);

        vec3 viewDir = normalize(viewPos - FragPos);

        vec3 output = vec3(0.0);

        //output += directionalLighting(dirlight, norm, viewDir);

        for (int i = 0; i < NR_POINT_LIGHTS; i++){
            output += pointLighting(pointlight[i], norm, FragPos, viewDir);
        }

        //output += spotLighting();

        FragColor = vec4(output, 1.0);

}

//directional light function definition
vec3 directionalLighting(dirLight light, vec3 normal, vec3 viewDir){

    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir),0.0f);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.specPow);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)); 
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)); 
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

//point light function definition
vec3 pointLighting(pointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir),0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.specPow);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)); 
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)); 
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    //ambient += attenuation;
    //diffuse += attenuation;
    //specular += attenuation;

    return (ambient + diffuse);
}

//spot light function definition
//vec3 spotLighting(){

//}