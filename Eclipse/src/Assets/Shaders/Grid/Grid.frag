#version 450 core

layout(location = 3) in vec3 nearPoint;
layout(location = 4) in vec3 farPoint; 

layout(location=0) out vec4 fFragClr;

uniform int QuadScale;
uniform int Inner;
uniform vec3 GridColour;
uniform mat4 viewMtx;
uniform mat4 projMtx;
uniform float CameraNearPlane;
uniform float CameraFarPlane;
uniform bool CheckDraw;
uniform int Z_Thickness;
uniform int X_Thickness;
uniform float XAxis_Colour;
uniform float ZAxis_Colour;

vec4 grid(vec3 fragPos3D, int scale, bool drawAxis) 
{
   if( drawAxis == false )
   {
      return vec4(0,0,0,0);
   }
   else
   {   
    vec2 coord = fragPos3D.xz / scale;

    vec2 derivative = fwidth(coord);

    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;

    float line = min(grid.x, grid.y);

    float minimumz = min(derivative.y, 1);

    float minimumx = min(derivative.x, 1);

    vec4 color = vec4(GridColour.x, GridColour.y, GridColour.z , 1.0 - min(line, 1.0));

      // z axis
      if(fragPos3D.x > -Z_Thickness * minimumx && fragPos3D.x < Z_Thickness * minimumx)
      {
            color.z = ZAxis_Colour;
      }

      // x axis
      if(fragPos3D.z > -X_Thickness * minimumz && fragPos3D.z < X_Thickness * minimumz)
      {
            color.x = XAxis_Colour;
      }

    return color;

    }
}

float computeDepth(vec3 pos) 
{
    vec4 ClipSpacePosition = projMtx * viewMtx * vec4(pos.xyz, 1.0);
    float ClipSpaceDepth =  (ClipSpacePosition.z / ClipSpacePosition.w);
    float far = gl_DepthRange.far;
    float near = gl_DepthRange.near;
    float depth = (((far-near) * ClipSpaceDepth) + near + far) / 2.0;

    return depth;
}

float computeLinearDepth(vec3 pos) 
{
    // we swill clipp it again
    vec4 ClipSpacePosition = projMtx * viewMtx * vec4(pos.xyz, 1.0);
    float ClipSpaceDepth = (ClipSpacePosition.z / ClipSpacePosition.w) * 2.0 - 1.0; 
    float linearDepth = (2.0 * CameraNearPlane * CameraFarPlane) / (CameraFarPlane + CameraNearPlane - ClipSpaceDepth * (CameraFarPlane - CameraNearPlane)); 

    return linearDepth / CameraFarPlane; // normalize
}

void main () 
{
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);

    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);

    gl_FragDepth = computeDepth(fragPos3D);

    // fk check this again
    float linearDepth = computeLinearDepth(fragPos3D);

    float fading = max(0, (0.5 - linearDepth));

    fFragClr =(grid(fragPos3D, QuadScale, CheckDraw) + grid(fragPos3D, Inner , CheckDraw)) * float(t > 0); 

    fFragClr.a *= fading;

    if ( fFragClr.a == 0.0 )
    {
      discard;
    }

}
