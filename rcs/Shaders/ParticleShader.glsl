//#Begin_prop
(vec4       dye_color           myColor)
(sampler2D  texture_diffuse1    albedoTexture)
//#End_prop

//#Begin_vert

#version 430 core
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec2 aPos_vertex;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aPos_instance;

out vec3 fColor;

uniform mat4 viewProjection;

void main()
{
    gl_Position = viewProjection * vec4(vec3(aPos_vertex,0.f) + aPos_instance, 1.0);
    //gl_Position.xy += vec2(.1f*gl_InstanceID);
    fColor = aColor;
}  

//#End_vert

//#Begin_frag

#version 430 core
out vec4 FragColor;
  
in vec3 fColor;

void main()
{
    FragColor = vec4(1,0,0, 1.0);
}


//#End_frag
