//#Begin_prop
//#End_prop

//#Begin_vert

#version 430 core
layout (location = 0) in vec3 aPos;

out vec4 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = model * vec4(aPos, 1.0);
}

//#End_vert



//#Begin_frag

#version 430 core

out vec4 FragColor;

in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{
    // get distance between fragment and light source
    float lightDistance = length(FragPos.xyz - lightPos);
    
    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / far_plane;

    FragColor = vec4(1) * lightDistance;
    FragColor.a = 1.f;
}

//#End_frag
