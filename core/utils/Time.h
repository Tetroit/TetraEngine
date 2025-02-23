#pragma once

namespace TetraEngine
{
	class Time
	{
	public:
		static float fps;
		static float time;
		static float prevTime;
		static float deltaTime;
		static float sdeltaTime;
		static float GetFPS();
		static void Update();
	};
}
