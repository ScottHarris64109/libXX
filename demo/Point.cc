/*#############################################################################
 * Point.cc -- A geometric point in an integer plane.
 * Copyright (C) 2026 by Scott Harris.  All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *#############################################################################
 */
#include <iostream>
#include <stdlib.h>
#include <errno.h>

#include "Point.hh"

//== Constructors =============================================================

/**
 *  Destroy and deallocate a Point object.
 */

Point::~Point( ) {
}

/**
 *  Create a Point at (0,0).
 *<p>
 * <code>Point point;</code>
 */

Point::Point( ) {
   this->coord[0] = 0;
   this->coord[1] = 0;
}

/**
 *  Create a Point at a given location.
 *<p>
 * <code>Point point(1,2);</code>
 */

Point::Point( int x, int y ) {
   this->coord[0] = x;
   this->coord[1] = y;
}

/**
 *  Create a Point at a given location.
 *<p>
 * <code>Point point({1,2});</code>
 */

Point::Point( const int *that ) {
   this->coord[0] = that[0];
   this->coord[1] = that[1];
}

//== Accessors ================================================================

/**
 *  Set the X coordinate.
 */

void Point::setX( int x ) {
   this->coord[0] = x;
}

/**
 *  Set the Y coordinate.
 */

void Point::setY( int y ) {
   this->coord[1] = y;
}

/**
 *  Set both coordinates from an array.
 *<p>
 * <code>point = {3,4};</code>
 */

Point Point::operator=( const int *that ) {
   this->coord[0] = that[0];
   this->coord[1] = that[1];
   return *this;
}

/**
 *  Move using an array.
 *<p>
 * <code>point.move( (int[]){3,4} );</code>
 */

Point Point::move( const int *that ) {
   this->coord[0] += that[0];
   this->coord[1] += that[1];
   return *this;
}

/**
 * Move.
 */
Point Point::move( int right, int down ) {
   this->coord[0] += right;
   this->coord[1] += down;
   return *this;
}

//== Stream I/O ===============================================================

/*
 *  Read a Point object from an istream.
 *
 * @param in the istream to read from.
 * @param object the Point object to read.
 * @return the istream.
 * @throws Exception if problem.

std::istream &operator>>( std::istream& in, Point &object ) {
   int xx;

   in >> xx;
   object.setX( xx );

   return in;
}
 */

/**
 *  Write a Point object to an ostream as "(x,y)".
 *
 * @param out the ostream to write to.
 * @param object the Point object to write.
 * @return the ostream.
 * @throws Exception if problem.
 */

std::ostream &operator<<( std::ostream& out, const Point &object ) {

   out << "(" << object.x() << "," << object.y() << ")";

   return out;
}
