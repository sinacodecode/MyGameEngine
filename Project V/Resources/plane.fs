#version 330 core
out vec4 FragColor;

struct Light{
    //vec3 direction;
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};
in vec3 Normal;
in vec3 FragPos;

uniform Light light;

void main()
{

    vec3 ambient = light.ambient * vec3(0.1, 0.1, 0.1);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    //vec3 lightDir = normalize( -light.direction );
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff *vec3(0.6, 0.6, 0.6));

    vec3 result = ambient + diffuse;
    FragColor = vec4(result, 1.0); // set all 4 vector values to 1.0
}