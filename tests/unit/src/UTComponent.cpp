#include <cabba/test/test.h>
#include <cabba/ecs/Entity.h>
#include <cabba/ecs/EntityManager.h>
#include <cabba/ecs/ComponentPool.h>

using namespace cabba::test;
using namespace cabba;

struct Comp
{
    int value;
};

Param& operator<<(Param& p, Comp c)
{
    printf("%i", c.value);
    return p;
}

class ComponentTesting : public Test
{
public:

    const int entity_size = 100;

    EntityManager* manager;
    World* world;

    void set_up()override
    {
        world   = new World();
        manager = new EntityManager(*world, entity_size);
    }

    void tear_down()override
    {
        delete world;
        delete manager;
    }
};

TEST_F(ComponentTesting, add_component)
{
    world->initializePool<Comp>();

    ComponentPool<Comp>* pool = world->getPool<Comp>();

    Entity* e = manager->Pool();

    Comp*c = e->AddComponent<Comp>();
    c->value = 5;

    //assert_that(c, non_equals(nullptr));
    assert_that(pool->left(), equals(entity_size - 1));
}