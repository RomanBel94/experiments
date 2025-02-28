#include<iostream>

class Dummy
{
	int val{0};

public:
	Dummy(int val) : val(val)
	{
		std::cout << "Dummy created with val: " << val << "\n";
	}

	~Dummy()
	{
		std::cout << "Dummy destroyed\n";
	}
};

int main()
{
	alignas(Dummy) unsigned char buf[sizeof(Dummy)];
	std::cout << "Buffer created with size: " << sizeof(Dummy) << '\n';

	Dummy* ptr{ nullptr };

	ptr = new(buf) Dummy(5);
	ptr->~Dummy();
	ptr = nullptr;

	ptr = new(buf) Dummy(50);
	ptr->~Dummy();
	ptr = nullptr;

	std::cout << "End of main()\n";

	return 0;
}