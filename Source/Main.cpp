#include "Game.h"

int main(int argc, char *argv[]) {
    
    const RayEngine::GameSpec gameSpec{
        .WindowTitle = "Ray", 
        .WindowSize = {1280, 720}
    };

    RayEngine::Game game = RayEngine::Game();
    game.Init(gameSpec);
    game.Run();
    game.Shutdown();
    
    return 0;
}
