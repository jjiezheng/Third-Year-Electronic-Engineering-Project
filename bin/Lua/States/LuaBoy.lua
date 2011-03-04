dofile( include( "bit" ) )
--dofile( include( "hex" ) )

LuaBoy = {}



function Build_Z80()

	Z80 =
	{
		clock = { m = 0, t = 0 },
		r =	{
			a = 0 ,b = 0 ,c = 0 ,d = 0 ,e = 0 ,h = 0 ,l = 0 ,f = 0 ,
			pc = 0, sp = 0,
			m = 0,t = 0
		}
	}

	-- Add E to A, leaving result in A (ADD A, E)
	function Z80.ADDr_e(self)
		self.r.a = self.r.a + self.r.e
		self.r.f = 0

		if not bit.band(self.r.a, 255) then self.r.f = bit.bor(self.r.f,0x80) end
		if self.r.a > 255 then self.r.f = bit.bor(self.r.f,0x10) end
		self.r.a = bit.band(self.r.a, 255)
		self.r.m = 1
		self.r.t = 4
	end

	-- Compare B to A, setting flags (CP A, B)
	function Z80.CPr_b(self)
		local i = self.r.a
		i = i - self.r.b
		self.r.f = bit.bor(self.r.f,0x40)
		if not bit.band(i, 255) then self.r.f = bit.bor(self.r.f,0x80) end
		if i < 0 then self.r.f = bit.bor(self.r.f,0x10) end
		self.r.m = 1
		self.r.t = 4

	end

	function Z80.NOP(self)
		self.r.m = 1
		self.r.t = 4
	end

	return Z80
end



function LuaBoy.Start(self)
	self.cpu = Build_Z80()
end

function LuaBoy.HandleEvents( self, _events )
	return false
end

function LuaBoy.Logic( self, _deltaTime )
	self.cpu:NOP()
end

function LuaBoy.Render(self)

end

function LuaBoy.Reload(self)

end
