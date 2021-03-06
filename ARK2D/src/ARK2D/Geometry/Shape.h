/*
 * Shape.h
 *
 *  Created on: Jan 22, 2012
 *      Author: ashleygwinnell
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include "../ARK2D.h"
#include "../Core/GameContainer.h"
#include "Vector2.h"
#include <limits>
namespace ARK {
	namespace Geometry {

		template <class T>
		class Polygon;

		template <class T>
		class Rectangle;

		template <class T>
		class Circle;

		template <class T>
		class Line;

		/*!
		 * \brief Abstract class for all Geometry.
		 *
		 * @todo Some collision-pairs are missing.
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		template <class T>
		class Shape {
			public:
				Shape() {

				}

				virtual T getMinX() = 0;
				virtual T getMaxX() = 0;
				virtual T getCenterX() = 0;

				virtual T getMinY() = 0;
				virtual T getMaxY() = 0;
				virtual T getCenterY() = 0;

				virtual T getWidth() = 0;
				virtual T getHeight() = 0;

				virtual void setLocation(T x, T y) = 0;
				virtual void setLocationByCenter(T x, T y) = 0;

				virtual bool contains(T x, T y) = 0;
				//virtual bool contains(Vector2<T>* vec) { return contains(vec->getX(), vec->getY()); }
				virtual bool collides(Shape<T>* s) = 0;
				virtual void resolve(Shape<T>* s) = 0;

				virtual void render() = 0;

				virtual bool isOffscreen() {
					GameContainer* container = ARK2D::getContainer();
					if (	(signed int)(getMaxY()) <= 0
						|| 	(signed int)(getMaxX()) <= 0
						|| 	(signed int)(getMinX()) >= (signed int) container->getWidth()
						|| 	(signed int)(getMinY()) >= (signed int) container->getHeight()) {
						return true;
					}
					return false;
				}
				virtual bool isPartlyOffscreen() {
					GameContainer* container = ARK2D::getContainer();
					if (	(signed int)(getMinX()) <= 0
						|| 	(signed int)(getMinY()) <= 0
						|| 	(signed int)(getMaxX()) >= (signed int) container->getWidth()
						|| 	(signed int)(getMaxY()) >= (signed int) container->getHeight()) {
						return true;
					}
					return false;
				}

				Rectangle<T>* asRectangle() {
					return dynamic_cast<Rectangle<T>* >(this);
				}
				Circle<T>* asCircle() {
					return dynamic_cast<Circle<T>* >(this);
				}
				Polygon<T>* asPolygon() {
					return dynamic_cast<Polygon<T>* >(this);
				}

				virtual ~Shape() {

				}

				/**
				 * Static collision strategies.
				 */
				static bool collision_circleCircle(T x1, T y1, T r1, T x2, T y2, T r2) {
					float distance = (float) MathUtil::distance(x1, y1, x2, y2);
					if (distance <= r1 + r2) {
						return true;
					}
					return false;
				}
				static bool collision_rectangleRectangle(T x1, T y1, T w1, T h1, T x2, T y2, T w2, T h2) {
					if ((x1 + w1) >= x2 && x1 <= (x2 + w2) && (y1 + h1) >= y2 && y1 <= (y2 + h2))  {
						return true;
					}
					return false;
				}
				static Vector2<T> collision_rectangleRectangleVec(T x1, T y1, T w1, T h1, T x2, T y2, T w2, T h2) {
					Vector2<T> adjustmentVector(0, 0);

					if ((	x1 + w1) >= x2
							&& x1 <= (x2 + w2)
							&& (y1 + h1) >= y2
							&& y1 <= (y2 + h2)
					)  {

						// Axis stores the value of X or Y.  X = 0, Y = 1.
						// Side stores the value of left (-1) or right (+1).
						int axis = 0;
						int side = 0;

						 // This stores the absolute minimum distance we'll need to separate our colliding object.
						float minimumTranslationDistance = 0.0f;

						 // This is re-used to calculate the difference in distance between sides.
						float difference = 0.0f;

						// left
						difference = (x1 + w1) - x2; // left.getMaxX() - right.getMinX();
						if (difference < 0.0f) {
							return adjustmentVector;
						} else {
							minimumTranslationDistance = difference;
							axis = 0;
							side = -1;
						}

						//right
						difference = (x2 + w2) - x1; //right.getMaxX() - left.getMinX();
						if (difference < 0.0f) {
							return adjustmentVector;
						}
						if (difference < minimumTranslationDistance) {
							minimumTranslationDistance = difference;
							axis = 0;
							side = 1;
						}

						// bottom
						difference = (y1 + h1) - y2;// left.getMaxY() - right.getMinY();
						if (difference < 0.0f) {
							return adjustmentVector;
						}
						if (difference < minimumTranslationDistance) {
							minimumTranslationDistance = difference;
							axis = 1;
							side = -1;
						}

						// top
						difference = (y2 + h2) - y1;// right.getMaxY() - left.getMinY();
						if (difference < 0.0f) {
							return adjustmentVector;
						}
						if (difference < minimumTranslationDistance) {
							minimumTranslationDistance = difference;
							axis = 1;
							side = 1;
						}

						// collided with
						if (axis == 1) { // Y Axis
							adjustmentVector.setY(side * minimumTranslationDistance);
						} else { // X Axis
							adjustmentVector.setX(side * minimumTranslationDistance);
						}
					}
					return adjustmentVector;
				}
				static bool collision_rectangleLine(T x1, T y1, T w1, T h1, T x2, T y2, T x3, T y3) {
					if (collision_rectangleRectangle(x1,y1,w1,h1,x2,y2,0,0) || collision_rectangleRectangle(x1,y1,w1,h1,x3,y3,0,0)) {
						return true;
					}
					T lines[16] = {
						x1, 	y1,		x1+w1, 	y1,
						x1+w1, 	y1, 	x1+w1, 	y1+h1,
						x1+w1, 	y1+h1, 	x1, 	y1+h1,
						x1, 	y1+h1, 	x1, 	y1
					};
					for(unsigned int i = 0; i < 16; i += 4) {
						bool b = collision_lineLine(lines[i], lines[i+1], lines[i+2], lines[i+3], x2, y2, x3, y3);
						if (b) { return true; }
					}
					return false;
				}
				static bool collision_circleLine(T x1, T y1, T r1, T x2, T y2, T x3, T y3) { // x1, y1 center of circle.
					Vector2<T> pos(x1, y1);
					Line<T> line(x2,y2,x3,y3);
					T r2 = (r1 * r1);
					float dis = line.distanceSquared(&pos);
					if (dis < r2) {
						return true;
					}
					return false;
				}
				static bool collision_circleRectangle(T x1, T y1, T r1, T x2, T y2, T w2, T h2) {

					if (collision_rectangleRectangle(x1, y1, 0, 0, x2, y2, w2, h2)) {
						return true; // center of circle checked first.
					}

					// clockwise rect.
					Line<T> topside(	x2, 	y2,		x2+w2, 	y2);
					Line<T> rightside(	x2+w2, 	y2, 	x2+w2, 	y2+h2);
					Line<T> bottomside(	x2+w2, 	y2+h2, 	x2, 	y2+h2);
					Line<T> leftside(	x2, 	y2+h2, 	x2, 	y2);
					Line<T>* lines[4] = {&topside, &rightside, &bottomside, &leftside};

					T r2 = (r1 * r1);

					Vector2<T> pos(x1, y1);

					for (unsigned int i = 0; i < 4; i++) {
						float dis = lines[i]->distanceSquared(&pos);
						if (dis < r2) {
							return true;
						}
					}
					return false;

				}
				static bool collision_lineLine(T x1, T y1, T x2, T y2, T x3, T y3, T x4, T y4) {
					x1 = int(x1); x2 = int(x2); x3 = int(x3); x4 = int(x4);
					y1 = int(y1); y2 = int(y2); y3 = int(y3); y4 = int(y4);

					T d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
					if (d == 0) {
						return false;
					}
					T pre = (x1*y2 - y1*x2);
					T post = (x3*y4 - y3*x4);
					T x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
					T y = (pre * (y3 - y4) - (y1 - y2) * post) / d;

					if (x < min<T>(x1, x2) || x > max<T>(x1, x2) || x < min<T>(x3, x4) || x > max<T>(x3, x4)) { return false; }
					if (y < min<T>(y1, y2) || y > max<T>(y1, y2) || y < min<T>(y3, y4) || y > max<T>(y3, y4)) { return false; }
					return true;
				}
				/*static bool collision_lineLine(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
					//x1 = int(x1); x2 = int(x2); x3 = int(x3); x4 = int(x4);
					//y1 = int(y1); y2 = int(y2); y3 = int(y3); y4 = int(y4);

					int d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
					if (d == 0) {
						return false;
					}
					int pre = (x1*y2 - y1*x2);
					int post = (x3*y4 - y3*x4);
					int x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
					int y = (pre * (y3 - y4) - (y1 - y2) * post) / d;

					if (x < min<int>(x1, x2) || x > max<int>(x1, x2) || x < min<int>(x3, x4) || x > max<int>(x3, x4)) { return false; }
					if (y < min<int>(y1, y2) || y > max<int>(y1, y2) || y < min<int>(y3, y4) || y > max<int>(y3, y4)) { return false; }
					return true;
				}*/
				static bool collision_polygonPolygon(Polygon<T>* one, Polygon<T>* two) { // convex polys only.

					// ----------
					// just check lines for now. SAT can come later 'cause i said so! >:O
					// ----------
					
					vector<Vector2<T>* >* onePoints = one->getPoints();
					vector<Vector2<T>* >* twoPoints = two->getPoints();

					for(unsigned int i = 0; i < onePoints->size() - 1; i++) 
					{
						Line<T> oneLine(
							onePoints->at(i)->getX(), onePoints->at(i)->getY(),
							onePoints->at(i+1)->getX(), onePoints->at(i+1)->getY()
						);

						for(unsigned int j = 0; j < twoPoints->size() - 1; i++) 
						{
							Line<T> twoLine(
								twoPoints->at(j)->getX(), twoPoints->at(j)->getY(),
								twoPoints->at(j+1)->getX(), twoPoints->at(j+1)->getY()
							);

							//if (oneLine.collides(&twoLine)) { 
							//	return true;
							//}
							if (
								collision_lineLine
								(
									onePoints->at(i)->getX(), onePoints->at(i)->getY(), onePoints->at(i+1)->getX(), onePoints->at(i+1)->getY(),
									twoPoints->at(j)->getX(), twoPoints->at(j)->getY(),	twoPoints->at(j+1)->getX(), twoPoints->at(j+1)->getY()
								)
							) 
							{
								return true;
							}
						}
					}			

					return false;
				}
				static bool collision_polygonRectangle(Polygon<T>* one, Rectangle<T>* rect) { // convex polys only.

					// ----------
					// just check lines for now. SAT can come later 'cause i said so! >:O
					// ----------

					// check the rect against polygon lines :|
					vector<Vector2<T>* >* onePoints = one->getPoints();
					for(unsigned int i = 0; i < onePoints->size(); i++) 
					{
						int thisIndex = i;
						int nextIndex = ((i+1) == onePoints->size()) ? 0 : (i+1);

						bool collision = collision_rectangleLine(
							(int) rect->getMinX(), (int)rect->getMinY(), (int)rect->getWidth(), (int)rect->getHeight(), 
							(int)onePoints->at(thisIndex)->getX(), (int)onePoints->at(thisIndex)->getY(), 
							(int)onePoints->at(nextIndex)->getX(), (int)onePoints->at(nextIndex)->getY()
						);
						if (collision) { return true; }
					}

					
					
					/*vector<Vector2<T>* >* onePoints = one->getPoints();
					
					// TODO auto_ptr;
					Polygon<T>* two = new Polygon<T>(); 
					two->addPoint(0,0); 
					two->addPoint(0,10); 
					two->addPoint(10,10); 
					two->addPoint(10,0);
					rect->asPolygon(two, 0);
					vector<Vector2<T>* >* twoPoints = two->getPoints();

					// TODO polygon destruction?

					for(unsigned int i = 0; i < onePoints->size() - 1; i++) 
					{
						Line<T> oneLine(
							onePoints->at(i)->getX(), onePoints->at(i)->getY(),
							onePoints->at(i+1)->getX(), onePoints->at(i+1)->getY()
						);

						for(unsigned int j = 0; j < twoPoints->size() - 1; i++) 
						{
							Line<T> twoLine(
								twoPoints->at(j)->getX(), twoPoints->at(j)->getY(),
								twoPoints->at(j+1)->getX(), twoPoints->at(j+1)->getY()
							);

							if (oneLine.collides(&twoLine)) { 
								return true;
							}
						}
					}	*/


					return false;
				}
				static bool collision_polygonCircle(Polygon<T>* one, T x1, T y1, T r1) { // any poly even with holes in - not very efficient.
					vector<Vector2<T>* >* points = one->getPoints();

					//! @todo: Contains check here.
                    vector<Line<T> > lines;
					//Line<T> lines[points->size()];
					for(unsigned int i = 0; i < points->size(); i++) {
                        unsigned int nextIndex = i+1;
						if (nextIndex == points->size()) { nextIndex = 0; }
                        Line<T> thisLine;
						/*lines[i]*/ thisLine.setStart(points->at(i)->getX(), points->at(i)->getY());
						/*lines[i]*/ thisLine.setEnd(points->at(nextIndex)->getX(), points->at(nextIndex)->getY());
                        lines.push_back(thisLine);
					}

					T r2 = (r1 * r1);
					Vector2<T> pos(x1, y1);
					for (unsigned int i = 0; i < points->size(); i++) {
						float dis = lines[i].distanceSquared(&pos);
						if (dis < r2) {
							return true;
						}
					}
					return false;
				}
		};

		
	}
}
#endif /* SHAPE_H_ */
