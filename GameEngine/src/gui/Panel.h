#pragma once

class Panel
{
public:
	Panel();
	~Panel();

	virtual void Update();

protected:
	virtual void destroy();
};