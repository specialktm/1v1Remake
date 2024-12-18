#pragma once
#include "../pch/pch.h"

#include "../util/logger/logger.hpp"
#include "../fiber/Fiber.h"
#include "../fiber/Manager.h"
#include "../fiber/Queue.h"

namespace cheat
{
	unsigned long main_entry(void*);
	inline bool hookedDx11 = false;
}