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

class ComponentA
{
public:
    int value = 1;
};

class ComponentB
{
public:
    int value = 5;
};

class System1Pool: public SystemInterface
{
public:

    int sum = 0;

    void update(World& world, float elapsed_time)override 
    {
        cabba::ComponentPool<ComponentA>& pool = *world.get_pool<ComponentA>();

        for (ComponentA& component : pool)
        {
            sum += component.value;
        }
    }
};

class System2Pool : public SystemInterface
{
public:

    int sum = 0;

    void update(World& world, float elapsed_time)override
    {
        cabba::ComponentPool<ComponentA>& poolA = *world.get_pool<ComponentA>();
        cabba::ComponentPool<ComponentB>& poolB = *world.get_pool<ComponentB>();

        for (int i = 0; i < poolA.used(); ++i)
        {
            // Not exactly the most clear way to use the pool but it's either that,
            // create an iterator, or to store the entity id inside the component

            int entity_id = poolA.get_entity_id(i);

            if (poolB.exist(entity_id))
            {
                ComponentB* b = poolB.get_raw(entity_id);
                sum += poolA.get_at(i).value;
                sum += b->value;
            }
        }
    }
};

TEST(Integration, SuperTest)
{
    World world{ 100 };

    world.initialize_pool<ComponentA>(100);
    world.initialize_pool<ComponentB>(100);

    Entity* e1 = world.entity_manager().pool();
    Entity* e2 = world.entity_manager().pool();
    Entity* e3 = world.entity_manager().pool();

    e1->add_component<ComponentA>();
    e1->add_component<ComponentB>();

    e2->add_component<ComponentA>();

    e3->add_component<ComponentB>();

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