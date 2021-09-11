// sokoban.cpp : Defines the entry point for the application.
//
#include "sokoban.hpp"

int main(int argc, char* argv[])
{
   if (argc < 2)
   {
      std::cout << "usage: sokoban <filename>" << std::endl;
      return 1;
   }

   std::string solverType = "";
   bool valid = false;
   do
   {
      std::cout << "sokoban: select solver ([b]fts|[i]ddfts|[g]befgs|[a]star): " << std::flush;
      std::cin >> solverType;

      if (solverType.empty())
      {
         continue;
      }

      valid = solverType[0] == 'b' ||
         solverType[0] == 'i' ||
         solverType[0] == 'g' ||
         solverType[0] == 'a';

   } while (!valid);

   std::string filename = argv[1];
   std::ifstream fin{ filename };
   
   Board::Builder builder;
   builder.FromStream(fin);
   fin.close();

   Board initial = builder.Build();
   std::unique_ptr<Solver> solver;

   switch (solverType[0])
   {
   case 'b':
      solver = std::make_unique<BreadthFirstTreeSearchSolver>(initial);
      break;
   case 'i':
   case 'g':
   case 'a':
   default:
      std::cout << "sokoban: solver not implemented yet, exiting" << std::endl;
      break;
   }

   solver->Exec();
   solver->PrintToStream(std::cout);

   return 0;
}
