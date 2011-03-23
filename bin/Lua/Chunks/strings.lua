function makeString( _text, _font, X, Y, _align )
	local String = stringObject()
	String.pos = vec3( X, Y, -950 )
	String.colour = vec4( 1, 1, 1,0.0 )
	String.alignTo = _align
	String.lua_string = _text

	String:font( _font ):text( _text ):shader( "String" ):depth( false )
	String:uniform( "Projection", perspective( 45.0, 16.0/10.0, 0.1, 1000.0 ) )
	String:uniform( "View", scale( rotate( translate( mat4(1.0), String.pos ), 180.0, vec3( 1.0,0.0,0.0) ), vec3(0.5) ) )
	String:uniform( "Colour", String.colour )
	return String
end

function UpdateText( text, _update )

	local _pos = vec3(0)

	if text.alignTo == "left" then
		_pos = text.pos + vec3(text:align():x(),0,0 )
	elseif text.alignTo == "right" then
		_pos = text.pos - vec3(text:align():x(),0,0 )
	else
		_pos = text.pos
	end

	text.lua_string = _update

	text:uniform( "View", scale( rotate( translate( mat4(1.0), _pos ), 180.0, vec3( 1.0,0.0,0.0) ), vec3(0.5) ) )
	text:uniform( "Colour", text.colour )
	text:text( _update )
end