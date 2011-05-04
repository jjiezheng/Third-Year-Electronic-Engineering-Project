dofile( include( "math" ) )
dofile( include( "FreeFormCamera" ) )
dofile( include( "make_earth" ) )
dofile( include( "load_settings" ) )
dofile( include( "player" ) )
dofile( include( "strings" ) )

Projection = perspective( 45.0, 16.0/10.0, 10, 1000.0 )
farProjection = perspective( 45.0, 16.0/10.0, 10000, 100000000.0 )

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


function firing_closure()
	--local osc = 0
	local time_passed = 0
	local bullet_time = 0
	local rate_of_fire = 50
	return function (pos, _delta)
		time_passed = time_passed + _delta
		bullet_time = bullet_time + _delta
		if bullet_time > rate_of_fire then
			bullet_time = bullet_time - rate_of_fire

			osc = math.sin((time_passed/1000)*180)
			local b1 = create_bullet(5,10,vec2(pos:x()-5,pos:y()+10))
			b1.set_dir( vec2(0, 0.75) )

			local b2 = create_bullet(5,10,vec2(pos:x()+5,pos:y()+10))
			b2.set_dir( vec2(0, 0.75) )


			bullets.add( b1, true )
			bullets.add( b2, true )
		end
	end
end



function gen_lookup_catmull(_positions, resolution)

	local samples = {}
	local pos, old_pos = _positions[2], _positions[2]
	local arc_length = 0

	table.insert(samples, {p=pos,l=length(pos-old_pos) } )
	for seg=2, #_positions-2 do
		
		for i=1,resolution do
			local j = i/resolution

			pos = catmullRom( _positions[seg-1],
							  _positions[seg],
					  		  _positions[seg+1],
					  		  _positions[seg+2],
					  		  j )

			arc_length = arc_length + length(pos-old_pos)
			table.insert(samples, {p=pos,l=arc_length } )
			old_pos = pos
		end
	end
	return samples
end


function enemy_move_closure(points)

	local samples = gen_lookup_catmull(points,20)
	local dist = 0
	local low,high = 1,1

	return function(_deltaTime)

		dist = dist + _deltaTime*0.01
		
		while (samples[high].l < dist )  do
			if high < #samples then
				low,high = low+1
				high = low+1
			else return samples[high].p end
		end

		local p_dist = samples[high].l - samples[low].l
		local cur_dist = dist - samples[low].l
		local ratio = cur_dist/p_dist

		return mix(samples[low].p,samples[high].p,ratio) 
	end
end


function make_temp_enemys(main_table)
	local p0 =
	{
		vec3(-10,-100,0),
		vec3(0,0,0),
		vec3(10,10,0),
		vec3(20,30,0),
		vec3(-30,100,0),
		vec3(-100,60,0),
		vec3(10,60,0)
	}

	local p1 =
	{
		vec3(-20,-110,0),
		vec3(1,10,0),
		vec3(20,20,0),
		vec3(30,40,0),
		vec3(-40,110,0),
		vec3(-110,40,0),
		vec3(20,40,0)
	}

	enemys.add( make_ship( "dasein", main_table.View, 
						   firing_closure(),
						   enemy_move_closure(p0) ) )

	enemys.add( make_ship( "dasein", main_table.View,						   
						   firing_closure(),
						   enemy_move_closure(p1) ) )

	enemys.ships[1]:position( vec3(10,10,0))
	enemys.ships[2]:position( vec3(-10,10,0))
end


























function mouse_ray(mouse_position)
	local w, h = 1240, 775
	local ray_start =  unProject( mouse_position, 
								  mat4(1),
								  Projection, 
								  vec4(0,0,w,h) )

	local ray_end =  unProject( vec3(mouse_position:x(), mouse_position:y(), 1), 
								mat4(1),
								Projection, 
								vec4(0,0,w,h) )

	local current =  unProject( vec3(mouse_position:x(), mouse_position:y(), 0.96855), 
							translate(mat4(1), vec3(0,0,0)),
							Projection, 
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
	render_hitbox:uniform( "ProjectionView", Projection*mat4(1.0) )
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

		hitbox = collision.obb(-(_size:x()/2)+2,-(_size:y()/4)+2)
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
		render_hitbox:uniform( "ProjectionView", Projection*mat4(1.0) )
		render_hitbox:uniform( "Colour", colour )
		render_hitbox:flip(true)
		render_hitbox:update( hitbox )

		return box_selected
	end

	function button.render() render_hitbox:render() end

	return button
end






function make_text_entry(_x, _y, _pos)
	local button = make_button(_x, _y, _pos)
	local text = makeString( "Type Here", "Circle", 0, 0, "centre" )
	text.pos = vec3( _pos:x(), _pos:y(), -241 )
	text.colour = (vec4(3,22,52,255)/256)

	local text_box = {}
	local active = false
	local finished = false
	local first_time = true
	text:uniform( "Model", scale(mat4(1), vec3(0.45) ) )

	function text_box.handle_events(_events)
		button.handle_events(_events)

		if active then
			if first_time then 
				text.lua_string = ""
				first_time = false 
			end
			text.lua_string = text.lua_string..events.getKeyPressed(_events)

			if events.isKeyDown( _events, key['backspace'] ) then
				text.lua_string = string.sub(text.lua_string, 1, #text.lua_string-1)
			elseif events.isKeyDown( _events, key['return'] ) then
				 button.focus(false)
			end
		end
	end

	function text_box.logic(_deltaTime)
		if button.logic(_deltaTime, text:align()) then 
			active = true
			finished = false
			text.colour = (vec4(3,22,52,255)/256)
		else
			text.colour = (vec4(250,228,31,255)/256)
			active = false
			finished = true
		end

		UpdateText( text, text.lua_string )
--[[
		if finished then
			return { done=finished, text=text.lua_string}
		end
		return { done=finished, text=text.lua_string}--]]
	end

	function text_box.render()
		button.render()
		text:render()
	end

	return text_box
end








































Game = {}
function Game.Start( self )

	sys.title( "A Game" )


	self.button0 = make_text_entry(30,6,vec2(-50,10) )
	self.button1 = make_text_entry(30,6,vec2(-50,18) )
	self.button2 = make_text_entry(30,6,vec2(-50,2) )


	self.modelMatrix = scale( 
		translate( 
			rotate( mat4(1.0), 180,  vec3( 0,0,1) ), 
			vec3(0,0,-0) ), 
		vec3(2.5) )

	zDepth = 241
	self.View = translate( mat4(1.0), vec3(0,0,-zDepth) )
	self.borders = make_level_borders( self.View )

	self.player = make_player(self.View, firing_closure())


	make_temp_enemys(self)

	self.world = make_earth(10)
	self.camera = FreeFormCamera()
	self.angle = 0
	self.speed_up = false

	self.world_camera = make_world_fancy_camera()
end

function Game.HandleEvents( self, _events )

	self.player:handleEvents( _events )
	self.camera:handleEvents( _events )
	if events.isKeyDown( _events, key[']'] ) then
		zDepth = zDepth+1
	elseif events.isKeyDown( _events, key['['] ) then
		zDepth = zDepth-1
	elseif events.isKeyDown( _events, key['space'] ) then
		self.speed_up = true
	elseif not events.isKeyDown( _events, key['space'] ) then
		self.speed_up = false
	end


	self.button0.handle_events(_events)
	self.button1.handle_events(_events)
	self.button2.handle_events(_events)

	return false
end

function Game.Logic( self, _deltaTime )

	if self.speed_up then
		_deltaTime = _deltaTime*10
	end
	self.View = translate( mat4(1.0), vec3(0,0,-zDepth) )

	bullets.player_move(_deltaTime, enemys.ships )
	bullets.enemy_move(_deltaTime, self.player.hitbox )

	self.player:logic( _deltaTime, self.View )
	enemys.logic(_deltaTime, self.View)

	bullets.player_test(_deltaTime, enemys.ships )
	bullets.enemy_test(_deltaTime, self.player.hitbox )


	self.borders:logic( _deltaTime, self.View )
	self.camera:logic( _deltaTime )
	self.earth_logic( self, _deltaTime )


	self.button0.logic(_deltaTime)
	self.button1.logic(_deltaTime)
	self.button2.logic(_deltaTime)
end

function Game.Render( self )
	self.world:render()
	self.borders:render()

	self.player:render()
	enemys.render()

 	bullets.render()
 	self.button0.render()
 	self.button1.render()
 	self.button2.render()
end

function Game.Reload( self )
	self.ship:reload()
	self.world:reload()
	self.borders:reload()

	self.player:reload()
	enemys.reload()

	bullets.reload()
end



function Game.earth_logic( self, _deltaTime )

	local pos, rot = self.world_camera.logic(_deltaTime)
	self.angle = self.angle + 0.0005*_deltaTime
	local cameraPos = pos  - (self.camera.currentPosition)*mat3(rot)
	local _camera = (self.camera.view*rot*translate( mat4(1), vec3(0)-pos) )

	self.world:uniform( "Model", 
		rotate( rotate( 
			scale(mat4(1.0),vec3(10)), 23.44, vec3(0,0,-1) ), 
			self.angle,vec3(0,1,0) ) )

	self.world:uniform( "View", _camera )

	self.world:uniform( "v3CameraPos", cameraPos )
	self.world:uniform( "fCameraHeight", length(cameraPos) )
	self.world:uniform( "fCameraHeight2", length(cameraPos)^2 )
end































enemys = {}
enemys.ships = {}
function enemys.add( new_enemy )
	table.insert(enemys.ships, new_enemy )
end

function enemys.logic( _deltaTime, _view)
	for i,v in ipairs( enemys.ships ) do
		if v:is_dead() then	table.remove( enemys.ships, i ) end
	end


	for i,v in ipairs( enemys.ships )  do v:logic( _deltaTime, _view ) end
end


function enemys.render()
	for i,v in ipairs( enemys.ships ) do v:render() end
end

function enemys.reload()
	for i,v in ipairs( enemys.ships ) do v:reload() end
end




















function create_bullet(_x,_y,_pos)
	local hitbox = collision.obb(_x,_y)
	hitbox:teleport( _pos )
	hitbox:z( -241 )

	local render_hitbox = collision.visualHitbox( hitbox )
	render_hitbox:uniform( "Colour", vec3(1,0.16863,0.1647) )
	render_hitbox:uniform( "ProjectionView", Projection*mat4(1.0) )

	local dir, pos  = vec2(0), _pos
	local dead = false
	local already_hit = false
	return 
	{
		test = function(_hitbox) 
			if not dead or not already_hit  then
				if collision.test( hitbox, _hitbox ) then
					render_hitbox:uniform( "Colour", vec3(0.1647,0.16863,1) )
					already_hit = true
					return true
				else
					render_hitbox:uniform( "Colour", vec3(1,0.16863,0.1647) )
					return false
				end
			else return false end
		end,
		teleport = function(_pos) 
			pos = _pos
			hitbox:teleport( pos ) 
		end,
		set_dir = function(_dir) dir = _dir	end,
		get_hitbox = function() return hitbox end,
		is_dead = function() return dead end,
		kill = function() dead = true end,
		damage = function() return 4 end,
		logic = function(_deltaTime) 
			if not dead then 
				already_hit = false

				pos = pos + dir*_deltaTime
				hitbox:teleport( pos ) 

				if ( pos:x() > 100 or pos:x() < -100 ) or
				   ( pos:y() > 120 or pos:y() < -120 ) then
					dead = true
				end
			end
		end,
		render = function() 
			render_hitbox:update( hitbox )
			render_hitbox:render() 
		end,
		reload = function() 
			render_hitbox:reload()
			render_hitbox:uniform( "Colour", vec3(1,0.16863,0.1647) )
			render_hitbox:uniform( "ProjectionView", Projection*mat4(1.0) )
			render_hitbox:uniform( "Z", -241.0 )
		 end
	}
end













bullets = {}
bullets.player_bullets = {}
bullets.enemy_bullets = {}
function bullets.add( new_bullet, _player )	
	if _player == true then	table.insert(bullets.player_bullets, new_bullet )
	else table.insert(bullets.enemy_bullets, new_bullet ) end
end

function bullets.player_move( _deltaTime, _enemys)
	
	for i,v in ipairs( bullets.player_bullets ) do
		if v.is_dead() then table.remove( bullets.player_bullets, i ) end
	end

	for i,v in ipairs( bullets.player_bullets ) do v.logic(_deltaTime) end

	--FIX - fix with somethin faster like a spacial hash
	for i,v in ipairs( bullets.player_bullets ) do	
		for j,q in ipairs( _enemys ) do	
			v.test( q.hitbox )
		end
	end

	sys.title( tostring(#bullets.player_bullets))
end

function bullets.player_test( _deltaTime, _enemys)
	
	--FIX - fix with somethin faster like a spacial hash
	for i,v in ipairs( bullets.player_bullets ) do	
		for j,q in ipairs( _enemys ) do	
			if v.test( q.hitbox ) then
				v.kill()
				q.damaged_by( v.damage() )
			end
		end
	end

end

function bullets.enemy_move( _deltaTime, player_hitbox)
	
	for i,v in ipairs( bullets.enemy_bullets ) do
		if v.is_dead() then	table.remove( bullets.enemy_bullets, i ) end
	end
	for i,v in ipairs( bullets.enemy_bullets ) do v.move(_deltaTime) end
end

function bullets.enemy_test( _deltaTime, player_hitbox)
	for i,v in ipairs( bullets.enemy_bullets ) do v.test( player_hitbox ) end
end

function bullets.render()
	for i,v in ipairs( bullets.player_bullets ) do	v.render() end
	for i,v in ipairs( bullets.enemy_bullets )  do	v.render() end
end

function bullets.reload()
	for i,v in ipairs( bullets.player_bullets ) do	v.reload() end
	for i,v in ipairs( bullets.enemy_bullets )  do	v.reload() end
end






















































function make_level_borders( _view )

	local borderVertBuff, borderElemBuff = make_rect(200, 94)
	local border = {}

	border.left = mesh()
	border.left:shader("vbo"):vert_type("TriangleStrip")
	border.left:add("Position",borderVertBuff,borderElemBuff)

	border.left:uniform( "Colour", vec3(0.16863,0.1451,0.1647) )
	border.left:uniform( "Model", mat4(1.0) )
	border.left:uniform( "View", _view )
	border.left:uniform( "Projection", Projection )
	border.left:uniform( "NormalMatrix", mat4(1.0) )
	border.left:depth( true )

	border.right = mesh()
	border.right:shader("vbo"):vert_type("TriangleStrip")
	border.right:add("Position",borderVertBuff,borderElemBuff)

	border.right:uniform( "Colour", vec3(0.16863,0.1451,0.1647) )
	border.right:uniform( "Model", mat4(1.0) )
	border.right:uniform( "View", _view )
	border.right:uniform( "Projection", Projection )
	border.right:uniform( "NormalMatrix", mat4(1.0) )
	border.right:depth( true )



	function border.logic( self, _deltaTime, _view )
		self.left:uniform( "View", translate( _view,vec3(93.9,0,100) ) )
		self.right:uniform( "View", translate( _view,vec3(-93.9,0,100) ) )
	end

	function border.reload(self)
		self.right:reload()
		self.left:reload()
	end

	function border.render(self)
		self.right:render()
		self.left:render()
	end

	return border
end