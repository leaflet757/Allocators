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
// Linear Allocator -----------
// -------------------------------------------------------

//allocs::linear_allocator::linear_allocator(const std::size_t sizeBytes) :
//	m_capacityBytes(sizeBytes),
//	m_sizeBytesRemaining(sizeBytes)
//{
//	// Create a new stack of given stackSize and allocate memory;
//	void* linear_address = malloc(sizeBytes);
//
//	// Set the stack marker and update the top/bottom of the stack.
//	m_startAddress = reinterpret_cast<Marker>(linear_address);
//
//	// Set the first free addreses
//	m_nextFreeAddress = m_startAddress;
//}
//
//allocs::linear_allocator::~linear_allocator()
//{
//	// release all allocateed memory
//	free(reinterpret_cast<void*>(m_startAddress));
//}
//
//void* allocs::linear_allocator::alloc(std::size_t sizeBytes)
//{
//	// Check if there is enough memory to allocate
//	// If not, throw an allocation error
//	if (sizeBytes > m_sizeBytesRemaining)
//	{
//		std::bad_alloc exception;
//		throw exception;
//	}
//
//	// Get the free address
//	void* freeAddress = reinterpret_cast<void*>(m_nextFreeAddress);
//
//
//
//	// Update the remaining bytes
//	m_sizeBytesRemaining -= sizeBytes;
//}
//
//void allocs::linear_allocator::clear()
//{
//	m_sizeBytesRemaining = m_capacityBytes;
//
//	m_nextFreeAddress = m_startAddress;
//}

// -------------------------------------------------------
// Free List Allocator -----------
// -------------------------------------------------------


// -------------------------------------------------------
// Pool Allocator -----------
// -------------------------------------------------------

allocs::pool_allocator::pool_allocator(const std::size_t elementSizeBytes, const std::size_t numElements) :
	m_capacityBytes(elementSizeBytes * numElements),
	m_sizeBytesRemaining(m_capacityBytes),
	m_elementSize(elementSizeBytes),
	m_maxElements(numElements),
	m_numElements(0)
{
	// Create a new stack of given stackSize and allocate memory;
	void* pool_address = malloc(m_capacityBytes);

	// Set the address location of the newly allocated pool.
	m_poolAddress = reinterpret_cast<Marker>(pool_address);

	// Set all the free elements in the free list
	clear();

#ifdef _DEBUG
	printf("The address of the pool is: %p\n", pool_address);
	for (int i = 0; i < 4; ++i)
	{
		printf("The value of address(%d) is: %p\n", m_freeList[i], m_freeList[i]);
	}
#endif
}

void allocs::pool_allocator::clear() 
{
	// Save all free addresses as a linked list in the pool
	m_freeList = reinterpret_cast<void**>(m_poolAddress);

	// Point to the head of the list
	void** p = m_freeList;
	
	// Iterate through the number of elements, setting each one to the next
	for (int i = 0; i < m_maxElements; ++i)
	{
		// Set the value of p to point to the next free address space
		*p = reinterpret_cast<void**>(reinterpret_cast<Marker>(p) + m_elementSize);
		
		// Set p to the address of the newly added address space.
		p = static_cast<void**>(*p);
	}
	// Set the last element in the list to point at nothing
	*p = nullptr;

	// Set the number of elements in the stack to zero
	// and reset the bytes remaining.
	m_numElements = 0;
	m_sizeBytesRemaining = m_capacityBytes;
}

allocs::pool_allocator::~pool_allocator()
{
	// free all allocated memeory
	free(reinterpret_cast<void*>(m_poolAddress));
}

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
