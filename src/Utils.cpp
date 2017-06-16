#include "Utils.h"



Utils::Utils() {
}


Utils::~Utils() {
}


float Utils::quicksort(float* a,int p,int q) {
	if (p < q) {
		int i = partition(a, p, q);
		quicksort(a,p, i - 1);
		quicksort(a,i + 1, q);
	}
	return 0;
}


int Utils::partition(float* a, int p, int q) {
	float t = a[p];
	int i = p;
	int j = q;
	while (i < j) {
		while (a[j] >= t && i < j) {
			j--;
		}
		while (a[i] <= t &&	i < j) {
			i++;
		}
		if (i < j) {
			float temp = a[i];
			a[i] = a[j];
			a[j] = temp;
		}
	}
	a[p] = a[i];
	a[i] = t;
	return i;
}
