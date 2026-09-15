// Line.hh -- Line for libXX demo.
// Copyright (C) 2026 by Scott Harris.  
/**
 * @brief Line is a straight line.
 *
 */

#ifndef LINE_HH_
#define LINE_HH_

#include <iostream>
#include "Point.hh"

class Line {

private:
   Point point_[2];

protected:

public:
   //== Constructors ===========================================================

   virtual ~Line();

   Line();
   Line( Point&, Point& );

   //== Comparisons ============================================================

   /// Equal-to comparison.
   inline bool operator==( const Line &that ) const { 
      return 
         ((this->point(0) == that.point(0) && this->point(1) == that.point(1))
               ||
          (this->point(0) == that.point(1) && this->point(1) == that.point(0)))
      ? true: false; 
   };

   //== Accessors ==============================================================

   inline const Point &point( int which ) const { 
      return this->point_[ which ]; 
   };

   Line operator=( const Point * );

   Line setPoint( int which, const Point & );
};

// Stream input operator.
// std::istream &operator>>( std::istream& in, Line &object );

// Stream output operator.
std::ostream &operator<<( std::ostream& out, const Line &object );

#endif /* LINE_HH_ */
