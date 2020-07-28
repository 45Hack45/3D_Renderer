
//#Begin_prop
(int        use_Transparency    tranparency)
(vec4       dye_color           myColor)
(float      specular_shinines   shinines)
(float      specular_intensity  specularIntensity)
(sampler2D  texture_diffuse1    albedoTexture)
//#End_prop



//#Begin_vert----------------------------------------------------------------------------

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    TexCoord = aTexCoord;

    FragPos = vec3(model * vec4(aPos, 1.0));

    Normal = aNormal;

    //Normal = mat3(transpose(inverse(model))) * aNormal;//
}

//#End_vert




//#Begin_frag----------------------------------------------------------------------------

#version 330 core

#define MAX_LIGHTS 10

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

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;

uniform vec4 dye_color = vec4(1.f);
uniform int use_Transparency = 1;

uniform float ambientLight;
uniform vec4 ambientColor;

uniform float specular_shinines = 32.f;
uniform float specular_intensity = 1.f;


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

    if(albedo.a == 0.0f)
        discard;

    //Ambient light
    vec4 ambient = ambientLight * ambientColor;

    vec3 lightDir= normalize(lightsrc_directional_direction);//directional light direction

    vec4 lightColor = lightsrc_directional_color;
    float lightIntensity = lightsrc_directional_intensity;
    vec3 normal = normalize(Normal);


    //Directional Light Diffuse
    float NDotL = max(dot(normal, lightDir), 0.0);

    vec4 diffuse = lightColor * NDotL * lightIntensity;


    //Directional Light Specular
    
//    vec3 viewDir = normalize(viewPos - FragPos);
//    vec3 reflectDir = reflect(-lightDir, normal);
//
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specular_shinines);
//    
//    vec4 specular = spec * lightColor * lightIntensity;
    

    vec3 viewDir    = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), specular_shinines);
    vec4 specular = spec * lightColor * lightIntensity * specular_intensity;



    //Multiple Light
    vec4 pointLight = vec4(.0f);
    vec4 spotLight = vec4(.0f);

    for(int i = 0; i < nPointLights; i++){
        pointLight += calculatePointLightPhong( viewPos,FragPos, normal, pointLights[i]);
    }

    for(int i = 0; i < nSpotLights; i++){
        spotLight += calculateSpotLightPhong( viewPos,FragPos, normal, spotLights[i]);
    }

    FragColor = dye_color * albedo * (ambient + diffuse + specular + pointLight + spotLight) ;
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
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), specular_shinines);
    vec4 specular = spec * light.color * light.intensity  * specular_intensity;

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
    float epsilon   = light.angle-light.outerAngle;
    float intensity = clamp((theta - light.outerAngle) / epsilon, 0.0, 1.0);



    //Diffuse
    float NDotL = max(dot(normal, lightDir), 0.0);

    vec4 diffuse = light.color * NDotL;

    //Specular
    vec3 viewDir = normalize(cam_position - frag_position);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(normal, halfwayDir), 0.0), specular_shinines);
    vec4 specular = spec * light.color * light.intensity  * specular_intensity;

    return (diffuse + specular) * light.intensity * attenuation * intensity;
}

//#End_frag
