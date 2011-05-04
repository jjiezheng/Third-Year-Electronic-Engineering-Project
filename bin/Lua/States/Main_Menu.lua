dofile( include( "math" ) )
dofile( include( "save" ) )
dofile( include( "FreeFormCamera" ) )
dofile( include( "make_earth" ) )
dofile( include( "strings" ) )
dofile( include( "load_settings" ) )
dofile( include( "menu" ) )

Projection = perspective( 45.0, 16.0/10.0, 1, 10000.0 )

function normalSpace( _matrix )
	return inverseTranspose( mat3(_matrix) )
end






function make_new_game_menu()

	return {
		handle_events = function( _events )
		end,

		logic = function( _deltaTime, pos )
			setState( "Game" )
			return true
		end,

		reload = function()
		end,

		render = function()
		end,

		refresh = function()
		end
	}
end





function make_controls()

	local O = menu.make_table()
	O.menu = {
		makeString( "set keyboard", "Circle_110", 0,100.0, "centre" ),
		makeString( "set joystick", "Circle_110", 0, 0.0, "centre" )
	}
	O.sub_menus = {
		make_set_keyboard(),
		make_set_joystick()
	}
	O.world_position = 6

	return {
		handle_events = function( _events )	return menu.handle_events( _events, O )	end,
		logic = function( _deltaTime, pos )	menu.logic( _deltaTime, O, 150, 200 ) end,
		reload = function()	menu.reload(O) end,
		render = function()	menu.render(O) end,
		refresh = function() menu.refresh(O) end
	}
end





function make_options_menu()

	local O = menu.make_table()
	O.menu = {
		makeString( "controls", "Circle_110", 0,100.0, "centre" ),
		makeString( "video", "Circle_110", 0, 0.0, "centre" ),
		makeString( "audio", "Circle_110", 0, -100.0, "centre" )
	}
	O.sub_menus = {
		make_controls(),
		make_video(),
		make_audio()
	}
	O.world_position = 4

	return {
		handle_events = function( _events )	return menu.handle_events( _events, O )	end,
		logic = function( _deltaTime, pos )	menu.logic( _deltaTime, O, 200, 200 ) end,
		reload = function()	menu.reload(O) end,
		render = function()	menu.render(O) end,
		refresh = function() menu.refresh(O) end
	}
end









function make_main_menu()

	local O = menu.make_table()
	O.menu = {
		makeString( "new game", "Circle_110", 0, 100.0, "centre" ),
		makeString( "options", "Circle_110", 0, 0.0, "centre" ),
		makeString( "exit", "Circle_110", 0, -100.0, "centre" )
	}
	O.sub_menus = {
		make_new_game_menu(),
		make_options_menu(),
		make_exit_menu()
	}

	O.world_position = 3

	return {
		handle_events = function( _events )
			if not O.sub_menu  then
				if events.isKeyDown( _events, key.backspace ) then
					return false
				end
			end
			menu.handle_events( _events, O )
			return true
		end,

		logic = function( _deltaTime ) menu.logic( _deltaTime, O, 250, 200 ) end,
		reload = function()	menu.reload(O) end,
		render = function() menu.render(O) end,
		refresh = function() menu.refresh(O) end
	}
end








function make_exit_menu()

	local O = menu.make_table()
	O.menu = {
		makeString( "yes", "Circle_110", -50,100.0, "centre" ),
		makeString( "no", "Circle_110", 50, 105.0, "centre" )
	}

	O.world_position = 5

	return {
		handle_events = function( _events )

			if O.event_dead_time > O.event_dead_time_total then
				if events.isKeyDown( _events, key.left ) then
					if O.current > 1 then O.current = O.current - 1 end

				elseif events.isKeyDown( _events, key.right ) then
					if O.current < #O.menu then	O.current = O.current + 1 end

				elseif events.isKeyDown( _events, key.backspace ) then
					return false

				elseif events.isKeyDown( _events, key.enter ) then
					if O.current == 1 then
						sys.exit()
					elseif O.current == 2 then
						return false
					end
				end
			end
			return O.active
		end,
		logic = function( _deltaTime, pos )	menu.logic( _deltaTime, O, 250, 200, true )	end,
		reload = function()	menu.reload(O) end,
		render = function() menu.render(O) end,
		refresh = function() menu.refresh(O) end
	}
end































function make_video()
	sys.load_settings( "Graphics" )
	local _quality_table = { "low", "medium", "high" }

	local O = menu.make_table()
	O.menu = {
		menu.make_bool_control("fullscreen", Graphics.fullScreen, vec3(0,100,-950)),
		menu.make_string_control("quality", 1, _quality_table, vec3(0,0,-950))
	}
	O.world_position = 7
	return {
		handle_events = function( _events )

			O.changes = O.menu[O.current].handle_events( _events )

			if events.isKeyDown( _events, key.up ) and O.current > 1 then
				O.current = O.current - 1
			elseif events.isKeyDown( _events, key.down ) and O.current < #O.menu then
				O.current = O.current + 1
			end
			
			if events.isKeyDown( _events, key.backspace ) then
				return false
			else return true end
		end,

		logic = function( _deltaTime, pos )

			for i, v in ipairs(O.menu) do
				if i == O.current then 
					v.logic(_deltaTime, true)
				else 
					v.logic(_deltaTime, false) 
				end
			end

			if O.changes then
				O.changes = false
				Graphics.fullScreen = O.menu[1].value()
				--sys.push_settings( "Graphics = " .. serialize( Graphics ), "Graphics" )
			end

			if world_camera and O.world_move then
				world_camera.change_position(O.world_position) 
				O.world_move = false
			end
		end,

		reload = function() menu.reload(O) end,
		render = function() menu.render(O) end,
		refresh = function() menu.refresh(O) end
	}
end








function make_audio()
	sys.load_settings( "Sound" )
	Sound.music_volume = Sound.music_volume or 5
	Sound.fx_volume = Sound.fx_volume or 5
	Sound.channels = Sound.channel or 100

	local O = menu.make_table()
	O.menu = {
		menu.make_number_control("music", Sound.music_volume, {0,10}, vec3(0,100,-950)),
		menu.make_number_control("fx", Sound.fx_volume, {0,10}, vec3(0,0,-950))
	}

	O.blep = sound_effect( "cannonblast_m_04" )
	O.blep.blop = false
	O.world_position = 8

	return {
		handle_events = function( _events )

			O.changes = O.menu[O.current].handle_events( _events )

			if events.isKeyDown( _events, key.up ) and O.current > 1 then
				O.current = O.current - 1
			elseif events.isKeyDown( _events, key.down ) and O.current < #O.menu then
				O.current = O.current + 1
			end
			
			if events.isKeyDown( _events, key.backspace ) then return false
			else return true end
		end,

		logic = function( _deltaTime, pos )

			if O.blep.blop then	
				O.blep:play() 
				O.blep.blop = false
			end

			for i, v in ipairs(O.menu) do
				if i == O.current then v.logic(_deltaTime, true)
				else v.logic(_deltaTime, false) end
			end

			if O.changes then
				O.changes = false

				Sound.music_volume = O.menu[1].value()
				if Sound.fx_volume ~= O.menu[2].value() then
					Sound.fx_volume = O.menu[2].value()
					 O.blep.blop = true
				end
				sys.push_settings( "Sound = " .. serialize( Sound ), "Sound" )
			end

			if world_camera and O.world_move then
				world_camera.change_position(O.world_position) 
				O.world_move = false
			end
		end,

		reload = function() menu.reload(O) end,
		render = function() menu.render(O) end,
		refresh = function() menu.refresh(O) end
	}
end









































function make_set_keyboard()

	local message = makeString( "press a key for up", "Circle_110", 0,0.0, "centre" )

	local buttons = { "up", "down", "left", "right", "fire1", "fire2" }
	local controls = {}
	local current = 1
	local active = true
	local intro = true
	local outro = false
	local save_to_file = true
	local the_time = 0
	local animationTime = 400

	--O.world_position = 9
	local function do_intro( _deltaTime )
		the_time = the_time + _deltaTime
		if the_time < 500 then
			message.colour = vec4(1,1,1,0)
		elseif the_time > 500 and the_time < 1000 then
			message.colour = vec4(1,1,1, 0.0016*(the_time-500))
		else
			the_time = 0
			return false
		end
		return true
	end

	local function do_outro( _deltaTime )

		if save_to_file then
			save_to_file = false
			local serialised_controls = serialize( controls )
			sys.save_setting( serialised_controls, "player_controls" )
		end

		the_time = the_time + _deltaTime
		message.lua_string = "settings saved"
		if the_time < 500 then
			message.colour = vec4(1,1,1,0)
		elseif the_time > 500 and the_time < 1000 then
			message.colour = vec4(1,1,1, 0.0016*(the_time-500))
		elseif the_time > 1000 and the_time < 1500 then
			message.colour = vec4(1,1,1,1)
		else
			the_time = 0
			return false, false
		end
		return true, true
	end

	return {
		handle_events = function( _events )
			if not outro then
				local button = events.get_key( _events )
				if button ~= 0 then
					if current <= #buttons then
						controls[ buttons[current] ] = button
						if current ~= #buttons then
							current = current + 1
						else
							outro = true
							the_time = 0
						end
					end
				end
			end
			return active
		end,

		logic = function( _deltaTime )
			if intro then
				intro = do_intro(_deltaTime)
				message.lua_string = "press a key for " .. buttons[current]
			elseif outro then
				outro, active = do_outro(_deltaTime)
			elseif active then
				message.colour = vec4(1,1,1,0.8 )
				message.lua_string = "press a key for " .. buttons[current]
			end
			UpdateText(message, message.lua_string )

			return active
		end,

		reload = function()
			message:reload()
		end,

		render = function()
			message:render()
		end,

		refresh = function()
			intro = true
			outro = false
			active = true
			save_to_file = true
			the_time = 0
			current = 1
		end
	}
end




function make_set_joystick()

	local message = makeString( "press a key for up", "Circle_110", 0,0.0, "centre" )

	local buttons = { "up", "down", "left", "right", "fire1", "fire2" }
	local controls = {}
	local current = 1
	local active = true
	local intro = true
	local outro = false
	local save_to_file = true
	local the_time = 0
	local animationTime = 400

	--O.world_position = 10
	local function do_intro( _deltaTime )
		the_time = the_time + _deltaTime
		if the_time < 500 then
			message.colour = vec4(1,1,1,0)
		elseif the_time > 500 and the_time < 1000 then
			message.colour = vec4(1,1,1, 0.0016*(the_time-500))
		else
			the_time = 0
			return false
		end
		return true
	end

	local function do_outro( _deltaTime )

		if save_to_file then
			save_to_file = false
			local serialised_controls = serialize( controls )
			sys.save_setting( serialised_controls, "player_controls" )
		end

		the_time = the_time + _deltaTime
		message.lua_string = "settings saved"
		if the_time < 500 then
			message.colour = vec4(1,1,1,0)
		elseif the_time > 500 and the_time < 1000 then
			message.colour = vec4(1,1,1, 0.0016*(the_time-500))
		elseif the_time > 1000 and the_time < 1500 then
			message.colour = vec4(1,1,1,1)
		else
			the_time = 0
			return false, false
		end
		return true, true
	end

	return {
		handle_events = function( _events )
			if not outro then
				local button = events.get_key( _events )
				if button ~= 0 then
					if current <= #buttons then
						controls[ buttons[current] ] = button
						if current ~= #buttons then
							current = current + 1
						else
							outro = true
							the_time = 0
						end
					end
				end
			end
			return active
		end,

		logic = function( _deltaTime )
			if intro then
				intro = do_intro(_deltaTime)
				message.lua_string = "press a key for " .. buttons[current]
			elseif outro then
				outro, active = do_outro(_deltaTime)
			elseif active then
				message.colour = vec4(1,1,1,0.8 )
				message.lua_string = "press a key for " .. buttons[current]
			end
			UpdateText(message, message.lua_string )

			return active
		end,

		reload = function()
			message:reload()
		end,

		render = function()
			message:render()
		end,

		refresh = function()
			intro = true
			outro = false
			active = true
			save_to_file = true
			the_time = 0
			current = 1
		end
	}
end
































function make_splash()
	local title = makeString( "Attrition", "Circle_110", 0, 0, "centre" )
	local press_start = makeString( "press enter", "Circle_110", 0, -80, "centre" )
	title.pos = vec3( 0, 20, -400 )
	title.colour = vec4(1,1,1,0.8)

	local flash = 2000
	local flash_time = 0
	local dir = true
	local the_time = 0
	local first_colour = vec4(0.804,0.384,0.384,0)
	local second_colour = vec4(0.67,0.16,0.16,0.8)

	local show = true
	return {
		handle_events = function( _events )

			if events.isKeyDown( _events, key.enter ) then 
				return false
			else return true end

		end,
		logic = function( _deltaTime )
			the_time = the_time + _deltaTime
			flash_time = flash_time + _deltaTime

			if flash_time > flash then
				dir = not dir
				flash_time = 0
				if dir then 
					first_colour = vec4(0.804,0.384,0.384,0)
					second_colour = vec4(0.67,0.16,0.16,0.8)
				else
					first_colour = vec4(0.67,0.16,0.16,0.8)
					second_colour = vec4(0.804,0.384,0.384,0)
				end
			end

			local pulse = mix( first_colour, second_colour, 
				smoothstep( 0, flash ,flash_time ) )

			press_start.colour = pulse
			UpdateText( title, title.lua_string )
			UpdateText( press_start, press_start.lua_string )

		end,
		reload = function()
			title =  makeString( "Attrition", "Circle_110", 0, 0, "centre" )
			title.pos = vec3( 0, 20, -400 )
			title.colour = vec4(1,1,1,0.8)
			press_start:reload()
			Log("I'm alive!")
		end,
		render = function()
			title:render()
			press_start:render()
		end
	}
end

















function make_world_fancy_camera()

	local quats = 
	{ 
		math.uQuat(),
		rotateQuat( math.uQuat(), 0, vec3(0,1,0) ),
		rotateQuat( math.uQuat(), -50, vec3(0,1,0) ),
		rotateQuat( rotateQuat( math.uQuat(), -10, vec3(0,1,0) ), 35, vec3(1,0,0) ),
		rotateQuat( rotateQuat( math.uQuat(), 130, vec3(0,1,0) ), 20, vec3(1,0,0) ),
		rotateQuat( math.uQuat(), 50, vec3(0,1,0) ),
		rotateQuat( math.uQuat(), -100, vec3(0,0,1) ),
		rotateQuat( math.uQuat(), 100, vec3(1,0,0) ),
		rotateQuat( math.uQuat(), 20, vec3(0,1,0) ),
		rotateQuat( math.uQuat(), -50, vec3(0,1,0) ),
		rotateQuat( math.uQuat(), -50, vec3(0,1,0) ), }

	local positions = 
	{	
		vec3(0,0,50),
		vec3(0,0,30),
		vec3(0,0,20),
		vec3(0,0,20),
		vec3(-2,0,25),
		vec3(0,0,30),
		vec3(0,0,40),
		vec3(0,0,25),
		vec3(0,0,30),
		vec3(0,0,30),
		vec3(0,0,30), }

	local orientations = {}
	for i = 1, #quats do
		orientations[i] = { rot = quats[i], pos = positions[i] }
	end

	orientations.last = { rot = rotateQuat( math.uQuat(), 10, vec3(1,0,0) ), pos = vec3(0,0,60) }
	orientations.current = { rot = rotateQuat( math.uQuat(), 65, vec3(1,-1,1) ), pos = vec3(10,0,35) }


	local rot_timer = 0
	local current_position, new_position = 2,2
	local change_in_position = false
	return 
	{
		change_position = function(_new_position)
			if current_position ~= _new_position and _new_position <= #orientations then
				new_position = _new_position
				change_in_position = true

				orientations.last.rot = orientations.current.rot
				orientations.last.pos =  orientations.current.pos
			end
		end,

		logic = function(_deltaTime)

			if change_in_position then

				rot_timer = 0
				current_position = new_position
				change_in_position = false
			end

			rot_timer = rot_timer + _deltaTime

			orientations.current.rot = mix( orientations.last.rot, 
					orientations[current_position].rot, 
					smoothstep( 0, 5000, rot_timer ) )

			orientations.current.pos = mix( orientations.last.pos,
											orientations[current_position].pos,
											smoothstep( 0, 5000, rot_timer ) )


			return orientations.current.pos, mat4Cast( orientations.current.rot )
		end
	}
end

























Main_Menu = {}
function Main_Menu.Start( self )
	sys.title( "Attrition - Main Menu" )

	self.splash = make_splash()
	self.main_menu = make_main_menu()

	self.show_splash = true
	self.show_menu = false

	self.camera = FreeFormCamera()
	self.world = make_earth()
	self.angle = 0
	self.time_passed = 0



	world_camera = make_world_fancy_camera()
	self.title_song = music( "Neptune" )
	self.title_song:play()
end

function Main_Menu.HandleEvents( self, _events )
	self.camera:handleEvents( _events )

	if self.show_splash then 
		self.show_splash = self.splash.handle_events( _events )
		self.show_menu = not self.show_splash
		if self.show_menu then
			self.main_menu.refresh()
		end

	elseif self.show_menu then 
		self.show_menu = self.main_menu.handle_events( _events )
		self.show_splash = not self.show_menu

		if self.show_splash then
			world_camera.change_position(2)
		end
	end

	for i=1, 3 do
		if events.isKeyDown( _events, key[i] ) then

			world_camera.change_position(i)
		end
	end
	return false
end

function Main_Menu.Logic( self, _deltaTime )

	self.time_passed = self.time_passed + _deltaTime




	local pos, rot = world_camera.logic(_deltaTime)


	self.camera:logic( _deltaTime )
	self.angle = self.angle + 0.0005*_deltaTime
	local cameraPos =pos*normalSpace(rot) - self.camera.currentPosition 
	local _camera = (self.camera.view*translate( mat4(1), vec3(0)-pos)*rot )

	self.world:uniform( "Model", 
		rotate( rotate( 
			mat4(1.0), 23.44, vec3(0,0,-1) ), 
			self.angle,vec3(0,1,0) ) )

	self.world:uniform( "View", _camera )

	self.world:uniform( "v3CameraPos", cameraPos )
	self.world:uniform( "fCameraHeight", length(cameraPos) )
	self.world:uniform( "fCameraHeight2", length(cameraPos)^2 )

	self.main_menu.logic( _deltaTime )
	self.splash.logic( _deltaTime )
end

function Main_Menu.Render( self )
	self.world:render()

	if self.show_menu then 
		self.main_menu.render()
	end
	if self.show_splash then 
		self.splash:render()
	end
end

function Main_Menu.Reload( self )
	self.world = make_earth()

	self.main_menu.reload()
	self.splash.reload()
end