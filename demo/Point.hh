// Point.hh -- Geometric point.
// Copyright (C) 2026 by Scott Harris.  All rights reserved.
/**
 * @brief Point is a geometric point in a 2D integer space.
 *
 */

#ifndef POINT_HH_
#define POINT_HH_

#include <iostream>

class Point {

private:
   int coord[2];

protected:

public:
   //== Constructors ===========================================================

   virtual ~Point();

   Point();
   Point( int, int );
   Point( const int* );

   //== Comparisons ============================================================

   /// Equal-to comparison.
   inline bool operator==( const Point &that ) const { 
      return (this->x() == that.x() && this->y() == that.y()) ? true: false; 
   };

   //== Accessors ==============================================================

   inline int x( void ) const { return coord[0]; };
   inline int y( void ) const { return coord[1]; };

   void setX( int x );
   void setY( int y );
   Point operator=( const int *that );

   Point move( const int *that );
   Point move( int right, int down );

};

// Stream input operator.
// std::istream &operator>>( std::istream& in, Point &object );

// Stream output operator.
std::ostream &operator<<( std::ostream& out, const Point &object );

#endif /* POINT_HH_ */
