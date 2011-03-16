HapticsDemo1 = {}
function HapticsDemo1.Start(self)
	sys.title( "Haptics" )

	haptics.init()
	haptics.workspace()

	self.running = false
	self.effectType = "constant"
end

function HapticsDemo1.HandleEvents( self, _events )

	if events.wasKeyPressed( _events, key.space ) then
		if self.running then
			self.running = false
			haptics.stop_effect( self.effect )
		else
			self.running = true
			self.effect = haptics.start_effect( self.effectType, 0.8, 0.9 )
		end
	end

	if events.wasKeyPressed( _events, key['1'] ) then
		self.effectType = "constant"
	elseif events.wasKeyPressed( _events, key['2'] ) then
		self.effectType = "viscous"
	elseif events.wasKeyPressed( _events, key['3'] ) then
		self.effectType = "spring"
	elseif events.wasKeyPressed( _events, key['4'] ) then
		self.effectType = "friction"
	end

	return false
end

function HapticsDemo1.Logic( self, _deltaTime )

end

function HapticsDemo1.Render(self)
	haptics.run()
end

function HapticsDemo1.Reload(self)

end
