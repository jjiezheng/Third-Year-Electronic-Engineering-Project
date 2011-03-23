dofile( include( "math" ) )
dofile( include( "save" ) )
dofile( include( "FreeFormCamera" ) )
dofile( include( "make_earth" ) )
dofile( include( "strings" ) )


Projection = perspective( 45.0, 16.0/10.0, 1, 10000.0 )



















function default_intro( _deltaTime, O )
	O.the_time = O.the_time + _deltaTime
	if O.the_time < 500 then

		for i,v in ipairs(O.menu) do
			v.colour = vec4(1,1,1,0)
		end
	elseif O.the_time > O.fade_in and O.the_time < O.pause+O.fade_in then

		for i,v in ipairs(O.menu) do
			v.colour = vec4(1,1,1, (0.4/(O.pause))*(O.the_time-O.fade_in))
		end
		O.menu[O.current].colour = vec4(1,1,1, (0.8/(O.pause))*(O.the_time-O.fade_in))
	else
		O.the_time = 0
		return false
	end
	return true
end


function default_handle_events( _events, O )
	if O.sub_menu then
		O.sub_menu = O.sub_menus[O.current].handle_events( _events )
		return O.active
	end

	if events.isKeyDown( _events, key.enter ) then
		O.sub_menu = true
		O.sub_menu_time = 0
		O.sub_menus[O.current].refresh()
	elseif events.isKeyDown( _events, key.backspace ) then
		if O.sub_menu then
			O.sub_menu = false
		else
			return false
		end
	elseif events.isKeyDown( _events, key.up ) then
		if O.current > 1 then
			O.current = O.current - 1
		end
	elseif events.isKeyDown( _events, key.down ) then
		if O.current < #O.menu then
			O.current = O.current + 1
		end
	end
	return O.active
end

function default_logic( _deltaTime, O, title, pos, horizontal )

	if O.sub_menu then
		O.sub_menus[O.current].logic( _deltaTime, pos )
		--O.sub_menus[O.current].logic( _deltaTime )
	end

	if O.intro then
		--O.intro = O.do_intro(_deltaTime)
		O.intro = default_intro( _deltaTime, O )
	elseif O.sub_menu then
		if O.sub_menu_time == 0 then
			O.menu_start_pos = O.menu[ O.current ].pos
		end

		O.sub_menu_time = O.sub_menu_time + _deltaTime

		local menu_pos = mix( O.menu_start_pos, vec3(0,title,-950), smoothstep( 0, O.animationTime, O.sub_menu_time ) )
		local menu_colour = mix( vec4(1,1,1,0.8), vec4(0.67,0.16,0.16,0.8), smoothstep( 0, O.animationTime, O.sub_menu_time ) )
		local menu_fade = mix( vec4(1,1,1,0.8), vec4(1,1,1,0), smoothstep( 0, O.animationTime, O.sub_menu_time ) )

		for i,v in ipairs(O.menu) do
			v.colour = menu_fade
			if not horizontal then
				v.pos = vec3(0,pos-(100*i),-950)
			end
		end

		O.menu[O.current].colour = menu_colour
		O.menu[O.current].pos = menu_pos
	else
		for i,v in ipairs(O.menu) do
			v.colour = vec4(1,1,1,0.4 )
			if not horizontal then
				v.pos = vec3(0,pos-(100*i),-950)
			end
		end
		O.menu[O.current].colour = vec4(1,1,1,0.8 )
	end

	for i,v in ipairs(O.menu) do
		UpdateText(v, v.lua_string )
	end
end



function default_render( O )
	if O.sub_menu then
		O.sub_menus[O.current].render()
	end
	for i,v in ipairs(O.menu) do
		v:render()
	end
end

function default_reload( O )
	if O.sub_menu then
		O.sub_menus[O.current].reload()
	end
	for i,v in ipairs(O.menu) do
		v:reload()
	end
end

function default_refresh(O)
	O.intro = true
	O.active = true
	O.the_time = 0
end


















function make_new_game_menu()

	return {
		handle_events = function( _events )
		end,

		logic = function( _deltaTime, pos )

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
			return true
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
			return true
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





















function make_controls()

	local O = {}
	O.menu = {
		makeString( "set keyboard", "Circle_110", 0,100.0, "centre" ),
		makeString( "set joystick", "Circle_110", 0, 0.0, "centre" )
	}
	O.sub_menus = {
		make_set_keyboard(),
		make_set_joystick()
	}

	O.current = 1
	O.active = true
	O.intro = true
	O.sub_menu = false
	O.sub_menu_time = 0
	O.the_time = 0
	O.animationTime = 400
	O.fade_in = 500
	O.pause = 500

	return {
		handle_events = function( _events )
			return default_handle_events( _events, O )
		end,
		logic = function( _deltaTime, pos )
			default_logic( _deltaTime, O, 200, 200 )
		end,
		reload = function()	default_reload(O) end,
		render = function()	default_render(O) end,
		refresh = function()
			O.intro = true
			O.the_time = 0
		end
	}
end
















function make_video()

	local O = {}
	O.menu = {
		makeString( "graphics quality", "Circle_110", 0,100.0, "centre" ),
		makeString( "resolution", "Circle_110", 0, 0.0, "centre" ),
		makeString( "fullscreen", "Circle_110", 0, -100.0, "centre" )
	}
	O.sub_menus = {
		make_controls(),
		make_controls(),
		make_controls()
	}

	O.current = 1
	O.active = true
	O.intro = true
	O.sub_menu = false
	O.sub_menu_time = 0
	O.the_time = 0
	O.animationTime = 400
	O.fade_in = 500
	O.pause = 500

	return {
		handle_events = function( _events )
			return default_handle_events( _events, O )
		end,
		logic = function( _deltaTime, pos )
			default_logic( _deltaTime, O, 200, 200 )
		end,
		reload = function()	default_reload(O) end,
		render = function()	default_render(O) end,
		refresh = function()
			O.intro = true
			O.the_time = 0
		end
	}
end


















function make_audio()

	local O = {}
	O.menu = {
		makeString( "music", "Circle_110", 0,100.0, "centre" ),
		makeString( "sound effects", "Circle_110", 0, 0.0, "centre" ),
		makeString( "audio", "Circle_110", 0, -100.0, "centre" )
	}
	O.sub_menus = {
		make_controls(),
		make_controls(),
		make_controls()
	}

	O.current = 1
	O.active = true
	O.intro = true
	O.sub_menu = false
	O.sub_menu_time = 0
	O.the_time = 0
	O.animationTime = 400
	O.fade_in = 500
	O.pause = 500

	return {
		handle_events = function( _events )
			return default_handle_events( _events, O )
		end,
		logic = function( _deltaTime, pos )
			default_logic( _deltaTime, O, 200, 200 )
		end,
		reload = function()	default_reload(O) end,
		render = function()	default_render(O) end,
		refresh = function()
			O.intro = true
			O.the_time = 0
		end
	}
end

















function make_options_menu()

	local O = {}
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

	O.current = 1
	O.active = true
	O.intro = true
	O.sub_menu = false
	O.sub_menu_time = 0
	O.the_time = 0
	O.animationTime = 400
	O.fade_in = 500
	O.pause = 500

	return {
		handle_events = function( _events )
			return default_handle_events( _events, O )
		end,
		logic = function( _deltaTime, pos )
			default_logic( _deltaTime, O, 200, 200 )
		end,
		reload = function()	default_reload(O) end,
		render = function()	default_render(O) end,
		refresh = function()
			O.intro = true
			O.the_time = 0
		end
	}
end













function make_exit_menu()

	local O = {}
	O.menu = {
		makeString( "yes", "Circle_110", -50,100.0, "centre" ),
		makeString( "no", "Circle_110", 50, 105.0, "centre" )
	}
	O.sub_menu = false
	O.current = 1
	O.active = true
	O.intro = true
	O.sub_menu_time = 0
	O.the_time = 0
	O.fade_in = 500
	O.pause = 500

	return {
		handle_events = function( _events )

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
			return O.active
		end,
		logic = function( _deltaTime, pos )
			default_logic( _deltaTime, O, 250, 200, true )
		end,

		reload = function()	default_reload(O) end,
		render = function() default_render(O) end,
		refresh = function() default_refresh(O) end
	}
end















function make_main_menu()

	local O = {}
	O.title = makeString( "Attrition", "Circle_110", 0, 325.0, "centre" )
	O.title.colour = vec4(1,1,1,0.8)
	O.menu = {
		makeString( "new game", "Circle_110", 0, 100.0, "centre" ),
		makeString( "options", "Circle_110", 0, 0.0, "centre" ),
		makeString( "exit", "Circle_110", 0, -100.0, "centre" )
	}

	O.sub_menus =
	{
		make_new_game_menu(),
		make_options_menu(),
		make_exit_menu()
	}

	O.current = 1
	O.intro = true
	O.sub_menu = false
	O.the_time = 0
	O.animationTime = 400
	O.menu_start_pos = vec3(0)
	O.fade_in = 500
	O.pause = 1000

	return {
		handle_events = function( _events )
			default_handle_events( _events, O )
		end,
		logic = function( _deltaTime )
			UpdateText( O.title, O.title.lua_string )
			default_logic( _deltaTime, O, 250, 200 )
		end,
		reload = function() 
			default_reload(O) 
			O.title:reload()
		end,
		render = function()
			default_render(O)
			O.title:render()
		end
	}
end







PlanetTest = {}
function PlanetTest.Start( self )

	sys.title( "Planet Render Test" )
	self.main_menu = make_main_menu()
	self.camera = FreeFormCamera()
	self.world = make_earth()
	self.angle = 0
end

function PlanetTest.HandleEvents( self, _events )

	self.camera:handleEvents( _events )
	self.main_menu.handle_events( _events )
	return false
end

function PlanetTest.Logic( self, _deltaTime )

	self.camera:logic( _deltaTime )
	self.angle = self.angle + 0.0005*_deltaTime

	local cameraPos = vec3(0,0,35) - self.camera.currentPosition


	self.world:uniform( "Model", rotate( rotate( mat4(1.0), 23.44, vec3(0,0,-1) ), self.angle,vec3(0,1,0) ) )
	self.world:uniform( "View", self.camera.view*translate(mat4(1), vec3(0,0,-35)) )

	self.world:uniform( "v3CameraPos", cameraPos )
	self.world:uniform( "fCameraHeight", length(cameraPos) )
	self.world:uniform( "fCameraHeight2", length(cameraPos)^2 )

	self.main_menu.logic( _deltaTime )
end

function PlanetTest.Render( self )
	self.world:render()
	self.main_menu.render()
end

function PlanetTest.Reload( self )
	self.world:reload()
	self.main_menu.reload()
end