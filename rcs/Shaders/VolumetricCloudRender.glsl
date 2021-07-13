//#Begin_prop
(vec4       dye_color           myColor)
//#End_prop

//#Begin_vert

#version 430 core
layout (location = 0) in vec3 aPos;

out vec3 FragPos;

uniform mat4 viewProjection;
uniform mat4 model;

uniform vec3 vResolution;

void main()
{
    gl_Position = model * viewProjection * vec4(aPos, 1.0);

    //gl_Position = vec4(aPos, 1.0);

    //FragPos = (aPos*0.5f) + vec3(0.5f);//Localspace position

    FragPos = aPos;
    //FragPos = vec3(FragPos.x/vResolution.x,FragPos.y/vResolution.y,FragPos.z/vResolution.z);

    
}

//#End_vert

//#Begin_frag

#version 430 core
in vec3 FragPos;

out vec4 FragColor;

uniform vec4 cameraPosition;//Cam position in cube localspace
uniform vec4 dye_color = vec4(1.f);

uniform sampler3D volume;

int samples = 128;

bool inRange(vec3 uv){
    return uv.x >= 0 && uv.x <= 1 &&
            uv.y >= 0 && uv.y <= 1 &&
            uv.z >= 0 && uv.z <= 1;
}

void main()
{
    vec3 dir = -normalize(FragPos - vec3(cameraPosition));
    vec3 pos = FragPos;

    float stepLength = 1.f/float(samples);

    float density = 0;
    int i = 1;

    while(i <= samples && inRange(pos)){
        density += texture(volume,pos).r;
        pos = pos + (dir * stepLength);
    }

    //FragColor = vec4(texture(volume,FragPos).r);
    //FragColor = vec4(FragPos.rbg,1);

    FragColor = vec4(dye_color.rgb, density/(i*50));

    //FragColor = vec4(vec3(texture(volume,FragPos).r),1);
}

//#End_frag
