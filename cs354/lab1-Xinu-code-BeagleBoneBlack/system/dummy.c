#include <xinu.h>

void func1(int32 arg) {
	int i = 0; for (i = 0; i < 10000000; i++) {
		kprintf("func1 - %d\n", i);
	}
}

void func2(int32 arg) {
	int i = 0; for (i = 0; i < 10000000; i++) {
		kprintf("func2 - %d\n", i);
	}
}

void func3(int32 arg) {
	int i = 0; for (i = 0; i < 10000000; i++) {
		kprintf("func3 - %d\n", i);
	}
}

void func4(int32 arg) {
	int i = 0; for (i = 0; i < 10000000; i++) {
		kprintf("func4 - %d\n", i);
	}
}

void func5(int32 arg) {
	int i = 0; for (i = 0; i < 10000000; i++) {
		kprintf("func5 - %d\n", i);
	}
}