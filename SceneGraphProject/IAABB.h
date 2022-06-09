#pragma once
struct AABB;

class IAABB
{
public:
	virtual ~IAABB() = default;
	virtual AABB& GetAABB() = 0;
};