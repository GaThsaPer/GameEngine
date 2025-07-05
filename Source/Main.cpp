#include "Game.h"
/*
    Wykład 1    (UNC)
    Wykład 2    (C)
    Wykład 3    (C)
    Wykład 4    (C)
    Wykład 5    (C)
    Wykład 6    (UNC)
    Wtkład 7    (UNC)
*/
int main(int argc, char *argv[]) {
    
    const RayEngine::GameSpec gameSpec{
        .WindowTitle = "GameEngine", 
        .WindowSize = {1280, 720}
    };

    RayEngine::Game game = RayEngine::Game();
    game.Init(gameSpec);
    game.Run();
    game.Shutdown();
    
    return 0;
}
