#include "../Component.h"
#include "../Entity.h"

#include <iostream>
#include <raymath.h>


class PlayerMovementComponent : public RayEngine::Component{
    public:
        static const std::string ID;
        PlayerMovementComponent(float playerSpeed):PlayerSpeed(playerSpeed){
            Velocity = {0.0f, 0.0f};
        }
        Component *Clone() const override {return new PlayerMovementComponent(PlayerSpeed);}
        const std::string &GetID() const override {return ID;}
        void OnUpdate(const RayEngine::UpdateContext &context) override{
            auto *owner = GetOwner();
            if(context.Input->GetKey(RayEngine::KeyCode::W, RayEngine::InputState::Held)){
                Velocity.y -= PlayerSpeed;
            }
            if(context.Input->GetKey(RayEngine::KeyCode::S, RayEngine::InputState::Held)){
                Velocity.y += PlayerSpeed;
            }
            if(context.Input->GetKey(RayEngine::KeyCode::A, RayEngine::InputState::Held)){
                Velocity.x -= PlayerSpeed;
            }
            if(context.Input->GetKey(RayEngine::KeyCode::D, RayEngine::InputState::Held)){
                Velocity.x += PlayerSpeed;
            }
            Vector2 Position = owner->GetPosition();
            Position.x += Velocity.x;
            Position.y += Velocity.y;
            owner->SetPosition(Position);
            // float deltaTime = static_cast<float>(context.DeltaTime);
            Velocity = {Velocity.x * 0.9f, Velocity.y * 0.9f};

            Vector2 corsorPosition = context.Input->GetCursorWorldPosition(*context.Camera);
            Vector2 CalcPos = {corsorPosition.x - Position.x, corsorPosition.y - Position.y};
            float angle = atan2(CalcPos.x, CalcPos.y) * RAD2DEG;
            owner->SetRotation(-angle + 180);
        }
    private:
        float PlayerSpeed;
        Vector2 Velocity;
};

const std::string PlayerMovementComponent::ID = "PlayerMovementComponent";