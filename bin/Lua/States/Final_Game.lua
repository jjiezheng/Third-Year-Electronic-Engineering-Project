dofile( include( "math" ) )
dofile( include( "ExamineCamera" ) )
dofile( include( "haptic_effects" ) )
dofile( include( "make_earth" ) )
dofile( include( "FreeFormCamera" ) )
dofile( include( "strings" ) )
dofile( include( "load_settings" ) )

Projection = perspective( 45.0, 16.0/10.0, 1, 10000.0 )
View = mat4(1.0)


positions = 
{	
	vec3(0,500,1000),
	vec3(0,300,900),
	vec3(0,100,800),
	vec3(0,0,700),
	vec3(0,0,600),
	vec3(0,0,500),
	vec3(0,0,400),
	vec3(0,0,300),
	vec3(300,100,300),
	vec3(200,500,100),
	vec3(-200,-100,200),
	vec3(-400,50,300),
	vec3(-500,-50,0),
	vec3(-600,0,100),
	vec3(-600,0,100)
}

player_score = 0



hit_astroid = {}
hit_astroid.name = "constant"
hit_astroid.pos = vec3(0.5)
hit_astroid.dir = vec3(0.5)
hit_astroid.gain = 0.5
hit_astroid.mag = 0.4


function normalSpace( _matrix )
	return inverseTranspose( mat3(_matrix) )
end



function make_haptics_player()

	local model = mesh()
	model:load( "ship2.3ds", "CelShaded" )
	model:uniform( "Model", mat4(1) );
	model:uniform( "Projection", Projection )
	model:uniform( "AmbientMaterial", vec3( 0.04, 0.04, 0.04 ) )
	model:uniform( "DiffuseMaterial", vec3( 0.75, 0.75, 0.5 ) )
	model:uniform( "SpecularMaterial", vec3( 0.5, 0.5, 0.5 ) )
	model:uniform( "Shininess", 100 )

	model:uniform( "NormalMatrix", normalSpace(View) )
	model:uniform( "View", View )
	model:uniform( "LightPosition", vec3( -10, 10, 10 ) )
	model:depth( true )
	model:writeToDepth( true )

	return
	{
		logic = function(_delta,_trans, _pos, _camera)
			local _model = rotate( 
				rotate( 
					scale( translate( --[[_trans--]]mat4(4), vec3(0,0,-_pos:z()) ), 
						vec3(1) ), 
					90, 
					vec3(1,0,0) ), 
				180,
				vec3(0,1,0) )
			model:uniform( "Model", _model )

			model:uniform( "View", _camera )
			model:uniform( "NormalMatrix", normalSpace(_camera*_model) )
		end,
		render = function()
			model:render()
		end,
		reload = function()
			model:reload()
		end
	}
end




















function astroid_postition(_pos, _i, _blend)
	return catmullRom( _pos[_i-1],
					   _pos[_i],
					   _pos[_i+1],
					   _pos[_i+2],
					   _blend ) - vec3(0,0,0)
end




function make_astroids(_positions, _number)
	local astroids = {}
	local positions = _positions
	local number_of_points = #positions - 3
	local roids_between_points = math.floor((_number/number_of_points)+0.5)
	local roids = number_of_points*roids_between_points
	local blend = 0
	local current = 2
	local time_passed = 0


	for i=1,roids do
		astroids[i] = model()
		astroids[i]:load( "dactyl.3ds", "TexturedVbo" )
		astroids[i]:texture( "Texture0", "dactyl", 0 )
		astroids[i]:depth( true )
		astroids[i]:writeToDepth( true )

		astroids[i]:uniform( "AmbientMaterial", vec3( 0.04, 0.04, 0.04 ) )
		astroids[i]:uniform( "DiffuseMaterial", vec3( 0.75, 0.75, 0.5 ) )
		astroids[i]:uniform( "SpecularMaterial", vec3( 0.5, 0.5, 0.5 ) )
		astroids[i]:uniform( "Shininess", 100 )
		astroids[i]:uniform( "NormalMatrix", normalSpace(View) )
		astroids[i]:uniform( "LightPosition", vec3( -10, 10, 10 ) )

		blend = (i%roids_between_points)/roids_between_points
		if i%roids_between_points == 0 and i~=roids then 
			current = current+1
		end
		local X = math.random( -7.5, 7.5 )
		local Y = math.random( -7.5, 7.5 )
		local Z = math.random( 0, 0 )
		local _X = math.random( -1, 1 )
		local _Y = math.random( -1, 1 )
		local _Z = math.random( -1, -1 )
		local _amount = math.random()
		astroids[i].deviation = vec3( X,Y,Z )
		--astroids[i].deviation = vec3( 0 )
		astroids[i].dir = vec3( 0,0,0.01 )
		astroids[i].rot = vec3( _X, _Y, _Z )
		astroids[i].rotAmount = _amount/10

		
		astroids[i].pos = astroid_postition(positions, current, blend)
		astroids[i].model_matrix = rotate( scale( translate( mat4(1), astroids[i].pos+astroids[i].deviation ), vec3(0.01) ), 90, vec3(1,0,0))
		
		astroids[i]:uniform( "Model",astroids[i].model_matrix )
		astroids[i]:uniform( "View", mat4(1) )
		astroids[i]:uniform( "Projection", Projection )
	end

	return 
	{
		logic = function(_delta,_camera, ship_position, distance_traveled)
			time_passed = time_passed + _delta

			for i, v in ipairs(astroids) do
				v:uniform( "View", _camera )
				v:uniform( "Model",
					rotate(v.model_matrix, v.rotAmount*time_passed, v.rot ) )


				local distance = 
					distance_traveled 
					+ vec3(ship_position:x(),ship_position:y(),0) 
					- v.pos
					- v.deviation

				local to_player = length(distance)

				--Close enough to score points.
				if to_player < 10 and to_player > 2 then
					player_score = player_score + 1*(10-to_player)*_delta*0.09
				--Close enough to hit an astroid.
				elseif to_player < 2 then 
					player_score = player_score - 1000
					hit_astroid.mag = (2-to_player)
					hit_astroid.dir = normalise(distance)
				end
			end
		end,

		render = function()
			for i, v in ipairs(astroids) do
				v:render()
			end
		end,
		reload = function()
			for i, v in ipairs(astroids) do
				v:reload()
			end
		end	
	}
end































Final_Game = {}
function Final_Game.Start(self)

	sys.title( "Haptics" )
	haptics.workspace( perspective( 65.0, 16.0/10.0, 1, 10.0 ), mat4(1) )
	hit_astroid.handle = haptics.start_effect( hit_astroid )
	self.song = music( "Jupiter" )
	self.song:play()

	self.camera = ExamineCamera()
	self.wind = make_wind()
	self.player = make_haptics_player()
	self.astroids = make_astroids(positions, 400)

	self.camera = FreeFormCamera()
	self.world_camera = make_world_fancy_camera(positions)
	self.world = make_earth(10)
	self.angle = 0
	self.speed_up = false
	self.got_hit = false
	self.time_passed = 0
	self.game_over = false
	player_score = 0

	self.score = makeString( "Score:0", "Circle_110", 0, 340, "centre" )
	self.score.colour = vec4(1,1,1,0.8)

	self.end_text = makeString( "Game Over please enter your name", "Circle_110", 0, 0, "centre" )
	self.end_text.colour = vec4(1,1,1,0.8)
 	self.player_name = make_text_entry(30,6,vec2(0,-10) )

end

function Final_Game.HandleEvents( self, _events )
	self.camera:handleEvents( _events )

	if self.game_over then
		self.player_name.handle_events(_events)
	end

	return false
end


function Final_Game.Logic( self, _deltaTime )
	if not self.game_over then
		_deltaTime = _deltaTime*2

		hit_astroid.mag = 0.
		haptics.stop_effect( hit_astroid.handle )



		self.camera:logic( _deltaTime )
		local cameraPos = vec3(0,0,50) - self.camera.currentPosition
		local view = translate(self.camera.view, vec3(0,0,-15) )

		self.time_passed = self.time_passed + _deltaTime
		self.camera:logic( _deltaTime )
		local ship_position = haptics.proxy_position()
		local trans = haptics.proxy_transform()

		self.wind()
		local _camera,distance = self.earth_logic( 
			self, 
			_deltaTime, 
			view )


		self.player.logic( _deltaTime, trans, ship_position, view )
		self.astroids.logic( _deltaTime, _camera, ship_position, distance )
				hit_astroid.handle = haptics.start_effect( hit_astroid )

		UpdateText(self.score,"Score:"..string.format("%d",player_score) )
	else
		UpdateText(self.end_text,self.end_text.lua_string)
		local finished, name = self.player_name.logic(_deltaTime)
 		if finished then

 			local f = io.open("../Settings/Highscores.lua", "w")
 			table.insert(Highscores, {name,math.floor(player_score+0.5)})
 			f:write( "Highscores = " .. serialize( Highscores ) )
 			f:close()
 			Final_Game:Start()
 		end
	end
end

function Final_Game.earth_logic( self, _deltaTime, _other_camera )

	local pos, rot,finished = self.world_camera.logic(_deltaTime)
	if finished then
		self.game_over = finished
		dofile( "../Settings/Highscores.lua" )
	end

	self.angle = self.angle + 0.0005*_deltaTime
	local cameraPos = pos  - vec3(0)*mat3(rot)
	local _camera = (_other_camera*rot*translate( mat4(1), vec3(0,0,0)-pos) )

	self.world:uniform( "Model", 
		rotate( rotate( 
			scale(mat4(1.0),vec3(10)), 23.44, vec3(0,0,-1) ), 
			self.angle,vec3(0,1,0) ) )

	self.world:uniform( "View", _camera )

	self.world:uniform( "v3CameraPos", cameraPos )
	self.world:uniform( "fCameraHeight", length(cameraPos) )
	self.world:uniform( "fCameraHeight2", length(cameraPos)^2 )

	return _camera,pos
end

function Final_Game.Render(self)
	
	self.world:render()
	self.player.render()
	self.astroids.render()
	self.score:render()
	if self.game_over then
		self.end_text:render()
		self.player_name.render()
	end
end

function Final_Game.Reload(self)
	
	Haptics_Demo_Globals:Start()
	self.world:reload()
	self.player.reload()
	self.astroids.reload()
	self.score:reload()	

	if self.game_over then
		self.end_text:reload()
	end
end









































function mouse_ray(mouse_position)
	local w, h = sys.get("width"), sys.get("height")
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
	local text_entered = false
	text:uniform( "Model", scale(mat4(1), vec3(0.45) ) )

	function text_box.handle_events(_events)
		text_entered = false
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
				 text_entered = true
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
		return text_entered, text.lua_string
	end

	function text_box.render()
		text_entered = false
		button.render()
		text:render()
	end

	return text_box
end
