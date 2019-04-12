#version 460

uniform sampler2D diffuse;

in vec2 texCoord0;
in vec3 normal0;
in vec3 position0;
in vec3 LightDirection_cameraspace;
in vec3 EyeDirection_cameraspace;
uniform vec3 LightPosition;
uniform float Time;

out vec4 color;

void main()
{

	vec2 uv = texCoord0;
    float zoom = pow(10.,6.*-abs(sin(Time/5.)));

	uv -= .5;

	vec2 c = uv*zoom*3.;
    c+=vec2(-.69955,.37999);
    
    vec2 z = vec2(0.);
    float iter = 0.;
    
    const float max_iter = 120.;
    
    for (int i=0; i < max_iter; i++)
    {
        z = vec2(z.x*z.x - z.y*z.y, 2.*z.x*z.y) + c;
        if (length(z)>2.)break;
        iter++;
    }
    float f = iter/max_iter;
    f = pow(f,.25);
    
    vec3 col = vec3(f);

    color = vec4(col,1.0);

}