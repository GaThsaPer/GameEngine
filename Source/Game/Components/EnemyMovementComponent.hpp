#pragma once 
#include "../Component.h"
#include "../Entity.h"
#include "../WorldManager.h"

class EnemyMovementComponent : public RayEngine::Component{
    public:
        static const std::string ID;
        EnemyMovementComponent(float Speed): speed(Speed){

        }
        const std::string &GetID() const override {return ID;}
        Component *Clone() const override {return new EnemyMovementComponent(speed);}

        void OnUpdate(const RayEngine::UpdateContext &context) override{
            auto *player = context.worldManager->FindEntity("Player");
            auto *owner = GetOwner();
            Vector2 playerPos = player->GetPosition();
            Vector2 Position = owner->GetPosition();
            Vector2 CalcPos = {playerPos.x - Position.x, playerPos.y - Position.y};
            float angle = atan2(CalcPos.x, CalcPos.y) * RAD2DEG;
            owner->SetRotation(-angle + 180);

            angle = (owner->GetRotation() - 90) * DEG2RAD;
            Position.x += speed * cos(angle);
            Position.y += speed * sin(angle);
            owner->SetPosition(Position);
        }
    private:
        float speed;
};

const std::string EnemyMovementComponent::ID = "EnemyMovementComponent";