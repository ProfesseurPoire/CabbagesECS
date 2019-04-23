#include "entity.h"

namespace cabba
{
    Entity::Entity() {}

    EntityId Entity::id()const
    {
        return key;
    }
}