------------ Vertex Shaders ------------

ogl_21_Vert = [[

uniform mat4 ProjectionView;
uniform vec3 offset;
uniform float angle;

attribute vec4 Position;
attribute vec4 Tex;
varying vec4 TexCoord;


void main()
{
	vec2 dir = vec2( cos( angle ), sin( angle ) );

	mat4 rotMat = mat4(dir.x,dir.y,0.0,0.0,
					  -dir.y,dir.x,0.0,0.0,
					   0.0,0.0,1.0,0.0,
					   0.0,0.0,0.0,1.0);

	vec4 rotPosition = rotMat*Position;
	TexCoord = Tex;
	gl_Position = ProjectionView*(rotPosition + vec4(offset,1.0));
}

]]

oglVert = [[

uniform mat4 ProjectionView;
uniform vec3 offset;
uniform float angle;

in vec4 Position;
in vec4 Tex;
out vec4 TexCoord;

void main()
{
	vec2 dir = vec2( cos( angle ), sin( angle ) );

	mat4 rotMat = mat4(dir.x,dir.y,0.0,0.0,
					  -dir.y,dir.x,0.0,0.0,
					   0.0,0.0,1.0,0.0,
					   0.0,0.0,0.0,1.0);

	vec4 rotPosition = rotMat*Position;
	TexCoord = Tex;
	gl_Position = ProjectionView*(rotPosition + vec4(offset,1.0));
}

]]

oglVertNew = [[

uniform mat4 ProjectionView;
uniform vec3 offset;
uniform float angle;

in vec4 Position;
in vec4 Tex;
out vec4 TexCoord;

void main()
{
	float num = gl_InstanceID;
	vec2 dir = vec2( cos( angle ), sin( angle ) );

	mat4 rotMat = mat4(dir.x,dir.y,0.0,0.0,
					  -dir.y,dir.x,0.0,0.0,
					   0.0,0.0,1.0,0.0,
					   0.0,0.0,0.0,1.0);

	vec4 rotPosition = rotMat*Position;
	TexCoord = Tex;
	gl_Position = ProjectionView*(rotPosition + vec4(offset,1.0) + num/300.0 );
}

]]

DeclareShader('Vertex.GL20', ogl_21_Vert )
DeclareShader('Vertex.GL21', ogl_21_Vert )

DeclareShader('Vertex.GL30', oglVert)
DeclareShader('Vertex.GL31', oglVertNew )
DeclareShader('Vertex.GL32', oglVertNew )
DeclareShader('Vertex.GL33', oglVertNew )

DeclareShader('Vertex.GL40', oglVertNew )
DeclareShader('Vertex.GL41', oglVertNew )


------------ Fragment Shaders ------------

ogl_21_Frag = [[

uniform sampler2D Texture0;
varying vec4 TexCoord;

void main()
{
    gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);
}


]]


oglFrag = [[

uniform sampler2D Texture0;

in vec4 TexCoord;
out vec4 FragColor;

void main()
{
	FragColor = vec4(0.0, 1.0, 1.0, 1.0);
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
