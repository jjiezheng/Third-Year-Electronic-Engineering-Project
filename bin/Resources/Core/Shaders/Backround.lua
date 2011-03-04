------------ Vertex Shaders ------------

oldStyleVert = [[

varying vec2 texcoord[2];

void main()
{
	gl_FrontColor = gl_Color;
	texcoord[0] = gl_MultiTexCoord0.xy;
	texcoord[1] = gl_MultiTexCoord1.xy;
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
uniform sampler2D Texture1;
varying vec2 texcoord[2];


#define BlendScreenf(base, blend) 		(1.0 - ((1.0 - base) * (1.0 - blend)))
#define BlendOverlayf(base, blend) 	(base < 0.5 ? (2.0 * base * blend) : (1.0 - 2.0 * (1.0 - base) * (1.0 - blend)))
#define BlendPhoenix(base, blend) 		(min(base, blend) - max(base, blend) + vec4(1.0))

vec3 BlendOverlay(vec3 base, vec3 blend)
{
  return vec3(BlendOverlayf(base.r, blend.r), BlendOverlayf(base.g, blend.g), BlendOverlayf(base.b, blend.b) );
}
vec3 BlendScreen(vec3 base, vec3 blend)
{
  return vec3(BlendScreenf(base.r, blend.r), BlendScreenf(base.g, blend.g), BlendScreenf(base.b, blend.b) );
}



// Performs “over”-type blending (colors must be premultiplied by alpha)
vec4 alphaBlendSimple(vec4 overlying, vec4 underlying)
{
  vec3 blended = overlying.rgb + ((1-overlying.a)*underlying.rgb);
  float alpha = underlying.a + (1-underlying.a)*overlying.a;
  return vec4(blended, alpha);
}


vec4 alphaBlend(vec3 newcolor, vec4 overlying, vec4 underlying)
{

  float alpha = overlying.a * underlying.a;
  vec3 blended = newcolor*alpha + ((1-alpha)*overlying.rgb);

  return alphaBlendSimple(vec4(blended, overlying.a), underlying);
}



void main()
{
  vec4 tex0 = texture2D( Texture0, texcoord[0].xy);
  vec4 tex1 = texture2D( Texture1, texcoord[1].xy);

  tex0 *= gl_Color.a;
  tex1 *= ( 1 - gl_Color.a );

  vec3 colour0 = BlendPhoenix( tex0.rgb, tex1.rgb );
  gl_FragColor = vec4( colour0, 1.0 );
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

