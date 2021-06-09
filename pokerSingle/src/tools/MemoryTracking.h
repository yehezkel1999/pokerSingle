
#include "../general/Flags.h"

#if MEMORY_FLAG

#ifndef MEMORY_TRACKING_H_
#define MEMORY_TRACKING_H_

#include "../general/pch.h"

/**
 * Singleton struct as only one memory tracker (per program) is needed.
 */

struct MemoryTracking {
	std::ofstream m_file;
	std::uint32_t m_allocated;
	std::uint32_t m_freed;

	inline std::uint32_t current() noexcept { return m_allocated - m_freed; }

	MemoryTracking(const MemoryTracking &) = delete;
	MemoryTracking &operator=(const MemoryTracking &) = delete;

	static inline MemoryTracking &get() noexcept {
		static MemoryTracking instance;
		return instance;
	}

private:
	MemoryTracking();
	~MemoryTracking();
};


// operator new[] calls the overloaded new operator so there is no reason to overload it.

extern void *operator new(size_t size);

extern void operator delete(void *memory, size_t size);

extern void operator delete[](void *memory, size_t size);


#endif // !MEMORY_TRACKING_H_
#endif // MEMORY_FLAG