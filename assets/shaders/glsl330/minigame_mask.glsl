#version 330

// Input vertex attributes (from vertex shader)
in vec3 vertexPos;
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform vec4 colDiffuse;

out vec4 finalColor;

void main()
{
    // Texel color fetching from texture sampler
    vec4 texColor = texture(texture0, fragTexCoord);
    vec4 maskColor = texture(texture1, vec2(fragTexCoord.x, fragTexCoord.y+0.005)); // I don't know WHY I need this.

    finalColor = vec4(texColor.rgb, maskColor.r);
}
