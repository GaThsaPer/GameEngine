#pragma once
#include "../Component.h"
#include "../Entity.h"

class SpinComponent : public RayEngine::Component{
    public:
        static const std::string ID;
        SpinComponent(float rotateSpeed) : speed(rotateSpeed){}
        Component *Clone() const override {return new SpinComponent(speed); }
        const std::string &GetID() const override {return ID; } 
        void OnUpdate(const RayEngine::UpdateContext &context) override{
            RayEngine::Entity *owner = GetOwner();
            float rotation = owner->GetRotation();
            rotation += speed * context.DeltaTime;
            owner->SetRotation(rotation);
        }
    private:
        float speed;
};
const std::string SpinComponent::ID = "SpinComponent";