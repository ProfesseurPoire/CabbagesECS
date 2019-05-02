#pragma once


#include <stack>
#include <cabba/containers/List.h>

namespace cabba
{
class World;
class Entity;

/*
 * @brief   Use this class to create and delete new entities. This class
 *          is managed by World
 */
class EntityManager
{
    int keys = 0;

public:
    
    // EntityManager(World& game);

    /*
     * @brief   Builds a new Entity Manager object that will contains
     */
    EntityManager(World& game, int size);

    /*
     * @brief   Returns how many entities have been pooled
     */
    int used()const;

    /*
     * @brief   Returns how many entity the user can pool from the manager
     */
    int left()const;

    /*
     * @brief   Returns how many entity the manager owns
     */
    int size()const;
    
    /*
     * @brief   Returns an unsued entity
     */
    Entity* pool();

    Entity* get(const int index);

    /*
     * @brief   Release an entity
     *
     *          Actually adds the entity to a list of entity that needs to be released
     *          You still need to call "cleanup"/"finish"/"clear". Ideally, you'll
     *          want to use the release function to tag entities that you want to
     *          destroy during the game's loop. Then, after you're done iterating through
     *          every component/entity/system, you call the clean function to actually
     *          release everything that was tagged for release
     *
     * @param   entity  Entity that will be released (put back into the stack for 
     *                  later reuse)
     */
    void release(Entity* entity);

    /*
     * @brief   Immediatly release an entity. You might want to avoid doing that
     *          if you're currently looping your entity or components since you might
     *          get into undefined behavior 
     */
    void release_immediate(Entity* e);

    /*
     * @brief   Actualy release the entities that have been marked for release
     */
    void finalize_release();

private:

    World& _world;
    Entity* _entities;

    List<int>        _registered_entities;
    List<Entity*>    _released_entities;
    std::stack<Entity*>     _entity_pool;
};
}