#include "../pch/pch.h"
#include "Fiber.h"

namespace cheat
{
    Fiber::Fiber(std::string name, Function function)
        : m_name(std::move(name)), m_function(std::move(function))
    {
        m_fiber = CreateFiber(0, [](void* param) { reinterpret_cast<Fiber*>(param)->fiber_proc(); },this);
    }

    Fiber::~Fiber() 
    {
        if (m_fiber) {
            DeleteFiber(m_fiber);
        }
    }

    bool Fiber::is_ready() const
    {
        return !m_yield_until ||
            std::chrono::high_resolution_clock::now() >= *m_yield_until;
    }

    bool Fiber::is_finished() const 
    {
        return m_finished;
    }

    std::string_view Fiber::get_name() const { return m_name; }

    std::uint32_t Fiber::get_name_hash() const { return util::joaat(m_name); }

    void Fiber::tick() 
    {
        if (!m_finished)
        {
            m_parent_fiber = GetCurrentFiber();

            if (is_ready())
            {
                SwitchToFiber(m_fiber);
            }
        }
    }

    void Fiber::yield(std::optional<std::chrono::high_resolution_clock::duration> yield_time)
    {
        if (yield_time) {
            m_yield_until = std::chrono::high_resolution_clock::now() + *yield_time;
        }
        else {
            m_yield_until = std::nullopt;
        }

        SwitchToFiber(m_parent_fiber);
    }

    Fiber* Fiber::get() 
    {
        return reinterpret_cast<Fiber*>(GetFiberData());
    }

    void Fiber::ensure_thread_is_a_fiber() 
    {
        if (!IsThreadAFiber())
        {
            ConvertThreadToFiber(nullptr);
        }
    }

    void Fiber::fiber_proc() 
    {
        if (m_function) 
        {
            m_function();
        }

        m_finished = true;
        SwitchToFiber(m_parent_fiber);
    }

    void yield(std::optional<std::chrono::high_resolution_clock::duration> time)
    {
        Fiber::get()->yield(time);
    }
}