#include <cabba/ecs/World.h>
#include <cabba/ecs/System.h>

#include <cabba/test/test.h>

using namespace cabba;
using namespace cabba::test;

class SystemUT : public cabba::test::Test
{
    public:

        World world{ 100 };
};

class System        : public SystemInterface
{
public:
    System() = default;
};
class OtherSystem   : public SystemInterface
{
public:
    OtherSystem() = default;
};

TEST_F(SystemUT, add_system)
{
    assert_that(world.system_size(), equals(0));
    assert_that(world.has_system<System>(), equals(false));
    world.add_system<System>();
    assert_that(world.has_system<System>(), equals(true));
    assert_that(world.system_size(), equals(1));
}

TEST_F(SystemUT, remove_system)
{
    world.add_system<System>();
    assert_that(world.has_system<System>(), equals(true));
    assert_that(world.system_size(), equals(1));

    world.remove_system<System>();

    assert_that(world.has_system<System>(), equals(false));
    assert_that(world.system_size(), equals(0));
}

TEST_F(SystemUT, clear)
{
    world.add_system<System>();
    world.add_system<OtherSystem>();

    assert_that(world.has_system<System>(), equals(true));
    assert_that(world.has_system<OtherSystem>(), equals(true));
    assert_that(world.system_size(), equals(2));

    world.clear_systems();

    assert_that(world.system_size(), equals(0));
    assert_that(world.has_system<System>(), equals(false));
    assert_that(world.has_system<OtherSystem>(), equals(false));
}