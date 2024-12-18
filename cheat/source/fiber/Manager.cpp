#include "../pch/pch.h"
#include "Manager.h"

namespace cheat::fiber_manager
{
    void add_fiber(std::string name, std::function<void()> function)
    {
        auto fiber = std::make_shared<Fiber>(name, function);
        add_fiber(fiber);
    }

    void add_fiber(std::shared_ptr<Fiber> fiber)
    {
        std::lock_guard lock(g_fibers_mutex);
        g_fibers.push_back(std::move(fiber));
    }

    bool remove_fiber(std::string_view name)
    {
        return remove_fiber(util::joaat(name));
    }

    bool remove_fiber(std::uint32_t hash)
    {
        std::lock_guard lock(g_fibers_mutex);
        for (auto it = g_fibers.begin(); it != g_fibers.end(); ++it)
        {
            const auto& fiber = *it;
            if (fiber->get_name_hash() == hash)
            {
                g_fibers.erase(it);
                return true;
            }
        }
        return false;
    }

    void remove_all_fibers()
    {
        std::lock_guard lock(g_fibers_mutex);
        g_fibers.clear();
    }

    void tick()
    {
        std::lock_guard lock(g_fibers_mutex);
        for (auto it = g_fibers.begin(); it != g_fibers.end();) 
        {
            const auto& fiber = *it;

            fiber->tick();
            if (fiber->is_finished())
            {
                it = g_fibers.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}