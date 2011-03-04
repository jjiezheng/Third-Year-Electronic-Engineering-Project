------------ Vertex Shaders ------------

oldStyleVert = [[

void main()
{
    gl_FrontColor = gl_Color;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

]]

DeclareShader('Vertex.GL20', oldStyleVert )
DeclareShader('Vertex.GL21', oldStyleVert )

DeclareShader('Vertex.GL30', oldStyleVert )
DeclareShader('Vertex.GL31', oldStyleVert )
DeclareShader('Vertex.GL32', oldStyleVert )
DeclareShader('Vertex.GL33', oldStyleVert )

DeclareShader('Vertex.GL40', oldStyleVert )
DeclareShader('Vertex.GL41', oldStyleVert )


------------ Fragment Shaders ------------

oldStyleFrag = [[

uniform sampler2D Texture0;

const float width = 1024.0;
const float height = 1024.0;
const float step_w = 1.0/width;
const float step_h = 1.0/height;

#define KERNEL_SIZE 9;

const float kernel[KERNEL_SIZE] = { 1.0/16.0, 2.0/16.0, 1.0/16.0,
									2.0/16.0, 4.0/16.0, 2.0/16.0,
									1.0/16.0, 2.0/16.0, 1.0/16.0 };

const vec2 offset[KERNEL_SIZE] = { vec2(-step_w, -step_h), vec2(0.0, -step_h), vec2(step_w, -step_h),
								   vec2(-step_w, 0.0), 	   vec2(0.0, 0.0), 	   vec2(step_w, 0.0),
								   vec2(-step_w, step_h),  vec2(0.0, step_h),  vec2(step_w, step_h) };



#define BlendOverlayf(base, blend) 	(base < 0.5 ? (2.0 * base * blend) : (1.0 - 2.0 * (1.0 - base) * (1.0 - blend)))
vec3 BlendOverlay(vec3 base, vec3 blend)
{
  return vec3(BlendOverlayf(base.r, blend.r), BlendOverlayf(base.g, blend.g), BlendOverlayf(base.b, blend.b) );
}

void main()
{
	vec4 sum = vec4(0.0);
	vec2 texcoord = vec2(gl_TexCoord[0]);
	vec4 original = texture2D( Texture0, gl_TexCoord[0].xy);
	original.rgb = gl_Color.rgb;


	int i = 0;


	for( i=0; i<KERNEL_SIZE; i++ )
	{
		vec4 tmp = texture2D(Texture0, texcoord + offset[i]);
		tmp.rgb = gl_Color.rgb;
		sum += tmp * kernel[i];
	}


	gl_FragColor = vec4( BlendOverlay( original, sum ), sum.a );

}

]]


DeclareShader('Fragment.GL20', oldStyleFrag )
DeclareShader('Fragment.GL21', oldStyleFrag )

DeclareShader('Fragment.GL30', oldStyleFrag )
DeclareShader('Fragment.GL31', oldStyleFrag )
DeclareShader('Fragment.GL32', oldStyleFrag )
DeclareShader('Fragment.GL33', oldStyleFrag )

DeclareShader('Fragment.GL40', oldStyleFrag )
DeclareShader('Fragment.GL41', oldStyleFrag )




