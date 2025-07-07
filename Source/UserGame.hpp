#pragma once
#include "Game.h"
#include "Game/Components/SpriteComponent.hpp"
#include "Game/Components/SpinComponent.hpp"

class UserGame : public RayEngine::Game{
    protected:
        void RegisterLevels(std::vector<RayEngine::Level> &outLevels) const override{
            auto level1 = RayEngine::Level::Create("Level 1")
            .WithEntity(
                RayEngine::Entity("Object 1")
                .WithPosition(Vector2{50.0f, 100.0f})
                .WithScale(Vector2{100.0f, 100.0f})
                .WithComponent(new SpriteComponent("../Data/Sprites/wmii.png", RED))
                .WithComponent(new SpinComponent(70.0f))
            );
            // .WithEntity(
            //     RayEngine::Entity("Object 2")
            //     .WithRotation(45.0f)
            // )
            // .WithEntity(
            //     RayEngine::Entity("Object 3")
            //     .WithPosition(Vector2{-50.0f, -50.0f})
            //     .WithRotation(-30.0f)
            //     .WithScale(Vector2{2.0f, 3.0f})
            // );

            outLevels.emplace_back(level1);
        }
};

inline RayEngine::Game *User_CreateGame(){
    const RayEngine::GameSpec gameSpec{
        .WindowSize = {1280, 720},
        .WindowTitle = "RayEngine Game"
    };
    RayEngine::Game *game = new UserGame();
    game->Init(gameSpec);
    return game;
}