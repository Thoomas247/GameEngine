#ifdef DEV_MODE
#include "core/Engine.h"
#else
#include "core/Game.h"
#endif // DEV_MODE

int main()
{
#ifdef DEV_MODE
	return Engine::Run();
#else
	return Game::Run();
#endif // DEV_MODE
}