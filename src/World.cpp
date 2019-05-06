#include <cabba/ecs/World.h>
#include <cabba/ecs/Entity.h>

namespace cabba
{
void World::release_entity_immediate(Entity* e)
{
    // Making sure to also delete the components I guess
    for (auto& pair : _component_pools)
    {
        pair.second->exist(e->id());

        // Check if there's a component associated to the entity
        if (pair.second->exist(e->id()))
        {
            // Then we remove the component from the entity
            pair.second->remove(e->id());
        }
    }

    _entity_manager._registered_entities.remove(e->id());
    _entity_manager._entity_pool.emplace(e);
}
}