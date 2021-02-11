#include <stdio.h>

int main(void) {
	int start = 1;
	int end = 6;
	int d[7] = { 0, 1, 2, 3, 4, 5, 6 };
	for (int i = end; i != start; i = d[i]) {
		printf("%d", i);
		printf("\n");
	}
	return 0;
}