#version 120

uniform float near_plane;
uniform float far_plane;
uniform sampler2D colorImage;

float LinearizeDepth(in vec2 uv)
{
    float zNear = near_plane;    // TODO: Replace by the zNear of your perspective projection
    float zFar  = far_plane; // TODO: Replace by the zFar  of your perspective projection
    float depth = texture2D(colorImage, uv).x;
    return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}


void main()
{       
    float c = LinearizeDepth(gl_TexCoord[0].st);
    gl_FragColor = vec4(c, c, c, 1.0);
}