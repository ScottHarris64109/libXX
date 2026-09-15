/*#############################################################################
 * Line.cc -- Line for libXX demo.
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
#include "Line.hh"

//== Constructors =============================================================

/**
 *  Destroy and deallocate a Line object.
 */

Line::~Line( ) {
}

/**
 *  Create a Line with default values.
 */

Line::Line( ) {
   this->point_[0] = {0,0};
   this->point_[1] = {0,0};
}

/**
 *  Create a Line with known endpoints.
 */

Line::Line( Point &from, Point &to ) {
   point_[0] = from; 
   point_[1] = to;
}

//== Accessors ================================================================

/**
 *  Replace both endpoints.
 */

Line Line::operator=( const Point *p ) {
   point_[0] = p[0]; 
   point_[1] = p[1];
   return *this;
}

/**
 *  Replace one endpoint.
 */

Line Line::setPoint( int which, const Point &p ) {
   if ((which < 0) || (which >1))
   {
      throw std::range_error( "Point number must be 0 or 1." );
   }
   point_[which] = p;
   return *this;
}

//== Stream I/O ===============================================================

/**
 *  Read a Line object from an istream.
 *
 * @param in the istream to read from.
 * @param object the Line object to read.
 * @return the istream.
 * @throws Exception if problem.

std::istream &operator>>( std::istream& in, Line &object ) {
   int xx;

   in >> xx;
   object.setX( xx );

   return in;
}
 */

/**
 *  Write a Line object to an ostream as "((1,1),(2,3))".
 *
 * @param out the ostream to write to.
 * @param object the Line object to write.
 * @return the ostream.
 * @throws Exception if problem.
 */

std::ostream &operator<<( std::ostream& out, const Line &object ) {

   out << "(("  << object.point(0).x()
       << ","   << object.point(0).y()
       << "),(" << object.point(1).x()
       << ","   << object.point(1).y() << "))";

   return out;
}


