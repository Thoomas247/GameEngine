
#ifdef DEV_MODE

#include "core/Engine.h"

int main()
{
	return Engine::Run();
}

#else

#include "core/Game.h"

int main()
{
	return Game::Run();
}

#endif // DEV_MODE