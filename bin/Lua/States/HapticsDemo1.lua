dofile( include( "math" ) )


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


HapticsDemo1 = {}
function HapticsDemo1.Start(self)
	sys.title( "Haptics" )

	haptics.init()
	haptics.workspace( mat4(1), mat4(1) )

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
end

function HapticsDemo1.HandleEvents( self, _events )
--[[
	if events.wasKeyPressed( _events, key.space ) then
		if self.running then
			self.running = false
			haptics.stop_effect( self.effect )
		else
			self.running = true
			self.effect = haptics.start_effect( self.effectType, 0.8, 0.9 )
		end
	end
	--]]
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

function HapticsDemo1.Logic( self, _deltaTime )
--
	if self.reload then
		self.reload = false
		haptics.stop_effect( self.effect.handle )
		self.effect.handle = haptics.start_effect( self.effect )
	end
	--]]
end

function HapticsDemo1.Render(self)
	if self.running then
		haptics.run()
	end
end

function HapticsDemo1.Reload(self)
	HapticsDemo1:Start()
end
