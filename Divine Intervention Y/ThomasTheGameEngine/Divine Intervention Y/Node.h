#pragma once
#include <Billboard.h>
#include <Flipbook.h>

class Node :
	public GameObject
{
public:
	Node(Level *, Vec3, std::string fileName_);
	~Node();

	void Update(float _deltaTime) override;

	std::string GetFileName();
private:

	//String that dictates which xml to load
	std::string levelFileName;

	float elapsedTime;

	Billboard * b;
	Flipbook * fb;
};

