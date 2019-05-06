#include <cabba/test/test.h>

#include <cabba/ecs/World.h>
#include <cabba/ecs/System.h>
#include <cabba/ecs/Entity.h>

using namespace cabba::test;
using namespace cabba;

int main()
{
    return cabba::test::run_all();
}

struct ComponentA{int value = 1;};
struct ComponentB{int value = 5;};

class System1Pool: public SystemInterface
{
public:

    int sum = 0;

    void update(World& world, float elapsed_time)override 
    {
        /*ComponentPool<ComponentA>& pool = *world.get_component_pool<ComponentA>();

        for (ComponentA& component : pool)
            sum += component.value;*/
    }
};

class System2Pool : public SystemInterface
{
public:

    int sum = 0;

    void update(World& world, float elapsed_time)override
    {
        //ComponentPool<ComponentA>& poolA = *world.get_component_pool<ComponentA>();
        //ComponentPool<ComponentB>& poolB = *world.get_component_pool<ComponentB>();

        //for (int i = 0; i < poolA.used(); ++i)
        //{
        //    // Not exactly the most clear way to use the pool but it's either that,
        //    // create an iterator, or to store the entity id inside the component

        //    int entity_id = poolA.get_entity_id(i);

        //    if (poolB.exist(entity_id))
        //    {
        //        ComponentB* b = poolB.get_raw(entity_id);
        //        sum += poolA.get_at(i).value;
        //        sum += b->value;
        //    }
        //}
    }
};

TEST(Integration, SuperTest)
{
    // We need to tell how many entity the game/world can have in total
    // since there's no dynamic allocation here

    // I could also define a default reasonable value for newcomers?
    World world{ 100 };

    world.add_component_pool<ComponentA>(100);
    world.add_component_pool<ComponentB>(100);

    // I guess I could technically return a shared_ptr? Or a const
    // shared_ptr? 
    ObserverPointer<ComponentPool<ComponentA>> c = world.get_component_pool<ComponentA>();

    delete c._ptr;
    

    // In the end you can still hack even that though
    //delete c.operator->();

    // Ok so I can't create or delete a componentPool, so, that's nice...
    // I think there's no reallocation for maps so I "should" not lose
    // the ptr

    
    // Because there's 2 problematic when you return a raw pointer

    // The first one is that you can delete it from the outside and simply
    // breaking everything apart. That can be fixed by setting the 
    // destructor in protected

    // The second one is that the pointer will likely outlive the ECS
    // and if someones tries to do something with it that the ECS is dead,
    // it will obviously corrupt the heap. 
    // One way to fix that is either using a shared_ptr, and thus the 
    // component_pool will only be delete when there is no more reference
    // or maybe to return a weak_ptr? So I can check if the ptr still exist?

    
    
    // If you ask for more than you ask you're likely for a ton of troubles
    // I really wonder about allowing the entity pool to reallocate for more
    // entity in case you use more. Allowing resize would break the references,
    // though an entity is really just an int with this system. I could
    // even return Entity object instead of references, this way it wouldn't
    // break, and if needed the entitypool could reallocate, though
    // it's not really the design I was going for. The idea was to avoid
    // any kind of runtime check. If I have to check if there's enough
    // place in the pool that kinda breaks that 

    // Also I really prefer to first call the entity_pool, to make it obvious
    // for the user that entities are inside a pool.
    Entity& e1 = world.entity_pool().get();
    Entity& e2 = world.entity_pool().get();
    Entity& e3 = world.entity_pool().get();

    e1.add_component<ComponentA>();
    e1.add_component<ComponentB>();

    e2.add_component<ComponentA>();

    e3.add_component<ComponentB>();

    // nothing prevent me from deleting this later on
    // ok, sure, actually nothing prevent me that, or maybe
    // a virtual private destructor?
    //new SuperSystem;

    /*world.add_system<System1Pool>();

    world.system_factory().create<System1Pool>();
    world.System_factory().create<system2Pool>();*/
    //world.system_map().get<System1Pool>();

    //world.add_system<System1Pool>();
    world.system_manager().add<System1Pool>();
    world.system_manager().add<System2Pool>();

    // Emulates a game loop
    for (int i = 0; i < 10; i++)
    {
        world.system_manager().update(world, 0.1f);
    }

    assert_that(world.system_manager().get<System1Pool>().sum, equals(20));
    assert_that(world.system_manager().get<System2Pool>().sum, equals(60));
}