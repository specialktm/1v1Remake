#pragma once
#include "../pch/pch.h"
#include "Fiber.h"

namespace cheat
{
    namespace queue
    {
        inline std::vector<std::shared_ptr<Fiber>> g_fibers;
        inline std::recursive_mutex g_tasks_mutex;
        inline std::deque<std::function<void()>> g_tasks;

        void add(std::function<void()> task);

        void initialize();
        void free();
        void tick();
        void run(std::size_t index);
    };

    template <typename F> void queue_task(F&& task)
    {
        queue::add(std::forward<F>(task));
    }
}