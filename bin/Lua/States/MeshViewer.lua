dofile( include( "math" ) )
dofile( include( "firstPersonCamera" ) )
dofile( include( "ExamineCamera" ) )


Projection = perspective( 45.0, 16.0/10.0, 0.1, 100.0 )
View = mat4(1.0)
defaultLight = vec3( 0, -0, 15 )

function normalSpace( _matrix )
	return inverseTranspose( mat3(_matrix) )
end

function lightingmode(_type)
	local type = _type or 1
	if type > 3 then type = 1 end
	if type < 1 then type = 3 end
	local angle = 0
	local light = vec3( 0, 0, -15 )

	if type == 1 then
		return function( _time, mesh, _model, _view )
			light = defaultLight
			mesh:uniform( "NormalMatrix", normalSpace(_view*_model) )
			mesh:uniform( "View", _view )
			mesh:uniform( "LightPosition", light )
			return type
		end, type
	elseif type == 2 then
		return function( _time, mesh, _model, _view )
			angle = (_time/100)%360
			light = vec3( math.cos(angle)*15, 0, math.sin(angle)*15 )
			mesh:uniform( "NormalMatrix", normalSpace(_model) )
			mesh:uniform( "View", _view )
			mesh:uniform( "LightPosition", light )
			return type
		end, type
	elseif type == 3 then
		return function( _time, mesh, _model, _view )
			angle = (_time/100)%360
			light = vec3( math.cos(angle)*15, 0, math.sin(angle)*15 )
			mesh:uniform( "NormalMatrix", normalSpace(_view*_model) )
			mesh:uniform( "View", _view )
			mesh:uniform( "LightPosition", light )
			return type
		end, type
	end
end




MeshViewer = {}
function MeshViewer.Start( self )

	self.time = 0
	self.camera = ExamineCamera()

	self.modelMatrix = scale( translate( rotate( mat4(1.0), 90,  vec3( -1,0,0) ), vec3(0,0,-0) ), vec3(1) )
	self.Model = mesh()

	--self.Model:load( "russian_am", "TexturedVbo" )
	--self.Model:load( "auditt_wheel", "CelShaded" )
	--self.Model:load( "box", "CelShaded" )
	--self.Model:load( "HeadlessGiant", "CelShaded" )
	self.Model:load( "bunny", "CelShaded" )

	self.Model:uniform( "Model", self.modelMatrix );
	self.Model:uniform( "Projection", Projection )
	self.Model:uniform( "AmbientMaterial", vec3( 0.04, 0.04, 0.04 ) )
	self.Model:uniform( "DiffuseMaterial", vec3( 0.75, 0.75, 0.5 ) )
	self.Model:uniform( "SpecularMaterial", vec3( 0.5, 0.5, 0.5 ) )
	self.Model:uniform( "Shininess", 100 )


	self.lighting, currentMode = lightingmode(1)
end

function MeshViewer.HandleEvents( self, _events )

	if events.wasKeyPressed( _events, key['1'] ) then
		self.lighting, currentMode = lightingmode()
	elseif events.wasKeyPressed( _events, key['2'] ) then
		self.lighting, currentMode = lightingmode(2)
	elseif events.wasKeyPressed( _events, key['3'] ) then
		self.lighting, currentMode = lightingmode(3)
	elseif events.wasKeyPressed( _events, key['['] ) then
		self.lighting, currentMode = lightingmode(currentMode-1)
	elseif events.wasKeyPressed( _events, key[']'] ) then
		self.lighting, currentMode = lightingmode(currentMode+1)
	elseif events.wasKeyPressed( _events, key['e'] ) then
		self.camera = ExamineCamera()
	elseif events.wasKeyPressed( _events, key['f'] ) then
		self.camera = firstPersonCamera()
	end


	if events.wasKeyPressed( _events, key['space'] ) then
		defaultLight = vec3(0) - defaultLight
	end


	self.camera:handleEvents( _events )
	if events.wasKeyPressed( _events, key.f11 ) then
		return true
	else
		return false
	end
end

function MeshViewer.Logic( self, _deltaTime )
	self.time = self.time + _deltaTime

	self.camera:logic( _deltaTime )
	self.lighting( self.time, self.Model, self.modelMatrix, self.camera.view )
end

function MeshViewer.Render( self )
	self.Model:render()
end

function MeshViewer.Reload( self )
	self.Model:reload()
end
