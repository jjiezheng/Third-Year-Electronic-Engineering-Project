dofile( include( "strings" ) )
dofile( include( "load_settings" ) )


Projection = perspective( 45.0, 16.0/10.0, 1, 10000.0 )
View = mat4(1.0)

Highscores = {}

function score_sort(a,b)
	if a[2] > b[2] then
		return true
	end
	return false
end



function Highscores.Start(self)
	sys.title( "Highscores")
	dofile( "../Settings/Highscores.lua" )

	self.title = makeString( "High scores", "Circle_110", 0, 300, "centre" )
	self.title.colour = vec4(0.67,0.16,0.16,0.8) 


	table.sort(Highscores, score_sort)
	self.highscore_text = {}
	for i,v in ipairs(Highscores) do
		self.highscore_text[i] =  makeString( 
			v[1].." : "..tostring(v[2]), 
			"Circle_110", 
			0, 340-(60*i)-60, 
			"centre" )
		self.highscore_text[i].colour = vec4(((11-i)/15)+0.3)

		if i == 10 then break end
	end
end

function Highscores.HandleEvents( self, _events )
	return false
end

function Highscores.Logic( self, _deltaTime )

	UpdateText( self.title, "High scores" )
	for i,v in ipairs(self.highscore_text) do
		UpdateText( v, v.lua_string )
	end
end

function Highscores.Render(self)
	self.title:render()

	for i,v in ipairs(self.highscore_text) do
		v:render()
	end
end

function Highscores.Reload(self)
	self.title:reload()

	for i,v in ipairs(self.highscore_text) do
		v:reload()
	end
end
