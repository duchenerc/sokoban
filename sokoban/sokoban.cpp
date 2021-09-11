// sokoban.cpp : Defines the entry point for the application.
//
#include "sokoban.hpp"

int main(int argc, char* argv[])
{
   std::string filename;
   if (argc < 2)
   {
      std::cout << "usage: sokoban <filename>" << std::endl;
      return 1;
   }
   else
   {
      filename = argv[1];
   }

   std::ifstream fin{ filename };

   fin.close();

   return 0;
}
