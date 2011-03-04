------------ Vertex Shaders ------------

ogl_21_Vert = [[

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

attribute vec4 Position;
attribute vec4 Tex;
varying vec4 TexCoord;

void main()
{
	TexCoord = Tex;
	gl_Position = Projection*View*Model*Position;

}

]]

oglVert = [[

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

in vec4 Position;
in vec4 Tex;
out vec4 TexCoord;

void main()
{
	TexCoord = Tex;
	gl_Position = Projection*View*Model*Position;

}

]]


DeclareShader('Vertex.GL20', ogl_21_Vert )
DeclareShader('Vertex.GL21', ogl_21_Vert )

DeclareShader('Vertex.GL30', oglVert)
DeclareShader('Vertex.GL31', oglVert )
DeclareShader('Vertex.GL32', oglVert )
DeclareShader('Vertex.GL33', oglVert )

DeclareShader('Vertex.GL40', oglVert )
DeclareShader('Vertex.GL41', oglVert )


------------ Fragment Shaders ------------

ogl_21_Frag = [[

uniform sampler2D Texture0;
uniform vec4 Colour;

varying vec4 TexCoord;

void main()
{
    vec4 tex = texture2D( Texture0, TexCoord.xy);
    gl_FragColor = vec4( Colour.rgb, tex.a*Colour.a );
}


]]


oglFrag = [[

uniform sampler2D Texture0;
uniform vec4 Colour;

in vec4 TexCoord;
out vec4 FragColor;

void main()
{
    vec4 tex = texture2D( Texture0, TexCoord.xy);
    FragColor = vec4( Colour.rgb, tex.a*Colour.a );
}


]]

oglFragGlow = [[

uniform sampler2D Texture0;
uniform vec4 Colour;

in vec4 TexCoord;
out vec4 FragColor;


const float width = 512.0;
const float height = 512.0;
const float step_w = 1.0/width;
const float step_h = 1.0/height;

#define KERNEL_SIZE 9

const float kernel[KERNEL_SIZE] = float[KERNEL_SIZE]( 1.0/16.0, 2.0/16.0, 1.0/16.0,
													  2.0/16.0, 4.0/16.0, 2.0/16.0,
													  1.0/16.0, 2.0/16.0, 1.0/16.0 );

const vec2 offset[KERNEL_SIZE] = vec2[KERNEL_SIZE]( vec2(-step_w, -step_h), vec2(0.0, -step_h), vec2(step_w, -step_h),
													vec2(-step_w, 0.0), 	   vec2(0.0, 0.0), 	   vec2(step_w, 0.0),
													vec2(-step_w, step_h),  vec2(0.0, step_h),  vec2(step_w, step_h) );



#define BlendOverlayf(base, blend) 	(base < 0.5 ? (2.0 * base * blend) : (1.0 - 2.0 * (1.0 - base) * (1.0 - blend)))
vec3 BlendOverlay(vec3 base, vec3 blend)
{
  return vec3(BlendOverlayf(base.r, blend.r), BlendOverlayf(base.g, blend.g), BlendOverlayf(base.b, blend.b) );
}

void main()
{

	vec4 sum = vec4(0.0);
	vec2 texcoord = vec2(TexCoord);
	vec4 original = texture2D( Texture0, texcoord);
	original.rgb = Colour.rgb;


	int i = 0;
	for( i=0; i<KERNEL_SIZE; i++ )
	{
		vec4 tmp = texture2D(Texture0, texcoord + offset[i]);
		tmp.rgb = Colour.rgb;
		sum += tmp * kernel[i];
	}

	//FragColor = vec4( BlendOverlay( original.rgb, sum.rgb ), sum.a );
	FragColor = sum;

//	FragColor = vec4(1.0);
}









/*
void main()
{
   vec4 sum = vec4(0);
   vec2 texcoord = vec2(TexCoord.xy);
   int j;
   int i;

	for( i= -4 ;i < 4; i++)
	{
		for (j = -3; j < 3; j++)
		{
			sum += texture2D(Texture0, texcoord + vec2(j, i)*0.004) * 0.25;
		}
	}

	if (texture2D(Texture0, texcoord).r < 0.3)
    {
       FragColor = vec4(sum*sum*0.012 + texture2D(Texture0, texcoord))*Colour;
    }
    else
    {
        if (texture2D(Texture0, texcoord).r < 0.5)
        {
            FragColor = vec4(sum*sum*0.009)*Colour;
        }
        else
        {
            FragColor = vec4(sum*sum*0.0075 )*Colour;
        }
    }
}*/
]]

DeclareShader('Fragment.GL20', ogl_21_Frag )
DeclareShader('Fragment.GL21', ogl_21_Frag )

DeclareShader('Fragment.GL30', oglFrag )
DeclareShader('Fragment.GL31', oglFrag )
DeclareShader('Fragment.GL32', oglFrag )
DeclareShader('Fragment.GL33', oglFrag )

DeclareShader('Fragment.GL40', oglFrag )
DeclareShader('Fragment.GL41', oglFrag )

DeclareShader('Fragment.GL30.Glow', oglFragGlow )
DeclareShader('Fragment.GL31.Glow', oglFragGlow )
DeclareShader('Fragment.GL32.Glow', oglFragGlow )
DeclareShader('Fragment.GL33.Glow', oglFragGlow )

DeclareShader('Fragment.GL40.Glow', oglFragGlow )
DeclareShader('Fragment.GL41.Glow', oglFragGlow )
