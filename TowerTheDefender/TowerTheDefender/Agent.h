#pragma once
#include <StateMachine.h>
#include <core.h>
#include <LinearAlgebra.h>
#include <map>

auto randomAction(const ActionsVector& actions, const core::Vector& state) {

    ActionId actionId = rand() % actions.size();
    return transition(actions, state, actionId);
}

struct SearchNode
{
    SearchNode(StateAction sa, SearchNode* parentNode = 0) : prevNode(parentNode), stateAction(sa), cost(0)
    {
        if (parentNode)
        {
            cost = distance(stateAction.state, parentNode->stateAction.state);
        }
    }

    SearchNode* prevNode;
    float       cost;
    StateAction stateAction;
};

ActionId getFirstAction(SearchNode* node)
{
    ActionId actionId = 0;
    while (node != 0 && node->prevNode != 0)
    {
        actionId = node->stateAction.action;
        node = node->prevNode;
    }
    return actionId;
}

template<typename GetNextStatesFunc, typename FcostFunc, typename GCostFunc>
std::vector<SearchNode*> astar(std::vector<SearchNode*>& openList, GetNextStatesFunc makeAllActions, FcostFunc getFCost, GCostFunc getGCost, const core::Vector& initialState, const core::Vector& endState)
{
    std::map<core::Vector, bool> closedList;
    openList.push_back(new SearchNode(StateAction{ initialState, 0 }));

    while (openList.size() > 0)
    {
        //Find min f from open list
        size_t minIndex = 0;
        float minFCost = getFCost(openList[0], endState);
        for (size_t i = 0; i < openList.size(); i++)
        {
            float newCost = getFCost(openList[i], endState);
            if (newCost < minFCost)
            {
                minIndex = i;
                minFCost = newCost;
            }
        }

        SearchNode* currentNode = openList[minIndex];
        openList.erase(openList.begin() + minIndex, openList.begin() + minIndex + 1);
        closedList[currentNode->stateAction.state] = true;

        // EXIT: arrived at the endState, return entire path towards it
        if (currentNode->stateAction.state == endState)
        {
            return std::vector<SearchNode*>({ currentNode });
        }

        core::Matrix neighbousStates = makeAllActions(currentNode->stateAction.state);
        for (ActionId actionId = 0; actionId < neighbousStates.size(); ++actionId)
        {
            auto state = neighbousStates[actionId];
            if (state == currentNode->stateAction.state)
            {
                continue;
            }
            if (closedList.find(state) != closedList.end())
            {
                continue;
            }
            assert(actionId < 4);

            SearchNode* newNode = new SearchNode(StateAction{ state, actionId }, currentNode);
            auto it = std::find_if(openList.begin(), openList.end(), [newNode](const SearchNode* n) -> bool {
                return n->stateAction.state == newNode->stateAction.state; });

            if (it != openList.end())
            {
                if (getGCost(*it) > getGCost(newNode))
                {
                    (*it) = newNode;
                }
                else
                {
                    delete newNode;
                }
            }
            else
            {
                openList.push_back(newNode);
            }
        }

    }
}
template<typename GetNextStatesFunc>
auto breadthFirstSearch(GetNextStatesFunc makeAllActions, const core::Vector& initialState, const core::Vector& endState)
{
    std::vector<SearchNode*>openList;
    std::map<core::Vector, bool> closedList;
    openList.push_back(new SearchNode(StateAction{ initialState, 0 }));

    while (openList.size() > 0)
    {
        SearchNode* currentNode = openList[0];
        openList.erase(openList.begin(), openList.begin() + 1);
        closedList[currentNode->stateAction.state] = true;

        if (currentNode->stateAction.state == endState)
        {
            return std::vector<SearchNode*>({ currentNode });
        }

        core::Matrix neighbousStates = makeAllActions(currentNode->stateAction.state);
        for (ActionId actionId = 0; actionId < neighbousStates.size(); ++actionId)
        {
            auto state = neighbousStates[actionId];
            if (state == currentNode->stateAction.state)
            {
                continue;
            }
            if (closedList.find(state) != closedList.end())
            {
                continue;
            }
            assert(actionId < 4);

            SearchNode* newNode = new SearchNode(StateAction{ state, actionId }, currentNode);


            openList.push_back(newNode);
        }

    }

    for (size_t i = 0; i < 7; ++i)
    {

    }
    return openList;
}
auto greedyAction(const ActionsVector& actions, const core::Vector& state, const core::Vector& end) {

    ActionId bestActionId = 0;
    float bestDistance = distance(end, transition(actions, state, 0));
    for (ActionId actionId = 1; actionId < actions.size(); ++actionId)
    {
        auto newState = transition(actions, state, actionId);
        auto curDistance = distance(end, newState);
        if (curDistance < bestDistance)
        {
            bestActionId = actionId;
            bestDistance = curDistance;
        }
    }

    return transition(actions, state, bestActionId);
}
