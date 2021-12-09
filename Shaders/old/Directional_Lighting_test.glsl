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
//uniform vec3 lightPos;

uniform vec3 viewPos;

struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform DirectionalLight directional_light;


void main()
{
    //ambient
    vec3 ambient = directional_light.ambient * material.ambient * vec3(texture(material.diffuse, TexCoords));

    //diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-directional_light.direction);
//vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = directional_light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    //specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = directional_light.specular * spec *  vec3(texture(material.specular, TexCoords));

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);

   
}