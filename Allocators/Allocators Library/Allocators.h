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

		// Allocates a new block of data of given size from the top
		// of the stack.
		void* alloc(unsigned int sizeBytes);

		// Returns the Marker to the address the stack is stored.
		const Marker getStackMarker() { return m_stackBottom; }

		// Returns a Marker to the top of the stack.
		const Marker getStackTop() { return m_next; }

		// Returns the total capacity of the stack in size of bytes.
		const std::size_t getStackCapacity() { return m_stackSizeBytes; }

		// Returns the total remaining bytes within the stack.
		const std::size_t getStackSize() { return m_sizeBytesRemaining; }

		// Rolls the stack back to the given marker.
		void freeToMarker(Marker marker);

		// Clears the entire stack.
		void clear();

	private:
		std::size_t m_stackSizeBytes;		// Size in bytes of the stack.
		std::size_t m_sizeBytesRemaining;	// Remaining bytes in the stack.
		Marker m_stackBottom;				// Stack Marker.
		Marker m_next;						// Top of the stack.
	};

	class de_stack_allocator
	{

	};

	class pool_allocator
	{

	};

	class aligned_allocator
	{

	};

	class double_buffer_allocator
	{

	};
}