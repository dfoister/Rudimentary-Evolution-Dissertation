#include "EvolutionSimulation.h"
#include "Physics/SimPhysics.h"
#include "Bodies/BaseObject.h"
#include "Bodies/Food.h"
#include "Bodies/Wall.h"
#include "Bodies/Organism.h"
#include "Bodies/Poison.h"
#include <iostream>

EvolutionSimulation::EvolutionSimulation(Rendering* r)
{
	physics = new SimPhysics();
	this->renderer = r;
	gameTime = 0.0f;
	InitialiseGame();
	CreateWallColliders();
}

EvolutionSimulation::~EvolutionSimulation()
{
}



void EvolutionSimulation::addNewObject(BaseObject* object)
{
	newObjects.emplace_back(object);
	physics->addBody(object);
	if (object->getCollider()) {
		physics->addCollider(object->getCollider());
	}
}

void EvolutionSimulation::update(float dt)
{

	for (auto i : newObjects) {
		gameObjects.emplace_back(i);
	}
	newObjects.clear();


	//renderer->DrawCircle(sf::Vector2f(100, 100), 50.0f, sf::Color::Green);

	gameTime += dt;
	physics->update(dt);


	std::cout << "BBBBBBBBBBBBBBB";
	//srand((int)(gameTime * 1000.0f));

	for (auto i = gameObjects.begin(); i != gameObjects.end(); ) {
		if (!(*i)->updateObject(dt)) { //object has said its finished with
			physics->removeCollider((*i)->getCollider());
			physics->removeBody((*i));
			delete (*i);
			i = gameObjects.erase(i);
		}
		else {
			CollisionBounds* col = (*i)->getCollider();
			std::cout << "AAAAAAAA";
			if (col) {
				std::cout << "EEEEEEEEE";
				/// DRAWS ALL COLLIDER BOXES
				if (col->getShape() == CollisionBounds::Shapes::CIRCLE) {
					std::cout << "Hi";
					renderer->DrawCircle(sf::Vector2f(col->getPosition().x(), col->getPosition().y()), col->getRadius(), sf::Color::Blue);
				}
				else {
					renderer->DrawBox(sf::Vector2f(col->getPosition().x(), col->getPosition().y()), col->getWidth(), col->getHeight() / 2, sf::Color::Green);
				}
				
			}
			

			++i;
		}
	}
}

void EvolutionSimulation::InitialiseGame()
{
	for (auto o : gameObjects) {
		delete o;
	}
	gameObjects.clear();

	Food* foodthing = new Food();
	foodthing->setPos(Eigen::Vector2f(640, 360));
	foodthing->updateCollider();
	addNewObject(foodthing);
	

	Organism* organisms[20];
	Food* food[20];
	Poison* poison[15];

	CreateWallColliders();

	gameTime = 0;

}

void EvolutionSimulation::CreateWallColliders()
{
	CollisionBounds* leftWallC = new CollisionBounds(CollisionBounds::Shapes::BOX, CollisionBounds::Types::WALL, 5.0f, 720.0f);
	Wall* leftWall = new Wall(leftWallC,Vector2f(2.5f,360.0f));
	addNewObject(leftWall);

	CollisionBounds* rightWallC = new CollisionBounds(CollisionBounds::Shapes::BOX, CollisionBounds::Types::WALL, 5.0f, 720.0f);
	Wall* rightWall = new Wall(rightWallC, Vector2f(1277.5f, 360.0f));
	addNewObject(rightWall);

	CollisionBounds* topWallC = new CollisionBounds(CollisionBounds::Shapes::BOX, CollisionBounds::Types::WALL, 1280.0f,5.0f);
	Wall* topWall = new Wall(topWallC, Vector2f(640.0f, 717.5));
	addNewObject(topWall);

	CollisionBounds* bottomWallC = new CollisionBounds(CollisionBounds::Shapes::BOX, CollisionBounds::Types::WALL, 1280.0f, 5.0f);
	Wall* bottomWall = new Wall(bottomWallC, Vector2f(640.0f, 2.5f));
	addNewObject(bottomWall);
}
