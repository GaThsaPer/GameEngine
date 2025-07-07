#pragma once 

#include <string>
#include "../Context.h"

namespace RayEngine{
    class Entity;
    class Component{
        public:
            virtual ~Component() = default;
            virtual const std::string &GetID() const = 0;
            Entity *GetOwner() const {return owner;}
            virtual void OnStart() {}
            virtual void OnUpdate(const UpdateContext &context) {}
            virtual void OnRender(const RenderContext &context) const {}
            virtual void OnRenderUI(const RenderUiContext &context) const {}
            virtual void OnDestroy() {}

            virtual Component *Clone() const = 0;
            virtual void Init(Entity *entity) {
                owner = entity;
            }
        private:
            Entity *owner = nullptr;
    };
}