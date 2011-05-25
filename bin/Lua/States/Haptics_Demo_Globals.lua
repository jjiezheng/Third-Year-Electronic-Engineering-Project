dofile( include( "math" ) )
dofile( include( "ExamineCamera" ) )
dofile( include( "haptic_effects" ) )
dofile( include( "make_earth" ) )
dofile( include( "strings" ) )

Projection = perspective( 65.0, 16.0/10.0, 1, 500.0 )
View = mat4(1.0)

Haptics_Demo_Globals = {}
function Haptics_Demo_Globals.Start(self)
	sys.title( "Haptics Forces Demo" )
	haptics.workspace( perspective( 65.0, 16.0/10.0, 1, 10.0 ), mat4(1) )

	self.running = true
	self.effect = {}
	self.effect.name = "constant"
	self.effect.pos = vec3(0.5)
	self.effect.dir = vec3(-0.5, 0, -1)
	self.effect.gain = 0.1
	self.effect.mag = 0.7
	self.effect.handle = haptics.start_effect{self.effect}

	self.controls1 = makeString( "Press 1 for a constant force effect", "Circle_110", 0, 120, "centre" )
	self.controls2 = makeString( "Press 2 for a viscous force effect", "Circle_110", 0, 40, "centre" )
	self.controls3 = makeString( "Press 3 for a spring force effect", "Circle_110", 0, -40, "centre" )
	self.controls4 = makeString( "Press 4 for a friction force effect", "Circle_110", 0, -120, "centre" )

	self.controls1.colour = vec4(1,1,1,0.8)
	self.controls2.colour = vec4(1,1,1,0.8)
	self.controls3.colour = vec4(1,1,1,0.8)
	self.controls4.colour = vec4(1,1,1,0.8)
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
	local position = haptics.proxy_position()
	local trans = haptics.proxy_transform()

	if self.reload then
		self.reload = false
		haptics.stop_effect( self.effect.handle )
		self.effect.handle = haptics.start_effect( self.effect )
	end

	self.controls1.colour = vec4(1,1,1,0.8)
	self.controls2.colour = vec4(1,1,1,0.8)
	self.controls3.colour = vec4(1,1,1,0.8)
	self.controls4.colour = vec4(1,1,1,0.8)

	if self.effect.name == "constant" then
		self.controls1.colour = vec4(0.67,0.16,0.16,0.8)
	elseif self.effect.name == "viscous" then
		self.controls2.colour = vec4(0.67,0.16,0.16,0.8)
	elseif self.effect.name == "spring" then
		self.controls3.colour = vec4(0.67,0.16,0.16,0.8)
	elseif 	self.effect.name == "friction" then
		self.controls4.colour = vec4(0.67,0.16,0.16,0.8)
	end

	UpdateText( self.controls1, self.controls1.lua_string )
	UpdateText( self.controls2, self.controls2.lua_string )
	UpdateText( self.controls3, self.controls3.lua_string )
	UpdateText( self.controls4, self.controls4.lua_string )
end


function Haptics_Demo_Globals.Render(self)
	self.controls1:render()
	self.controls2:render()
	self.controls3:render()
	self.controls4:render()
end

function Haptics_Demo_Globals.Reload(self)
	Haptics_Demo_Globals:Start()
end