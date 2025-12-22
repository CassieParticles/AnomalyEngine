#include "Entity.h"
Engine::Entity::Entity():entityId{UINT32_MAX} {}
Engine::Entity::Entity(EntityId entity):entityId(entity){}
Engine::Entity::Entity(const Entity &other)
{
    this->entityId = other.entityId;
}
