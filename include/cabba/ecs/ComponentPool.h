#pragma once

namespace cabba
{
    class AbstractPool
    {
        public:

        virtual ~AbstractPool(){}
        virtual void remove(const int id) = 0;
        virtual bool exist(const int entityId)const = 0;
    };

    /*
     * @brief   The goal of the component pool is to store components into
     *          a tightly packed array for fast iteration. 
     *          
     *          When creating a new pool, you need to specify how many entities
     *          your game will contain, as well as how many component 
     *          you expect the pool to store. 
     *          
     *          It is recommended that the amount of entity is the same as the
     *          amount of component, but if you know in advance how many 
     *          component of a type T you will have, you have the option to 
     *          make a smaller pool and thus reduce memory usage.
     *          
     *          Components are stored inside a "tight" array, meanings the 
     *          pool will always make sure that data locality is optimal
     *          to achieve the best iterating time possible. Thus, components
     *          inside the array are not stored in any particular order.
     *          
     *          To retrieve a particular component from an Entity, a
     *          look_up table is created. This table is as big as the number
     *          of max entities as you can get, and its index correspond
     *          to the entities index. The value correspond to the position
     *          of the associated component inside the array. If no component
     *          was associated to the entity, the value stored is -1.
     *          
     *          Also, the pool is not concerned about keeping references/
     *          pointers valid (Some pointers will get invalidated after a 
     *          remove operation), so don't try to store a reference or pointer
     *          to a specific component in your code
     */
    template<class T>
    class ComponentPool : public AbstractPool
    {
    public:

        /*
         * @brief   Tracks the component associated to an EntityID. Use 
         *          the -> operator to access the component. And to make sure
         *          the entityId is still associated to a component, use the 
         *          valid function.
         *          
         *          /!\ Don't take ownership of the pointer returner by the 
         *          -> operator, since it can gets invalidated by the pool
         * 
         *          When asking the pool for a component, a Handle of type T
         *          is provided, instead of pointer. The reason for that is 
         *          because the pool can invalidate pointers. 
         *          
         *          Thus, the handle keeps a reference to the pool and doesn't
         *          directly track the pointer, but the EntityID instead, and
         *          fetch the right pointer each time the user ask for it.
         *          
         *          Use the valid function to check if the component is still
         *          associated with the entityId
         *          
         * 
         */
        class Handle
        {
        public:

            friend class ComponentPool;

            /*
             * @brief   Returns a pointer to the component associated 
             *          with the handle's entity id
             * 
             * @return  Returns the pointer to the component if it exist. 
             *          Returns nullptr otherwise
             */
            T* operator->()
            {
                // Here we make sure there's still a component linked to
                // the entity
                int pos = _pool->_entity_lookup_table[_entity_id];

                if (pos == -1)
                    return nullptr;

                return &_pool->_components[pos];
            }

            /*
             * @brief   Checks if the handle's entity id is still associated
             *          with a component
             *          
             * @return  Returns true if the handle's entityID is associated
             *          with a component. Return false otherwise
             */
            bool valid()const
            {
                return _pool->_entity_lookup_table[_entity_id]!=-1;
            }

        private:

            Handle(ComponentPool* pool, int entity_id)
                : _pool(pool), _entity_id(entity_id){}

            ComponentPool * _pool;
            int _entity_id;

        };

        friend class Handle;

        /*
         * @brief   When constructing the Component pool, we need to know
         *          how many entity there is to build our lookup table,
         *          but we can chose to allocate "less" component if we 
         *          know only a few will actually be used
         */
        ComponentPool(const int entity_size, const int component_size)
            :   _entity_lookup_table    (new int[entity_size]),
                _component_lookup_table (new int[component_size]),
                _components             (new T[component_size]),
                _size                   (component_size)
        {
            for(int i=0; i< entity_size ; ++i)
            {
                _entity_lookup_table[i]     = -1;
            }

            for (int i = 0; i < component_size; ++i)
            {
                _component_lookup_table[i] = -1;
            }
        }

        ~ComponentPool()
        {
            delete[] _components;
            delete[] _entity_lookup_table;
            delete[] _component_lookup_table;
        }

        // For now all of this is deleted because I don't really
        // think I'll copy the thing around once it's created-
        ComponentPool(const ComponentPool&)     = delete;
        ComponentPool(const ComponentPool&&)    = delete;
        ComponentPool& operator=(const ComponentPool&) = delete;
        ComponentPool& operator=(const ComponentPool&&) = delete;

        /*
         * @brief   Returns the number of element currently used from the pool
         * 
         * @return  The number of element used in the pool
         */
        int used() const
        {
            return _used;
        }

        /*
         * @brief   Returns the number of element that are left to be used 
         *          in the pool
         *          
         * @return  The number of element left in the pool
         */
        int left()const
        {
            return _size - _used;
        }

        /*
         * @brief   Associates a component to an Entity 
         * 
         * @param   id  Id to the Entity that will be associated
         */
        void add(const int id)
        {
            _component_lookup_table[_used]  = id;
            _entity_lookup_table[id]        = _used++;
        }

        /*
         * @brief   Checks if a component is already associated with the given 
         *          entityId parameter
         *
         *          No out of bound check are made on the entity id 
         *
         * @param   entityId    Entity's identifier 
         *
         * @return  Returns true if the given entityId parameter is associated
         *          with a component. Return false otherwise
         */
        bool exist(const int entityId)const override
        {
            return _entity_lookup_table[entityId] != -1;
        }
        /*
         * @brief   Returns an handle to the component associated with the
         *          given EntityId
         *          
         * @param   id  Entity's identifier
         * 
         * @return  Returns an Handle to the associated component
         */
        Handle get(const int id)
        {
            return Handle(this, id);
        }

        /*
         * @brief   Directly returns the pointer associated with the given 
         *          entity. WARNING : the pool can invalidate pointers so 
         *          don't cache them, and be careful with it!! Also,
         *          no checks are made, the function is totally unsafe to use
         *          
         * @return  Returns a pointer to the component associated with the
         *          given id. Undefined behavior if the entityId is not
         *          associated with a component.
         */
        T* get_raw(const int id)
        {
            return &_components[_entity_lookup_table[id]];
        }

        /*
         * @brief   Removes the component associated to the given entity id
         * 
         *          What we're actually doing here is copying the content of
         *          the last item inside the pool to the 
         *          
         *          TODO : Makes this more sane looking
         */
        void remove(const int entityId) override
        {
            _used--;

            // We first copy the last element to the removed element

            int pos_component_associated = _entity_lookup_table[entityId];
            _components[pos_component_associated] = _components[_used];

            // We then update the component lookup table

            _component_lookup_table[pos_component_associated] =
                _component_lookup_table[_used];

            _component_lookup_table[_used] = -1;

            // And now we update the entity lookup table

            if (_used != 0) // avoid writing outside of the array' limits
            {               // when we remove the last element, since in
                            // that situation, the remove element and the
                            // moved one are the same 
                _entity_lookup_table[
                    _component_lookup_table[pos_component_associated]] =
                    _entity_lookup_table[entityId];
            }

            _entity_lookup_table[entityId]  = -1;
        }

        T* begin()  {return &_components[0];}
        T* end()    {return &_components[_used];}

    private:

        /* 
         * @brief   This lookup table associate the entityId to the 
         *          component's position inside the array
         *
         *  EntityId            ComponentPosition
         *      0                       1
         *      1                       -1
         *      2                       11
         */
        int * _entity_lookup_table;

        /*
         * @brief   This lookup table associate a component position inside 
         *          the array to an EntityId
         */
        int * _component_lookup_table;

        /*
         * @brief   Stores the pool's components
         */
        T * _components;

        /*
         * @brief   Stores the amount of element currently associated with an Entity
         */
        int _used = 0;

        /*
         * @brief   Stores the maximum number of element the pool can hold
         */
        int _size;
    };
}