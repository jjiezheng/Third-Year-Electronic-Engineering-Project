dofile( include( "math" ) )
dofile( include( "firstPersonCamera" ) )
dofile( include( "ExamineCamera" ) )
dofile( include( "strings" ) )


Projection = perspective( 45.0, 16.0/10.0, 0.1, 100.0 )
perspective_box = perspective( 45.0, 16.0/10.0, 10, 1000.0 )
View = mat4(1.0)
defaultLight = vec3( 0, -0, 15 )
weird_lighting = false

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
			if weird_lighting then 
				light = vec3(0) - defaultLight
			else
				light = defaultLight
			end
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































function mouse_ray(mouse_position)
	--local w, h = 1240, 775
	local w, h = sys.get("width"), sys.get("height")
	--local w,h = 1680,1050
	local ray_start =  unProject( mouse_position, 
								  mat4(1),
								  Projection, 
								  vec4(0,0,w,h) )

	local ray_end =  unProject( vec3(mouse_position:x(), mouse_position:y(), 1), 
								mat4(1),
								perspective_box, 
								vec4(0,0,w,h) )

	local current =  unProject( vec3(mouse_position:x(), mouse_position:y(), 0.96855), 
							translate(mat4(1), vec3(0,0,0)),
							perspective_box, 
							vec4(0,0,w,h) )	

	return ray_start, ray_end, current
end



function make_button(_x, _y, _pos)
	local button = {}

	local hitbox = collision.obb(_x,_y)
	hitbox:z(-241.0)
	hitbox:teleport( vec2(_pos:x(),-_pos:y()) )

	local render_hitbox = collision.visualHitbox( hitbox )
	render_hitbox:uniform( "Colour", vec3(3,22,52)/256 )
	render_hitbox:uniform( "ProjectionView", perspective_box*mat4(1.0) )
	render_hitbox:uniform( "alpha", 0.2 )

	local mouse_position = vec3(0)
	local box_selected = false
	local colour =  vec3(3,22,52)/256
	local pos = vec2(_pos:x(),-_pos:y())

	function button.handle_events(_events)
		if events.isButtonDown( _events, mouse.left ) then
			selection = true
		else 
			selection = false 
		end
		mouse_position = events.mouse_position( _events )
	end


	function button.focus(_focus)
		box_selected = _focus

		if box_selected then colour = vec3(250,228,31)/256 
		else colour = vec3(3,22,52)/256 end
	end

	function button.logic(_deltaTime, _size)

		--hitbox = collision.obb(_size:x()/2)+2,-(_size:y()/4)+2)
		hitbox:z(-241.0)
		hitbox:teleport( pos )
		render_hitbox = collision.visualHitbox( hitbox )

		local ray_start, ray_end, current = mouse_ray(mouse_position)
		if collision.test( hitbox, ray_start, ray_end ) then
			if selection then
				box_selected = true
				colour = vec3(250,228,31)/256
				--render_hitbox:uniform( "alpha", 1 )
			end
		else 
			if selection then
				box_selected = false
				colour = vec3(3,22,52)/256
				--render_hitbox:uniform( "alpha", 0.2 )
			end
		end
		render_hitbox:uniform( "ProjectionView", perspective_box*mat4(1.0) )
		render_hitbox:uniform( "Colour", colour )
		render_hitbox:flip(true)
		render_hitbox:update( hitbox )

		return box_selected
	end

	function button.render() render_hitbox:render() end

	return button
end



























function make_forward_button(_button, _arrow)
	local forward = {}
	local button = make_button(10,10,_button)
	local arrow =  makeString( ">", "Circle_110", _arrow:x(), _arrow:y(), "centre" )
	arrow.colour = vec4(1,1,1,0.8)
	local pressed = false

	function forward.handle_events(_events)
		if events.isButtonUp( _events, mouse.left ) then
			pressed = true
		else pressed = false end

		button.handle_events(_events)
	end

	function forward.logic(_deltaTime)
		UpdateText( arrow, arrow.lua_string )
		if button.logic(_deltaTime) and pressed  then
			 button.focus(false)
			 pressed = false
			 return true
		end
		return false
	end

	function forward.render() 
		button.render()
		arrow:render()
	end

	return forward
end



function make_backward_button(_button, _arrow)
	local backward = {}
	local button = make_button(10,10,_button)
	local arrow =   makeString( "<", "Circle_110", _arrow:x(), _arrow:y(), "centre" )
	arrow.colour = vec4(1,1,1,0.8)
	local pressed = false


	function backward.handle_events(_events)
		if events.isButtonUp( _events, mouse.left ) then
			pressed = true
		else pressed = false end

		button.handle_events(_events)
	end

	function backward.logic(_deltaTime)
		UpdateText( arrow, arrow.lua_string )
		if button.logic(_deltaTime) and pressed  then
			 button.focus(false)
			 pressed = false
			 return true
		end
		return false
	end

	function backward.render() 
		button.render()
		arrow:render()
	end

	return backward
end








palettes =
{
	{ 
		a=vec3( 17,25,34 )/256,
		d=vec3( 15,80,93 )/256,
		s=vec3(56,110,119)/256
	},
	{ 
		a=vec3(49,54,64 )/256,
		d=vec3(112,105,90 )/256,
		s=vec3(245,244,213)/256
	},
	{ 
		a=vec3(125,129,116 )/256,
		d=vec3(149,159,104 )/256,
		s=vec3(193,213,155)/256
	},
	{ 
		a=vec3(73,77,102 )/256,
		d=vec3( 121,117,142 )/256,
		s=vec3(209,193,180)/256
	},
	{ 
		a=vec3(255,95,9 )/256,
		d=vec3(209,193,180 )/256,
		s=vec3(255,243,4)/256
	},
	{ 
		a=vec3(24,47,63 )/256,
		d=vec3(172,153,120 )/256,
		s=vec3(109,92,62)/256
	},
	{ 
		a=vec3(250,105,0 )/256,
		d=vec3(224,228,204 )/256,
		s=vec3(105,210,231)/256
	},
	{ 
		a=vec3(3,22,52 )/256,
		d=vec3(3,101,100 )/256,
		s=vec3(205,179,128)/256
	}
}



current_palette = 1
function pick_palette(model, _current_palette )
	model:uniform( "AmbientMaterial", palettes[_current_palette].a )
	model:uniform( "DiffuseMaterial", palettes[_current_palette].d )
	model:uniform( "SpecularMaterial", palettes[_current_palette].s )
	model:uniform( "Shininess", 200 )
end




active_shader = "CelShaded"
function change_shader()
	if active_shader == "CelShaded" then
		active_shader = "lighting"
	else active_shader = "CelShaded" end
end

























function make_ship()
	weird_lighting = false

	local model = model()
	model.modelMatrix = scale( translate( rotate( mat4(1.0), 90,  vec3( -1,0,0) ), vec3(0,0,-0) ), vec3(2) )

	model:load( "ship.3ds", active_shader )
	model:depth( true )
	model:writeToDepth( true )

	model:uniform( "Model", model.modelMatrix );
	model:uniform( "Projection", Projection )

	pick_palette(model, current_palette )

	return model, "Ship.3ds"
end

function make_ship2()
	weird_lighting = false

	local model = model()
	model.modelMatrix = scale( translate( rotate( mat4(1.0), 90,  vec3( -1,0,0) ), vec3(0,0,-0) ), vec3(2) )

	model:load( "ship2.3ds", active_shader )
	model:depth( true )
	model:writeToDepth( true )

	model:uniform( "Model", model.modelMatrix );
	model:uniform( "Projection", Projection )

	pick_palette(model, current_palette )

	return model, "Ship2.3ds"
end

function make_ceres()
	weird_lighting = false

	local model = model()
	model.modelMatrix = scale( translate( rotate( mat4(1.0), 90,  vec3( -1,0,0) ), vec3(0,0,-0) ), vec3(0.02) )

	model:load( "ceres.3ds", "TexturedVbo" )
	model:texture( "Texture0", "ceres", 0 )
	model:depth( true )
	model:writeToDepth( true )

	model:uniform( "Model", model.modelMatrix )
	model:uniform( "View", mat4(1) )
	model:uniform( "Projection", Projection )

	pick_palette(model, current_palette )

	return model, "ceres.3ds"
end

function make_gaspra()
	weird_lighting = false

	local model = model()
	model.modelMatrix = scale( translate( rotate( mat4(1.0), 90,  vec3( -1,0,0) ), vec3(0,0,-0) ), vec3(0.2) )

	model:load( "gaspra.3ds", "TexturedVbo" )
	model:texture( "Texture0", "gaspra", 0 )
	model:depth( true )
	model:writeToDepth( true )

	model:uniform( "Model", model.modelMatrix )
	model:uniform( "View", mat4(1) )
	model:uniform( "Projection", Projection )

	pick_palette(model, current_palette )

	return model, "gaspra.3ds"
end

function make_dactyl()
	weird_lighting = false

	local model = model()
	model.modelMatrix = scale( translate( rotate( mat4(1.0), 90,  vec3( -1,0,0) ), vec3(0,0,-0) ), vec3(0.02) )

	model:load( "dactyl.3ds", "TexturedVbo" )
	model:texture( "Texture0", "dactyl", 0 )
	model:depth( true )
	model:writeToDepth( true )

	model:uniform( "Model", model.modelMatrix )
	model:uniform( "View", mat4(1) )
	model:uniform( "Projection", Projection )

	pick_palette(model, current_palette )

	return model, "dactyl.3ds"
end


function make_giant()
	weird_lighting = true

	local model = model()
	model.modelMatrix =  rotate( mat4(1.0), 90,  vec3( -1,0,0) )

	model:load( "HeadlessGiant.3ds", active_shader )
	model:depth( true )
	model:writeToDepth( true )

	model:uniform( "Model", model.modelMatrix )
	model:uniform( "Projection", Projection )

	pick_palette(model, current_palette )

	return model, "HeadlessGiant.3ds"
end

function make_wheel()
	weird_lighting = true

	local model = model()
	model.modelMatrix = scale(mat4(1), vec3(1))

	model:load( "auditt_wheel.obj", active_shader )
	model:depth( true )
	model:writeToDepth( true )

	model:uniform( "Model", model.modelMatrix )
	model:uniform( "Projection", Projection )

	pick_palette(model, current_palette )


	return model, "auditt_wheel.obj"
end

function make_bunny()
	weird_lighting = false

	local model = model()
	model.modelMatrix = scale(mat4(1), vec3(2.5))

	model:load( "bunny.obj", active_shader )
	model:depth( true )
	model:writeToDepth( true )

	model:uniform( "Model", model.modelMatrix )
	model:uniform( "Projection", Projection )

	pick_palette(model, current_palette )

	return model, "bunny.obj"
end





















































MeshViewer = {}
function MeshViewer.Start( self )

	sys.title( "Mesh Viewer" )
	haptics.workspace( perspective( 65.0, 16.0/10.0, 1, 10.0 ), mat4(1) )
	self.time = 0

	self.list_of_models = 
	{
		make_wheel,
		make_bunny,
		make_giant,
		make_ship,
		make_ship2,
		make_ceres,
		make_gaspra,
		make_dactyl
	}
	self.current = 1
	self.reload = false


	self.current_text = makeString( 
		tostring(self.current).." of "..tostring(#self.list_of_models), 
		"Circle_110", 0, 295, "centre" )
	self.current_text.colour = vec4(1,1,1,0.8)

	self.palette_text = makeString( 
		tostring(current_palette).." of "..tostring(#palettes), 
		"Circle_110", 0, 255, "centre" )
	self.palette_text.colour = vec4(1,1,1,0.8)


	local text = ""
	self.Model, text = self.list_of_models[self.current]()

	self.model_text = makeString( text, "Circle_110", 0, 340, "centre" )
	self.model_text.colour = vec4(1,1,1,0.8)

	self.controls1 = makeString( "Use the mouse to change model and colour", "Circle_110", -348, -300, "centre" )
	self.controls1.colour = vec4(1,1,1,0.8)

	self.controls2 = makeString( "Use the haptics device to examine the model", "Circle_110", -340, -275, "left" )
	self.controls2.colour = vec4(1,1,1,0.8)

	self.controls3 = makeString( "Use spacebar to change shader", "Circle_110", -408, -325, "left" )
	self.controls3.colour = vec4(1,1,1,0.8)

	self.forward_model = make_forward_button(vec2(23,74),vec2(93, 293))
	self.backward_model = make_backward_button(vec2(-23,74),vec2(-93, 293))

	self.forward_palatte = make_forward_button(vec2(23,64),vec2(93, 255))
	self.backward_palatte = make_backward_button(vec2(-23,64),vec2(-93, 255))



	self.lighting, currentMode = lightingmode(1)
end

function MeshViewer.HandleEvents( self, _events )
	self.forward_model.handle_events(_events)
	self.backward_model.handle_events(_events)

	self.forward_palatte.handle_events(_events)
	self.backward_palatte.handle_events(_events)


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
	elseif  events.wasKeyPressed( _events, key["space"] ) then
		change_shader()
		self.reload = true
	end


	if events.wasKeyPressed( _events, key.f11 ) then
		return true
	else
		return false
	end
end

function MeshViewer.Logic( self, _deltaTime )
	self.time = self.time + _deltaTime

	local position = haptics.proxy_position()
	local trans = haptics.proxy_transform()


	local change = false
	if self.forward_model.logic(_deltaTime) then
		change = true
		if self.current < #self.list_of_models then
			self.current = self.current + 1
		end
	elseif self.backward_model.logic(_deltaTime) then
		change = true
		if self.current > 1 then
			self.current = self.current - 1
		end
	end

	if self.forward_palatte.logic(_deltaTime) then
		if current_palette < #palettes then
			current_palette = current_palette + 1
			pick_palette(self.Model, current_palette)

			self.palette_text.lua_string = 
				tostring(current_palette).." of "..tostring(#palettes)
		end
	elseif self.backward_palatte.logic(_deltaTime) then
		if current_palette > 1 then
			current_palette = current_palette - 1
			pick_palette(self.Model, current_palette)

			self.palette_text.lua_string = 
				tostring(current_palette).." of "..tostring(#palettes)
		end
	end

	if change or self.reload then
		self.current_text.lua_string =
			tostring(self.current).." of "..tostring(#self.list_of_models)
		
		self.Model, self.model_text.lua_string = self.list_of_models[self.current]()
		self.reload = false
	end


	

	self.lighting( self.time, self.Model, self.Model.modelMatrix, translate( mat4(1), vec3(-position:x(),-position:y(),-5) )*trans )


	UpdateText( self.model_text, self.model_text.lua_string )
	UpdateText( self.palette_text, self.palette_text.lua_string )


	UpdateText( self.current_text, self.current_text.lua_string )
	UpdateText( self.controls1, self.controls1.lua_string )
	UpdateText( self.controls2, self.controls2.lua_string )
	UpdateText( self.controls3, self.controls3.lua_string )

	self.controls1:uniform( "View", scale( rotate( translate( mat4(1.0), self.controls1.pos ), 180.0, vec3( 1.0,0.0,0.0) ), vec3(0.25) ) )
	self.controls2:uniform( "View", scale( rotate( translate( mat4(1.0), self.controls2.pos ), 180.0, vec3( 1.0,0.0,0.0) ), vec3(0.25) ) )
	self.controls3:uniform( "View", scale( rotate( translate( mat4(1.0), self.controls3.pos ), 180.0, vec3( 1.0,0.0,0.0) ), vec3(0.25) ) )

end

function MeshViewer.Render( self )
	self.Model:render()

	self.model_text:render()
	self.palette_text:render()

	self.forward_model.render()
	self.backward_model.render()

	self.forward_palatte.render()
	self.backward_palatte.render()

	self.controls1:render()
	self.controls2:render()
	self.controls3:render()
	self.current_text:render()
end

function MeshViewer.Reload( self )
	self.Model:reload()
	self.palette_text:reload()
	self.model_text:reload()
	self.current_text:reload()

	self.forward_model.reload()
	self.backward_model.reload()

	self.forward_palatte.reload()
	self.backward_palatte.reload()

	self.controls1:reload()
	self.controls2:reload()
	self.controls3:reload()
end
