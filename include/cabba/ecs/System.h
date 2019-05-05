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

    virtual void update(World&, float elapsed_time) {};
};
}