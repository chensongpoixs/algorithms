#include "csort.h"
#include "csearch.h"

void _sort()
{
	for (int i = 1; i <= 8; ++i)
	{
		int arr[] = { 45, 38, 26, 77, 128, 38, 25, 444, 61, 153, 9999, 1012, 43, 128 };
		switch (i)
		{
		case 1:
			insert_sort(arr, sizeof(arr) / sizeof(arr[0]));
			break;
		case 2:
			bubble_sort(arr, sizeof(arr) / sizeof(arr[0]));
			break;
		case 3:
			select_sort(arr, sizeof(arr) / sizeof(arr[0]));
			break;
		case 4:
			shell_sort(arr, sizeof(arr) / sizeof(arr[0]));
			break;
		case 5:
			quick_sort(arr, sizeof(arr) / sizeof(arr[0]));
			break;
		case 6:
			merge_sort(arr, sizeof(arr) / sizeof(arr[0]));
			break;
		case 7:
			heap_sort(arr, sizeof(arr) / sizeof(arr[0]));
			break;
		case 8:
			lst_radix_sort(arr, sizeof(arr) / sizeof(arr[0]), 10, 4);
			break;
		default:
			break;
		}
		print_array(arr, sizeof(arr) / sizeof(arr[0]));
	}	
}

void _search()
{
	double a[] = {1.2, 2.3, 3.4, 4.5, 5.6, 6.7, 7.8, 8.9};
	int n = sizeof(a) / sizeof(a[0]);
	std::cout << "The number of array a is : " << n << std::endl;

	int index = bianrysearch(a, n, 4.5);

	std::cout << "bianrySearch The index of 4.5 in array a is : " << index + 1 << std::endl;
	index = rebinarysearch(a, n, 7.8);

	std::cout << "rebinarysearch The index of 4.5 in array a is : " << index + 1 << std::endl;
}
int main(int argc, char *argv[])
{
	_sort();
	_search();
#if defined(_MSC_VER)
				system("pause");
#elif defined(__GNUC__)
				
#else
#pragma error "unknow platform!!!"
#endif
	
	return 0;
}