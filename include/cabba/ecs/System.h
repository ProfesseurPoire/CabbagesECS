#pragma once

namespace cabba
{
class World;

/*!
 * @brief	A system is simply an update function
 *			Could probably be simplified even more
 */
class SystemInterface   
{
public:

    friend class SystemMap;

// Lifecycle

    SystemInterface() = default;
    virtual ~SystemInterface() = default;

    SystemInterface(const SystemInterface&) = default;
    SystemInterface(SystemInterface&&)      = default;

// Assignment Operators

    SystemInterface& operator=(const SystemInterface&)  = default;
    SystemInterface& operator=(SystemInterface&&)       = default;

// Virtual Functions

    virtual void update(World&, float elapsed_time) {};
};
}