
if math.included == nill then
	local sin, cos = math.sin, math.cos
	math.sin = function (x) return sin(math.rad(x)) end
	math.cos = function (x) return cos(math.rad(x)) end
	math.uQuat = function() return quat( 1.0, vec3(0.0) ) end
	math.uMat3 = function() return mat3( 1.0 ) end
	math.uMat4 = function() return mat4( 1.0 ) end
	math.included = true
end
