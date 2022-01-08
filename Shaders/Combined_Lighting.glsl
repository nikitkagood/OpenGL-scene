// "//" at the START (i.e. symbol 0 and 1) of a line means this comment wont get into shader source code in runtime
//obviously each comment slows down reading from the file a little

#shader vertex
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    FragPos = vec3(model * vec4(position, 1.0f));
    Normal = mat3(transpose(inverse(model))) * normal;
    TexCoords = aTexCoords;
}


#shader fragment
#version 330 core

out vec4 color;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 viewPos;

uniform unsigned int nr_materials;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
//sampler2D normal;
//sampler2D height;

    float shininess;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
};

#define NR_MAX_MATERIALS 16
#define NR_MAX_DIRECTIONAL_LIGHTS 16
#define NR_MAX_POINT_LIGHTS 16
#define NR_MAX_SPOT_LIGHTS 16

uniform Material materials[NR_MAX_MATERIALS];
uniform DirectionalLight directional_lights[NR_MAX_DIRECTIONAL_LIGHTS];
uniform PointLight point_lights[NR_MAX_POINT_LIGHTS];
uniform SpotLight spot_lights[NR_MAX_SPOT_LIGHTS];

//function prototypes
vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

//one directional light is mandatory
    vec3 result = CalcDirectionalLight(directional_lights[0], norm, viewDir);
    for(int i = 1; i < NR_MAX_DIRECTIONAL_LIGHTS; ++i)
    {
        result += CalcDirectionalLight(directional_lights[i], norm, viewDir);
    }
    for(int i = 0; i < NR_MAX_POINT_LIGHTS; ++i)
    {
        result += CalcPointLight(point_lights[i], norm, FragPos, viewDir);
    }
    for(int i = 0; i < NR_MAX_POINT_LIGHTS; ++i)
    {
        result += CalcSpotLight(spot_lights[i], norm, FragPos, viewDir);    
    }

    color = vec4(result, 1.0);
}

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    DirectionalLight empty_struct;
    if(light == empty_struct)
    {
        return vec3(0.0, 0.0, 0.0);
    }

    vec3 lightDir = normalize(-light.direction);

    //diffused
    float diff = max(dot(normal, lightDir), 0.0);
    //specular
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec;
    for(int i = 0; i < nr_materials; ++i)
    {
        spec += pow(max(dot(viewDir, reflectDir), 0.0), materials[i].shininess);
    }

    //combined
    vec3 ambient;
    for(int i = 0; i < nr_materials; ++i)
    {
        ambient  += light.ambient  * vec3(texture(materials[i].diffuse, TexCoords));
    }

    vec3 diffuse;
    for(int i = 0; i < nr_materials; ++i)
    {
        diffuse  += light.diffuse  * diff * vec3(texture(materials[i].diffuse, TexCoords));
    }

    vec3 specular;
    for(int i = 0; i < nr_materials; ++i)
    {
        specular += light.specular * spec * vec3(texture(materials[i].specular, TexCoords));
    }

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    PointLight empty_struct;
    if(light == empty_struct)
    {
        return vec3(0.0, 0.0, 0.0);
    }

    vec3 lightDir = normalize(light.position - fragPos);

    //diffused
    float diff = max(dot(normal, lightDir), 0.0);

    //specular
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec;
    for(int i = 0; i < nr_materials; ++i)
    {
        spec += pow(max(dot(viewDir, reflectDir), 0.0), materials[i].shininess);
    }

    //fade
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    //combined
    vec3 ambient;
    for(int i = 0; i < nr_materials; ++i)
    {
        ambient  += light.ambient  * vec3(texture(materials[i].diffuse, TexCoords));
    }
    vec3 diffuse;
    for(int i = 0; i < nr_materials; ++i)
    {
        diffuse  += light.diffuse  * diff * vec3(texture(materials[i].diffuse, TexCoords));
    }
    vec3 specular;
    for(int i = 0; i < nr_materials; ++i)
    {
        specular += light.specular * spec * vec3(texture(materials[i].specular, TexCoords));
    }

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
} 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    SpotLight empty_struct;
    if(light == empty_struct)
    {
        return vec3(0.0, 0.0, 0.0);
    }

    vec3 lightDir = normalize(light.position - fragPos);

    //ambient
    vec3 ambient;
    for(int i = 0; i < nr_materials; ++i)
    {
        ambient += light.ambient * vec3(texture(materials[i].diffuse, TexCoords));
    }

    //diffuse 
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse;
    for(int i = 0; i < nr_materials; ++i)
    {
        diffuse += light.diffuse * diff * vec3(texture(materials[i].diffuse, TexCoords));
    }

    //specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec;
    for(int i = 0; i < nr_materials; ++i)
    {
        spec += pow(max(dot(viewDir, reflectDir), 0.0), materials[i].shininess);
    }

    vec3 specular = light.specular * spec *  vec3(texture(materials[0].specular, TexCoords));
    for(int i = 0; i < nr_materials; ++i)
    {
        specular += light.specular * spec *  vec3(texture(materials[i].specular, TexCoords));
    }

    //soft light edges
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);  
    diffuse  *= intensity;
    specular *= intensity;

    //fade
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);

} 