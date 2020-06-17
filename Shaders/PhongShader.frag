#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;

uniform float ambientLight;
uniform vec4 ambientColor;

uniform vec3 lightsrc_directional_direction;
uniform vec4 lightsrc_directional_color;
uniform float lightsrc_directional_intensity;

void main()
{
    //Albedo
    vec4 albedo = texture(texture_diffuse1, TexCoord);

    //Ambient light
    vec4 ambient = ambientLight * ambientColor;

    vec3 lightDir= normalize(lightsrc_directional_direction);
    vec4 lightColor = lightsrc_directional_color;
    float lightIntensity = lightsrc_directional_intensity;
    vec3 normal = normalize(Normal);

    //Diffuse
    
    float NDotL = max(dot(normal, lightDir), 0.0);

    vec4 diffuse = lightColor * NDotL * lightIntensity;

    //Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec4 specular = spec * lightColor;  


    FragColor = albedo * (ambient + diffuse + specular);
}