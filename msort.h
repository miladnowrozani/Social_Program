#include <vector>
using namespace std;


// Prototype functions
template <class T, class Comparator>
void msort(vector<T>& myList, vector<T>& output, int start, int end, Comparator comp);

template <class T, class Comparator>
void merge(vector<T>& other, vector<T>& output, int s1, int e1, int s2, int e2, Comparator comp);

// Main function
template <class T, class Comparator>
void mergeSort(vector<T>& myList, Comparator comp)
{
	vector<T> other(myList);
	int end = (int)(myList.size()) - 1;
	msort(other, myList, 0, end, comp);
}

// Implemention of msort
template <class T, class Comparator>
void msort(vector<T>& myList, vector<T>& output, int start, int end, Comparator comp)
{
	if (start < end)
	{
		int mid = (start + end)/2;
		msort(myList, output, start, mid, comp);
		msort(myList, output, mid + 1, end, comp);
		merge(myList, output, start, mid, mid+1, end, comp);		
	}
	else
	{
		return;
	}
}

// Implementation of merge
template <class T, class Comparator>
void merge(vector<T>& input, vector<T>& output, int s1, int e1, int s2, int e2, Comparator comp)
{
	for (int i = s1; i <= e2; i++)
	{
		if (s1 > e1)
		{
			output[i] = input[s2];
			s2++;
			continue;			
		}
		if (s2 > e2)
		{
			output[i] = input[s1];
			s1++;
			continue;
		}
		if (comp(input[s1], input[s2]))
		{
			output[i] = input[s1];
			s1++;
		}
		else
		{
			output[i] = input[s2];
			s2++;
		}
	}

	input = output;
}