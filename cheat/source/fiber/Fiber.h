#pragma once
#include "../pch/pch.h"
#include "../util/util.h"

namespace cheat
{
    class Fiber
    {
    public:
        using Function = std::function<void()>;
        explicit Fiber(std::string name, Function function);

        ~Fiber();
        Fiber(const Fiber&) = delete;
        Fiber(Fiber&&) = delete;

        Fiber& operator=(const Fiber&) = delete;
        Fiber& operator=(Fiber&&) = delete;

        [[nodiscard]] bool is_ready() const;
        [[nodiscard]] bool is_finished() const;
        [[nodiscard]] std::string_view get_name() const;
        [[nodiscard]] std::uint32_t get_name_hash() const;

        void tick();
        void yield(std::optional<std::chrono::high_resolution_clock::duration> time = std::nullopt);

        static Fiber* get();
        static void ensure_thread_is_a_fiber();

    private:
        std::string m_name;
        Function m_function;

        void* m_fiber = nullptr;
        void* m_parent_fiber = nullptr;

        void fiber_proc();

        bool m_finished = false;
        std::optional<std::chrono::high_resolution_clock::time_point> m_yield_until;
    };
};