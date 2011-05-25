------------ Vertex Shaders ------------

ogl_21_Vert = [[

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

attribute vec4 Position;
attribute vec2 UV_0;
varying vec2 TexCoord;

void main()
{
	TexCoord = UV_0;
	gl_Position = Projection*View*Model*Position;

}

]]

oglVert = [[

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

in vec4 Position;
in vec2 UV_0;
out vec2 TexCoord;

void main()
{
	TexCoord = UV_0;
	//TexCoord = UV_0;
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

varying vec2 TexCoord;

void main()
{
    gl_FragColor = texture2D( Texture0, TexCoord);
}


]]


oglFrag = [[

uniform sampler2D Texture0;

in vec2 TexCoord;
out vec4 FragColor;

void main()
{
	vec4 colour = texture2D( Texture0, TexCoord);
	FragColor = vec4( colour.xyz, colour.a );
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
