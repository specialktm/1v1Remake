#include "../pch/pch.h"
#include "Queue.h"

namespace cheat::queue
{
    constexpr std::size_t FIBER_POOL_SIZE = 15;

    void add(std::function<void()> task)
    {
        std::lock_guard lock(g_tasks_mutex);
        g_tasks.push_back(std::move(task));
    }

    void initialize() 
    {
        g_fibers.reserve(FIBER_POOL_SIZE);

        for (std::size_t i = 0; i < FIBER_POOL_SIZE; ++i)
        {
            auto fiber = std::make_shared<Fiber>(std::format("Fiber {}", i), [i]
            {
                run(i);
            });

            g_fibers.push_back(std::move(fiber));
        }
    }

    void free()
    {
        g_fibers = {};
    }

    void tick()
    {
        for (auto& fiber : g_fibers)
        {
            fiber->tick();
        }
    }

    void run(std::size_t index)
    {
        while (true) 
        {
            decltype(g_tasks)::value_type task;
            {
                std::lock_guard lock(g_tasks_mutex);
                if (!g_tasks.empty())
                {
                    task = std::move(g_tasks.front());
                    g_tasks.pop_front();
                }
            }

            if (task) 
            {
                std::invoke(task);
            }

            Fiber::get()->yield();
        }
    }
}