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
        auto pool = world.get_component_pool<ComponentA>();

        for (ComponentA& component : *pool.operator->())
            sum += component.value;
    }
};

class System2Pool : public SystemInterface
{
public:

    int sum = 0;

    void update(World& world, float elapsed_time)override
    {
        auto poolA = world.get_component_pool<ComponentA>();
        auto poolB = world.get_component_pool<ComponentB>();

        for (int i = 0; i < poolA->used(); ++i)
        {
            // Not exactly the most clear way to use the pool but it's either that,
            // create an iterator, or to store the entity id inside the component

            int entity_id = poolA->get_entity_id(i);

            if (poolB->exist(entity_id))
            {
                ComponentB* b = poolB->get_raw(entity_id);
                sum += poolA->get_at(i).value;
                sum += b->value;
            }
        }
    }
};

TEST(Integration, SuperTest)
{
    // We need to tell how many entity the game/world can have in total
    // since there's no dynamic allocation here

    World world{ 100 };

    world.add_component_pool<ComponentA>(100);
    world.add_component_pool<ComponentB>(100);

    world.add_system<System1Pool>();
    world.add_system<System2Pool>();

    Entity& e1 = world.entity_pool().get();
    Entity& e2 = world.entity_pool().get();
    Entity& e3 = world.entity_pool().get();

    e1.add_component<ComponentA>();
    e1.add_component<ComponentB>();

    e2.add_component<ComponentA>();

    e3.add_component<ComponentB>();

    // Emulates a game loop
    for (int i = 0; i < 10; i++)
        world.update();

    assert_that(world.get_system<System1Pool>()->sum, equals(20));
    assert_that(world.get_system<System2Pool>()->sum, equals(60));
}