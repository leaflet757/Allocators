#include <cstdio>
#include <stdlib.h>

#include <Allocators.h>

int main(char** argv, int argc)
{
	const int MEM_SIZE = 256;
	
	// -----------------------------------------------
	// ----- Stack alloc test -----
	//------------------------------------------------

	allocs::stack_allocator stack(MEM_SIZE);

	std::cout << "Begin Stack Allocation Test...." << std::endl 
		<< "-------------------------------" << std::endl;
	std::cout << "Stack Size is: " << stack.getStackSize() << std::endl;

	std::cout << "Filling all bytes..." << std::endl;
	for (int i = 0; i < MEM_SIZE; ++i)
	{
		stack.alloc(sizeof(char));
	}

	std::cout << "Stack Size is: " << stack.getStackSize() << std::endl;

	stack.clear();
	std::cout << "Clearing stack..." << stack.getStackSize() << std::endl;

	std::cout << "Stack Size is: " << stack.getStackSize() << std::endl;
	
	struct stuff {
		int one;
		int two;
	};

	struct stuff2
	{
		int one;
		int two;
		float three;
		stuff four;
	};

	int* intp = (int*)stack.alloc(sizeof(int));
	*intp = 10;

	std::cout << "Allocating int... " << std::endl;
	std::cout << "Stack Size is: " << stack.getStackSize() << std::endl;
	std::cout << "Stack Capacity is: " << stack.getStackCapacity() << std::endl;
	printf("Address of int*: %p\nValue of int*: %d\n", intp, *intp);
	
	float* fp = (float*)stack.alloc(sizeof(float));
	*fp = 1000.324f;

	std::cout << "Allocating float..." << std::endl;
	std::cout << "Stack Size is: " << stack.getStackSize() << std::endl;
	std::cout << "Stack Capacity is: " << stack.getStackCapacity() << std::endl;
	printf("Address of float*: %p\nValue of float*: %f\n", fp, *fp);

	stuff* thing = (stuff*)stack.alloc(sizeof(stuff));
	thing->one = 20;
	thing->two = 30;
	
	std::cout << "Allocating struct with sizeof(8)... " << std::endl;
	std::cout << "Stack Size is: " << stack.getStackSize() << std::endl;
	std::cout << "Stack Capacity is: " << stack.getStackCapacity() << std::endl;
	printf("Address of stuff*: %p\nValue of stuff* one: %d\nValue of stuff* two: %d\n", thing, thing->one, thing->two);

	std::cout << "Free-ing memory to float pointer..." << std::endl;
	stack.freeToMarker((allocs::Marker)fp);

	std::cout << "Stack Size is: " << stack.getStackSize() << std::endl;
	std::cout << "Stack Capacity is: " << stack.getStackCapacity() << std::endl;

	stuff2* thing2 = (stuff2*)stack.alloc(sizeof(stuff2));
	thing2->one = 40;
	thing2->two = 50;
	thing2->three = 60;
	thing2->four.one = 70;
	thing2->four.two = 75;

	std::cout << "Allocating struct with sizeof(20)... " << std::endl;
	std::cout << "Stack Size is: " << stack.getStackSize() << std::endl;
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
		de_stack.alloc(sizeof(char), i % 2);
	}

	std::cout << "Stack Size is: " << de_stack.getStackSize() << std::endl;

	de_stack.clear();
	std::cout << "Clearing stack..." << de_stack.getStackSize() << std::endl;


	intp = (int*)de_stack.alloc(sizeof(int), false);
	*intp = 10;

	std::cout << "Allocating int... " << std::endl;
	
	fp = (float*)de_stack.alloc(sizeof(float), false);
	*fp = 1000.324f;

	float * fp2 = (float*)de_stack.alloc(sizeof(float), false);
	*fp2 = 1000.324f;

	std::cout << "Allocating float..." << std::endl;
	std::cout << "de_stack Size is: " << de_stack.getStackSize() << std::endl;
	std::cout << "de_stack Capacity is: " << de_stack.getStackCapacity() << std::endl;
	printf("Address of int*: %p\nValue of int*: %d\n", intp, *intp);
	printf("Address of float*: %p\nValue of float*: %f\n", fp, *fp);

	thing = (stuff*)de_stack.alloc(sizeof(stuff));
	thing->one = 20;
	thing->two = 30;

	std::cout << "Allocating struct with sizeof(8)... " << std::endl;
	std::cout << "de_stack Size is: " << de_stack.getStackSize() << std::endl;
	std::cout << "de_stack Capacity is: " << de_stack.getStackCapacity() << std::endl;
	printf("Address of stuff*: %p\nValue of stuff* one: %d\nValue of stuff* two: %d\n", thing, thing->one, thing->two);

	std::cout << "Free-ing memory to float pointer..." << std::endl;
	de_stack.freeToMarker((allocs::Marker)fp, false);

	std::cout << "de_stack Size is: " << de_stack.getStackSize() << std::endl;
	std::cout << "de_stack Capacity is: " << de_stack.getStackCapacity() << std::endl;

	thing2 = (stuff2*)de_stack.alloc(sizeof(stuff2));
	thing2->one = 40;
	thing2->two = 50;
	thing2->three = 60;
	thing2->four.one = 70;
	thing2->four.two = 75;

	std::cout << "Allocating struct with sizeof(20)... " << std::endl;
	std::cout << "de_stack Size is: " << de_stack.getStackSize() << std::endl;
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

	system("pause");

	return 0;
}