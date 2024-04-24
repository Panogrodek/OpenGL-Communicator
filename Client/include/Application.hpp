#pragma once

class Application {
public:
	Application();
	~Application();

	void Run();
private:
	void Update();
	void Render();

	float m_timeAccumulator = 0.f;
	float m_lastFrame = 0.f;
};