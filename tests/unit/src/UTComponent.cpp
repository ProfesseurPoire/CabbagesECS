#include <cabba/test/test.h>
#include <cabba/ecs/Entity.h>
#include <cabba/ecs/EntityManager.h>
#include <cabba/ecs/ComponentPool.h>

using namespace cabba::test;
using namespace cabba;

struct Component
{
    int value = 5;
};

Param& operator<<(Param& p, Component c)
{
    printf("%i", c.value);
    return p;
}

class ComponentTesting : public Test
{
public:

    const int entity_size       = 100;
    const int component_size    = 100;

    ComponentPool<Component> pool{ entity_size, component_size };

    void set_up()override
    {
        pool.add(0);
    }
};

TEST_F(ComponentTesting, add_component)
{
    assert_that(pool.left(), equals(component_size - 1));
    assert_that(pool.used(), equals(1));
}

TEST_F(ComponentTesting, get_component)
{
    // Just making sure the get returns a handle
    // to the same component
    ComponentPool<Component>::Handle component1 =  pool.get(0);

    component1->value = 10;

    auto component2 = pool.get(0);

    assert_that(component1.valid(), equals(true));
    assert_that(component2->value, equals(10));

    auto component3 = pool.get(1);

    assert_that(component3.valid(), equals(false));
}

TEST_F(ComponentTesting, get_raw_component)
{
    Component* component = pool.get_raw(0);

    assert_that(component->value, equals(5));
}

TEST_F(ComponentTesting, remove_component)
{
    assert_that(pool.left(), equals(component_size - 1));
    assert_that(pool.used(), equals(1));

    pool.remove(0);

    assert_that(pool.left(), equals(component_size));
    assert_that(pool.used(), equals(0));
}

TEST_F(ComponentTesting, remove_unorder)
{
    pool.add(1);
    pool.add(2);

    auto c1     = pool.get(0);
    auto c2     = pool.get(1);
    auto c3     = pool.get(2);

    c1->value   = 1;
    c2->value   = 2;
    c3->value   = 3;

    assert_that(pool.left(), equals(component_size - 3));
    assert_that(pool.used(), equals(3));

    pool.remove(1);

    assert_that(pool.left(), equals(component_size-2));
    assert_that(pool.used(), equals(2));

    // Since we remove the second entity, the handle should
    // now tell us that it is not valid anymore
    assert_that(c2.valid(), equals(false));

    int sum = 0;

    for (Component& comp : pool)
        sum += comp.value;

    assert_that(sum, equals(4));
}

TEST_F(ComponentTesting, loop_components)
{
    pool.add(1);

    auto c1     = pool.get(0);
    auto c2     = pool.get(1);

    c1->value   = 1;
    c2->value   = 3;

    int sum     = 0;

    for(Component& comp : pool)
    {
        sum += comp.value;
    }

    assert_that(sum, equals(4));
}

TEST_F(ComponentTesting, component_exist)
{
    assert_that(pool.exist(0), equals(true));
    assert_that(pool.exist(1), equals(false));
}


