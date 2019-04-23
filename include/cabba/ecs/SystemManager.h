#pragma once

#include <cabba/containers/List.h>

namespace cabba
{
    struct SystemInterface;
    class World;

    class SystemManager
    {
    public:

    // Lifecycle

		SystemManager(){}
		SystemManager(const SystemManager&) = delete;
        ~SystemManager();

    // Assignment Operators

		SystemManager& operator=(const SystemManager&) = delete;

    // Methods

        void Add(SystemInterface* system);

        void Clear();

        // Update every System and lambda registered inside the Manager
        void Update(World& world, float elapsedTime);

    private:

        List<SystemInterface*> systems;
    };
};