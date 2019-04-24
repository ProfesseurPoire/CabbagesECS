#include <cabba/ecs/Entity.h>

namespace cabba
{
    Entity::Entity() {}

    EntityId Entity::id()const
    {
        return key;
    }
}