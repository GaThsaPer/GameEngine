#include "Level.h"
#include "World.h"

RayEngine::Level RayEngine::Level::Create(const std::string &name){
    return Level(name);
}

RayEngine::Level& RayEngine::Level::WithEntity(const Entity &entity){
    Entities.emplace_back(entity);
    return *this;
}

void RayEngine::Level::LoadInto(World &world) const{
    for(const auto &entity : Entities){
        world.Spawn(entity);
    }
}