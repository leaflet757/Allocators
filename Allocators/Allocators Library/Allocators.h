#pragma once

#include <iostream>
#include <new>

namespace allocs
{
	// Stack Marker: Represents allocated address space in the stack.
	// You can only pop back to a Marker, not to arbitrary locations
	// within the stack.
	typedef unsigned int Marker;

	class stack_allocator
	{
	public:
		// Constructs a stack_allocator with the given total size.
		explicit stack_allocator(const std::size_t stackSizeBytes);

		// Free all allocated memory
		~stack_allocator();

		// Allocates a new block of data of given size from the top
		// of the stack.
		void* alloc(unsigned int sizeBytes);

		// Returns the Marker to the address the stack is stored.
		const Marker getStackAddressMarker() { return m_stackBottom; }

		// Returns a Marker to the top of the stack.
		const Marker getStackTop() { return m_next; }

		// Returns the total capacity of the stack in size of bytes.
		const std::size_t getStackCapacity() { return m_capacityBytes; }

		// Returns the total remaining bytes within the stack.
		const std::size_t getStackSize() { return m_sizeBytesRemaining; }

		// Rolls the stack back to the given marker.
		void freeToMarker(Marker marker);

		// Clears the entire stack.
		void clear();

	private:
		const std::size_t m_capacityBytes;	// Size in bytes of the stack.
		std::size_t m_sizeBytesRemaining;	// Remaining bytes in the stack.
		Marker m_stackBottom;				// Stack Marker.
		Marker m_next;						// Top of the stack.
	};

	class de_stack_allocator
	{
	public:
		// Constructs a stack_allocator with the given total size.
		explicit de_stack_allocator(const std::size_t stackSizeBytes);

		// Free all allocated memory and destroy this object
		~de_stack_allocator();

		// Allocates a new block of data of given size from the designated
		// fill direction of the stack. If the fillBottom parameter is
		// not passed, then this allocator functions as a regular stack
		// allocator.
		void* alloc(unsigned int sizeBytes, bool fillBottom = true);

		// Returns the Marker to the address the stack is stored.
		const Marker getStackAddressMarker() { return m_stackBottom; }

		// Returns the Marker to last address avaiable in the stack.
		const Marker getTopStackAddressMarker() { return m_stackTop; }

		// Returns the next free address Marker on the bottom of the stack.
		const Marker getStackBottom() { return m_nextBottom; }

		// Returns next free address Marker on the top of the stack.
		const Marker getStackTop() { return m_nextTop; }

		// Returns the total capacity of the stack in size of bytes.
		const std::size_t getStackCapacity() { return m_capacityBytes; }

		// Returns the total remaining bytes within the stack.
		const std::size_t getStackSize() { return m_sizeBytesRemaining; }

		// Rolls the top or bottom of the stack back to the the given marker.
		// Rolls the bottom of the stack by default unless rollBottom is set
		// as false.
		void freeToMarker(Marker marker, bool rollBottom = true);

		// Clears the entire stack.
		void clear();

	private:
		const std::size_t m_capacityBytes;	// Size in bytes of the stack.
		std::size_t m_sizeBytesRemaining;	// Remaining bytes in the stack.
		Marker m_stackBottom;				// Address of allocated stack.
		Marker m_nextBottom;				// Next Free Slot on Bottom of Stack.
		Marker m_stackTop;					// Stack Top Marker.
		Marker m_nextTop;					// Next Free Slot on Top of Stakc.
	};

	class pool_allocator
	{
	public:
		explicit pool_allocator(const std::size_t poolSizeBytes, 
								const std::size_t elementSizeBytes);
		~pool_allocator();
	private:
		Marker m_poolAddress;				// Location of the memory pool.
		const std::size_t m_capacityBytes;	// Number of total bytes allocated.
		std::size_t m_sizeBytesRemaining;	// Remaining Bytes in the pool.
		std::size_t m_elementSize;			// Number of Bytes for an element.
		const std::size_t m_numElements;	// Maximum number of elements in pool.
	};

	// TODO: alignedpool?

	class linear_allocator
	{

	};

	class scope_stack_allocator
	{

	};

	class aligned_allocator
	{

	};

	class double_buffer_allocator
	{

	};
}

// TODO: add check if freeToMarker is invalid