#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform vec4 colDiffuse;

void main()
{
    // Texel color fetching from texture sampler
    vec4 mainColors = texture2D(texture0, fragTexCoord.xy);
    vec4 textureMask = texture2D(texture1, fragTexCoord.xy);

    gl_FragColor = vec4(mainColors.rgb, textureMask.r);
}
