#pragma once
#include <unordered_map>

class RenderManager
{
private:

public:
	static RenderManager& GetInstance();
	RenderManager(RenderManager const&) = delete;
	void operator=(RenderManager const&) = delete;


private:
	RenderManager() {}
};

