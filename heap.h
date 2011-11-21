#ifndef _SORT_H
#define _SORT_H

#define define_heap_operation(my_type) \
void upheap_##my_type(my_type a[], void *s, int n) \
{ \
  int i = (n-1)/2; \
  while (n > 0 && heap_compare(n, i)) { \
    heap_swap(i,n);\
    n = i; \
    i = (n-1)/2; \
  } \
} \
void heapfy_##my_type(my_type a[], void *s, int i, int n) \
{ \
  int j; \
  while (i < n/2){ \
    j = 2*i+1; \
    if (j<n-1 && heap_compare((j+1), j)) j++; \
    if (heap_compare(i,j)) break; \
    heap_swap(i, j); \
    i = j; \
  } \
} \
void heap_sort_##my_type(my_type a[], void *s, int n) \
{ \
  int i; \
  for (i=1; i<n; i++) \
    upheap_##my_type (a, s, i); \
  for (i=n-1; i>0; i--){ \
    heap_swap(0, i); \
    heapfy_##my_type (a, s, 0, i); \
  } \
} \
my_type extract_first_##my_type(my_type a[], void *s, int n) \
{ \
  heap_swap(0, n-1); \
  heapfy_##my_type(a, s, 0, n-1); \
  return a[n-1]; \
} \
void heap_adjust_##my_type(my_type a[], void *s, int i, int n) \
{ \
  upheap_##my_type (a, s, i); \
  heapfy_##my_type (a, s, i, n); \
} \
void heap_remove_##my_type(my_type a[], void *s, int i, int n) \
{ \
    heap_swap(i, n-1); \
	 heap_adjust_##my_type(a, s, i, n-1); \
} \
void heap_create_##my_type(my_type a[], void *s, int n) \
{ \
  int i; \
  for (i=1; i<n; i++) \
    upheap_##my_type (a, s, i); \
}
 
#endif /* _SORT_H */
