
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
   mSolvedBoardPtr->PrintToStream(aOut);
}

void BreadthFirstTreeSearchSolver::Solve()
{
   mInitialNodePtr = std::make_unique<SearchNode>(nullptr, Direction::Up, *mInitialBoardPtr);
   mFrontier.push(mInitialNodePtr.get());
   SearchNode* currentPtr = nullptr;

   while (!mFrontier.empty())
   {
      currentPtr = mFrontier.front();
      mFrontier.pop();

      if (currentPtr->mBoardPtr->IsSolved())
      {
         mIsSolved = true;
         break;
      }
      else if (mExplored.find(*currentPtr->mBoardPtr) != mExplored.end())
      {
         // already explored
         continue;
      }
      mExplored.insert(*currentPtr->mBoardPtr);

      std::vector<Direction> legalMoves = currentPtr->mBoardPtr->LegalMoves();
      for (const Direction move : legalMoves)
      {
         auto childPtr = std::make_unique<SearchNode>(currentPtr, move, *currentPtr->mBoardPtr);
         childPtr->mBoardPtr->MakeMove(move);
         mFrontier.push(childPtr.get());
         currentPtr->mChildren[static_cast<size_t>(move)] = std::move(childPtr);
      }
   }

   if (mIsSolved && currentPtr)
   {
      mSolvedBoardPtr = std::make_unique<Board>(*currentPtr->mBoardPtr);
      while (currentPtr->mParentPtr != nullptr)
      {
         mMovesToSolve.push_front(currentPtr->mParentMove);
         currentPtr = currentPtr->mParentPtr;
      }
   }
}

void IterativeDeepeningDepthFirstTreeSearchSolver::Solve()
{
   int maxDepth = 0;
   SearchNode* solvedPtr = nullptr;

   while (true)
   {
      solvedPtr = SolveToDepth(maxDepth);
      if (mIsSolved)
      {
         break;
      }
      else
      {
         maxDepth++;
         mExplored.clear();
      }
   };

   if (mIsSolved && solvedPtr)
   {
      mSolvedBoardPtr = std::make_unique<Board>(*solvedPtr->mBoardPtr);
      while (solvedPtr->mParentPtr != nullptr)
      {
         mMovesToSolve.push_front(solvedPtr->mParentMove);
         solvedPtr = solvedPtr->mParentPtr;
      }
   }
}

IterativeDeepeningDepthFirstTreeSearchSolver::SearchNode* IterativeDeepeningDepthFirstTreeSearchSolver::SolveToDepth(const int aMaxDepth)
{
   mInitialNodePtr = std::make_unique<SearchNode>(nullptr, Direction::Up, *mInitialBoardPtr);
   mFrontier.push(mInitialNodePtr.get());
   SearchNode* currentPtr = nullptr;

   while (!mFrontier.empty())
   {
      currentPtr = mFrontier.top();
      mFrontier.pop();
      if (currentPtr->mBoardPtr->IsSolved())
      {
         mIsSolved = true;
         return currentPtr;
      }
      else if (mExplored.find(*currentPtr->mBoardPtr) != mExplored.end())
      {
         // already explored
         continue;
      }
      else if (currentPtr->mDepth >= aMaxDepth)
      {
         continue;
      }

      mExplored.insert(*currentPtr->mBoardPtr);
      std::vector<Direction> legalMoves = currentPtr->mBoardPtr->LegalMoves();
      for (const Direction move : legalMoves)
      {
         auto childPtr = std::make_unique<SearchNode>(currentPtr, move, *currentPtr->mBoardPtr);
         childPtr->mBoardPtr->MakeMove(move);
         mFrontier.push(childPtr.get());
         currentPtr->mChildren[static_cast<size_t>(move)] = std::move(childPtr);
      }
   }

   return nullptr;
}
