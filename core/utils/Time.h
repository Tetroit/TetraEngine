#pragma once

namespace TetraEngine
{
	class Time
	{
	public:
		static int frameN;
		static float fps;
		static float time;
		static float prevTime;
		static float deltaTime;
		static float sDeltaTime;
		static float GetFPS();
		static void Update();
	};
}
