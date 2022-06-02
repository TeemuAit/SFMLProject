#include <functional>
#include <random> // srand, rand,
#include <time.h> // time
#include <assert.h>
#include <math.h>
#include <map>
#include "Enemy.h"
#include "Agent.h"
#include "StateMachine.h"
#include <LinearAlgebra.h>
#include "ext/pixelgridworld/include/pixelgridworld/core.h"

namespace TowerDefense
{

	Enemy::Enemy()
	{
		//Enemy textures & position
		enemyTexture = new sf::Texture();
		enemyTexture->loadFromFile("Resources/Enemy.png");
		enemySprite = new sf::Sprite(*enemyTexture);
		enemySprite->setScale(Vector2f(1.0f, 1.0f));
		sf::Vector2f enemyPosition(265.0f, 330.0f);
		enemySprite->setPosition(enemyPosition);

		pathIndex = 0;
		enemyTimer = 1.f;
	}

	void Enemy::Pathfinding(const std::vector<std::vector<int> >& map)
	{

		auto getPixel = [&map](const core::Vector& pos) -> int
		{
			auto x = pos[0];
			auto y = pos[1];
			return map[y][x];
		};

		auto isLegalState = [&map](const core::Vector& pos) -> bool {
			auto x = pos[0];
			auto y = pos[1];
			if (y >= 0 && y < map.size() && x >= 0 && x < map[y].size()) {
				return map[y][x] != 1;
			}
			return false;
		};

		ActionsVector actions;

		auto getNextLegalStates = [&isLegalState, &map, &actions](const core::Vector& currentState) -> core::Matrix {
			core::Matrix results;
			for (ActionId actionId = 0; actionId < actions.size(); ++actionId) {
				auto newState = transition(actions, currentState, actionId);
				if (isLegalState(newState)) {
					results.push_back(newState);
				}
				else {
					results.push_back(currentState);
				}
			}
			return results;
		};

		// calculate initial node:
		int i = 0;
		do {
			state = { i, (int)map.size() - 1 };
		} while (map[map.size() - 1][i++] == 1);

		// calculate goal node:
		i = 0;
		do {
			goalState = { i, 0 };
		} while (map[0][i++] == 1);

		//Movement
		actions.push_back([](core::Vector state) -> core::Vector {
			state[0] += 1; // Walk right
			return state;
			});
		actions.push_back([](core::Vector state) -> core::Vector {
			state[0] -= 1; // Walk left
			return state;
			});
		actions.push_back([](core::Vector state) -> core::Vector {
			state[1] += 1; // Walk down
			return state;
			});
		actions.push_back([](core::Vector state) -> core::Vector {
			state[1] -= 1; // Walk up
			return state;
			});


		auto getGCost = [](SearchNode* node) -> float {
			float totalG = 0;

			while (node)
			{
				totalG += node->cost;
				node = node->prevNode;
			}
			return totalG;
		};

		auto getHCost = [](SearchNode* node, core::Vector endState) -> float {
			return distance(endState, node->stateAction.state);
		};

		// F = G + H
		auto getFCost = [getGCost, getHCost](SearchNode* node, core::Vector endState) -> float {
			return getGCost(node) + getHCost(node, endState);
		};
		std::vector<SearchNode*> openList;

		openList = astar(openList, getNextLegalStates, getFCost, getGCost, state, goalState);

		storeFinalPath(openList[0]);
	}

	void Enemy::revive() {
		health = 100;
		IWon = false;

		sf::Vector2f enemyPosition(265.0f, 330.0f);
		enemySprite->setPosition(enemyPosition);

		pathIndex = 0;
		enemyTimer = 1.f;
	}

	bool Enemy::checkHit(sf::Vector2f pos)
	{
		if (enemySprite->getGlobalBounds().contains(pos))
		{
			health -= 5;
			return true;
		}
		return false;
	}

	void Enemy::update(float deltaTime)
	{
		if (!isAlive())return;

		if (pathIndex > FinalPath.size() - 1)
		{
			IWon = true;
			return;
		}

		//Timer: 1 second
		enemyTimer -= deltaTime;
		if (enemyTimer <= 0.f)
		{
			enemyTimer = 1.f;
			core::Vector newPos = FinalPath[pathIndex++]->stateAction.state;
			enemySprite->setPosition(newPos[0] * 64.0f, newPos[1] * 64.0f);
		}
	}

	void Enemy::storeFinalPath(SearchNode* sn)
	{
		// arrived at initial state
		if (sn->stateAction.state == state) {
			// stored
			FinalPath.push_back(sn);
			return;
		}
		storeFinalPath(sn->prevNode);
		FinalPath.push_back(sn);
	}

}
