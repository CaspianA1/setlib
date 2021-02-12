#include "setlib.c"

int main() {
	Set* set1 = set_init(3, 1, 2, 3), *set2 = set_init(4, 2, 3, 4, 5);
	printf("%d\n", sets_equal(set1, set2));

	printf("Intersection:\n");
	set_print(set_intersection(set1, set2));
	printf("-----\nUnion:\n");
	set_print(set_union(set1, set2));

	set_delete(set1);
	set_delete(set2);
}