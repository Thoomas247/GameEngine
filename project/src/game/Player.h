#ifndef PLAYER
#define PLAYER

/*
	Player class
	Class used in-engine purely for testing pusposes, but
	should be implemented in game (or not if not needed)
*/

#include "../core/GameObject.h"

class Player : public GameObject
{
private:
	float Speed = 10.0f;	// m/s

public:
	Player();

private:
	void onUpdate(const float& deltaTime) override;
};

#endif // !PLAYER
