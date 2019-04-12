#include "Mouse.h"



void Mouse::Movement(int x, int y)
{
	if(this->MouseState){
		if (this->FirstClick) {
			this->FirstClick = false;
			LastPosition = Vector2(x, y);
		}
	
		Vector2 offset = Vector2(LastPosition.coordinates[0] - x, LastPosition.coordinates[1] - y);
	LastPosition.coordinates[0] = x;
	LastPosition.coordinates[1] = y;

	offset.coordinates[0] *= this->Sensivity;
	offset.coordinates[1] *= this->Sensivity;
	this->MouseX = offset.coordinates[0];
	this->MouseY = offset.coordinates[1];
//	std::cout << LastPosition << std::endl;
//	std::cout << Vector2(x, y) << std::endl;
//	std::cout << offset << std::endl;

	}
}

Mouse::Mouse()
{
	this->LastPosition = Vector2();
	this->Position = Vector2();
}


Mouse::~Mouse()
{
}
