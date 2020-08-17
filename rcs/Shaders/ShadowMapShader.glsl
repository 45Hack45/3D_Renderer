//#Begin_prop
(sampler2D  texture_diffuse1    albedoTexture)
//#End_prop

//#Begin_vert

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}

//#End_vert

//#Begin_frag

#version 330 core
layout(location = 0) out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(texture_diffuse1, TexCoord);
    //FragColor = vec4(0,0,1,1.f);
}

//#End_frag
