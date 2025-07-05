#include "Body.h"

int physic::PhysicBody::HighestID = 0;

physic::PhysicBody::PhysicBody(PhysicBody &&other) noexcept
    : ID(other.ID), position(other.position), velocity(other.velocity),
    BodyShape(std::move(other.BodyShape))
{
    other.ID = -1;
}
physic::PhysicBody& physic::PhysicBody::operator=(physic::PhysicBody&& other) noexcept {
    if(this != &other){
        ID = other.ID;
        position = other.position;
        velocity = other.velocity;
        BodyShape = std::move(other.BodyShape);
        other.ID = -1;
    }
    return *this;
}

void physic::PhysicBody::SetCircleBody(float r){
    BodyShape.reset(new Circle(r));
}

void physic::PhysicBody::SetRectangleBody(float x, float y){
    BodyShape.reset(new Rectangle(x, y));
}

physic::PhysicBody::PhysicBody(){
    ID = HighestID + 1;
    HighestID++;
    BodyShape = nullptr;
}
physic::PhysicBody::~PhysicBody(){}

void physic::PhysicBody::Update(double deltaTime){
    position += velocity * deltaTime;
}

bool physic::PhysicBody::operator!=(const physic::PhysicBody &OtherBody){
    if(ID == OtherBody.ID)
        return false;
    return true;
}

bool physic::PhysicBody::CheckCollision(const PhysicBody &otherBody){
    if(auto circle = dynamic_cast<physic::Circle*>(BodyShape.get()))
        return circle->CheckCollision(otherBody.BodyShape.get(), position, otherBody.position);
    else if(auto rectangle = dynamic_cast<physic::Rectangle*>(BodyShape.get()))
        return rectangle->CheckCollision(otherBody.BodyShape.get(), position, otherBody.position);
    return false;
}

void physic::PhysicBody::HandleCollision(PhysicBody &otherBody){
    std::cout << "Collision detected!\n";
}

void physic::PhysicBody::RenderCollider() const{
    if(auto circle = dynamic_cast<physic::Circle*>(BodyShape.get())){
        DrawRing(position,circle->GetRad() - 1.0f, circle->GetRad() + 1.0f, 0, 360, 0, RED);
    }
    else if(auto rectangle = dynamic_cast<physic::Rectangle*>(BodyShape.get())){
        DrawRectangleLines(position.x, position.y, rectangle->GetW(), rectangle->GetH(), RED);
    }
}