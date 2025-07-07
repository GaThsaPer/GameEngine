#include "World.h"
#include <iostream>

void RayEngine::World::LoadLevel(const Level &level){
    level.LoadInto(*this);
}

void RayEngine::World::Spawn(const Entity &entity){
    Entities.push_back(entity);
    Entity &newEntity = Entities.back();
    LastEntityID++;
    newEntity.SetID(LastEntityID);
    newEntity.Start();
}

void RayEngine::World::Clear(){
    for(auto &entity : Entities){
        entity.Destroy();
    }
    Entities.clear();
}

void RayEngine::World::Update(const UpdateContext &context){
    for(auto &entity : Entities){
        if(!entity.MarkedDorDestruction()){
            entity.Update(context);
        }
    }
    
    for(int i = Entities.size() - 1; i >= 0; i--){
        if(Entities.at(i).MarkedDorDestruction()){
            Entities.erase(Entities.begin() + i);
        }
    }
}

void RayEngine::World::Render(const RenderContext &context) const{
    for(auto &entity : Entities){
        entity.Render(context);
    }
}

void RayEngine::World::RenderUI(const RenderUiContext &context) const{
    for(auto &entity : Entities){
        entity.RenderUI(context);
    }
}