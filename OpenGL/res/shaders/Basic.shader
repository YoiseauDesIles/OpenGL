#shader vertex
#version 330 core
        
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 color;
layout(location = 3) in float texIndex;
        
out vec2 v_TexCoord;
out vec4 v_Color;
out float v_texIndex;
//Model View Projection matrix
uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * position;
    v_TexCoord = texCoord;
    v_Color = color;
    v_texIndex = texIndex;
};

#shader fragment
#version 330 core
        
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_texIndex;

uniform vec4 u_Color;
uniform sampler2D u_Texture[3];
        
void main()
{
    int index = int(v_texIndex);
    vec4 texColor = texture(u_Texture[index], v_TexCoord);
    color = texture(u_Texture[index], v_TexCoord);
};
