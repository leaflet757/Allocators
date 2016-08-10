#include "./Allocators.h"
// -------------------------------------------------------
// Stack Allocator ----------------
// -------------------------------------------------------

allocs::stack_allocator::stack_allocator(const std::size_t stackSizeBytes) :
	m_capacityBytes(stackSizeBytes),
	m_sizeBytesRemaining(stackSizeBytes)
{
	// Create a new stack of given stackSize and allocate memory;
	void* stack_address = malloc(stackSizeBytes);

	// Set the stack marker and update the top of the stack.
	m_stackBottom = reinterpret_cast<Marker>(stack_address);
	m_next = m_stackBottom;
}

allocs::stack_allocator::~stack_allocator()
{
	// free all allocated memory
	free(reinterpret_cast<void*>(m_stackBottom));
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
	m_sizeBytesRemaining = m_capacityBytes;
}

// -------------------------------------------------------
// Double Ended Stack Allocator -----------
// -------------------------------------------------------

allocs::de_stack_allocator::de_stack_allocator(const std::size_t stackSizeBytes) :
	m_capacityBytes(stackSizeBytes),
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

allocs::de_stack_allocator::~de_stack_allocator()
{
	// free all allocated memory
	free(reinterpret_cast<void*>(m_stackBottom));
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

void allocs::de_stack_allocator::freeToMarker(Marker marker, bool rollBottom) 
{
	std::size_t difference = 0;

	// Check if we want to roll back the top or bottom of the stack
	// NOTE: Assumes the user knows where the address was allocated.
	if (rollBottom)
	{
		// Find the difference between the given marker and the bottom of the stack.
		difference = m_nextBottom - marker;

		// Move the bottom of the stack to the given marker.
		m_nextBottom = marker;
	}
	else
	{
		// Find the difference between the given marker and the top of the stack.
		difference = marker - m_nextTop;

		// Move the top of the stack to the newly freed space.
		m_nextTop = m_nextTop + marker;
	}

	// Update the remaining bytes in the stack.
	m_sizeBytesRemaining += difference;
}

void allocs::de_stack_allocator::clear()
{
	// Reset the top/bottom of the stack and reset any used bytes.
	m_sizeBytesRemaining = m_capacityBytes;
	m_nextBottom = m_stackBottom;
	m_nextTop = m_stackTop;
}

// -------------------------------------------------------
// Pool Allocator -----------
// -------------------------------------------------------

allocs::pool_allocator::pool_allocator(const std::size_t poolSizeBytes, const std::size_t elementSizeBytes) :
	m_capacityBytes(poolSizeBytes),
	m_sizeBytesRemaining(poolSizeBytes),
	m_elementSize(elementSizeBytes),
	m_numElements(poolSizeBytes / elementSizeBytes)
{
	// Create a new stack of given stackSize and allocate memory;
	void* stack_address = malloc(poolSizeBytes);

	// Set the address location of the newly allocated pool.
	m_poolAddress = reinterpret_cast<Marker>(stack_address);
}

allocs::pool_allocator::~pool_allocator()
{

}

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
