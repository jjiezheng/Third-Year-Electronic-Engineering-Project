Overlay = {}

rotation = 0
_time = 0

function makeString( text, font, X, Y, _align )
	local String = stringObject()
	String.pos = vec3( X, Y, -950 )
	String.colour = vec4( 0.67, 0.16, 0.16,0.8 )
	String.alignTo = _align

	String:font( font ):text( text ):shader( "String" ):depth( false )
	String:uniform( "Projection", perspective( 45.0, 16.0/10.0, 0.1, 1000.0 ) )
	String:uniform( "View", rotate( translate( mat4(1.0), String.pos ), 180.0, vec3( 1.0,0.0,0.0) ) )
	String:uniform( "Colour", String.colour )
	return String
end

function UpdateText( text, update )
	local _pos = vec3(0)

	if text.alignTo == "left" then
		_pos = text.pos + text:align()
	elseif text.alignTo == "right" then
		_pos = text.pos - text:align()
	else
		_pos = text.pos
	end

	text:uniform( "View", rotate( translate( mat4(1.0), _pos ), 180.0, vec3( 1.0,0.0,0.0) ) )
	text:uniform( "Colour", text.colour )
	text:text( update )
end






















function AnimationClosure(active)
	local Z = 0
	local start = -950
	local finish = -750
	local animationTime = 100
	local currentTime = 0
	local lastFrameShow = false


	if not active then
		currentTime = 100
	end

	return function( show, _deltaTime )

		if lastFrameShow ~= show then
			currentTime = currentTime - animationTime
			if currentTime < 0 then currentTime = 0 end
		end

		if currentTime < animationTime then
			currentTime = currentTime + _deltaTime
		else currentTime = animationTime end

		if show then
			Z = mix( finish, start, smoothstep( 0, animationTime, currentTime) )
		else
			Z = mix( start, finish, smoothstep( 0, animationTime, currentTime) )
		end

		lastFrameShow = show
		return Z
	end
end

















function Overlay.ToggleActive(self)
	if self.isActive then
		self.isActive = false
	else
		self.isActive = true
	end

end

function Overlay.ToggleShowCounter(self)
	if self.showCounters then
		self.showCounters = false
	else
		self.showCounters = true
	end
end





























function Overlay.Start(self)
	self.preAlpha = makeString( "Daniel Hartnett - ELX3", "Circle" ,510.0, 375.0, "centre" )
	self.title = makeString( "overlay", "visitor" ,510.0, 375.0, "centre" )

	self.theTime = makeString( "Caturday", "visitor" ,510.0, 375.0, "left" )
	self.theResolution = makeString( "Caturday", "visitor" ,510.0, 355.0, "left" )
	self.theFrameRate = makeString( "Caturday", "visitor" ,510.0, 335.0, "left" )

	self.hint = makeString( "F1 for help", "visitor" ,-610.0, 365.0, "right" )
	self.refresh = makeString( "F5 for refresh", "visitor" ,-610.0, 345.0, "right" )
	self.restart = makeString( "F6 for restart", "visitor" ,-610.0, 325.0, "right" )

	self.isActive = false
	self.finishedAnimation = false
	self.showCounters = false

	self.ToggleUI = AnimationClosure(false)
end

function Overlay.HandleEvents(self, _events)
	if events.isKeyDown( _events, key.lctrl ) and events.wasKeyPressed( _events, key.tab ) then
		self:ToggleActive()
	end

	if events.wasKeyPressed( _events, key.f1 ) then
		self:ToggleShowCounter()
	end

	return false
end


function Overlay.Logic(self, _deltaTime)
	_time = _time + _deltaTime

	local Z = self.ToggleUI(self.showCounters, _deltaTime)

	self.preAlpha.pos = vec3(0.0, -375.0, Z)
	UpdateText( self.preAlpha, "Daniel Hartnett - ELX3" )

	self.title.pos = vec3( 0, 375.0, Z)
	UpdateText( self.title, "overlay" )

	UpdateText( self.hint, "F1 for help" )

	self.refresh.pos = vec3(-610.0, 345.0, Z)
	UpdateText( self.refresh, "F5 for refresh" )

	self.restart.pos = vec3(-610.0, 325.0, Z)
	UpdateText( self.restart, "F6 for restart" )


	self.theTime.pos = vec3(610.0, 385.0, Z)
	UpdateText( self.theTime, sys.getTime() )

	self.theResolution.pos = vec3(610.0, 365.0, Z)
	UpdateText( self.theResolution, sys.getResolution() )

	self.theFrameRate.pos = vec3(610.0, 345.0, Z)
	UpdateText( self.theFrameRate, sys.getFrameRate() )

end


function Overlay.Render(self)
	self.theTime:render()
	self.theResolution:render()
	self.theFrameRate:render()

	self.preAlpha:render()
	self.title:render()

	self.hint:render()
	self.refresh:render()
	self.restart:render()
end

function Overlay.Reload(self)
	self.theTime:reload()
	self.theResolution:reload()
	self.theFrameRate:reload()

	self.preAlpha:reload()
	self.title:reload()

	self.hint:reload()
	self.refresh:reload()
	self.restart:reload()
end