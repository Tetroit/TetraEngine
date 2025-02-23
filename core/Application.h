#pragma once

namespace TetraEngine {
	class Application
	{
	private:
	public:
		Application() = default;
		virtual ~Application() = default;
		virtual void Update();
	};
}

