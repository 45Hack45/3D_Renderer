#version 330 core

#define MAX_LIGHTS 10

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

struct SpotLight{
    vec4 color;
    float intensity;
    float linear,quadratic;
    vec3 direction;
    vec3 position;
    float angle;
    float outerAngle;
};
struct PointLight{
    vec4 color;
    vec3 position;
    float intensity;
    float linear,quadratic;
};


uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;

uniform float ambientLight;
uniform vec4 ambientColor;


uniform vec3 lightsrc_directional_direction;
uniform vec4 lightsrc_directional_color;
uniform float lightsrc_directional_intensity;

uniform PointLight pointLights[MAX_LIGHTS];
uniform SpotLight spotLights[MAX_LIGHTS];
uniform int nPointLights;
uniform int nSpotLights;


vec4 calculatePointLightPhong(vec3 cam_position,vec3 frag_position, vec3 normal,const PointLight light);
vec4 calculateSpotLightPhong(vec3 cam_position,vec3 frag_position, vec3 normal,const SpotLight light);

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


    //Directional Light Diffuse
    float NDotL = max(dot(normal, lightDir), 0.0);

    vec4 diffuse = lightColor * NDotL * lightIntensity;


    //Directional Light Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec4 specular = spec * lightColor * lightIntensity;


    //Other Light
    vec4 pointLight = vec4(.0f);
    vec4 spotLight = vec4(.0f);

    for(int i = 0; i < nPointLights; i++){
        pointLight += calculatePointLightPhong( viewPos,FragPos, normal, pointLights[i]);
    }

    for(int i = 0; i < nSpotLights; i++){
        spotLight += calculateSpotLightPhong( viewPos,FragPos, normal, spotLights[i]);
    }

    FragColor = albedo * (ambient + diffuse + specular + pointLight + spotLight) ;
    //FragColor = vec4(pointLight);
}

vec4 calculatePointLightPhong(vec3 cam_position,vec3 frag_position, vec3 normal,const PointLight light){

    //Attenuation
    vec3 lightDir = light.position-frag_position;
    float dst = length(lightDir);
    lightDir = normalize(lightDir);

    float attenuation =  1.f / (1.f + light.linear * dst + light.quadratic * (dst * dst));

    //Diffuse
    float NDotL = max(dot(normal, lightDir), 0.0);

    vec4 diffuse = light.color * NDotL;

    //Specular
    vec3 viewDir = normalize(cam_position - frag_position);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec4 specular = spec * light.color;

    return (diffuse + specular) * light.intensity * attenuation;
}

vec4 calculateSpotLightPhong(vec3 cam_position,vec3 frag_position, vec3 normal,const SpotLight light){

    //Attenuation
    vec3 lightDir = light.position-frag_position;

    float dst = length(lightDir);
    lightDir = normalize(lightDir);

    float attenuation =  1.f / (1.f + light.linear * dst + light.quadratic * (dst * dst));

    //Spot cutoff
    float theta = dot(lightDir, normalize(-light.direction));

    if(theta < light.angle) 
    {// not do lighting calculations
      return vec4(.0f);
    }

    //Diffuse
    float NDotL = max(dot(normal, lightDir), 0.0);

    vec4 diffuse = light.color * NDotL;

    //Specular
    vec3 viewDir = normalize(cam_position - frag_position);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec4 specular = spec * light.color;

    return (diffuse + specular) * light.intensity * attenuation;
}




