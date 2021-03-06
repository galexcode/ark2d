/*
 * Pool.h
 *
 *  Created on: Dec 15, 2011
 *      Author: ashleygwinnell
 */

#ifndef ARKPOOL_H_
#define ARKPOOL_H_

#include "Vector.h"
#include "../../Core/GameObject.h"
#include "../MathUtil.h"
#include "../../Namespaces.h"
#include "../../UI/ErrorDialog.h"
#include "../Util.h"

using namespace std;


namespace ARK {
	namespace Util {
		namespace Containers {


				template <class T=GameObject*>
				class PoolIterator;

				/*!
				 * \brief Standard pooling of GameObjects.
				 *
				 * Holds pointers to dynamically created objects to prevent allocation throughout game-playing. They must be pointer types! 
				 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
				 */
				template <class T=GameObject*>
				class Pool {
					friend class PoolIterator<T>;
					private:
						Vector<T> m_inactive;
						Vector<T> m_active;
						PoolIterator<T>* it;
						unsigned int m_increaseAmount;

					public:
						Pool(): m_inactive(), m_active(), it(NULL), m_increaseAmount(1) {
							m_inactive.setUsingList(true);
							m_active.setUsingList(true);
						}
						Pool(unsigned int amount): m_inactive(), m_active(), it(NULL), m_increaseAmount(2) {
							m_inactive.setUsingList(true);
							m_active.setUsingList(true);
							
							increase(amount);
						}
						void increase() {
							increase(m_increaseAmount);
							m_increaseAmount *= 2;
						}
						void increase(unsigned int amount) {
							for(unsigned int i = 0; i < amount; ++i) { 
								if (is_pointer_type<T>::value) {
									typedef typename remove_pointer<T>::type type;
									T obj = new type();
									add(obj);
									//ErrorDialog::createAndShow("Pool(int) is broken for pointer types. Please populate yourself or fix it. :|");
									//break;
								} else {
									T obj;
									add(obj); 	
								}
							}

						}
						void add(T object) {
							add(object, false);
						}
						void add(T object, bool active) {
							if (active) {
								m_active.add(object);
							} else {
								m_inactive.add(object);
							}
						}
						Vector<T>* getActive() {
							return &m_active;
						}
						Vector<T>* getInactive() {
							return &m_inactive;
						}
						PoolIterator<T>* iterator() {
							if (it == NULL) {
								it = new PoolIterator<T>(this);
							}
							it->reset();
							return it;
						}
						PoolIterator<T>* newiterator() { // make sure to delete afterwards.
							PoolIterator<T>* it = new PoolIterator<T>(this);
							it->reset();
							return it;
						}
						void setUsingList(bool b) {
							m_active.setUsingList(b);
							m_inactive.setUsingList(b);
						}
						bool isUsingList() {
							return m_active.isUsingList();
						}
						T get() {
							if (m_inactive.size() == 0 && m_active.size() ==0) {
								increase();
								//ErrorDialog::createAndShow("Pool is not populated."); 
								//return NULL;
							}
							if (m_inactive.size() == 0) { 
								increase();
								//ErrorDialog::createAndShow("Pool is empty"); 
								//return NULL; 
							}
							
							T obj = m_inactive.pop();
							m_active.add(obj);
							return obj;
						}
						void prune(T obj) {
							obj->setPendingRemoval(false);
							obj->onPrune();
							m_active.remove(obj);
							m_inactive.add(obj);
						}
						void prune(VectorIterator<T>* it, T obj) {
							obj->setPendingRemoval(false);
							obj->onPrune();
							m_inactive.add(obj);
							it->remove();
						}

						void pruneAll() {
							VectorIterator<T>* it = m_active.iterator();
							while (it->hasNext()) {
								T obj = it->next();
								if (obj == NULL) { ErrorDialog::createAndShow("This is bad, very bad."); }
								if (obj->isPendingRemoval()) {

									prune(it, obj);
									/*obj->setPendingRemoval(false);
									obj->onPrune();
									m_inactive.add(obj);
									it->remove();*/
								}
							}
							/*for (unsigned int i = 0; i < m_active.size(); i++) {
								T obj = m_active.get(i);
								if (obj->isPendingRemoval()) {
									prune(obj);
									i = 0;
								}
							}*/
						}
						void reset() {
							VectorIterator<T>* it = m_active.iterator();
							while (it->hasNext()) {
								T obj = it->next();
								obj->setPendingRemoval(true);
							}
							pruneAll();
						}
						string toString() {
							string s = "{";
								s += "\"active\": ";
								s += m_active.toString();
								s += ", ";
								s += "\"usingList\":";
								s += Cast::toString<bool>(m_active.isUsingList());
							s += "}";
							return s;
						}
						unsigned int size() {
							return m_active.size() + m_inactive.size();
						}
						unsigned int sizeActive() {
							return m_active.size();
						}
						
						void updateAll(ARK::Core::GameContainer* container, GameTimer* timer) {
							m_active.updateAll(container, timer);
						}
						void updateSelection(void* selectionFunction) {
							m_active.updateSelection(selectionFunction);
						}

						void renderAll(ARK::Core::GameContainer* container, Renderer* graphics) {
							m_active.renderAll(container, graphics);
						}
						void renderSelection(void* selectionFunction) {
							m_active.renderSelection(selectionFunction);
						}
						virtual ~Pool() {

						}
				};
				template <class T>
				class PoolIterator {
					private:
						Pool<T>* parent;
						unsigned int index;
						typename list<T>::iterator iterator; // Used only for lists.
					public:
						PoolIterator(Pool<T>* p): parent(p), index(0), iterator(NULL) {

						}
						T next() {
							if (parent->isUsingList()) {
								if (hasNext()) {
									T obj = (*iterator);
									++iterator;
									return obj;
								}
								ErrorDialog::createAndShow("Out of items in Pool Iterator (lst) - returning NULL.");
								return NULL;
							}

							if (hasNext()) {
								T obj = parent->m_active.get(index);
								++index;
								return obj;
							}
							ErrorDialog::createAndShow("Out of items in Pool Iterator (vec) - returning NULL.");
							return NULL;
						}
						Pool<T>* getPool() {
							return parent;
						}
						bool hasNext() {
							signed int top = parent->m_active.size();
							if (top == 0) { return false; }

							if (parent->isUsingList()) {
								if (iterator != parent->m_active.lst.end()) {
									return true;
								}
								return false;
							}

							signed int thisIndex = index;
							if (thisIndex >= top) {
								return false;
							}
							return true;
						}
						/**
						 * -1 because .next() forwards to the next element which is not normally the one we are trying to remove.
						 */
						void remove() {
							if (parent->isUsingList()) {
								iterator--;
								T obj = (*iterator);
								iterator = parent->m_active.lst.erase(iterator);
								parent->m_inactive.add(obj);
								return;
							}

							T obj = parent->m_active.get(index-1);
							parent->m_active.remove(index-1);
							parent->m_inactive.add(obj);
						}
						void reset() {
							if (parent->isUsingList()) {
								iterator = parent->m_active.lst.begin();
								return;
							}
							index = 0;
						}
						~PoolIterator() {

						}
				};
		}
	}
}

#endif /* ARKPOOL_H_ */
