#pragma once

#include <iostream>
#include <new>

// TODO: write class summaries
//	All allocators have memory overhead

namespace allocs
{
	// Stack Marker: Represents allocated address space in the stack.
	// You can only pop back to a Marker, not to arbitrary locations
	// within the stack.
	typedef unsigned int Marker;

	/*class base_allocator
	{
	public:
		~base_allocator() { }

	protected:
		base_allocator(const std::size_t sizeBytes) :
			m_allocatorCapacity(sizeBytes), 
			m_remainingBytes(sizeBytes), 
			m_numAllocations(0)
		{ }

		virtual void* alloc() = 0;

		virtual void free(void* pointer) = 0;

		void* m_allocatorAddresss;
		const std::size_t m_allocatorCapacity;
		std::size_t m_remainingBytes;
		std::size_t m_numAllocations;

	private:
		base_allocator(base_allocator const &) = delete;
		base_allocator& operator=(base_allocator const &) = delete;
	};*/

	// TODO: all allocations need to return aligned addressses

	class stack_allocator
	{
	public:
		// Constructs a stack_allocator with the given total size.
		explicit stack_allocator(const std::size_t stackSizeBytes);

		// Release all allocated memory
		~stack_allocator();

		// Allocates a new block of data of given size from the top
		// of the stack.
		template <typename T> T* alloc()
		{
			// Get the size of the specified type.
			const std::size_t sizeBytesT = sizeof(T);

			// Check if there is enough memory to allocate
			// If not, throw an allocation error
			if (sizeBytesT > m_sizeBytesRemaining)
			{
				std::bad_alloc exception;
				throw exception;
			}

			// Cast the top of the stack as an addressable chunk of memory.
			void* address = reinterpret_cast<void*>(m_next);

			// Update the top of the stack.
			m_next = m_next + sizeBytesT;
			// Update the remaining bytes in the stack.
			m_sizeBytesRemaining -= sizeBytesT;

			return static_cast<T*>(address);
		}

		// Returns the Marker to the address the stack is stored.
		const void* getStackAddress() 
		{
			return reinterpret_cast<void*>(m_stackBottom); 
		}

		// Returns a Marker to the top of the stack.
		const Marker getStackTop() { return m_next; }

		// Returns the total capacity of the stack in size of bytes.
		const std::size_t getStackCapacity() { return m_capacityBytes; }

		// Returns the total remaining bytes within the stack.
		const std::size_t getRemainingStackSize() { return m_sizeBytesRemaining; }

		// Rolls the stack back to the given marker.
		void freeToMarker(Marker marker);

		// Clears the entire stack.
		void clear();

	private:
		const std::size_t m_capacityBytes;	// Size in bytes of the stack.
		std::size_t m_sizeBytesRemaining;	// Remaining bytes in the stack.
		Marker m_stackBottom;				// Stack Marker.
		Marker m_next;						// Top of the stack.

		stack_allocator();					// Do not allow default constructor.
											// Or copy constructors.
		stack_allocator(stack_allocator const &) = delete;
		stack_allocator& operator=(stack_allocator const &) = delete;
	};

	class de_stack_allocator
	{
	public:
		// Constructs a stack_allocator with the given total size.
		explicit de_stack_allocator(const std::size_t stackSizeBytes);

		// Releases all allocated memory and destroy this object
		~de_stack_allocator();

		// Allocates a new block of data of given size from the designated
		// fill direction of the stack. If the fillBottom parameter is
		// not passed, then this allocator functions as a regular stack
		// allocator.
		template <typename T> T* alloc(bool fillBottom = true)
		{
			// Get the size of the specified type
			const std::size_t sizeBytesT = sizeof(T);

			// Check if there is enough memory to allocate
			// If not, throw an allocation error
			if (sizeBytesT > m_sizeBytesRemaining)
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
				m_nextBottom = m_nextBottom + sizeBytesT;

				// Update the remaining bytes in the stack.
				m_sizeBytesRemaining -= sizeBytesT;
			}
			else
			{
				// Update the Top of the stack.
				m_nextTop = m_nextTop - sizeBytesT;

				// Cast the bottom of the stack as an addressable chunk of memory.
				address = reinterpret_cast<void*>(m_nextTop);

				// Update the remaining bytes in the stack.
				m_sizeBytesRemaining -= sizeBytesT;
			}

			return static_cast<T*>(address);
		}

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
		const std::size_t getRemainingStackSize() { return m_sizeBytesRemaining; }

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

		de_stack_allocator();				// Do not allow default constructor.
											// Or copy constructors.
		de_stack_allocator(de_stack_allocator const &) = delete;
		de_stack_allocator& operator=(de_stack_allocator const &) = delete;
	};

	//class linear_allocator
	//{
	//public:
	//	
	//	explicit linear_allocator(const std::size_t sizeBytes);
	//	
	//	~linear_allocator();

	//	void* alloc(std::size_t sizeBytes);

	//	void clear();
	//
	//private:
	//	Marker m_startAddress; 
	//	const std::size_t m_capacityBytes;	// Size in bytes of the stack.
	//	std::size_t m_sizeBytesRemaining;	// Remaining bytes in the stack.
	//	Marker m_nextFreeAddress;
	//};

	//class free_list_allocator
	//{

	//};

	class pool_allocator
	{
	public:
		explicit pool_allocator(const std::size_t elementSizeBytes,
			const std::size_t numElements);

		// Allocates a new block of data from the memory pool.
		// The type specified must be the same size specified during
		// the pool's construction.
		template <typename T> T* alloc()
		{
			assert(sizeof(T) == m_elementSize);

			// Check if there is enough memory to allocate
			// If not, throw an allocation error
			if (sizeBytesT > m_sizeBytesRemaining)
			{
				std::bad_alloc exception;
				throw exception;
			}

			// Decrease the bytes remaining and update the element count
			m_sizeBytesRemaining -= m_elementSize;
			++m_numElements;

			// Get the first address from the list
			void* address = m_freeList;

			// Pop the first element off the list
			m_freeList = static_cast<void**>(*m_freeList);

			// Return the newly popped address
			return static_cast<T*>(address);
		}

		template <typename T> void free(T* pointer)
		{
			// Update the bytes remaining and element count
			--m_numElements;
			m_sizeBytesRemaining += m_elementSize;

			// Set the value of the specifide pointer to point to the
			// first free address in the list.
			*((void**)pointer) = m_freeList;

			// Set the pointer to the head of the list
			m_freeList = (void**)pointer;
		}

		// Returns the Marker to the address the stack is stored.
		const Marker getAddressMarker() { return m_poolAddress; }

		// Returns the total capacity of the stack in size of bytes.
		const std::size_t getCapacity() { return m_capacityBytes; }

		// Returns the total remaining bytes within the stack.
		const std::size_t getRemainingPoolSize() { return m_sizeBytesRemaining; }

		// Returns the maximum number of elements allowed in the pool.
		const std::size_t getMaxElements() { return m_maxElements; }

		// Returns the current number of elements in the pool.
		std::size_t getNumElements() { return m_numElements; }

		// Returns the size of each element in the pool.
		const std::size_t getElementSize() { return m_elementSize; }

		// Clears the entire stack.
		void clear();

		// Release all allocated memeory in the pool
		~pool_allocator();

	private:
		Marker m_poolAddress;				// Location of the memory pool.
		const std::size_t m_capacityBytes;	// Number of total bytes allocated.
		std::size_t m_sizeBytesRemaining;	// Remaining Bytes in the pool.
		const std::size_t m_elementSize;	// Number of Bytes for an element.
		const std::size_t m_maxElements;	// Maximum number of elements in pool.
		std::size_t m_numElements;			// Current number of elements.
		void** m_freeList;					// Elements that are available.

		pool_allocator();					// Do not allow default constructor.
											// Or copy constructors.
		pool_allocator(pool_allocator const &) = delete;
		pool_allocator& operator=(pool_allocator const &) = delete;
	};

	class free_list_allocator
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