#include <cstdio>
#include <stdlib.h>

#include <Allocators.h>

struct stuff {
	int one;
	int two;
	char letter;
};

struct stuff2
{
	stuff four;
	int one;
	int two;
	float three;
};

class SomeClass
{
public:
	SomeClass() : one(4) {}
	SomeClass(int param) : one(param) {}
	~SomeClass() {}
	void print()
	{
		printf("The value off %p is %d.", this, this->one);
	}
private:
	int one;
};

int main(char** argv, int argc)
{
	const int MEM_SIZE = 256;
	
	std::cout << "Printing sizeOf stuff(" << sizeof(stuff) << ")." << std::endl;
	std::cout << "Printing sizeOf stuff2(" << sizeof(stuff2) << ")." << std::endl;
	std::cout << "Printing sizeOf SomeClass(" << sizeof(SomeClass) << ")." << std::endl << std::endl;

	// -----------------------------------------------
	// ----- Stack alloc test -----
	//------------------------------------------------

	allocs::stack_allocator stack(MEM_SIZE);

	std::cout << "Begin Stack Allocation Test...." << std::endl 
		<< "-------------------------------" << std::endl;
	std::cout << "Stack Size is: " << stack.getRemainingStackSize() << std::endl;

	std::cout << "Filling all bytes..." << std::endl;
	for (int i = 0; i < MEM_SIZE; ++i)
	{
		stack.alloc<char>();
	}

	std::cout << "Stack Size is: " << stack.getRemainingStackSize() << std::endl;

	stack.clear();
	std::cout << "Clearing stack..." << stack.getRemainingStackSize() << std::endl;

	std::cout << "Stack Size is: " << stack.getRemainingStackSize() << std::endl;

	int* intp = stack.alloc<int>();
	*intp = 10;

	std::cout << "Allocating int... " << std::endl;
	std::cout << "Stack Size is: " << stack.getRemainingStackSize() << std::endl;
	std::cout << "Stack Capacity is: " << stack.getStackCapacity() << std::endl;
	printf("Address of int*: %p\nValue of int*: %d\n", intp, *intp);
	
	float* fp = stack.alloc<float>();
	*fp = 1000.324f;

	std::cout << "Allocating float..." << std::endl;
	std::cout << "Stack Size is: " << stack.getRemainingStackSize() << std::endl;
	std::cout << "Stack Capacity is: " << stack.getStackCapacity() << std::endl;
	printf("Address of float*: %p\nValue of float*: %f\n", fp, *fp);

	stuff* thing = stack.alloc<stuff>();
	thing->one = 20;
	thing->two = 30;
	
	std::cout << "Allocating struct with sizeof(12)... " << std::endl;
	std::cout << "Stack Size is: " << stack.getRemainingStackSize() << std::endl;
	std::cout << "Stack Capacity is: " << stack.getStackCapacity() << std::endl;
	printf("Address of stuff*: %p\nValue of stuff* one: %d\nValue of stuff* two: %d\n", thing, thing->one, thing->two);
	printf("Address of stuff*: %p\nAddress of stuff* one: %p\nAddress of stuff* two: %p\n", thing, &(thing->one), &(thing->two));

	std::cout << "Free-ing memory to float pointer..." << std::endl;
	stack.freeToMarker((allocs::Marker)fp);

	std::cout << "Stack Size is: " << stack.getRemainingStackSize() << std::endl;
	std::cout << "Stack Capacity is: " << stack.getStackCapacity() << std::endl;

	stuff2* thing2 = stack.alloc<stuff2>();
	thing2->one = 40;
	thing2->two = 50;
	thing2->three = 60;
	thing2->four.one = 70;
	thing2->four.two = 75;

	std::cout << "Allocating struct with sizeof(24)... " << std::endl;
	std::cout << "Stack Size is: " << stack.getRemainingStackSize() << std::endl;
	std::cout << "Stack Capacity is: " << stack.getStackCapacity() << std::endl;
	printf("Address of stuff*: %p\nValue of stuff* three: %f\nValue of stuff* four: %d\n", thing2, thing2->three, thing2->four.two);

	// -----------------------------------------------
	// ----- Stack alloc test -----
	//------------------------------------------------

	std::cout << "-------------------------------" << std::endl
		<< "Begin double stack allocation test..." << std::endl
		<< "---------------------------------" << std::endl;

	allocs::de_stack_allocator de_stack(MEM_SIZE);

	std::cout << "Filling all bytes..." << std::endl;
	for (int i = 0; i < MEM_SIZE; ++i)
	{
		de_stack.alloc<char>(i % 2);
	}

	std::cout << "Stack Size is: " << de_stack.getRemainingStackSize() << std::endl;

	de_stack.clear();
	std::cout << "Clearing stack..." << de_stack.getRemainingStackSize() << std::endl;


	intp = (int*)de_stack.alloc<int>(false);
	*intp = 10;

	std::cout << "Allocating int... " << std::endl;
	
	fp = (float*)de_stack.alloc<float>(false);
	*fp = 1000.324f;

	float * fp2 = de_stack.alloc<float>(false);
	*fp2 = 1000.324f;

	std::cout << "Allocating float..." << std::endl;
	std::cout << "de_stack Size is: " << de_stack.getRemainingStackSize() << std::endl;
	std::cout << "de_stack Capacity is: " << de_stack.getStackCapacity() << std::endl;
	printf("Address of int*: %p\nValue of int*: %d\n", intp, *intp);
	printf("Address of float*: %p\nValue of float*: %f\n", fp, *fp);

	thing = de_stack.alloc<stuff>();
	thing->one = 20;
	thing->two = 30;

	std::cout << "Allocating struct with sizeof(8)... " << std::endl;
	std::cout << "de_stack Size is: " << de_stack.getRemainingStackSize() << std::endl;
	std::cout << "de_stack Capacity is: " << de_stack.getStackCapacity() << std::endl;
	printf("Address of stuff*: %p\nValue of stuff* one: %d\nValue of stuff* two: %d\n", thing, thing->one, thing->two);

	std::cout << "Free-ing memory to float pointer..." << std::endl;
	de_stack.freeToMarker((allocs::Marker)fp, false);

	std::cout << "de_stack Size is: " << de_stack.getRemainingStackSize() << std::endl;
	std::cout << "de_stack Capacity is: " << de_stack.getStackCapacity() << std::endl;

	thing2 = de_stack.alloc<stuff2>();
	thing2->one = 40;
	thing2->two = 50;
	thing2->three = 60;
	thing2->four.one = 70;
	thing2->four.two = 75;

	std::cout << "Allocating struct with sizeof(20)... " << std::endl;
	std::cout << "de_stack Size is: " << de_stack.getRemainingStackSize() << std::endl;
	std::cout << "de_stack Capacity is: " << de_stack.getStackCapacity() << std::endl;
	printf("Address of stuff*: %p\nValue of stuff* three: %f\nValue of stuff* four.two: %d\n", thing2, thing2->three, thing2->four.two);

	// -----------------------------------------------
	// ----- Pool alloc test -----
	//------------------------------------------------

	std::cout << "-------------------------------" << std::endl
		<< "Begin Linear allocation test..." << std::endl
		<< "---------------------------------" << std::endl;

	// -----------------------------------------------
	// ----- Pool alloc test -----
	//------------------------------------------------

	std::cout << "-------------------------------" << std::endl
		<< "Begin Pool allocation test..." << std::endl
		<< "---------------------------------" << std::endl;



	allocs::pool_allocator pool(4, 4);

	system("pause");

	return 0;
}