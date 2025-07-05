#include "Entity.h"
#include <iostream>
#include <ostream>
#include <cmath>

RayEngine::Entity::Entity(const Entity &other){
    *this = other;
}

RayEngine::Entity &RayEngine::Entity::operator=(const Entity &other){
    ID =other.ID;
    Name = other.Name;
    vPosition = other.vPosition;
    fRotation = other.fRotation;
    vScale = other.vScale;
    bDestroyed = other.bDestroyed;
    return *this;
}

RayEngine::Entity &RayEngine::Entity::WithPosition(const Vector2 &position){
    vPosition = position;
    return *this;
}

RayEngine::Entity &RayEngine::Entity::WithRotation(const float &rotation){
    fRotation = rotation;
    return *this;
}

RayEngine::Entity &RayEngine::Entity::WithScale(const Vector2 &scale){
    vScale = scale;
    return *this;
}

void RayEngine::Entity::Start(){
    std::cout << "Starting Entity :: " << Name << '\n';
}

void RayEngine::Entity::Update(const UpdateContext &context){
    std::cout << "Updating Entity :: " << Name << '\n';
    if(context.Input->GetKey(KeyCode::F1, InputState::Pressed)){
        context.LvlManager->ChangeLevel("Level 1");
    }
    if(context.Input->GetKey(KeyCode::F2, InputState::Pressed)){
        context.LvlManager->ChangeLevel("Level 2");
    }
    vPosition.x -= std::cos(GetTime() * 15.0f * context.DeltaTime);
    vPosition.y += std::sin(GetTime() * 15.0f * context.DeltaTime);
}

void RayEngine::Entity::Render(const RenderContext &context) const{
    std::cout << "Rendering Entity :: " << Name << '\n';
    const Vector2 halfSize = {10.0f * vScale.x, 10.0f * vScale.y};
    Rectangle rect;
    rect.x = vPosition.x;
    rect.y = vPosition.y;
    rect.width = halfSize.x * 2.0f;
    rect.height = halfSize.y * 2.0f;
    DrawRectanglePro(rect, halfSize, fRotation, PURPLE);
}

void RayEngine::Entity::RenderUI(const RenderUiContext &context) const{
    std::cout << "RenderUI Entity :: " << Name << '\n';
    if(context.bDebug){
        const Vector2 screenPos = GetWorldToScreen2D(Vector2{vPosition.x, vPosition.y}, *context.Camera);
        DrawText(Name.c_str(), screenPos.x, screenPos.y, 16, ORANGE);
    }
}

void RayEngine::Entity::Destroy(){
    std::cout << "Destroying Entity :: " << Name << '\n';
    bDestroyed = true;
}