#include "SystemManager.h"
#include "system.h"

namespace cabba
{
    SystemManager::~SystemManager()
    {
        Clear();
    }

	void SystemManager::Clear()
	{
		for (auto* system : systems)
		{
			delete system;
		}

		systems.clear();
	}

    void SystemManager::Add(SystemInterface* system)
    {
        systems.add(system);
    }

	void SystemManager::Update(World& game, float elapsedTime)
	{
		for (auto* system : systems)
		{
			system->Update(game, elapsedTime);
		}
	}
}