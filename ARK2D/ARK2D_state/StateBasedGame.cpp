/*
 * StateBasedGame.cpp
 *
 *  Created on: 15 Jul 2010
 *      Author: Ashley
 */

#include <vector>
#include "StateBasedGame.h"
#include "GameState.h"
#include "LoadingState.h"
#include "Transition.h"
#include "EmptyTransition.h"

StateBasedGame::StateBasedGame(const char* title): Game(title), m_loading_state(NULL) {

}

const char* StateBasedGame::getTitle() {
	return Game::getTitle();
}
void StateBasedGame::addState(GameState* state) {
	m_states.push_back(state);
	m_current_state = state;
}
void StateBasedGame::setLoadingState(LoadingState* state) {
	m_loading_state = state;
}
LoadingState* StateBasedGame::getLoadingState() {
	return m_loading_state;
}
void StateBasedGame::enterState(unsigned int id) {
	for (unsigned int i = 0; i < m_states.size(); i++) {
		if (m_states.at(i)->id() == id) {
			enterState(m_states.at(i));
			break;
		}
	}
}
void StateBasedGame::enterState(GameState* state) {
	enterState(state, new EmptyTransition(), new EmptyTransition());
}

void StateBasedGame::enterState(GameState* state, Transition* leave, Transition* enter) {

	m_from_state = m_current_state;
	m_current_state = state;

	if (leave == NULL) {
		//std::cout << "leave was null :(" << std::endl;
		leave = new EmptyTransition();
	}
	if (enter == NULL) {
		//std::cout << "enter was null :(" << std::endl;
		enter = new EmptyTransition();
	}

	m_leaveTransition = leave;
	m_enterTransition = enter;

	m_leaveTransition->init(m_container, this, m_from_state, state);
}
GameState* StateBasedGame::getCurrentState() {
	return m_current_state;
}
unsigned int StateBasedGame::getCurrentStateID() {
	return m_current_state->id();
}

vector<GameState*> StateBasedGame::getStates() {
	return m_states;
}

void StateBasedGame::init(GameContainer* container) {
	m_container = container;

	initStates(container);

	for (unsigned int i = 0; i < m_states.size(); i++) {
		m_states.at(i)->init(container, this);
	}
}

void StateBasedGame::preUpdate(GameContainer* container, GameTimer* timer) {

}
void StateBasedGame::postUpdate(GameContainer* container, GameTimer* timer) {

}
void StateBasedGame::update(GameContainer* container, GameTimer* timer) {

	preUpdate(container, timer);

	Game::update(container, timer);

	if (m_leaveTransition != NULL) {
		//std::cout << "leave wasn't null once! :(" << std::endl;
		m_leaveTransition->update(container, this, timer);
		if (m_leaveTransition->isComplete()) {
			m_from_state->leave(container, this, m_current_state);

			delete m_leaveTransition;
			m_leaveTransition = NULL;

			if (m_enterTransition == NULL) {
				m_current_state->enter(container, this, m_from_state);
			} else {
				m_enterTransition->init(container, this, m_from_state, m_current_state);
			}
		} else {
			return;
		}
	}

	if (m_enterTransition != NULL) {
		//std::cout << "enter wasn't null once! :(" << std::endl;
		m_enterTransition->update(container, this, timer);
		if (m_enterTransition->isComplete()) {
			m_current_state->enter(container, this, m_from_state);

			delete m_enterTransition;
			m_enterTransition = NULL;
		} else {
			return;
		}
	}


	m_current_state->update(container, this, timer);

	postUpdate(container, timer);
}

void StateBasedGame::preRender(GameContainer* container, Graphics* g) {

}
void StateBasedGame::postRender(GameContainer* container, Graphics* g) {

}
void StateBasedGame::render(GameContainer* container, Graphics* g) {
	preRender(container, g);


	// Render pre-enter transition
	if (m_leaveTransition != NULL) {
		m_leaveTransition->preRender(container, this, g);
	} else if (m_enterTransition != NULL) {
		m_enterTransition->preRender(container, this, g);
	}

	// Render Current State.
	m_current_state->render(container, this, g);

	// Render post-enter transition
	if (m_leaveTransition != NULL) {
		m_leaveTransition->postRender(container, this, g);
	} else if (m_enterTransition != NULL) {
		m_enterTransition->postRender(container, this, g);
	}

	postRender(container, g);
}

bool StateBasedGame::isTransitioning() {
	return (m_enterTransition != NULL || m_leaveTransition != NULL);
}

GameContainer* StateBasedGame::getContainer() {
	return m_container;
}

void StateBasedGame::keyPressed(unsigned int key) {
	Game::keyPressed(key);
	m_current_state->keyPressed(key);
}
void StateBasedGame::keyReleased(unsigned int key) {
	Game::keyReleased(key);
	m_current_state->keyReleased(key);
}

StateBasedGame::~StateBasedGame() {

}
