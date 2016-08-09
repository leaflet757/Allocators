#include "./Allocators.h"
// -------------------------------------------------------
// Stack Allocator ----------------
// -------------------------------------------------------

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

// -------------------------------------------------------
// Double Ended Stack Allocator -----------
// -------------------------------------------------------

allocs::de_stack_allocator::de_stack_allocator(const std::size_t stackSizeBytes) :
	m_stackSizeBytes(stackSizeBytes),
	m_sizeBytesRemaining(stackSizeBytes)
{
	// Create a new stack of given stackSize and allocate memory;
	void* stack_address = malloc(stackSizeBytes);

	// Set the stack marker and update the top/bottom of the stack.
	m_stackBottom = reinterpret_cast<Marker>(stack_address);
	m_nextBottom = m_stackBottom;
	m_stackTop = m_stackBottom + stackSizeBytes;
	m_nextTop = m_stackTop;
}

void* allocs::de_stack_allocator::alloc(unsigned int sizeBytes, bool fillBottom)
{
	// Check if there is enough memory to allocate
	// If not, throw an allocation error
	if (sizeBytes > m_sizeBytesRemaining)
	{
		std::bad_alloc exception;
		throw exception;
	}

	void* address = 0; // init return address
	if (fillBottom)
	{
		// Cast the bottom of the stack as an addressable chunk of memory.
		address = reinterpret_cast<void*>(m_nextBottom);

		// Update the Bottom of the stack.
		m_nextBottom = m_nextBottom + sizeBytes;

		// Update the remaining bytes in the stack.
		m_sizeBytesRemaining -= sizeBytes;
	}
	else
	{
		// Update the Top of the stack.
		m_nextTop = m_nextTop - sizeBytes;

		// Cast the bottom of the stack as an addressable chunk of memory.
		address = reinterpret_cast<void*>(m_nextTop);

		// Update the remaining bytes in the stack.
		m_sizeBytesRemaining -= sizeBytes;
	}
	
	return address;
}

void allocs::de_stack_allocator::clear()
{
	// Reset the top/bottom of the stack and reset any used bytes.
	m_sizeBytesRemaining = m_stackSizeBytes;
	m_nextBottom = m_stackBottom;
	m_nextTop = m_stackTop;
}

// -------------------------------------------------------
// Pool Allocator -----------
// -------------------------------------------------------

// -------------------------------------------------------
// Linear Allocator -----------
// -------------------------------------------------------


// -------------------------------------------------------
// Scope Stack Allocator -----------
// -------------------------------------------------------

// -------------------------------------------------------
// Aligned Allocator -----------
// -------------------------------------------------------

// -------------------------------------------------------
// Double Buffer Allocator -----------
// -------------------------------------------------------

// -------------------------------------------------------
