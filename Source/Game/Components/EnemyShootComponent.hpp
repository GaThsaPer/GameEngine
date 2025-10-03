#pragma once 
#include "../Component.h"
#include "../Entity.h"
#include <iostream>
#include "SpriteComponent.hpp"
#include "ColliderComponent.hpp"
#include "../WorldManager.h"

class EnemyShootComponent : public RayEngine::Component{
    public:
        static const std::string ID;
        EnemyShootComponent(std::string textPath, float speed, float maxDist, float intensity):ShootSpeed(speed), MaxDistance(maxDist), ShootingIntensity(intensity){
            TexturePath = textPath;
            texture = LoadTexture(TexturePath.c_str());
            if(!IsTextureValid(texture)){
                std::cout << "Failed to load a texture " << TexturePath << '\n';
            }
            DeltaSum=0;
        }
        Component *Clone() const override {return new EnemyShootComponent(TexturePath, ShootSpeed, MaxDistance, ShootingIntensity);}
        const std::string &GetID() const override {return ID;}
        void OnUpdate(const RayEngine::UpdateContext &context) override {
            RayEngine::Entity *owner = GetOwner();
            DeltaSum += context.DeltaTime;
            if(DeltaSum > ShootingIntensity){
                DeltaSum -= ShootingIntensity;
                Vector2 mousePos = context.Input->GetCursorWorldPosition(*context.Camera);
                RayEngine::Entity *newEntity = new RayEngine::Entity("EnemyShoot");
                newEntity->WithPosition(owner->GetPosition())
                         .WithRotation(owner->GetRotation())
                         .WithScale(Vector2{5.0f, 50.0f})
                         .WithComponent(new SpriteComponent(TexturePath))
                         .WithComponent(new ColliderComponent(physic::CollisionShape::Rectangle, 4.0f, 42.5f));
                int index = Shoots.size();
                Shoots.push_back(newEntity);
                context.worldManager->Spawn(Shoots.at(index));
            }

            for(auto &entity : Shoots){
                Vector2 position = entity->GetPosition();
                float agile = (entity->GetRotation() - 90) * DEG2RAD;
                position.x += ShootSpeed * cos(agile);
                position.y += ShootSpeed * sin(agile);
                entity->SetPosition(position);
            }

            for(int i=0; i<Shoots.size(); i++){
                Shoots.at(i)->Update(context);
                Vector2 entityPos = Shoots.at(i)->GetPosition();
                if(entityPos.x < -MaxDistance || entityPos.x > MaxDistance || entityPos.y < -MaxDistance || entityPos.y > MaxDistance){
                    Shoots.at(i)->Destroy();
                    Shoots.erase(Shoots.begin() + i);
                    i--;
                }
            }
        }
    private:
        float DeltaSum;
        float ShootingIntensity;
        std::string TexturePath;
        Texture2D texture;
        float ShootSpeed;
        std::vector<RayEngine::Entity*> Shoots;
        float MaxDistance;
};

const std::string EnemyShootComponent::ID = "EnemyShootComponent";