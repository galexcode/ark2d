/*
 * GameInterface.cpp
 *
 *  Created on: 15-Oct-2009
 *      Author: user
 */
#include "ARK2D.h"
#include "Game.h"
#include "GameContainer.h"
#include "Timeline.h"
//class GameContainer;

Game::Game(string title) {
	this->m_title = title;
	m_timeline = new Timeline();
}

string Game::getTitle() {
	return m_title;
}
Timeline* Game::getTimeline() {
	return m_timeline;
}

Game::~Game() {
	// delete &m_strTitle; std::strings clean themselves up!
}

//void Game::init(GameContainer& container) = 0;
void Game::update(GameContainer* container, GameTimer* timer) {
	m_timeline->update(timer->getDelta());
}
//void Game::render(GameContainer& container, const Graphics& g) = 0;
//void Game::resize(GameContainer& container, int width, int height) = 0;
void Game::resize(GameContainer* container, int width, int height) {
	#if !defined(ARK2D_ANDROID)
		if (container->getResizeBehaviour() == GameContainer::RESIZE_BEHAVIOUR_SCALE)
		{
			glLoadIdentity();
			glViewport(0, 0, width, height);

		}
		else if (container->getResizeBehaviour() == GameContainer::RESIZE_BEHAVIOUR_NOSCALE)
		{
			glLoadIdentity();
			glViewport(0, 0, width, height);

			glMatrixMode(GL_PROJECTION) ;
			glPushMatrix();
			glLoadIdentity();

			glOrtho(0, width, height, 0, -1, 1);

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
		}
	#else

	#endif
}

void Game::keyPressed(unsigned int key) {

}
void Game::keyReleased(unsigned int key) {

}
void Game::mouseMoved(int x, int y, int oldx, int oldy) {

}
