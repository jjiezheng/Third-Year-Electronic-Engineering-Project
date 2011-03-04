------------ Vertex Shaders ------------

ogl_21_Vert = [[

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 Colour;


attribute vec4 Position;
varying vec3 colour;


void main()
{
	colour = Colour;
	gl_Position = Projection*View*Model*Position;
}

]]

oglVert = [[

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 Colour;


in vec4 Position;
out vec3 colour;


void main()
{
	colour = Colour;
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

varying vec3 colour;

void main()
{
    gl_FragColor = vec4(colour, 1.0);
}


]]


oglFrag = [[

in vec3 colour;
out vec4 FragColor;

void main()
{
    FragColor = vec4(colour, 1.0);
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
