#include <cabba/test/test.h>
#include <cabba/ecs/Entity.h>
#include <cabba/ecs/EntityManager.h>
#include <cabba/ecs/ComponentPool.h>

using namespace cabba::test;
using namespace cabba;

struct SuperComp
{
    int value = 5;
};

Param& operator<<(Param& p, SuperComp c)
{
    printf("%i", c.value);
    return p;
}

class ComponentTesting : public Test
{
public:

    const int entity_size       = 100;
    const int component_size    = 100;
};

TEST_F(ComponentTesting, add_component)
{
    ComponentPool<SuperComp> pool(100, component_size);
    SuperComp * c = pool.Add(0);

    assert_that(pool.left(), equals(component_size - 1));
    assert_that(pool.used(), equals(1));
}

TEST_F(ComponentTesting, remove_component)
{
    ComponentPool<SuperComp> pool(100, component_size);
    SuperComp * c = pool.Add(0);

    assert_that(pool.left(), equals(component_size - 1));
    assert_that(pool.used(), equals(1));

    pool.remove(0);

    assert_that(pool.left(), equals(component_size));
    assert_that(pool.used(), equals(0));
}

TEST_F(ComponentTesting, loop_components)
{
    ComponentPool<SuperComp> pool(100, component_size);
    SuperComp* p    = pool.Add(0);
    SuperComp* p1   = pool.Add(1);

    p->value    = 1;
    p1->value   = 3;

    int sum = 0;

    for(SuperComp& comp : pool)
    {
        sum += comp.value;
    }

    assert_that(sum, equals(4));
}


TEST_F(ComponentTesting, remove_unorder)
{
    ComponentPool<SuperComp> pool(100, component_size);
    SuperComp * c1 = pool.Add(0);
    SuperComp * c2 = pool.Add(1);
    SuperComp * c3 = pool.Add(2);

    c1->value = 1;
    c2->value = 2;
    c3->value = 3;

    assert_that(pool.left(), equals(component_size - 3));
    assert_that(pool.used(), equals(3));

    pool.remove(1);

    assert_that(pool.left(), equals(component_size-2));
    assert_that(pool.used(), equals(2));

    int sum = 0;

    for (SuperComp& comp : pool)
    {
        sum += comp.value;
    }

    assert_that(sum, equals(4));
}

