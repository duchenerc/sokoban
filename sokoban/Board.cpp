
#include "Board.hpp"

#include <array>

const std::array<Direction, 4> DIRECTIONS = { Direction::Up, Direction::Right, Direction::Down, Direction::Left };

enum class BoardInput
{
   Empty = '.',
   Wall = 'w',
   Crate = 'c',
   Target = 't'
};

bool Board::IsLocationCrate(const Location& aLoc) const
{
   return mCrates.find(aLoc) != mCrates.end();
}

bool Board::IsLocationTarget(const Location& aLoc) const
{
   return mTargets.find(aLoc) != mTargets.end();
}

bool Board::IsLocationWall(const Location& aLoc) const
{
   return mWalls.find(aLoc) != mWalls.end();
}

bool Board::IsLocationInside(const Location& aLoc) const
{
   int x = aLoc.X();
   int y = aLoc.Y();
   return x >= 0 && y >= 0 && x < mSize.X() && y < mSize.Y();
}

bool Board::IsLocationEmpty(const Location& aLoc) const
{
   return !IsLocationCrate(aLoc) && !IsLocationWall(aLoc);
}

std::vector<Direction> Board::LegalMoves() const
{
   std::vector<Direction> legalMoves;
   Location moveLoc, crateLoc;

   for (const Direction direction : DIRECTIONS)
   {
      moveLoc = mPlayer.Nudge(direction);
      if (!IsLocationInside(moveLoc) || IsLocationWall(moveLoc))
      {
         // this space is either outside or wall -- illegal move
         continue;
      }
      else if (IsLocationCrate(moveLoc))
      {
         crateLoc = moveLoc.Nudge(direction);
         if (!IsLocationInside(crateLoc) || !IsLocationEmpty(crateLoc))
         {
            // this crate is blocked by a wall, another crate, or the border -- illegal move
            continue;
         }
      }
      legalMoves.push_back(direction);
   }

   return legalMoves;
}

void Board::MakeMove(const Direction aDirection)
{
   Location cratePrev, crateNow;
   mPlayer = mPlayer.Nudge(aDirection);

   auto it = mCrates.find(mPlayer);
   if (it != mCrates.end())
   {
      // crate pushed, update crate entries
      cratePrev = *it;
      crateNow = cratePrev.Nudge(aDirection);
      mCrates.erase(it);
      mCrates.insert(crateNow);

      // update targets
      auto jt = mTargets.find(cratePrev);
      if (jt != mTargets.end())
      {
         jt->second = false;
      }
      jt = mTargets.find(crateNow);
      if (jt != mTargets.end())
      {
         jt->second = true;
      }
   }
}

size_t Board::Hash() const
{
   size_t hash = 0;
   for (const auto& crate : mCrates)
   {
      hash = crate.Hash() ^ (hash << 6);
   }
   return hash;
}

bool Board::operator==(const Board& aRhs) const
{
   return mPlayer == aRhs.mPlayer && mCrates == aRhs.mCrates;
}

Board Board::Builder::Build()
{
   return *mBoardPtr;
}

void Board::Builder::FromStream(std::istream& aIn)
{
   int width, height;
   int playerX, playerY;
   char space;
   
   aIn >> width;
   aIn >> height;
   mBoardPtr->mSize = Location{ width, height };

   aIn >> playerX;
   aIn >> playerY;
   mBoardPtr->mPlayer = Location{ playerX, playerY };

   for (int j = 0; j < height; j++)
   {
      for (int i = 0; i < width; i++)
      {
         aIn >> space;
         switch (static_cast<BoardInput>(space))
         {
         case BoardInput::Crate:
            mBoardPtr->mCrates.insert({ i, j });
            break;

         case BoardInput::Target:
            mBoardPtr->mTargets[{ i, j }] = false;
            break;

         case BoardInput::Wall:
            mBoardPtr->mWalls.insert({ i, j });
            break;

         default:
            break;
         }
      }
   }
}

void Board::PrintToStream(std::ostream& aOut) const
{
   int width = mSize.X();
   int height = mSize.Y();
   aOut << width << " " << height << std::endl;
   aOut << mPlayer.X() << " " << mPlayer.Y() << std::endl;

   Location loc;
   for (int j = 0; j < height; j++)
   {
      for (int i = 0; i < width; i++)
      {
         loc = { i, j };
         if (mCrates.find(loc) != mCrates.end())
         {
            aOut << static_cast<char>(BoardInput::Crate) << " ";
            continue;
         }
         else if (mWalls.find(loc) != mWalls.end())
         {
            aOut << static_cast<char>(BoardInput::Wall) << " ";
            continue;
         }
         else if (mTargets.find(loc) != mTargets.end())
         {
            aOut << static_cast<char>(BoardInput::Target) << " ";
            continue;
         }
         else
         {
            aOut << static_cast<char>(BoardInput::Empty) << " ";
         }
      }
      aOut << std::endl;
   }
}
