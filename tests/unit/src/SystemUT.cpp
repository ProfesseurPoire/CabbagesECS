#include <cabba/ecs/World.h>
#include <cabba/ecs/System.h>

#include <cabba/test/test.h>

using namespace cabba;
using namespace cabba::test;

class SystemUT : public cabba::test::Test
{
    public:

        const int entity_size = 100;
        World world{ entity_size };
        SystemManager* system_manager = nullptr;

        void set_up()override
        {
            system_manager = &world.system_manager();
        }
};

class System        : public SystemInterface{};
class OtherSystem   : public SystemInterface {};

TEST_F(SystemUT, add_system)
{
    assert_that(system_manager->size(), equals(0));
    assert_that(system_manager->has<System>(), equals(false));
    system_manager->add<System>();
    assert_that(system_manager->has<System>(), equals(true));
    assert_that(system_manager->size(), equals(1));
}

TEST_F(SystemUT, remove_system)
{
    system_manager->add<System>();
    assert_that(system_manager->has<System>(), equals(true));
    assert_that(system_manager->size(), equals(1));

    system_manager->remove<System>();
    assert_that(system_manager->has<System>(), equals(false));
    assert_that(system_manager->size(), equals(0));
}

TEST_F(SystemUT, clear)
{
    system_manager->add<System>();
    system_manager->add<OtherSystem>();

    assert_that(system_manager->has<System>(), equals(true));
    assert_that(system_manager->has<OtherSystem>(), equals(true));
    assert_that(system_manager->size(), equals(2));

    system_manager->clear();

    assert_that(system_manager->size(), equals(0));
    assert_that(system_manager->empty(), equals(true));
    assert_that(system_manager->has<System>(), equals(false));
    assert_that(system_manager->has<OtherSystem>(), equals(false));
}