#include <stdexcept>	//for exception classes
#include <iostream>




template <typename E>
class List
{
	virtual int search(const E& elem) = 0;		//searches for key and returns index if found, or some out of bounds integer to 
					//signal not found
	virtual E& getElem(const E& elem) = 0;		//getter using key (likely embeds search)
	virtual E& getElemByIndex(const int& index) = 0;	//getter using index (presumably from search)

	virtual void insert(const E& elem) = 0;	//inserts an element with a designated key (no duplicates)
	virtual void remove(const E& elem) = 0;	//deletes element with given key, does nothing if no such element exists		
};

template <typename E>
class TransposeList : public List<E>
	//This list is a self-organizing list using the "transpose" heuristic. Searching will be done using sequential search (which
	//should be quick if the data follows a Zipf distribution.
	//Whatever is stored in the list must be able to be compared for equality for searching purposes.
{
public:
	TransposeList(const int& initSize) { arr = new E[initSize]; count = 0; size = initSize; }
	~TransposeList() { delete[] arr; }

	int search(const E& elem)
		//returns index of element if found, error code when not found
	{
		for (int i = 0; i < count; i++)
		{
			if (elem == arr[i])
				return i;
		}
		return -1;
	}

	void insert(const E& elem)
		//inserts element at the end of the list - doubles size of list if list is full
	{
		if (count == size)		//check if list is at capacity
		{								//if full,
			size *= 2;					//double size
			E* temp = new E[size];		//temporary pointer for array to copy to
			for (int i = 0; i < count; i++)
				temp[i] = arr[i];
			delete[] arr;			//delete old array
			arr = temp;				//make list point to new double size array
		}
		arr[count] = elem;
		count++;
	}
	void remove(const E& elem)
		//searches for element in the list (deferring to search method) and deletes the element if it exists
		//deletion does put the last element out of place (switching with deleted element), but it should naturally
		//work its way back to its proper position and other elements will either be unaffected or require 1 more
		//check
	{
		int index = TransposeList::search(elem);
		if (index != -1)
		{
			std::cout << "Removing element at index " << index << std::endl;
			std::cout << "Swapping element at index " << index << " with element at index " << count - 1 << std::endl;
			//E temp = arr[index];
			//arr[index] = arr[count - 1];
			//arr[count - 1] = arr[index];
			std::swap(arr[index], arr[count - 1]);
			count--;
		}
	}

	E& getElem(const E& elem)
		//uses search to find element in list and returns it if found, otherwise throws out_of_range error
		//swaps element forward in the list by a set amount upon successful search
	{
		int index = search(elem);
		if (index == -1)
			throw std::out_of_range("Element not found");
		return getElemByIndex(index);

		
		
	}

	E& getElemByIndex(const int& index)
	//for use if know what index what you want is at (i.e. use manual search method beforehand)
	//swaps element forward in the list by a set amount upon successful search
	{
		if (index < 0 || index >= count)
			throw std::out_of_range("Invalid index");

		//std::cout << "Index is " << index << std::endl;
		int adjustedIndex = index - 4;
		//std::cout << "Adjusted index is " << adjustedIndex << std::endl;
		if (adjustedIndex < 0)
		{
			//std::cout << "Swapping with first element" << std::endl;
			//swap(this->arr, index, 0);
			std::swap(arr[index], arr[0]);
			return arr[0];
		}
		else
		{
			//std::cout << "Transposing accessed element..." << std::endl;
			//swap(this->arr, index, adjustedIndex);
			std::swap(arr[index], arr[adjustedIndex]);
			return arr[adjustedIndex];
		}

		/*if (index-4 < 0)				//if element is too close to front of the list,
										//swap with first element
		{
			swap(arr, arr[index], arr[0]);
			return arr[0];
		}

		swap(arr, arr[index], arr[index-4]);		//transpose accessed element forward 4 places
		return arr[index-4];*/
	}


	void print()
	{
		for (int i = 0; i < count; i++)
			std::cout << arr[i] << " ";
		std::cout << std::endl;
	}

private:
	E* arr;
	int count;
	int size;
};


const int LIST_SIZE = 50;
int main()
{
	TransposeList<int> myList(LIST_SIZE);
	for (int i = 0; i < LIST_SIZE+5; i++)
	{
		myList.insert(i);
	}
	//myList.print();

	/*cout << myList.search(9) << endl;
	myList.print();
	cout << myList.search(9) << endl;
	myList.print();*/
	for (int i = 0; i < 5; i++)
	{
		int temp = myList.getElem(LIST_SIZE-1);
		std::cout << temp << std::endl;
		myList.print();
	}
	
	std::cout << myList.search(49) << std::endl;		//outputs index where element containing 49 is located
	myList.remove(49);
	myList.print();
	return 0;
}