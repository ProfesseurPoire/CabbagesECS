#include <cabba/test/test.h>
#include <cabba/ecs/Entity.h>
#include <cabba/ecs/EntityManager.h>

struct TestComponent
{
    int value;
};

using namespace cabba::test;
using namespace cabba;

class EntityTesting : public cabba::test::Test
{
public:

    const int entity_size = 100;

    EntityManager* manager;
    World * world;

    void set_up()override
    {
        world   = new World(entity_size);
        manager = new EntityManager(*world, entity_size);
    }

    void tear_down() override
    {
        delete world;
        delete manager;
    }
};

TEST_F(EntityTesting, pool_size)
{
    assert_that(manager->size(), equals(entity_size));
    assert_that(manager->left(), equals(entity_size));
}

TEST_F(EntityTesting, pool)
{
    Entity* e = manager->Pool();
    assert_that(manager->left(), equals(entity_size - 1));
}

TEST_F(EntityTesting, release)
{
    Entity* e = manager->Pool();
    assert_that(manager->left(), equals(entity_size - 1));
    manager->release(e);
    assert_that(manager->left(), equals(entity_size));
}