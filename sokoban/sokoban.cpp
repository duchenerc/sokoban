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

   Board::Builder builder;
   std::ifstream fin{ filename };
   builder.FromStream(fin);
   fin.close();

   Board initial = builder.Build();

   return 0;
}
