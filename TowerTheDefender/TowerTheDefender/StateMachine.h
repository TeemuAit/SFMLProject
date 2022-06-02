#pragma once
#include <functional>
#include <ext/pixelgridworld/include/pixelgridworld/core.h>

// State Machine typedefs
typedef std::function<core::Vector(core::Vector)> ActionFunction;
typedef std::vector<ActionFunction> ActionsVector;
typedef size_t ActionId;

// Makes state Transition to new state from actions vector using action id.

/// <summary>
/// Makes state transition from current state to new state using actionId.
/// </summary>
/// <param name="actions">Actions</param>
/// <param name="state">Current state</param>
/// <param name="actionId">Action id to make the transition.</param>
/// <returns>Returns new state</returns>
auto transition(const ActionsVector& actions, const core::Vector& state, ActionId actionId) {
    return actions[actionId](state);
}

// State-action
struct StateAction {
    core::Vector    state;
    ActionId        action;
};



