#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define set_contains(set, value)\
	list_contains(set -> elems, set -> num_elems, (void*) value)

// all of the elements in s1 that are also in s2
#define set_intersection(s1, s2) _set_intersect_or_diff(s1, s2, 1)
// all of the elements in s1 that are not in s2
#define set_difference(s1, s2) _set_intersect_or_diff(s1, s2, 0)
#define set_is_subset(s1, s2) sets_equal(set_union(s1, s2), s2)

#define set_foldr(f, s, seed) _set_foldr(f, seed, s -> elems, s -> num_elems)
#define set_foldl(f, s, seed) _set_foldl(f, seed, s -> elems, s -> num_elems)

typedef struct {
	int num_elems, max_alloc;
	void** elems;
} Set;

void set_print(Set* set);
int list_contains(void** list, int length, void* value);
Set* set_of_list(void** list, int length);
Set* set_init(int init_length, ...);
void set_delete(Set* set);
void set_add(Set* set, void* value);
void set_remove(Set* set, void* value);
void set_clear(Set* set);
Set* set_union(Set* s1, Set* s2);
Set* _set_intersect_or_diff(Set* s1, Set* s2, int find_intersection);
int sets_equal(Set* s1, Set* s2);
Set* set_map(void* (*f)(void*), Set* s);
Set* set_filter(int (*f)(void*), Set* s);
void* _set_foldr(void* (*f) (void*, void*), void* seed, void** elems, int length);
void* _set_foldl(void* (*f) (void*, void*), void* seed, void** elems, int length);