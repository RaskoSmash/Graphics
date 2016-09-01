#pragma once

class Timer
{
	float currentTime, previousTime, deltaTime;
public:
	bool init();
	bool step();
	bool term();

	float getDeltaTime() const;
	float getTotalTime() const;

	float getRealTime() const;
};