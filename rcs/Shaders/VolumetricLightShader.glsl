//#Begin_prop
//#End_prop

//#Begin_vert

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec3 FragPos;

out vec2 TexCoord;

uniform mat4 viewProjectionInverseMatrix;
uniform vec3 cameraDirection;

void main()
{
    gl_Position = vec4(aPos,1.f);
    TexCoord = aTexCoord;

    FragPos = aPos;
    
    //FragPos = (aPos*.5f)+.5f;
    //FragPos = vec3(viewProjectionInverseMatrix * vec4(FragPos,1.f));

    //FragPos = vec3(viewProjectionInverseMatrix * vec4((aPos*.5f)+.5f,1.f));

}

//#End_vert

//#Begin_frag

#version 330 core
out vec4 FragColor;
in vec3 FragPos;

in vec2 TexCoord;

uniform sampler2D screenZbuffer;//used to calculate if the volumetric light is behind an object
uniform sampler2D shadowMap;

uniform float farPlane;
uniform float nearPlane;

uniform mat4 shadowMatrices[4];
uniform int numCascades;
uniform float shadowBias = .000001;

uniform vec3 camPos, camDir;

uniform int marchingSteps = 10000;
uniform float marchingDistance = 1000;

uniform vec4  color;
uniform float intensity = 1;
uniform float airDensity = 1;
uniform float attenuation_linear = .1;
uniform float attenuation_quadratic = .05;

float calcLightDensity(vec3 camPos, vec3 direction, int steps, float distance, float obstacleZ);
float ShadowCalculation(vec4 posShadowSpace, int cascadeIndex);

uniform mat4 viewProjectionInverseMatrix;
uniform mat4 viewMatrix;

void main()
{
    //define coordinate of the pixel in clip-space 
    vec4 coord2D = vec4( FragPos.x, FragPos.y, FragPos.z, 1.0 );

    //project 2D coordinate into 3D
    vec4 p = viewProjectionInverseMatrix * coord2D;

    //divide by W to convert from homogeneous?
    vec3 pos3D = vec3( p.x / p.w, p.y / p.w, p.z / p.w );

    //compute ray direction
    vec3 ray_dir = normalize( pos3D - camPos );

    float obstaculeZ = texture(screenZbuffer, TexCoord).r;

    float density = calcLightDensity( camPos, ray_dir, marchingSteps, marchingDistance, obstaculeZ);

    FragColor = vec4(color.xyz, density*intensity);
}

float LinearizeDepth(float depth){
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));	
}

float calcLightDensity(vec3 camPos, vec3 direction, int steps, float distance, float obstacleZ){

    direction = normalize(direction);

    vec3 pos = camPos;
    vec3 endPos = camPos + direction * LinearizeDepth(obstacleZ);

    float stepLenght = distance/steps;
    stepLenght = length(endPos-pos)/steps;

    float density = 0;
    float attenuation = 1;

    int i = 0;

    for(i = 0; i < steps; i++){
        pos = endPos - direction * stepLenght * i;//new position

        float dst = length(pos - camPos);

        attenuation =  1.f / (1.f + attenuation_linear + attenuation_quadratic * (dst));

        //calculate if the point it's in shadow
        float shadow = -1;
        int cascadeLayer = 0;

        while(shadow == -1 && cascadeLayer < numCascades && cascadeLayer < 4){
            vec4 shadowSpacePos = shadowMatrices[cascadeLayer] * vec4(pos,1.f);
            
            shadow = ShadowCalculation(shadowSpacePos, cascadeLayer);
            cascadeLayer++;
        }

        if(shadow == -1){
            //out of all shadowmaps
            break;
        }else{
            density += airDensity*(1 - shadow)*(1-attenuation);
        }
    }

    return density/steps;
}


float ShadowCalculation(vec4 posShadowSpace, int cascadeIndex){
    // perform perspective divide
    vec3 projCoords = posShadowSpace.xyz / posShadowSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    float xOffset = cascadeIndex*(1.f/numCascades);

    projCoords.x = projCoords.x/numCascades;//normalizing
    projCoords.x += xOffset;//offsetting


    float maxXvalue = (1.f/numCascades ) + xOffset;
    maxXvalue = (1.f/numCascades) + xOffset;

    //Check if it's out
    if(projCoords.x < xOffset || projCoords.x > maxXvalue || projCoords.y < 0 || projCoords.y > 1){
        return -1;
    }

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z; 

    float bias = shadowBias;

    float shadow = 0.0;

    //shadow = currentDepth - shadowBias >  closestDepth? 1.0 : 0.0;

    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1 ; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;

            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;


    return shadow;
}

//#End_frag
