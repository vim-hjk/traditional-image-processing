#include <stdio.h>

void Quicksort(int *num_list, int start, int end) {
	if (start >= end) {
		return;
	}
	int pivot, left_idx, right_idx, temp;
	pivot = start;
	left_idx = start + 1;
	right_idx = end;
	while (left_idx <= right_idx) {
		while (left_idx <= end && num_list[left_idx] <= num_list[pivot]) {
			left_idx += 1;
		}
		while (right_idx > start && num_list[right_idx] >= num_list[pivot]) {
			right_idx -= 1;
		}
		if (left_idx > right_idx) {
			temp = num_list[right_idx];
			num_list[right_idx] = num_list[pivot];
			num_list[pivot] = temp;
		}
		else {
			temp = num_list[left_idx];
			num_list[left_idx] = num_list[right_idx];
			num_list[right_idx] = temp;
		}
	}
	Quicksort(num_list, start, right_idx - 1);
	Quicksort(num_list, right_idx + 1, end);
}

void main() {
	int i;
	int num_list[10] = { 10, 5, 7, 8, 4, 9, 3, 6, 1, 2 };
	Quicksort(num_list, 0, 9);

	for (i = 0; i < 10; i++) {
		printf("%d ", num_list[i]);
	}
}