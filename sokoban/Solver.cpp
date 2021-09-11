
#include "Solver.hpp"

#include <vector>

void Solver::Exec()
{
   using std::chrono::system_clock;
   system_clock::time_point start = system_clock::now();
   this->Solve();
   system_clock::time_point end = system_clock::now();
   mWallTime = std::chrono::duration_cast<wall_time>(end - start);
}

void Solver::PrintToStream(std::ostream& aOut) const
{
   // print microseconds to solve
   aOut << mWallTime.count() << std::endl;

   // print moves to solve
   aOut << mMovesToSolve.size() << std::endl;
   for (const Direction move : mMovesToSolve)
   {
      switch (move)
      {
      case Direction::Up:
         aOut << 'U';
         break;

      case Direction::Right:
         aOut << 'R';
         break;

      case Direction::Down:
         aOut << 'D';
         break;

      case Direction::Left:
         aOut << 'L';
         break;

      default:
         break;
      }
   }
   aOut << std::endl;

   // print solved board
   mSolved.PrintToStream(aOut);
}

void BreadthFirstTreeSearchSolver::Solve()
{
   mInitialNodePtr = std::make_unique<SearchNode>(nullptr, Direction::Up, mInitial);
   mFrontier.push(mInitialNodePtr.get());
   SearchNode* currentPtr = nullptr;

   while (!mFrontier.empty())
   {
      currentPtr = mFrontier.front();
      mFrontier.pop();

      if (currentPtr->mBoard.IsSolved())
      {
         mIsSolved = true;
         break;
      }
      else if (mExplored.find(currentPtr->mBoard) != mExplored.end())
      {
         // already explored
         continue;
      }
      mExplored.insert(currentPtr->mBoard);

      std::vector<Direction> legalMoves = currentPtr->mBoard.LegalMoves();
      for (const Direction move : legalMoves)
      {
         Board childBoard = currentPtr->mBoard;
         childBoard.MakeMove(move);
         auto childPtr = std::make_unique<SearchNode>(currentPtr, move, childBoard);
         mFrontier.push(childPtr.get());
         currentPtr->mChildren[static_cast<size_t>(move)] = std::move(childPtr);
      }
   }

   if (mIsSolved && currentPtr)
   {
      mSolved = currentPtr->mBoard;
      while (currentPtr->mParentPtr != nullptr)
      {
         mMovesToSolve.push_front(currentPtr->mParentMove);
         currentPtr = currentPtr->mParentPtr;
      }
   }

}
