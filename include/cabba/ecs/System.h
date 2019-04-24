#pragma once

namespace cabba
{
class World;

/*!
 * @brief	A system is simply an update function
 *			Could probably be simplified even more
 */
struct SystemInterface
{
    virtual void Update(World&, float) {};
};

}