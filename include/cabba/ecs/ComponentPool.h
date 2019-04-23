#pragma once

#include <cabba/containers/List.h>

namespace cabba
{
	class AbstractPool
	{
		public:

		virtual ~AbstractPool(){}
        virtual void unregisterComponent(int id) {};
		virtual void stuff(){}
	};

	template<class T, int Size = 10000>
	class ComponentPool : public AbstractPool
	{
	public:
        
		T* Add(int id)
		{
			registeredComponents.add(id);
			return &components[id];
		}

        void unregisterComponent(int entityId) override
        {
            registeredComponents.remove(entityId);
        }

		T components[Size]{};
		List<int>	registeredComponents;
	};
}