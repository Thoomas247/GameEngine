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
	std::shared_ptr<GameObject> m_Camera;
	float m_Speed = 100.0f;	// m/s

public:
	Player();

private:
	void onSetUp() override;
	void onUpdate(const float& deltaTime) override;
};

#endif // !PLAYER
