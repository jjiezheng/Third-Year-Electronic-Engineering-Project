dofile( include( "math" ) )
dofile( include( "ExamineCamera" ) )



if haptics.included == nill then
	local start_effect = haptics.start_effect

	haptics.start_effect = function( effect )
		return start_effect(
			effect.name or "constant",
			effect.pos  or vec3(0),
			effect.dir  or vec3(0),
			effect.gain or 0,
			effect.mag  or 0 )
	end
	haptics.included = true
end

function normalSpace( _matrix )
	return inverseTranspose( mat3(_matrix) )
end






function make_rect( _width, _height )

	local width = _width or 1
	local height = _height or 1

	local _vertexBuffer = vertexArray()
	local _elementBuffer = intArray()

	_vertexBuffer:add( vertex(  height/2, -width/2, -0, 1 ) )
    _vertexBuffer:add( vertex(  height/2,  width/2, -0, 1 ) )
    _vertexBuffer:add( vertex( -height/2, -width/2, -0, 1 ) )
    _vertexBuffer:add( vertex( -height/2,  width/2, -0, 1 ) )

    _elementBuffer:add( 0 )
    _elementBuffer:add( 1 )
    _elementBuffer:add( 2 )
    _elementBuffer:add( 3 )

	return _vertexBuffer, _elementBuffer
end











Projection = perspective( 65.0, 16.0/10.0, 1, 500.0 )
View = mat4(1.0)

Haptics_Demo_Globals = {}
function Haptics_Demo_Globals.Start(self)


	sys.title( "Haptics" )
	haptics.init()
	haptics.workspace( perspective( 65.0, 16.0/10.0, 1, 10.0 ), mat4(1) )
	self.camera = ExamineCamera()


	local borderVertBuff, borderElemBuff = make_rect(500, 500)
	local _vertexBuffer = vertexArray()

	_vertexBuffer:add( vertex( 1, 0, -0, 1 ) )
    _vertexBuffer:add( vertex( 1, 1, -0, 1 ) )
    _vertexBuffer:add( vertex( 0, 0, -0, 1 ) )
    _vertexBuffer:add( vertex( 0, 1, -0, 1 ) )

	self.background = {}

	self.background = mesh()
	self.background:shader("TexturedVbo"):vert_type("TriangleStrip")
	self.background:add("Position",borderVertBuff,borderElemBuff)
	self.background:add("UV_0",_vertexBuffer)
	self.background:texture( "Texture0", "Earth" )

	self.background:uniform( "Model", mat4(1.0) )
	self.background:uniform( "View", translate( mat4(1), vec3(0,0,-240) ) )
	self.background:uniform( "Projection", Projection )
	self.background:uniform( "NormalMatrix", mat4(1.0) )
	self.background:depth( true )









	self.running = false
	self.running = true
	self.effectType = "constant"
	self.effect = {}
	self.effect.name = "constant"
	self.effect.pos = vec3(0.5)
	self.effect.dir = vec3(0.5)
	self.effect.gain = 0.3
	self.effect.mag = 0.3
	self.effect.handle = haptics.start_effect{self.effect}









	self.wind = new_effect
	self.wind = {}
	self.wind.reload = true
	self.wind.name = "constant"
	self.wind.handle = haptics.start_effect{self.wind}

	self.wind.logic = function(self)
		local x = math.random( -1, 1 )
		local y = math.random( -1, 1 )
		local z = math.random( -1, 1 )
		self.gain = 0.1
		self.mag = 0.05
		self.dir = vec3( x, y, z )
		self.reload = true

		if self.reload then
			self.reload = false
			haptics.stop_effect( self.handle )
			self.handle = haptics.start_effect(self)
		end
	end








	self.Model = mesh()
	self.Model:load( "ship2.3ds", "CelShaded" )
	self.Model:uniform( "Model", mat4(1) );
	self.Model:uniform( "Projection", Projection )
	self.Model:uniform( "AmbientMaterial", vec3( 0.04, 0.04, 0.04 ) )
	self.Model:uniform( "DiffuseMaterial", vec3( 0.75, 0.75, 0.5 ) )
	self.Model:uniform( "SpecularMaterial", vec3( 0.5, 0.5, 0.5 ) )
	self.Model:uniform( "Shininess", 100 )

	self.Model:uniform( "NormalMatrix", normalSpace(View) )
	self.Model:uniform( "View", View )
	self.Model:uniform( "LightPosition", vec3( -10, 10, 10 ) )
	self.Model:depth( true )
	self.Model:writeToDepth( true )











	self.earth = model()
	self.earth:load( "dactyl.3ds", "TexturedVbo" )
	self.earth:texture( "Texture0", "plutomap1k" )
	self.earth:depth( true )
	self.earth:writeToDepth( true )

	self.earth.model_matrix = rotate( scale( mat4(1), vec3(0.1) ), 90, vec3(1,0,0))
	self.earth:uniform( "Model", self.earth.model_matrix )
	self.earth:uniform( "View", translate( mat4(1), vec3(0,0,-200) ) )
	self.earth:uniform( "Projection", Projection )
	self.time_passed = 0



	self.astroids = {}
	for i=1,10 do
		self.astroids[i] = model()
		self.astroids[i]:load( "gaspra.3ds", "TexturedVbo" )
		self.astroids[i]:texture( "Texture0", "gaspra" )
		self.astroids[i]:depth( true )
		self.astroids[i]:writeToDepth( true )

		local X = math.random( -10, 10 )
		local Y = math.random( -10, 10 )
		local Z = math.random( -400, -25 )
		local _X = math.random( -1, 1 )
		local _Y = math.random( -1, 1 )
		local _Z = math.random( -1, -1 )
		local _amount = math.random()
		self.astroids[i].pos = vec3( X,Y,Z )
		self.astroids[i].dir = vec3( 0,0,0.01 )
		self.astroids[i].rot = vec3( _X, _Y, _Z )
		self.astroids[i].rotAmount = _amount/10

		self.astroids[i].model_matrix = rotate( scale( mat4(1), vec3(0.5) ), 90, vec3(1,0,0))

		self.astroids[i]:uniform( "Model", self.astroids[i].model_matrix )
		self.astroids[i]:uniform( "View", translate( mat4(1), self.astroids[i].pos ) )
		self.astroids[i]:uniform( "Projection", Projection )
	end













	self.haptics_ball = haptics_model()
	self.haptics_ball:load( "world.3ds", "CelShaded" )
	self.haptics_ball:uniform( "Projection", Projection )
	self.haptics_ball:uniform( "AmbientMaterial", vec3( 0.04, 0.04, 0.04 ) )
	self.haptics_ball:uniform( "DiffuseMaterial", vec3( 0.75, 0.75, 0.5 ) )
	self.haptics_ball:uniform( "SpecularMaterial", vec3( 0.5, 0.5, 0.5 ) )
	self.haptics_ball:uniform( "Shininess", 100 )


	local model_view = lookAt( vec3(0, 0, 0.1 + 1.0),
				              vec3(0, 0, 0),
				              vec3(0, 1, 0))

	self.haptics_ball:uniform( "NormalMatrix", 
		normalSpace(View) )

	self.haptics_ball:uniform( "Projection", Projection )
	self.haptics_ball:uniform( "View", mat4(1) )
	self.haptics_ball:uniform( "Model", model_view )
	self.haptics_ball:uniform( "LightPosition", vec3( 0, 0, 10 ) )
	self.haptics_ball:depth( true )
	self.haptics_ball:writeToDepth( true )


	self.got_hit = false
end













function Haptics_Demo_Globals.HandleEvents( self, _events )
	if events.wasKeyPressed( _events, key.space ) then
		if self.running then
			self.running = false
		else
			self.running = true
		end
	end

	if events.wasKeyPressed( _events, key['1'] ) then
		self.effect.name = "constant"
		self.reload = true
	elseif events.wasKeyPressed( _events, key['2'] ) then
		self.effect.name = "viscous"
		self.reload = true
	elseif events.wasKeyPressed( _events, key['3'] ) then
		self.effect.name = "spring"
		self.reload = true
	elseif events.wasKeyPressed( _events, key['4'] ) then
		self.effect.name = "friction"
		self.reload = true
	end

	return false
end















function Haptics_Demo_Globals.Logic( self, _deltaTime )

	self.time_passed = self.time_passed + _deltaTime
	self.camera:logic( _deltaTime )
	local position = haptics.proxy_position()
	local trans = haptics.proxy_transform()



	if self.reload then
		self.reload = false
		haptics.stop_effect( self.effect.handle )
		self.effect.handle = haptics.start_effect( self.effect )
	end
	self.wind:logic()



	self.earth:uniform( "Model", 
		rotate( self.earth.model_matrix, 0.01*self.time_passed, vec3(0,0,1)) )



	for i, v in ipairs(self.astroids) do
		v.pos = v.pos + v.dir*_deltaTime

		v:uniform( "View", translate( self.camera.view, v.pos ) )
		v:uniform( "Model",
			rotate( v.model_matrix, v.rotAmount*self.time_passed, v.rot) )

		local distance = position - v.pos

		local x = distance:x()
		local y = distance:y()
		local z = distance:z()
		
		if x < 0 then x = -x end
		if y < 0 then y = -y end
		if z < 0 then z = -z end

		if x < 3 and
		   y < 3 and
		   z < 3 then 
			self.got_hit = true 
		end
	end


	if self.got_hit then
		self.got_hit = false
		self.Model:uniform( "DiffuseMaterial", vec3( 0.75, 0, 0 ) )
	else
		self.Model:uniform( "DiffuseMaterial", vec3( 0.75, 0.75, 0.5 ) )
	end


	
	local _model = rotate( rotate( scale(trans, vec3(2)), 90, vec3(1,0,0)), 180,vec3(0,1,0))
	self.Model:uniform( "Model", _model );

	--self.Model:uniform( "Model", mat4(1) )
	self.Model:uniform( "View", self.camera.view )
	self.Model:uniform( "NormalMatrix", normalSpace(self.camera.view*_model) )


	self.haptics_ball:uniform( "Model", translate( mat4(1), vec3(0,0,-15) ) )
	self.haptics_ball:uniform( "View", mat4(1))
	self.haptics_ball:uniform( "NormalMatrix", normalSpace(trans) )

	self.haptics_ball:uniform( "LightPosition", position )


end















function Haptics_Demo_Globals.Render(self)
	--self.haptics_ball:render()
	self.Model:render()
	--self.earth:render()

	for i, v in ipairs(self.astroids) do
		v:render()
	end
	self.background:render()
end



















function Haptics_Demo_Globals.Reload(self)
	Haptics_Demo_Globals:Start()
	self.Model:reload()
	self.earth:reload()

	for i, v in ipairs(self.astroids) do
		v:reload()
	end
end