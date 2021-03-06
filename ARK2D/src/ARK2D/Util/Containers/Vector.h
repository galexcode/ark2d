/*
 * Vector.h
 *
 *  Created on: 21 May 2011
 *      Author: Ashley
 */

#ifndef ARKVECTOR_H_
#define ARKVECTOR_H_

#include "../../ARK2D.h"
#include "../../Namespaces.h"
#if (defined(ARK2D_MACINTOSH) || defined(ARK2D_WINDOWS) || defined(ARK2D_UBUNTU_LINUX))
	#include <vector>
	#include <list>
	#include <iterator>
 	#include <algorithm>
#endif

#include "../../Core/GameObject.h"
#include "../MathUtil.h"
 #include "../../Core/GameContainer.h"
#include "../Util.h"

using namespace std;

namespace ARK {
	namespace Util {
		namespace Containers {

			template <class T=GameObject*>
			class VectorIterator;

			template <class T>
			class PoolIterator;

			/*!
			 * \brief Standard dynamic sized vector/array for game objects. More than likely ARK::Util::Containers::Pool is what you're after.
			 *
			 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
			 */
			template <class T=GameObject*>
			class Vector {
				friend class VectorIterator<T>;
				friend class PoolIterator<T>;
				private:
					#if defined (STL_AVAILABLE)
						vector<T> vec;
						list<T> lst;
					#else
						void* m_data;
						unsigned int m_typeSize; // in bytes
						unsigned int m_sizeOccupied;
						unsigned int m_sizeTotal;
					#endif

					bool usingList;

					VectorIterator<T>* it;

				public:
					Vector():
						#if defined (STL_AVAILABLE)
							vec(), lst(),
						#else
							m_data(NULL),
							m_typeSize(1),
							m_sizeOccupied(0),
							m_sizeTotal(1),
						#endif
						usingList(false), it(NULL)
						{
						#if !defined (STL_AVAILABLE)
							m_typeSize = sizeof(T);
							m_data = malloc(m_typeSize * m_sizeTotal);
						#endif
					}
					void setUsingList(bool b) {
						usingList = b;
					}
					bool isUsingList() {
						return usingList;
					}
					bool contains(T val) {
						#if !defined(STL_AVAILABLE)
							return false;
						#else 
							if (!usingList) {
								typename vector<T>::iterator it = vec.begin();
								while (it != vec.end()) {
									T obj = (*it);
									if (obj == val) {
										return true;
									} 
									++it;
								}
								return false;
							}

							typename list<T>::iterator it = lst.begin();
							while (it != lst.end()) {
								T obj = (*it);
								if (obj == val) {
									return true;
								}
								++it;
							}
							return false; 
						#endif
					}
					void add(T o) {
						#if !defined(STL_AVAILABLE)
							m_sizeOccupied++;
							if (m_sizeOccupied > m_sizeTotal) {
								m_sizeTotal *= 2;
							}

							m_data = realloc(m_data, m_typeSize * (m_sizeTotal));
							memcpy(((char*)m_data) + (m_sizeOccupied*m_typeSize), o, m_typeSize);
							return;
						#else
							if (!usingList) {
								vec.push_back(o);
								return;
							}
							lst.push_back(o);
						#endif
					}
					void add(unsigned int index, T o) {
						if (!isIndexInBounds(index)) {
							add(o);
							return;
						} 

						#if !defined(STL_AVAILABLE)
							ARK2D::getLog()->e("Vector<?>::add(unsigned int, ?) is not implemented without STL.");
						#else 
							if (!usingList) {
								vec.insert(vec.begin() + index, o);
								return;
							}

							typename list<T>::iterator it = lst.begin();
							advance(it, index);
							lst.insert(it, o);
						#endif
					}
					void set(unsigned int i, T item) {
						#if !defined(STL_AVAILABLE)
							return NULL;
						#else

							if (!usingList) {
								vec[i] = item;
							} else {
								typename list<T>::iterator it;
								it = lst.begin();
								advance(it, i);
								it = lst.erase(it);
								lst.insert(it, item);
							}

						#endif
					}
					T first() {
						return get(0);
					}
					T last() {
						return get(size() - 1); 
					}
					T get(unsigned int i) {
						#if !defined(STL_AVAILABLE)
							if (!isIndexInBounds(i)) {
								return NULL;
							}

							return (T) m_data + (i * m_typeSize);
						#else
							if (!usingList) {
								return vec.at(i);
							}
							typename list<T>::iterator it = lst.begin();
							advance(it, i);
							return *it;
						#endif
					}
					T& getr(unsigned int i) {
						#if !defined(STL_AVAILABLE)
							if (!isIndexInBounds(i)) {
								return NULL;
							}

							return (T) m_data + (i * m_typeSize);
						#else
							if (!usingList) {
								return vec.at(i);
							}
							typename list<T>::iterator it = lst.begin();
							advance(it, i);
							return *it;
						#endif
					}
					T pop() {
						#if !defined(STL_AVAILABLE)
							return NULL;
						#else
							if (!usingList) {
								if (vec.size() == 0) {
									#ifdef EXCEPTIONS_AVAILABLE
										throw exception();
									#else
										ErrorDialog::createAndShow("Cannot pop because the Vector is empty.");
									#endif
								}

								int end = vec.size()-1;
								T endObj = vec.at(end);
								vec.pop_back();
								return endObj;
							}

							if (lst.size() == 0) {
								#ifdef EXCEPTIONS_AVAILABLE
									throw exception();
								#else
									ErrorDialog::createAndShow("Cannot pop because the Vector is empty.");
								#endif
							}
							int end = lst.size()-1;
							//T endObj = lst.begin() + end;
							typename list<T>::iterator it = lst.begin();
							advance(it, end);
							T endObj = (*it);

							lst.pop_back();
							return endObj;
						#endif
					}
					T random() {
						#if !defined(STL_AVAILABLE)
							return NULL;
						#else
							if (!usingList) {
								return vec.at( MathUtil::randBetween(0, vec.size()) );
							}
							typename list<T>::iterator it = lst.begin();
							advance(it, MathUtil::randBetween(0, lst.size()));
							return (*it);
						#endif
					}
					void shuffle() {
						//random_shuffle(vec.begin(). vec.end());
						#if !defined(STL_AVAILABLE)
							return NULL; 
						#else
							if (!usingList) {
								std::random_shuffle(vec.begin(), vec.end());
								return;
							}
							std::random_shuffle(lst.begin(), lst.end());
						#endif
					}
					void reverse() {
						//std::reverse(vec.begin(), vec.end());
						#if !defined(STL_AVAILABLE)
							return NULL; 
						#else
							if (!usingList) {
								std::reverse(vec.begin(), vec.end());
								return;
							}
							std::reverse(lst.begin(), lst.end());
						#endif
					}
					void* getData() {
						#if !defined(STL_AVAILABLE)
							return (void*) NULL;
						#else
							if (!usingList) {
								return (void*) vec;
							}
							return (void*) lst;
						#endif
					}
					vector<T>& getDataVec() { return vec; }
					void setData(void* data) {
						#if !defined(STL_AVAILABLE)
						#else
							if (!usingList) {
								vec = data;
								return;
							}
							lst = data;
						#endif
					}
					VectorIterator<T>* iterator() {
						if (it == NULL) {
							it = new VectorIterator<T>(this);
						}
						it->reset();
						return it;
					}

					unsigned int size() {
						#if !defined(STL_AVAILABLE)
							return (unsigned int) m_sizeOccupied;
						#else
							if (!usingList) {
								return vec.size();
							}
							return lst.size();
						#endif
					}
					void remove(unsigned int i) {
						removeByIndex(i);
					}
					void removeByIndex(unsigned int i) {
						#if !defined(STL_AVAILABLE)

						#else
							if (!usingList) {
								vec.erase(vec.begin() + i);
								return;
							}
							typename list<T>::iterator it = lst.begin();
							advance(it, i);
							lst.erase(it);
						#endif
					}
					void remove(T o) {
						removeByValue(o);
					}
					void removeByValue(T o) {
						#if !defined(STL_AVAILABLE)
						#else
							if (!usingList) {
								typename vector<T>::iterator it = vec.begin();
								while (it != vec.end()) {
									T obj = (*it);
									if (obj == o) {
										it = vec.erase(it);
										break;
									} else {
										++it;
									}
								}
								return;
							}

							typename list<T>::iterator it = lst.begin();
							while (it != lst.end()) {
								T obj = (*it);
								if (obj == o) {
									it = lst.erase(it);
									break;
								} else {
									++it;
								}
							}
						#endif
					}
					void removeAll() {
						clear();
					}
					void pruneAll() { // only use with ARKGameObject
						#if !defined(STL_AVAILABLE)
						#else
							if (!usingList) {
								typename vector<T>::iterator it = vec.begin();
								while (it != vec.end()) {
									T obj = (*it);
									if (obj->isPendingRemoval()) {
										obj->onPrune();
										it = vec.erase(it);
										//break;
									} else {
										++it;
									}
								}
								return;
							}
							typename list<T>::iterator it = lst.begin();
							while (it != lst.end()) {
								T obj = (*it);
								if (obj->isPendingRemoval()) {
									obj->onPrune();
									it = lst.erase(it);
									//break;
								} else {
									++it;
								}
							}
						#endif
					}

					void clear() {
						#if !defined(STL_AVAILABLE)

						#else
							if (!usingList) {
								vec.clear();
								return;
							}
							lst.clear();
						#endif
					}

					/**
					 * only use with ARKGameObject
					 * careful not to prune in any update functions!!
					 */
					void updateAll(ARK::Core::GameContainer* container, GameTimer* timer) {
						#if !defined(STL_AVAILABLE)

						#else
							if (!usingList) {
								for(unsigned int i = 0; i < vec.size(); i++) {
									T obj = vec.at(i);
									//if (obj == NULL) { continue; }
									obj->update(container, timer);
								}
								return;
							}

							typename list<T>::iterator it = lst.begin();
							while (it != lst.end()) {
								T obj = (*it);
								//if (obj == NULL) { continue; }
								obj->update(container, timer);
								++it;
							} 
						#endif
					}
					void updateSelection(void* selectionFunction) {
						#if !defined(STL_AVAILABLE)
							
						#else
							GameContainer* container = ARK2D::getContainer();
							GameTimer* timer = container->getTimer();
							bool (*pt)(T) = (bool(*)(T)) selectionFunction;

							if (!usingList) {
								for(unsigned int i = 0; i < vec.size(); i++) {
									T obj = vec.at(i);
									if (pt(obj)) {
										obj->update(container, timer);
									}
								}
								return;
							}
							typename list<T>::iterator it = lst.begin();
							while (it != lst.end()) {
								if (pt(*it)) {
									T obj = (*it);
									obj->update(container, timer);
								}
								++it;
							}
						#endif
					}

					/**
					 * only use with ARKGameObject
					 * careful not to prune in any update functions!!
					 */
					void renderAll(ARK::Core::GameContainer* container, Renderer* g) {
						#if !defined(STL_AVAILABLE)

						#else
							if (!usingList) {
								for(unsigned int i = 0; i < vec.size(); i++) {
									T obj = vec.at(i);
									//if (obj == NULL) { continue; }
									obj->render(container, g);
								}
								return;
							}
							typename list<T>::iterator it = lst.begin();
							while (it != lst.end()) {
								T obj = (*it);
								//if (obj == NULL) { continue; }
								obj->render(container, g);
								++it;
							}
						#endif
					}
					// function definition example
					// bool hasNoWave(LMTEnemy* enemy) { 
					// 		return (enemy->getWave() == NULL); 
					// }
					void renderSelection(void* selectionFunction) {
						#if !defined(STL_AVAILABLE)

						#else
							GameContainer* container = ARK2D::getContainer();
							Renderer* g = ARK2D::getRenderer();
							bool (*pt)(T) = (bool(*)(T)) selectionFunction;

							if (!usingList) {
								for(unsigned int i = 0; i < vec.size(); i++) {
									T obj = vec.at(i);
									if (pt(obj)) {
										obj->render(container, g);
									}
								}
								return;
							}
							typename list<T>::iterator it = lst.begin();
							while (it != lst.end()) {
								if (pt(*it)) {
									T obj = (*it);
									obj->render(container, g);
								}
								++it;
							}
						#endif
					}
					string toString() {
						string s = "[";
						#if !defined(STL_AVAILABLE)

						#else
							if (!usingList) {
								for(unsigned int i = 0; i < vec.size(); i++) {
									T obj = vec.at(i);
									s += obj->toString();
									if (i != vec.size() - 1) {
										s += ",";
									}
								}
							} else {
								/*for(unsigned int i = 0; i < lst.size(); i++) {
									typename list<T>::iterator it = lst.begin();
									advance(it, i);
									T obj = *it;
									if (obj == NULL) { continue; }
									s += obj->toString();
									if (i != lst.size() - 1) {
										s += ",";
									}
								}*/
								unsigned int ii = 0;
								typename list<T>::iterator it = lst.begin();
								while (it != lst.end()) {
									T obj = (*it);
									s += obj->toString();
									ARK2D::getLog()->i(s); 
									if (ii != size() - 1) {
										s += ",";
									}
									ii++;
									++it;
								}
							}
						#endif
						s += "]";
						return s;
					}
					~Vector() {
						
						/*if (!usingList) {
							for(unsigned int i = 0; i < vec.size(); i++) {
								
								//if (is_pointer<T>(vec.at(i))) {
								//	delete vec.at(i);	
								//}

								if (is_pointer<T>(vec.at(i))) {
									typedef typename remove_pointer<T>::type type;
									delete ((type)vec).at(i)
								}

							}
							vec.clear();
						} else {
							unsigned int ii = 0;
							typename list<T>::iterator it = lst.begin();
							while (it != lst.end()) {
								T obj = (*it);

								if (is_pointer_type<T>::value) {
									delete obj;
								} else {
									// do what?
								}

								ii++;
								++it;
							}
							lst.clear();
						}*/					
							
					}
				private:
					bool isIndexInBounds(unsigned int i) {
						return (i >= 0 && i < size());
					}

			};

			template <class T>
			class VectorIterator {
				private:
					Vector<T>* parent;
					unsigned int index;
					#if defined(STL_AVAILABLE)
						typename list<T>::iterator iterator; // Used only for lists.
					#endif

				public:
					VectorIterator(Vector<T>* p):
						parent(p), index(0)
						#if defined(STL_AVAILABLE)
							, iterator(NULL)
						#endif
					{

					}
					/*T current() {
						if (parent->isUsingList()) {
							--iterator;
							T obj = *iterator;
							++iterator;
							return obj;
						}
						--index;
						T obj = parent->get(index);
						++index;
					}*/
					T next() {
						#if !defined(STL_AVAILABLE)

						#else
							if (parent->isUsingList()) {
								if (hasNext()) {
									T obj = (*iterator);
									++iterator;
									return obj;
								}
								ErrorDialog::createAndShow("Out of items in Iterator (lst) - returning NULL.");
								return NULL;
							}

							if (hasNext()) {
								T obj = parent->get(index);
								++index;
								return obj;
							}
							ErrorDialog::createAndShow("Out of items in Iterator (vec) - returning NULL.");
							return NULL;
						#endif
					}
					Vector<T>* getVector() {
						return parent;
					}
					bool hasNext() {
						#if !defined(STL_AVAILABLE)

						#else
							signed int top = parent->size();
							if (top == 0) { return false; }

							if (parent->isUsingList()) {
								if (iterator != parent->lst.end()) {
									return true;
								}
								return false;
							}

							signed int thisIndex = index;
							if (thisIndex >= top) {
								return false;
							}
							return true;
						#endif
					}
					/**
					 * -1 because .next() forwards to the next element which is not normally the one we are trying to remove.
					 */
					void remove() {
						#if !defined(STL_AVAILABLE)

						#else
							if (parent->isUsingList()) {
								iterator--;
								iterator = parent->lst.erase(iterator);
								return;
							}

							parent->remove(index-1);
						#endif
					}
					void reset() {
						#if !defined(STL_AVAILABLE)

						#else
							if (parent->isUsingList()) {
								iterator = parent->lst.begin();
								return;
							}

							index = 0;
						#endif
					}
					~VectorIterator() {

					}
			};
		}
	}
}

#endif /* Vector_H_ */
