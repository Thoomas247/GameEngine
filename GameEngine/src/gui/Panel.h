#pragma once

class Panel
{
public:
	Panel();
	~Panel();

	virtual void Update(const float& deltaTime);

protected:
	virtual void destroy();
};