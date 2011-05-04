dofile( include( "strings" ) )



local function make_default_menu_table()

	local O = {}

	O.current = 1
	O.intro = true
	O.active = true
	O.sub_menu = false
	O.post_sub_menu = false
	O.world_move = false
	O.sub_menu_time = 0
	O.the_time = 0
	O.animationTime = 300
	O.menu_start_pos = vec3(0)
	O.fade_in = 100
	O.pause = 1000
	O.event_dead_time = 0
	O.event_dead_time_total = 500
	O.world_position = 2
	return O
end








local function make_number_control( _name, _var, _limits, _pos )
	local name = _name
	local var = _var
	local control_type = _control_type
	local limits = _limits
	local changes = false
	local message = makeString( _name, "Circle_110", 0, 0, "centre" )
	message.pos = _pos
	message.colour = vec4(1,1,1,0.8)

	return {
		handle_events = function( _events )
			if events.isKeyDown( _events, key.left ) then
				var = var - 1
				changes = true
			elseif events.isKeyDown( _events, key.right ) then
				var = var + 1
				changes = true
			end
			
			return changes
		end,

		logic = function( _deltaTime, _active )
			if changes then changes = not changes end

			if var > limits[2] then var = limits[2] end
			if var < limits[1] then var = limits[1] end

			local active = _active or false
			if active then 
				message.colour = vec4(0.67,0.16,0.16,0.8) 
				UpdateText( message, "< ".. name .. ": " .. tostring(var) .. " >" )
			else 
				message.colour = vec4(1,1,1,0.8) 
				UpdateText( message, name .. ": " .. tostring(var) )
			end

			return var
		end,

		reload = function()
			message:reload()
		end,

		render = function()
			message:render()
		end,

		value = function()
			return var
		end,
		refresh = function()
		end
	}
end


local function make_string_control( _name, _var, _table, _pos )
	local name = _name
	local var = _var
	local control_type = _control_type
	local possible_values = _table
	local changes = false
	local message = makeString( _name, "Circle_110", 0, 0, "centre" )
	message.pos = _pos
	message.colour = vec4(1,1,1,0.8)

	return {
		handle_events = function( _events )
			if events.isKeyDown( _events, key.left ) then
				var = var - 1
				changes = true
			elseif events.isKeyDown( _events, key.right ) then
				var = var + 1
				changes = true
			end
			
			return changes
		end,

		logic = function( _deltaTime, _active )
			if changes then changes = not changes end

			if var > #possible_values then var = #possible_values end
			if var < 1 then var = 1 end

			local active = _active or false
			if active then 
				message.colour = vec4(0.67,0.16,0.16,0.8) 
				UpdateText( message, "< ".. name .. ": " .. tostring(possible_values[var]) .. " >" )
			else 
				message.colour = vec4(1,1,1,0.8) 
				UpdateText( message, name .. ": " .. tostring(possible_values[var]) )
			end

			return possible_values[var]
		end,

		reload = function()
			message:reload()
		end,

		render = function()
			message:render()
		end,

		value = function()
			return var
		end,
		refresh = function()
		end
	}
end


local function make_bool_control( _name, _var, _pos )
	local name = _name

	local var
	if type(_var) == "Boolean" then 
		var = _var
	else var = false end
	
	local changes = false
	local message = makeString( _name, "Circle_110", 0, 0, "centre" )
	message.pos = _pos
	message.colour = vec4(1,1,1,0.8)

	return {
		handle_events = function( _events )
			if events.isKeyDown( _events, key.left ) 
			or events.isKeyDown( _events, key.right )  then
				var = not var
				changes = true
			end
			
			return changes
		end,

		logic = function( _deltaTime, _active )
			if changes then changes = not changes end

			local active = _active or false
			if active then 
				message.colour = vec4(0.67,0.16,0.16,0.8) 
				UpdateText( message, "< ".. name .. ": " .. tostring(var) .. " >" )
			else 
				message.colour = vec4(1,1,1,0.8) 
				UpdateText( message, name .. ": " .. tostring(var) )
			end

			return var
		end,

		reload = function()
			message:reload()
		end,

		render = function()
			message:render()
		end,

		value = function()
			return var
		end,
		refresh = function()
		end
	}
end














local function default_intro( _deltaTime, O )
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









local function default_handle_events( _events, O )

	if O.event_dead_time > O.event_dead_time_total then
		if O.sub_menu then
			O.sub_menu = O.sub_menus[O.current].handle_events( _events )
			O.post_sub_menu = not O.sub_menu
			if O.post_sub_menu then
				O.the_time = 0
				O.event_dead_time = 0
				O.world_move = true
			end
			return O.active
		end

		if events.isKeyDown( _events, key.enter ) then
			O.sub_menu = true
			O.sub_menu_time = 0
			O.event_dead_time = 0
			O.sub_menus[O.current].refresh()
		elseif events.isKeyDown( _events, key.backspace ) then
			return false
		elseif events.isKeyDown( _events, key.up ) then
			if O.current > 1 then
				O.current = O.current - 1
			end
		elseif events.isKeyDown( _events, key.down ) then
			if O.current < #O.menu then
				O.current = O.current + 1
			end
		end
	end
	return O.active
end















local function default_logic( _deltaTime, O, title, pos, horizontal )

	if world_camera and O.world_move then
		world_camera.change_position(O.world_position) 
		O.world_move = false
	end

	if type(O.event_dead_time) == "number" then
		O.event_dead_time = O.event_dead_time + _deltaTime
	end

	if O.sub_menu then
		O.sub_menus[O.current].logic( _deltaTime, pos )
	end

	if O.intro then
		O.intro = menu.intro( _deltaTime, O )
	elseif O.sub_menu then
		menu.sub_menu_logic(_deltaTime, O, title, pos, horizontal)

	elseif O.post_sub_menu then 
		menu.post_sub_menu_logic(_deltaTime, O, title, pos, horizontal)

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







local function sub_menu_logic( _deltaTime, O, title, pos, horizontal )
	if O.sub_menu_time == 0 then
		O.menu_start_pos = O.menu[ O.current ].pos
	end

	O.sub_menu_time = O.sub_menu_time + _deltaTime
	local menu_pos = mix( O.menu_start_pos, 
		vec3(0,title,-950), 
		smoothstep( 0, O.animationTime, 
		O.sub_menu_time ) )

	local menu_colour = mix( vec4(1,1,1,0.8), 
		vec4(0.67,0.16,0.16,0.8), 
		smoothstep( 0, 
			O.animationTime, 
			O.sub_menu_time ) )

	local menu_fade = mix( vec4(1,1,1,0.8), 
		vec4(1,1,1,0), 
		smoothstep( 0, 
			O.animationTime, 
			O.sub_menu_time ) )

	for i,v in ipairs(O.menu) do
		v.colour = menu_fade
		if not horizontal then
			v.pos = vec3(0,pos-(100*i),-950)
		end
	end
	O.menu[O.current].colour = menu_colour
	O.menu[O.current].pos = menu_pos

end



local function post_sub_menu_logic( _deltaTime, O, title, pos, horizontal )
	if O.the_time == 0 then
		O.menu_start_pos = O.menu[ O.current ].pos
		O.menu_end_pos = vec3(0,pos-(100*O.current),-950)
	end

	O.the_time = O.the_time + _deltaTime
	local menu_pos = mix( O.menu_start_pos, 
		vec3(0,pos-(100*O.current),-950), 
		smoothstep( 0, O.animationTime, 
		O.the_time ) )
		
	local menu_colour = mix( vec4(0.67,0.16,0.16,0.8), 
		vec4(1,1,1,0.8), 
		smoothstep( 0, 
			O.animationTime, 
			O.the_time ) )

	local menu_fade = mix( vec4(1,1,1,0), 
		vec4(1,1,1,0.4), 
		smoothstep( 0, 
			O.animationTime, 
			O.the_time ) )

	for i,v in ipairs(O.menu) do
		v.colour = menu_fade
		if not horizontal then
			v.pos = vec3(0,pos-(100*i),-950)
		end
	end
	O.menu[O.current].colour = menu_colour
	O.menu[O.current].pos = menu_pos
end












local function default_render( O )
	if O.sub_menu then
		O.sub_menus[O.current].render()
	end
	for i,v in ipairs(O.menu) do
		v:render()
	end
end

local function default_reload( O )
	if O.sub_menu then
		O.sub_menus[O.current].reload()
	end
	for i,v in ipairs(O.menu) do
		v:reload()
		
		v:uniform( "Colour", v.colour )
		v:uniform( "Projection", 
			perspective( 45.0, 16.0/10.0, 0.1, 1000.0 ) )
		v:uniform( "View", 
			scale( 
				rotate( 
					translate( mat4(1.0), v.pos ), 
					180.0, 
					vec3( 1.0,0.0,0.0) ), 
				vec3(0.5) ) )
	end
end

local function default_refresh(O)
	O.intro = true
	O.world_move = true
	O.active = true
	O.the_time = 0
	O.event_dead_time = 0
end



menu = 
{
	make_table = make_default_menu_table,
	make_number_control = make_number_control,
	make_string_control = make_string_control,
	make_bool_control = make_bool_control,
	intro = default_intro,
	handle_events = default_handle_events,
	logic = default_logic,
	sub_menu_logic  =sub_menu_logic,
	post_sub_menu_logic = post_sub_menu_logic,
	render = default_render,
	reload = default_reload,
	refresh = default_refresh
}



