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

Projection = perspective( 45.0, 16.0/10.0, 0.1, 500.0 )
View = mat4(1.0)

Haptics_Demo_Globals = {}
function Haptics_Demo_Globals.Start(self)
	sys.title( "Haptics" )

	haptics.init()
	haptics.workspace( Projection, mat4(1) )

	self.camera = ExamineCamera()

	self.running = false
	self.running = true
	self.effectType = "constant"

	self.effect = {}
	self.effect.name = "constant"
	self.effect.pos = vec3(0.5)
	self.effect.dir = vec3(0.5)
	self.effect.gain = 0.6
	self.effect.mag = 0.6
	self.effect.handle = haptics.start_effect{}

	self.balls = {}

	local max_num = 30
	for i = 1,max_num do
		self.balls[i] = mesh()
	end
--	
	for i,v in ipairs(self.balls) do
		v:load( "world.3ds", "CelShaded" )
		v:uniform( "Projection", Projection )
		v:uniform( "AmbientMaterial", vec3( 0.04, 0.04, 0.04 ) )
		v:uniform( "DiffuseMaterial", vec3( 0.75, 0.75, 0.5 ) )
		v:uniform( "SpecularMaterial", vec3( 0.5, 0.5, 0.5 ) )
		v:uniform( "Shininess", 100 )

		v:uniform( "Projection", Projection )
		v:uniform( "View", mat4(1) )
		v:uniform( "Model", mat4(1))
		v:uniform( "LightPosition", vec3( 0, 0, 1000 ) )
		v:depth( true )
		v:writeToDepth( true )

	end


	self.Model = mesh()
	self.Model:load( "ship2.3ds", "CelShaded" )
	self.Model:uniform( "Model", mat4(1) );
	self.Model:uniform( "Projection", Projection )
	self.Model:uniform( "AmbientMaterial", vec3( 0.04, 0.04, 0.04 ) )
	self.Model:uniform( "DiffuseMaterial", vec3( 0.75, 0.75, 0.5 ) )
	self.Model:uniform( "SpecularMaterial", vec3( 0.5, 0.5, 0.5 ) )
	self.Model:uniform( "Shininess", 100 )

	self.Model:uniform( "NormalMatrix", 
		normalSpace(View) )
	self.Model:uniform( "View", View )
	self.Model:uniform( "LightPosition", vec3( -10, 10, 10 ) )
	self.Model:depth( true )
	self.Model:writeToDepth( true )


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
	if self.reload then
		self.reload = false
		haptics.stop_effect( self.effect.handle )
		self.effect.handle = haptics.start_effect( self.effect )
	end

	self.camera:logic( _deltaTime )
	local position = haptics.proxy_position()
	local trans = haptics.proxy_transform()

	local _model = rotate( rotate( scale(trans, vec3(2)), 90, vec3(1,0,0)), 180,vec3(0,1,0))
	self.Model:uniform( "Model", _model );

	--self.Model:uniform( "Model", mat4(1) )
	self.Model:uniform( "View", self.camera.view )
	self.Model:uniform( "NormalMatrix", 
		normalSpace(trans*_model) )

	self.haptics_ball:uniform( "Model", translate( mat4(1), vec3(0,0,-15) ) )
	self.haptics_ball:uniform( "View", mat4(1))
	self.haptics_ball:uniform( "NormalMatrix", normalSpace(trans) )

	self.haptics_ball:uniform( "LightPosition", position )


	for i,v in ipairs(self.balls) do
		_rand_x = math.random( -10, 10 )
		_rand_y = math.random( -10, 10 )
		_rand_z = math.random( -10, 10 )
		v:uniform( "Model", translate( mat4(1), vec3((0.5*_rand_x),0.5*_rand_y,-30+_rand_z) ) )
	end
end

function Haptics_Demo_Globals.Render(self)

	for i,v in ipairs(self.balls) do
		v:render()
	end--]]
	--self.haptics_ball:render()
	self.Model:render()
end

function Haptics_Demo_Globals.Reload(self)
	Haptics_Demo_Globals:Start()
	self.Model:reload()
end