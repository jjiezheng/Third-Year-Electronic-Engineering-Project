------------ Vertex Shaders ------------

ogl_21_Vert = [[
uniform mat4 Model;
uniform mat4 ProjectionView;
uniform vec2 Centre;
uniform float Z;

attribute vec4 Position;

void main()
{
	gl_Position = ProjectionView*( vec4(Centre,Z,1.0) + Model*Position );
}
]]

oglVert = [[
uniform mat4 Model;
uniform mat4 ProjectionView;
uniform vec2 Centre;
uniform float Z;

in vec4 Position;

void main()
{
	gl_Position = ProjectionView*( vec4(Centre,Z,1.0) + Model*Position );
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
uniform vec3 Colour;
void main()
{
    gl_FragColor = vec4(Colour, 1.0);
}
]]


oglFrag = [[
uniform vec3 Colour;
uniform float alpha;
out vec4 FragColor;
void main()
{
    FragColor = vec4(Colour, alpha);
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
