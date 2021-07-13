//#Begin_prop
(vec4       dye_color           myColor)
(sampler2D  texture_diffuse1    albedoTexture)
//#End_prop

//#Begin_vert

#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;

uniform mat4 viewProjection;
uniform mat4 model;

void main()
{
    gl_Position = model * viewProjection * vec4(aPos, 1.0);

    //Normal = aNormal;

    Normal = mat3(transpose(inverse(model))) * aNormal;
}

//#End_vert

//#Begin_frag

#version 430 core
out vec4 FragColor;

in vec2 TexCoord;

vec4 dye_color = vec4(1.f);

uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(texture_diffuse1, TexCoord) * dye_color;
}

//#End_frag
