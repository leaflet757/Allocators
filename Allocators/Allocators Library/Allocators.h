#pragma once

#include <iostream>
#include <new>

// TODO: write class summaries
//	All allocators have memory overhead

// TODO: all allocations need to return aligned addressses

// deallocate with adjustment header

namespace allocs
{
	/**
	* Checks if the given pointer is memory aligned based on the current hardware.
	* 
	* @param pointer Address to check.
	* @param n The alignment specified by TYPE.
	*/
	template <typename TYPE>
	inline bool is_aligned(TYPE * p, std::size_t n = alignof(TYPE))
	{
		return 0 == reinterpret_cast<uintptr_t>(p) % n;
	}

	/**
	* Aligns the address based in the given alignment by masking off log2(x) least 
	* significant bits from alignment-1, then adding that result to the address.
	*
	* @param address The address that will be changed to an aligned address.
	* @param alignment The alignment relative to the type of address.
	*/
	inline void* alignForward(void * address, uint16_t alignment) 
	{
		return (void*)((reinterpret_cast<uintptr_t>(address) 
			+ static_cast<uintptr_t>(alignment - 1)) & static_cast<uintptr_t>(~(alignment - 1)));
	}

	/**
	* Returns the number of bytes needed to align the address based on its alignment
	* and optional overhead header size.
	*
	* @param address The address that will be changed to an aligned address.
	* @param alignment The alignment relative to the type of address.
	* @param headerSize Option overhead header size specification.
	*/
	inline uint16_t alignForwardAdjustment(const void * address, uint16_t alignment, uint16_t headerSize = 0)
	{
		uint16_t adjustment = alignment 
			- (reinterpret_cast<uintptr_t>(address) & static_cast<uintptr_t>(alignment - 1));
		
		adjustment = adjustment == alignment ? 0 : adjustment;

		uint16_t requiredSpace = headerSize;

		if (adjustment < requiredSpace)
		{
			requiredSpace -= adjustment;
			adjustment += alignment * (requiredSpace / alignment);

			if (requiredSpace % alignment > 0) adjustment += alignment;
		}
	
		return adjustment;
	}

	/**
	* alloc_header: Used to keep track of memory alignment overhead when 
	* allocating varrying data types.
	*/
	struct alloc_header
	{
		char alignment = 0;
	};

	/**
	* Stack Marker: Represents allocated address space in the stack.
	*
	* You can only pop back to a Marker, not to arbitrary locations
	* within the stack.
	*/ 
	typedef std::uintptr_t Marker;

	/**
	* TODO:
	*/
	class stack_allocator
	{
	public:
		/** 
		* Constructs a stack_allocator with the given total size.
		* 
		* @param stackSizeBytes The number of bytes to pre-allocate
		* for this allocator.
		*/
		explicit stack_allocator(const std::size_t stackSizeBytes);

		/** 
		* Release all allocated memory.
		*/
		~stack_allocator();

		/**
		* Allocates a new block of data of given size based on the template
		* type. This data is then placed on the top of the stack.
		*/
		template <typename T> T* alloc()
		{
			// Get the size of the specified type.
			const std::size_t sizeBytesT = sizeof(T);

			const std::size_t adjustment = alignForwardAdjustment(reinterpret_cast<void*>(m_next), alignof(T));

			// Check if there is enough memory to allocate
			// If not, throw an allocation error
			if (sizeBytesT + adjustment > m_sizeBytesRemaining)
			{
				std::cerr << "Note enough memory in stack: " << this << "\nSize Remaining: " 
					<< m_sizeBytesRemaining << "\nSize Requested: " << sizeBytesT 
					<< std::endl;
				std::bad_alloc exception;
				throw exception;
			}

			// Cast the top of the stack as an addressable chunk of memory.
			void* address = reinterpret_cast<void*>(m_next + adjustment);

			// Update the top of the stack.
			m_next = m_next + sizeBytesT + adjustment;
			// Update the remaining bytes in the stack.
			m_sizeBytesRemaining -= (sizeBytesT + adjustment);

			return static_cast<T*>(address);
		}

		/**
		* Returns the Marker to the address the stack is stored.
		*/
		const void* getStackAddress() 
		{
			return reinterpret_cast<void*>(m_stackBottom); 
		}

		/** 
		* Returns a Marker to the top of the stack.
		*/
		const Marker getStackTop() { return m_next; }

		/**
		* Returns the total capacity of the stack in size of bytes.
		*/
		const std::size_t getStackCapacity() { return m_capacityBytes; }

		/**
		* Returns the total remaining bytes within the stack.
		*/
		const std::size_t getRemainingStackSize() { return m_sizeBytesRemaining; }

		/**
		* Rolls the stack back to the given marker. NOTE: The function does
		* not check if the marker is valid.
		* 
		* @param marker The designated address to pop the stack back too.
		*/
		void freeToMarker(Marker marker);

		/**
		* Clears the entire stack.
		*/
		void clear();

	private:
		const std::size_t m_capacityBytes;	//!< Size in bytes of the stack.
		std::size_t m_sizeBytesRemaining;	//!< Remaining bytes in the stack.
		Marker m_stackBottom;				//!< Stack Marker.
		Marker m_next;						//!< Top of the stack.

		stack_allocator();					//!< Do not allow default constructor
											//!< or copy constructors.
		stack_allocator(stack_allocator const &) = delete;
		stack_allocator& operator=(stack_allocator const &) = delete;
	};

	/**
	* TODO:
	*/
	class de_stack_allocator
	{
	public:
		/** 
		* Constructs a stack_allocator with the given total size.
		*
		* @param stackSizeBytes The number of bytes to pre-allocate for
		* this allocator.
		*/
		explicit de_stack_allocator(const std::size_t stackSizeBytes);

		/**
		* Releases all allocated memory and destroy this object
		*/
		~de_stack_allocator();

		/**
		* Allocates a new block of data of given size from the designated
		* fill direction of the stack. If the fillBottom parameter is
		* not passed, then this allocator functions as a regular stack
		* allocator.
		*
		* @param fillBottom True fills the allocator from bottom to top
		* while False fills the allocator top to bottom.
		*/
		template <typename T> T* alloc(bool fillBottom = true)
		{
			// Get the size of the specified type
			const std::size_t sizeBytesT = sizeof(T);

			const std::size_t adjustment = alignForwardAdjustment(reinterpret_cast<void*>(m_nextBottom), alignof(T)); // next bottom or top doens't matter

			const std::size_t adjustment2 = alignForwardAdjustment(reinterpret_cast<void*>(m_nextTop), alignof(T)); // next bottom or top doens't matter


			// Check if there is enough memory to allocate
			// If not, throw an allocation error
			if (sizeBytesT + adjustment > m_sizeBytesRemaining)
			{
				std::bad_alloc exception;
				throw exception;
			}

			// TODO: add alignment adjustment here

			void* address = 0; // init return address
			if (fillBottom)
			{
				// Cast the bottom of the stack as an addressable chunk of memory.
				address = reinterpret_cast<void*>(m_nextBottom + adjustment);

				// Update the Bottom of the stack.
				m_nextBottom = m_nextBottom + sizeBytesT + adjustment;

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

		/**
		* Returns the Marker to the address the stack is stored.
		*/
		const Marker getStackAddressMarker() { return m_stackBottom; }

		/**
		* Returns the Marker to last address avaiable in the stack.
		*/
		const Marker getTopStackAddressMarker() { return m_stackTop; }

		/** 
		* Returns the next free address Marker on the bottom of the stack.
		*/
		const Marker getStackBottom() { return m_nextBottom; }

		/**
		* Returns next free address Marker on the top of the stack.
		*/
		const Marker getStackTop() { return m_nextTop; }

		/**
		* Returns the total capacity of the stack in size of bytes.
		*/
		const std::size_t getStackCapacity() { return m_capacityBytes; }

		/** 
		* Returns the total remaining bytes within the stack.
		*/
		const std::size_t getRemainingStackSize() { return m_sizeBytesRemaining; }

		/**
		* Rolls the top or bottom of the stack back to the the given marker.
		* Rolls the bottom of the stack by default unless rollBottom is set
		* as false. NOTE: This function does not check the validity of the 
		* marker.
		* 
		* @param marker The designated marker to pop the stack back to.
		* @param rollBottom Specifies to pop the stack towards the bottom or
		* the top.
		*/
		void freeToMarker(Marker marker, bool rollBottom = true);

		/**
		* Clears the entire stack.
		*/
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