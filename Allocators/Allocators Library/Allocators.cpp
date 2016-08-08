#include "./Allocators.h"

allocs::stack_allocator::stack_allocator(const std::size_t stackSizeBytes) :
	m_stackSizeBytes(stackSizeBytes),
	m_sizeBytesRemaining(stackSizeBytes)
{
	// Create a new stack of given stackSize and allocate memory;
	void* stack_address = malloc(stackSizeBytes);

	// Set the stack marker and update the top of the stack.
	m_stackBottom = reinterpret_cast<Marker>(stack_address);
	m_next = m_stackBottom;
}

void* allocs::stack_allocator::alloc(unsigned int sizeBytes) throw (std::bad_alloc)
{
	// Check if there is enough memory to allocate
	// If not, throw an allocation error
	if (sizeBytes > m_sizeBytesRemaining)
	{
		std::bad_alloc exception;
		throw exception;
	}

	// Cast the top of the stack as an addressable chunk of memory.
	void* address = reinterpret_cast<void*>(m_next);

	// Update the top of the stack.
	m_next = m_next + sizeBytes;
	// Update the remaining bytes in the stack.
	m_sizeBytesRemaining -= sizeBytes;

	return address;
}

void allocs::stack_allocator::freeToMarker(allocs::Marker marker)
{
	// Find the difference between the given marker and the top of the stack.
	std::size_t difference = m_next - marker;
	
	// Move the top of the stack to the given marker.
	m_next = marker;

	// Update the remaining bytes in the stack.
	m_sizeBytesRemaining += difference;
}

void allocs::stack_allocator::clear()
{
	// Reset the top of the stack and reset any used bytes.
	m_next = m_stackBottom;
	m_sizeBytesRemaining = m_stackSizeBytes;
}