
#ifndef BOARD_HPP
#define BOARD_HPP

#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Location.hpp"

class Board
{
public:
   Board() = default;
   Board(const Board& aRhs) = default;
   Board& operator=(const Board& aRhs) = default;

   bool IsLocationCrate(const Location& aLoc) const;
   bool IsLocationTarget(const Location& aLoc) const;
   bool IsLocationWall(const Location& aLoc) const;
   bool IsLocationInside(const Location& aLoc) const;
   bool IsLocationEmpty(const Location& aLoc) const;

   std::vector<Direction> LegalMoves() const;
   void MakeMove(const Direction aDirection);

   bool IsSolved() const;

   size_t Hash() const;
   bool operator==(const Board& aRhs) const;

   void PrintToStream(std::ostream& aOut) const;

private:

   Location mPlayer;
   Location mSize;
   std::unordered_set<Location> mWalls;
   std::unordered_set<Location> mCrates;
   std::unordered_map<Location, bool> mTargets;

public:
   class Builder
   {
   public:
      Builder() = default;

      Board Build();

      void FromStream(std::istream& aIn);

   private:
      std::unique_ptr<Board> mBoardPtr{ std::make_unique<Board>() };
   };
};

namespace std
{
template<>
struct hash<Board>
{
   size_t operator()(Board const& aRhs) const
   {
      return aRhs.Hash();
   }
};
}

#endif