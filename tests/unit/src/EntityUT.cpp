#include <cabba/test/test.h>
#include <cabba/ecs/Entity.h>
#include <cabba/ecs/EntityManager.h>

struct Component
{
    int value;
};

using namespace cabba::test;
using namespace cabba;

class EntityUT : public cabba::test::Test
{
public:

    const int entity_size = 100;
    const int component_pool_count = 100;

    World world{ entity_size };
    EntityPool* manager;

    void set_up()override
    {
        manager = &world.entity_manager();
        world.component_manager().add<Component>(component_pool_count);
    }
};

TEST_F(EntityUT, pool_size)
{
    assert_that(manager->size(), equals(entity_size));
    assert_that(manager->left(), equals(entity_size));
}

TEST_F(EntityUT, pool)
{
    Entity* e = manager->pool();
    assert_that(manager->left(), equals(entity_size - 1));
}

TEST_F(EntityUT, release)
{
    Entity* e = manager->pool();
    assert_that(manager->left(), equals(entity_size - 1));

    // Release should not be immidiate, except if asked 
    // for immediate release
    manager->release_immediate(e);
    assert_that(manager->left(), equals(entity_size));
}

TEST_F(EntityUT, release_multiple)
{
    manager->release(manager->pool());
    manager->release(manager->pool());

    assert_that(manager->left(), equals(entity_size - 2));
    assert_that(manager->used(), equals(2));

    manager->finalize_release();

    assert_that(manager->left(), equals(entity_size));
    assert_that(manager->used(), equals(0));
}

TEST_F(EntityUT, add_component)
{
    Entity* e = manager->pool();

    assert_that(e->has_component<Component>(), equals(false));

    e->add_component<Component>();

    assert_that(e->has_component<Component>(), equals(true));

    auto handle = e->get_component<Component>();
    handle->value = 5;

    auto handle2 = e->get_component<Component>();

    assert_that(handle2->value, equals(5));
}
