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

Projection = perspective( 45.0, 16.0/10.0, 0.1, 10000.0 )
View = mat4(1.0)

Haptics_Demo_Globals = {}
function Haptics_Demo_Globals.Start(self)
	sys.title( "Haptics" )

	haptics.init()
	haptics.workspace( mat4(1), mat4(1) )

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


	self.modelMatrix = scale( 
		translate( 
			rotate( mat4(1.0), 90, vec3( -1,0,20) ), 
			vec3(0,0,-0) ), 
		vec3(1) )

	self.Model = mesh()
	self.Model:load( "bunny.lwo", "CelShaded" )
	--self.Model:load( "bunny", "TestVbo" )
	self.Model:uniform( "Model", mat4(1) );
	self.Model:uniform( "Projection", Projection )
	self.Model:uniform( "AmbientMaterial", vec3( 0.04, 0.04, 0.04 ) )
	self.Model:uniform( "DiffuseMaterial", vec3( 0.75, 0.75, 0.5 ) )
	self.Model:uniform( "SpecularMaterial", vec3( 0.5, 0.5, 0.5 ) )
	self.Model:uniform( "Shininess", 100 )

	self.Model:uniform( "NormalMatrix", 
		normalSpace(View*self.modelMatrix) )
	self.Model:uniform( "View", View )
	self.Model:uniform( "LightPosition", vec3( -10, 10, 10 ) )
	self.Model:depth( true )
	self.Model:writeToDepth( true )


	self.haptics_ball = haptics_model()
	self.haptics_ball:load( "world.3ds", "CelShaded" )
	--self.Model:load( "bunny", "TestVbo" )
	self.haptics_ball:uniform( "Model", mat4(1) );
	self.haptics_ball:uniform( "Projection", Projection )
	self.haptics_ball:uniform( "AmbientMaterial", vec3( 0.04, 0.04, 0.04 ) )
	self.haptics_ball:uniform( "DiffuseMaterial", vec3( 0.75, 0.75, 0.5 ) )
	self.haptics_ball:uniform( "SpecularMaterial", vec3( 0.5, 0.5, 0.5 ) )
	self.haptics_ball:uniform( "Shininess", 100 )

	self.haptics_ball:uniform( "NormalMatrix", 
		normalSpace(View*self.modelMatrix) )
	self.haptics_ball:uniform( "View", mat4(1) )
	self.haptics_ball:uniform( "LightPosition", vec3( -10, 10, 10 ) )
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
	self.Model:uniform( "Model", translate( mat4(1), position ) );

	--self.Model:uniform( "Model", mat4(1) )
	self.Model:uniform( "View", self.camera.view )
	self.Model:uniform( "NormalMatrix", 
		normalSpace(self.camera.view*trans) )

	self.haptics_ball:uniform( "Model", mat4(1) )
	self.haptics_ball:uniform( "View", self.camera.view)
	self.haptics_ball:uniform( "NormalMatrix", 
		normalSpace(self.camera.view*trans) )
end

function Haptics_Demo_Globals.Render(self)
	--[[if self.running then
		haptics.run()
	end--]]

	self.haptics_ball:render()
	self.Model:render()
end

function Haptics_Demo_Globals.Reload(self)
	Haptics_Demo_Globals:Start()
	self.Model:reload()
end