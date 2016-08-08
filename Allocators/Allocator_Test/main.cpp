#include <cstdio>
#include <stdlib.h>

#include <Allocators.h>

int main(char** argv, int argc)
{
	allocs::stack_allocator stack(256);

	for (int i = 0; i < 256; ++i)
	{
		stack.alloc(sizeof(char));
	}

	std::cout << "Stack Size is: " << stack.getStackSize() << std::endl;

	stack.clear();

	std::cout << "Stack Size is: " << stack.getStackSize() << std::endl;
	
	struct stuff {
		int one;
		int two;
	};

	struct stuff2
	{
		int one;
		int two;
		int three;
		int four;
	};

	int* intp = (int*)stack.alloc(sizeof(int));
	*intp = 10;

	std::cout << "Stack Size is: " << stack.getStackSize() << std::endl;
	std::cout << "Stack Capacity is: " << stack.getStackCapacity() << std::endl;
	printf("Address of int*: %p\nValue of int*: %d\n", intp, *intp);
	
	float* fp = (float*)stack.alloc(sizeof(float));
	*fp = 1000.324f;

	std::cout << "Stack Size is: " << stack.getStackSize() << std::endl;
	std::cout << "Stack Capacity is: " << stack.getStackCapacity() << std::endl;
	printf("Address of float*: %p\nValue of float*: %f\n", fp, *fp);

	stuff* thing = (stuff*)stack.alloc(sizeof(stuff));
	thing->one = 20;
	thing->two = 30;

	std::cout << "Stack Size is: " << stack.getStackSize() << std::endl;
	std::cout << "Stack Capacity is: " << stack.getStackCapacity() << std::endl;
	printf("Address of stuff*: %p\nValue of stuff* one: %d\nValue of stuff* two: %d\n", thing, thing->one, thing->two);

	stack.freeToMarker((allocs::Marker)fp);

	std::cout << "Stack Size is: " << stack.getStackSize() << std::endl;
	std::cout << "Stack Capacity is: " << stack.getStackCapacity() << std::endl;

	stuff2* thing2 = (stuff2*)stack.alloc(sizeof(stuff2));
	thing2->one = 40;
	thing2->two = 50;
	thing2->three = 60;
	thing2->four= 70;

	std::cout << "Stack Size is: " << stack.getStackSize() << std::endl;
	std::cout << "Stack Capacity is: " << stack.getStackCapacity() << std::endl;
	printf("Address of stuff*: %p\nValue of stuff* three: %d\nValue of stuff* four: %d\n", thing2, thing2->three, thing2->four);



	system("pause");

	return 0;
}