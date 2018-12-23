#version 330 core
out vec4 FragColor;

struct Material {
   sampler2D diffuse;
   sampler2D specular;
   sampler2D emission;
   float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
   
    float distance = length(light.position - FragPos);
    float attenuation = 1.0/(light.constant +light.linear * distance + light.quadratic * (distance * distance));
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)) * attenuation;
    
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(-light.position);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)) * attenuation;
   
    //specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),material.shininess);
    vec3 specular = (light.specular) * spec * vec3(texture(material.specular, TexCoords)) * attenuation;

    float theta = length(light.position - FragPos);
    float epsion = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsion, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;



    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
