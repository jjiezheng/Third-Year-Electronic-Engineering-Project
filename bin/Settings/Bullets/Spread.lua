Spread =
{
	Type = "Spread",
	Damage = 5,
	maxVelocity = 2500,

	hitboxX = 0,
	hitboxY = 0,
	hitboxW = 40,
	hitboxH = 40,

	Delay =
	{
		WaitFor = 100,
		activeFor = 200
	},
	ParticleTrail =
	{
		particleType = "Explosion",
		timeBetweenParticles = 100
	},

	Sprites =
	{
		Normal =
		{
			Main =
			{
				Shader = "Basic",
				Texture = "bullets",
				AnimationType = "Forwards",
				timeBetweenFrames = 300,
				priority = 9,


				x = 0,
				y = 0,
				w = 100,
				h = 100,

				sprites =
				{
					n0 = "bullets.bullet5",
					n1 = "bullets.bullet6",
					n2 = "bullets.bullet7"
				}
			}
		}
	}
}
