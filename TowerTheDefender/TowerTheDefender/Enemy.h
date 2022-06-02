#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
using namespace sf;

class SearchNode;
namespace TowerDefense
{

	class Enemy
	{
	public:
		Enemy();

		void Pathfinding(const std::vector<std::vector<int> >& map);

		Sprite* getsprite()
		{
			return enemySprite;
		}

		bool isAlive() { return health > 0; }
		bool enemyWon() { return IWon; }
		void revive();

		bool checkHit(sf::Vector2f pos);
		void update(float deltaTime);

	private:
		int health = 100;
		bool IWon = false;
		void storeFinalPath(SearchNode* sn);
		std::vector<SearchNode*> FinalPath;
		int pathIndex;
		float enemyTimer;

		sf::Texture* enemyTexture;
		Sprite* enemySprite;

		//Initial node
		std::vector<int> state;
		//goal nodde
		std::vector<int> goalState;

	};

}

