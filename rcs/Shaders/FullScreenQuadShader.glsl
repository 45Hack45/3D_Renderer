//#Begin_prop
(sampler2D  texture_diffuse1    albedoTexture)
//#End_prop

//#Begin_vert

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos,1.f);
    TexCoord = aTexCoord;
}

//#End_vert

//#Begin_frag

#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(texture_diffuse1, TexCoord);
    //FragColor = vec4(0,0,1,1.f);
}

//#End_frag
