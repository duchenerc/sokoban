
#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <functional>

enum class Direction
{
   Up =    'U',
   Right = 'R',
   Down  = 'D',
   Left  = 'L'
};

class Location
{
public:
   Location()
      : mX{ 0 }
      , mY{ 0 }
   {}

   Location(const int aX, const int aY)
      : mX{ aX }
      , mY{ aY }
   {}

   static const Location Up;
   static const Location Right;
   static const Location Down;
   static const Location Left;

   int X() const { return mX; }
   int Y() const { return mY; }

   Location operator+(const Location& aRhs) const;
   Location operator-(const Location& aRhs) const;

   Location& operator+=(const Location& aRhs);
   Location& operator-=(const Location& aRhs);

   bool operator==(const Location& aRhs) const;
   bool operator!=(const Location& aRhs) const;

   Location Nudge(const Direction aDirection) const;

   size_t Hash() const;

private:
   int mX;
   int mY;
};

namespace std
{
template<>
struct hash<Location>
{
   size_t operator()(Location const& aLoc) const
   {
      return aLoc.Hash();
   }
};
}

#endif
