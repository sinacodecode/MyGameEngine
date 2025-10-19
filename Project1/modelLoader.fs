#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct Light
{
    vec3 position;
    vec3 direction;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
};

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform vec3 ambientLightColor;
uniform vec3 diffuseLightColor;
uniform Light light;
uniform vec3 lightPos;
uniform vec3 lightSpecular;
uniform vec3 viewPos;

void main()
{
    float ambientStrength = 0.3;
    vec3 ambientLight = ambientStrength * ambientLightColor;
    vec3 ambient = ambientLight * vec3(texture(texture_diffuse1, TexCoords));
    //vec3 ambient = ambientLight;
    //FragColor = vec4(ambientLight, 1.0);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuseLightColor * vec3(texture(texture_diffuse1, TexCoords));
    //ec3 diffuse = diff * diffuseLightColor;
    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256.0);
    vec3 specular = lightSpecular * spec * texture(texture_specular1, TexCoords).rgb; 
   // vec3 specular = lightSpecular * spec;

    vec3 result = (ambient + diffuse + specular) ;

    FragColor = vec4(result, 1.0);

    //FragColor = texture(texture_diffuse1, TexCoords);

    //FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}