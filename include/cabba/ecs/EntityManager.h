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

    // I should ask myself ... what does the entity Manager does
    // 1) It is used to query/create/remove new Entities
    // 2) It owns entities 
class EntityPool
{
    int keys = 0;

public:

    friend class World;
    
    // EntityPool(World& game);

    /*
     * @brief   Builds a new Entity Manager object that will contains
     */
    EntityPool(World& game, int size);

    ~EntityPool();
    EntityPool(const World&)            = delete;
    EntityPool(World&&)                 = delete;
    EntityPool& operator=(const World&) = delete;
    EntityPool& operator=(World&&)      = delete;

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
    
    /*!
     * @brief   Returns an unused entity
     */
    Entity& get();

    /*!
     * @brief   Returns the entity at the given position
     */
    Entity* get(const int position);

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

    // Some things really feels unnecessary here 

    World& _world;

    // This is where the entities exist for real
    Entity* _entities = nullptr;

    List<int>           _registered_entities;
    List<Entity*>       _released_entities;
    std::stack<Entity*> _entity_pool;
};
}