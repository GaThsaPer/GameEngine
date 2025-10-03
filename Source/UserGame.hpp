#pragma once
#include "Game.h"
#include "Game/Components/SpriteComponent.hpp"
#include "Game/Components/SpinComponent.hpp"
#include "Game/Components/ColliderComponent.hpp"
#include "Game/Components/PlayerMovementComponent.hpp"
#include "Game/Components/ShootComponent.hpp"
#include "Game/Components/EnemyShootComponent.hpp"
#include "Game/Components/EnemyMovementComponent.hpp"
#include "Game/Components/BackgroundComponent.hpp"

class UserGame : public RayEngine::Game{
    protected:
        void RegisterLevels(std::vector<RayEngine::Level> &outLevels) const override{
            auto level1 = RayEngine::Level::Create("Level 1")
            .WithEntity(
                RayEngine::Entity("Background")
                .WithPosition(Vector2{0.0f, 0.0f})
                .WithScale(Vector2{1500.0f, 1000.0f})
                .WithComponent(new BackgroundComponent("../Data/Sprites/background.png"))
            )
            .WithEntity(
                RayEngine::Entity("Player")
                .WithPosition(Vector2{50.0f, 100.0f})
                .WithScale(Vector2{50.0f, 50.0f})
                .WithRotation(45.0f)
                .WithComponent(new SpriteComponent("../Data/Sprites/PlayerStarship.png"))
                .WithComponent(new ColliderComponent(physic::CollisionShape::Triangle, 50.0f, 50.0f))
                .WithComponent(new PlayerMovementComponent(1.0f))
                .WithComponent(new ShootComponent("../Data/Sprites/Laser.png", "../Data/Sounds/Shoot.wav", 5.0f, 500.0f))
            )
            .WithEntity(
                RayEngine::Entity("Enemy-1")
                .WithPosition(Vector2{-150.0f, -150.0f})
                .WithRotation(-45.0f)
                .WithScale(Vector2{50.0f, 50.0f})
                .WithComponent(new SpriteComponent("../Data/Sprites/EnemyStarship.png"))
                .WithComponent(new ColliderComponent(physic::CollisionShape::Circle, 27.5f))
                .WithComponent(new EnemyMovementComponent(0.5f))
                .WithComponent(new EnemyShootComponent("../Data/Sprites/EnemyLaser.png", 5.0f, 1000.0f, 1.5f))
            );

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