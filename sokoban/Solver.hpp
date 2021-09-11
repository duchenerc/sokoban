
#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <array>
#include <chrono>
#include <iostream>
#include <list>
#include <memory>
#include <queue>
#include <stack>
#include <unordered_set>

#include "Board.hpp"
#include "Location.hpp"

class Solver
{
public:
   using wall_time = std::chrono::microseconds;

   Solver() = delete;
   Solver(const Board& aInitial)
      : mInitial{ aInitial }
   {}
   virtual ~Solver() = default;

   void Exec();

   void PrintToStream(std::ostream& aOut) const;

protected:
   virtual void Solve() = 0;

   struct SearchNode
   {
      SearchNode() = delete;
      SearchNode(SearchNode* aParentPtr, const Direction aParentMove, const Board& aBoard)
         : mParentPtr{ aParentPtr }
         , mParentMove{ aParentMove }
         , mBoard{ aBoard }
         , mDepth{ aParentPtr == nullptr ? 0 : aParentPtr->mDepth + 1 }
      {}

      virtual ~SearchNode() = default;

      SearchNode* mParentPtr;
      Direction mParentMove;
      Board mBoard;
      int mDepth;
      std::array<std::unique_ptr<SearchNode>, 4> mChildren;
   };

   Board mInitial;
   Board mSolved;
   std::list<Direction> mMovesToSolve;
   bool mIsSolved = false;

private:
   wall_time mWallTime;

};

class BreadthFirstTreeSearchSolver : public Solver
{
public:
   BreadthFirstTreeSearchSolver() = delete;
   BreadthFirstTreeSearchSolver(const Board& aInitial)
      : Solver{ aInitial }
   {}
   virtual ~BreadthFirstTreeSearchSolver() = default;

protected:
   void Solve() override;

private:
   std::unique_ptr<SearchNode> mInitialNodePtr;
   std::unordered_set<Board> mExplored;
   std::queue<SearchNode*> mFrontier;
};

class IterativeDeepeningDepthFirstTreeSearchSolver : public Solver
{
public:
   IterativeDeepeningDepthFirstTreeSearchSolver() = delete;
   IterativeDeepeningDepthFirstTreeSearchSolver(const Board& aInitial)
      : Solver{ aInitial }
   {}
   virtual ~IterativeDeepeningDepthFirstTreeSearchSolver() = default;

protected:
   void Solve() override;
   SearchNode* SolveToDepth(const int aMaxDepth);

private:
   std::unique_ptr<SearchNode> mInitialNodePtr;
   std::unordered_set<Board> mExplored;
   std::stack<SearchNode*> mFrontier;
};

#endif