#include "setlib.h"

// assumes integer elements
void set_print(Set* set) {
	printf("Number of elements: %d\nMaximum allocated: %d\n",
		set -> num_elems, set -> max_alloc);
	for (int i = 0; i < set -> num_elems; i++)
		printf("Set element: %d\n", (int) set -> elems[i]);
}

int list_contains(void** list, int length, void* value) {
	for (int i = 0; i < length; i++) {
		if (list[i] == value) return 1;
	}
	return 0;
}

Set* set_of_list(void** list, int length) {
	void** unique = malloc(length * sizeof(void*));
	int unique_length = 0;

	for (int i = 0; i < length; i++) {
		void* curr = list[i];
		if (!list_contains(unique, unique_length + 1, curr))
			unique[unique_length++] = curr;
	}
	Set* set = malloc(sizeof(Set));
	set -> num_elems = unique_length;
	set -> max_alloc = unique_length;
	set -> elems = realloc(unique, unique_length * sizeof(void*));
	return set;
}

Set* set_init(int init_length, ...) {
	va_list init_args;
	va_start(init_args, init_length);
	void** init_args_as_list = malloc(init_length * sizeof(void*));
	for (int i = 0; i < init_length; i++)
		init_args_as_list[i] = va_arg(init_args, void*);
	va_end(init_args);
	return set_of_list(init_args_as_list, init_length);
}

void set_delete(Set* set) {
	free(set -> elems);
	free(set);
}

void set_add(Set* set, void* value) {
	if (!set_contains(set, value)) {
		if (set -> num_elems++ == set -> max_alloc)
			set -> elems = realloc(set -> elems, ++set -> max_alloc * sizeof(void*));
		set -> elems[set -> num_elems - 1] = value;
	}
}

void set_remove(Set* set, void* value) {
	if (set -> num_elems == 0) {
		fprintf(stderr, "Cannot remove from an empty set.\n");
		return;
	}

	for (int i = 0; i < set -> num_elems; i++) {
		if (set -> elems[i] == value) {

			void** new_elems = malloc(--set -> num_elems * sizeof(void*));

			for (int j = 0; j < set -> num_elems + 1; j++) {
				void* orig = set -> elems[j];
				if (j < i) new_elems[j] = orig;
				else if (j > i) new_elems[j - 1] = orig;
			}
			set -> elems = new_elems;
			break;
		}
	}
}

void set_clear(Set* set) {
	for (int i = 0; i < set -> num_elems; i++)
		set -> elems[i] = 0;

	set -> num_elems = 0;
}

// all of the elements in either s1 or s2
Set* set_union(Set* s1, Set* s2) {
	int with_repeat_length = s1 -> num_elems + s2 -> num_elems;
	void** with_repeats = malloc(with_repeat_length * sizeof(void*));
	memcpy(with_repeats, s1 -> elems, s1 -> num_elems * sizeof(void*));
	memcpy(with_repeats + s1 -> num_elems, s2 -> elems, s2 -> num_elems * sizeof(void*));
	return set_of_list(with_repeats, with_repeat_length);
}

Set* _set_intersect_or_diff(Set* s1, Set* s2, int find_intersection) {
	Set* collection = set_init(0);
	for (int i = 0; i < s1 -> num_elems; i++) {
		void* s1_elem = s1 -> elems[i];
		int contains = set_contains(s2, s1_elem);
		if (contains && find_intersection || !contains && !find_intersection)
			set_add(collection, s1_elem);
	}
	return collection;
}

int sets_equal(Set* s1, Set* s2) {
	if (s1 -> num_elems != s2 -> num_elems) return 0;
	for (int i = 0; i < s1 -> num_elems; i++) {
		if (s1 -> elems[i] != s2 -> elems[i]) return 0;
	}
	return 1;
}

Set* set_map(void* (*f)(void*), Set* s) {
	Set* copy_s = set_of_list(s -> elems, s -> num_elems);
	for (int i = 0; i < copy_s -> num_elems; i++)
		copy_s -> elems[i] = f(copy_s -> elems[i]);
	return copy_s;
}

Set* set_filter(int (*f)(void*), Set* s) {
	Set* filtered = set_init(0);
	for (int i = 0; i < s -> num_elems; i++) {
		void* elem = s -> elems[i];
		if (f(elem)) set_add(filtered, elem);
	}
	return filtered;
}

void* _set_foldr(void* (*f) (void*, void*), void* seed, void** elems, int length) {
	if (length == 0) return seed;
	return f(elems[0], _set_foldr(f, seed, elems + 1, length - 1));
}

void* _set_foldl(void* (*f) (void*, void*), void* seed, void** elems, int length) {
	if (length == 0) return seed;
	return _set_foldl(f, f(seed, elems[0]), elems + 1, length - 1);
}