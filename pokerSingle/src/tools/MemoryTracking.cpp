
#include "MemoryTracking.h"

#if MEMORY_FLAG

MemoryTracking::MemoryTracking() 
	:m_file(), m_allocated(0), m_freed(0) {
	m_file.open(MEMORY_FILE_NAME, std::ios::out | std::ios::app);
	m_file << std::endl << "program started..." << std::endl << std::endl;
}

MemoryTracking::~MemoryTracking() {
	m_file << std::endl;
	m_file << "program terminated with " << current() << " unfreed bytes.";
	m_file << std::endl << std::endl;
	m_file.close();
}

void *operator new(size_t size) {
	MemoryTracking &instance = MemoryTracking::get();
	instance.m_allocated += size;
	instance.m_file << "allocating " << size << " bytes.";
	instance.m_file << " (overall personal bytes: ";
	instance.m_file << instance.current();
	instance.m_file << ")." << std::endl;
	return malloc(size);
}

void operator delete(void *memory, size_t size) {
	MemoryTracking &instance = MemoryTracking::get();
	instance.m_freed += size;
	instance.m_file << "freeing " << size << " bytes.";
	instance.m_file << " (overall personal bytes: ";
	instance.m_file << instance.current();
	instance.m_file << ")." << std::endl;
	free(memory);
}

void operator delete[](void *memory, size_t size) {
	MemoryTracking &instance = MemoryTracking::get();
	instance.m_freed += size;
	instance.m_file << "freeing " << size << " bytes.";
	instance.m_file << " (overall personal bytes: ";
	instance.m_file << instance.current();
	instance.m_file << ")." << std::endl;
	::delete[] memory;
}

#endif