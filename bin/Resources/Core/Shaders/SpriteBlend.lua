------------ Vertex Shaders ------------

ogl_21_Vert = [[

uniform mat4 ProjectionView;
uniform vec3 offset;
uniform float angle;

attribute vec4 Position;
attribute vec4 Tex;
attribute vec4 Tex1;
attribute vec4 Tex2;

varying vec4 TexCoord;
varying vec4 TexCoord1;
varying vec4 TexCoord0;


void main()
{
	vec2 dir = vec2( cos( angle ), sin( angle ) );

	mat4 rotMat = mat4(dir.x,dir.y,0.0,0.0,
					  -dir.y,dir.x,0.0,0.0,
					   0.0,0.0,1.0,0.0,
					   0.0,0.0,0.0,1.0);

	vec4 rotPosition = rotMat*Position;
	TexCoord = Tex;
	TexCoord1 = Tex1;
	TexCoord2 = Tex2;
	gl_Position = ProjectionView*(rotPosition + vec4(offset,1.0));
}

]]

oglVert = [[

uniform mat4 ProjectionView;
uniform vec3 offset;
uniform float angle;

in vec4 Position;
in vec4 Tex;
in vec4 Tex1;
in vec4 Tex2;

out vec4 TexCoord;
out vec4 TexCoord1;
out vec4 TexCoord2;

void main()
{
	vec2 dir = vec2( cos( angle ), sin( angle ) );

	mat4 rotMat = mat4(dir.x,dir.y,0.0,0.0,
					  -dir.y,dir.x,0.0,0.0,
					   0.0,0.0,1.0,0.0,
					   0.0,0.0,0.0,1.0);

	vec4 rotPosition = rotMat*Position;
	TexCoord = Tex;
	TexCoord1 = Tex1;
	TexCoord2 = Tex2;
	gl_Position = ProjectionView*(rotPosition + vec4(offset,1.0));
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
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform float blend;

varying vec4 TexCoord;
varying vec4 TexCoord1;
varying vec4 TexCoord2;

void main()
{
    vec4 texel0 = texture2D( Texture0, TexCoord.xy);
    vec4 texel1 = texture2D( Texture1, TexCoord1.xy);

    gl_FragColor = mix(texel0, texel1, blend );
    //gl_FragColor = vec4( vec3(0.5), 1.0);
}


]]


oglFrag = [[

uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform float blend;

in vec4 TexCoord;
in vec4 TexCoord1;
in vec4 TexCoord2;
out vec4 FragColor;

void main()
{
    vec4 texel0 = texture2D( Texture0, TexCoord.xy);
    vec4 texel1 = texture2D( Texture1, TexCoord1.xy);

    FragColor = mix(texel0, texel1, blend );
    //FragColor = vec4( vec3(0.5), 1.0);
}


]]


DeclareShader('Fragment.GL20', ogl_21_Frag )
DeclareShader('Fragment.GL21', ogl_21_Frag )

DeclareShader('Fragment.GL30', oglFrag )
DeclareShader('Fragment.GL31', oglFrag )
DeclareShader('Fragment.GL32', oglFrag )
DeclareShader('Fragment.GL33', oglFrag )

DeclareShader('Fragment.GL40', oglFrag )
DeclareShader('Fragment.GL41', oglFrag )
