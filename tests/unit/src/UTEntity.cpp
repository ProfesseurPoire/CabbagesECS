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
    const int component_pool_count = 100;

    World world{ entity_size };
    EntityManager* manager;

    void set_up()override
    {
        manager = &world.entity_manager();
        world.initialize_pool<TestComponent>(component_pool_count);
    }
};

TEST_F(EntityTesting, pool_size)
{
    assert_that(manager->size(), equals(entity_size));
    assert_that(manager->left(), equals(entity_size));
}

TEST_F(EntityTesting, pool)
{
    Entity* e = manager->pool();
    assert_that(manager->left(), equals(entity_size - 1));
}

TEST_F(EntityTesting, release)
{
    Entity* e = manager->pool();
    assert_that(manager->left(), equals(entity_size - 1));

    // Release should not be immidiate, except if asked 
    // for immediate release
    manager->release_immediate(e);
    assert_that(manager->left(), equals(entity_size));
}

TEST_F(EntityTesting, release_multiple)
{
    manager->release(manager->pool());
    manager->release(manager->pool());

    assert_that(manager->left(), equals(entity_size - 2));
    assert_that(manager->used(), equals(2));

    manager->finalize_release();

    assert_that(manager->left(), equals(entity_size));
    assert_that(manager->used(), equals(0));
}

TEST_F(EntityTesting, add_component)
{
    Entity* e = manager->pool();

    assert_that(e->has_component<TestComponent>(), equals(false));

    e->add_component<TestComponent>();

    assert_that(e->has_component<TestComponent>(), equals(true));

    auto handle = e->get_component<TestComponent>();
    handle->value = 5;

    auto handle2 = e->get_component<TestComponent>();

    assert_that(handle2->value, equals(5));
}