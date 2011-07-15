/*
 * Button.h
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "../ARK.h"
#include "../MyString.h"
#include "AbstractUIComponent.h"

class Button : public AbstractUIComponent {
	private:
		MyString m_text;
		unsigned int m_state; // 0 for mouse-off, 1 for mouse-over, 2 for mouse-down.
		void* m_event;
	public:
		static const unsigned int STATE_OFF = 0;
		static const unsigned int STATE_OVER = 1;
		static const unsigned int STATE_DOWN = 2;

		Button():
			AbstractUIComponent(),
			m_text(""),
			m_state(0),
			m_event(NULL)
		{

		}
		void setEvent(void* e) {
			m_event = e;
		}
		void* getEvent() {
			return m_event;
		}
		void keyPressed(unsigned int key) {
			Input* i = ARK2D::getInput();
			if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT
				&& GigaRectangle<int>::s_contains(m_x, m_y, (signed int) (m_width), (signed int) (m_height), (signed int) (i->getMouseX()), (signed int) (i->getMouseY()))) {
					m_state = STATE_DOWN;
					setFocussed(true);
			}
		}

		void keyReleased(unsigned int key) {
			Input* i = ARK2D::getInput();
			if (key == (unsigned int) Input::MOUSE_BUTTON_LEFT) {
				if (GigaRectangle<int>::s_contains(m_x, m_y, (signed int) (m_width), (signed int)(m_height), (signed int) (i->getMouseX()), (signed int) (i->getMouseY()))) {
					if (m_state == STATE_DOWN) {
						m_state = STATE_OVER;
						if (m_event != NULL) {
							void (*pt)() = (void(*)()) m_event;
							pt();
						}
					}
				} else {
					m_state = STATE_OFF;
					setFocussed(false);
				}
			}
		}


		void mouseMoved(int x, int y, int oldx, int oldy) {
			if (m_state == STATE_DOWN) { return; }
			if (GigaRectangle<int>::s_contains(m_x, m_y, m_width, m_height, x, y)) {
				m_state = STATE_OVER;
			} else {
				m_state = STATE_OFF;
			}
		}
		void setText(string s) {
			m_text += s;
		}
		const MyString& getText() {
			return m_text;
		}
		void setState(unsigned int i) {
			m_state = i;
		}
		unsigned int getState() {
			return m_state;
		}
		void render() {
			Graphics* g = ARK2D::getGraphics();
			renderBackground();

			int renderTextX = m_x + (m_width/2) - (g->getFont()->getStringWidth(m_text.get())/2);
			int renderTextY = m_y + (m_height/2) - (g->getFont()->getLineHeight()/2);
			if (m_state == STATE_DOWN) {
				renderTextX += 2;
				renderTextY += 2;
			}
			renderText(renderTextX, renderTextY);

			renderOverlay();
		}
		void renderBackground() {
			Graphics* g = ARK2D::getGraphics();
			g->setDrawColor(Color::black_50a);
			g->fillRect(m_x, m_y, m_width, m_height);
		}
		void renderText(int x, int y) {
			Graphics* g = ARK2D::getGraphics();
			g->setDrawColor(Color::white);
			g->drawString(m_text.get(), x, y);
		}
		void renderOverlay() {
			Graphics* g = ARK2D::getGraphics();
			g->setDrawColor(Color::white);
			if (m_state == STATE_OVER || m_state == STATE_DOWN) {
				g->setDrawColor(Color::white_50a);
			}
			g->drawRect(m_x, m_y, m_width, m_height);
		}
};

#endif /* BUTTON_H_ */
