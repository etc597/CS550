#include "Core/Engine.hpp"

int main(int argc, char* argv[])
{

  Engine engine(argc, argv);

  if (!engine.Init()) 
  {
    engine.Deinit();
    return -1;
  }

  engine.Run();

  engine.Deinit();

  return 0;
}


