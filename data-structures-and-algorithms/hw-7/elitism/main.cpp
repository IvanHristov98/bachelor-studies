#include <iostream>
#include <cmath>

class MinHeap
{
private:
    long * arr;
    int capacity;
    int size;

    int parent(int i) { return (i - 1) / 2; }

    int left(int i) { return 2 * i + 1; }

    int right(int i) { return 2 * i + 2; }

public:
    MinHeap(int capacity) : arr(nullptr)
    {
        this->capacity = capacity;
        this->size = 0;
        this->arr = new long[this->capacity];
    }

    void insert(int k)
    {
        if (this->size == this->capacity) { return; }

        // prevents segfault with size = 0
        this->size++;
        int i = this->size - 1;
        this->arr[i] = k;

        while (i != 0 && this->arr[this->parent(i)] > this->arr[i])
        {
            std::swap(this->arr[this->parent(i)], this->arr[i]);
            i = parent(i);
        }
    }

    long extractMin()
    {
        //if (this->size <= 0) { return; }

        if (this->size == 1)
        {
            this->size--;
            return this->arr[0];
        }

        long root = this->arr[0];
        this->arr[0] = this->arr[this->size - 1];
        this->size--;
        this->minHeapify(0);

        return root;
    }

    void minHeapify(int index)
    {
        int smallest = index;
        int left = this->left(index);
        int right = this->right(index);

        if (left < this->size)
        {
            if (this->arr[left] < this->arr[smallest]) { smallest = left; }
        }
        
        if (right < this->size)
        {
            if (this->arr[right] < this->arr[smallest]) { smallest = right; }
        }

        if (smallest != index)
        {
            std::swap(this->arr[smallest], this->arr[index]);
            minHeapify(smallest);
        }
    }

    long getMin() const { return this->arr[0]; }

    long getSize() const { return this->size; }
};

class MaxHeap
{
private:
    long * arr;
    int capacity;
    int size;

    int parent(int i) { return (i - 1) / 2; }

    int left(int i) { return 2 * i + 1; }

    int right(int i) { return 2 * i + 2; }

public:
    MaxHeap(int capacity) : arr(nullptr)
    {
        this->capacity = capacity;
        this->size = 0;
        this->arr = new long[capacity];
    }

    void insert(int k)
    {
        if (this->size == this->capacity) { return; }

        this->size++;
        int i = this->size - 1;
        this->arr[i] = k;

        while (i != 0 && this->arr[this->parent(i)] < this->arr[i])
        {
            std::swap(this->arr[this->parent(i)], this->arr[i]);
            i = parent(i);
        }
    }

    long extractMax()
    {
        //if (this->size <= 0) { return ; }

        if (this->size == 1)
        {
            this->size--;
            return this->arr[0];
        }

        long root = this->arr[0];
        std::swap(this->arr[0], this->arr[this->size - 1]);
        this->size--;
        this->maxHeapify(0);

        return root;
    }

    void maxHeapify(int index)
    {
        int largest = index;
        int left = this->left(index);
        int right = this->right(index);

        if (left < this->size)
        {
            if (this->arr[left] > this->arr[largest]) { largest = left; }
        }

        if (right < this->size)
        {
            if (this->arr[right] > this->arr[largest]) { largest = right; }
        }

        if (index != largest)
        {
            std::swap(this->arr[index], this->arr[largest]);
            maxHeapify(largest);
        }
    }

    long getMax() const { return this->arr[0]; }

    long getSize() const { return this->size; }
};

class MedianHeap
{
private:
    MinHeap * minHeap;
    MaxHeap * maxHeap;
    double median;

public:
    MedianHeap(int capacity) : minHeap(nullptr), maxHeap(nullptr)
    {
        this->minHeap = new MinHeap(capacity);
        this->maxHeap = new MaxHeap(capacity);
        this->median = 0.0;
    }

    void insert(int k)
    {
        // inserting median
        if (k >= this->median)
        {
            this->minHeap->insert(k);
        }
        else { this->maxHeap->insert(k); }

        // balancing heaps
        if (abs(this->minHeap->getSize() - this->maxHeap->getSize()) > 1)
        {
            this->balance();
        }

        // updating median
        if (this->minHeap->getSize() > this->maxHeap->getSize()) { this->median = (double)this->minHeap->getMin(); }

        if (this->maxHeap->getSize() > this->minHeap->getSize()) { this->median = (double)this->maxHeap->getMax(); }

        if (this->maxHeap->getSize() == this->minHeap->getSize())
        {
            this->median = ((double)this->minHeap->getMin() + (double)this->maxHeap->getMax()) / 2;
        }
    }

    void balance()
    {
        if (this->maxHeap->getSize() - this->minHeap->getSize() > 1)
        {
            this->minHeap->insert(this->maxHeap->extractMax());
        }
        
        if (this->minHeap->getSize() - this->maxHeap->getSize() > 1)
        {

            this->maxHeap->insert(this->minHeap->getMin());
            this->minHeap->extractMin();
        }
    }

    double getMedian() { return this->median; }
};

int main()
{
    int arrSize = 0;
    std::cin >> arrSize;

    MedianHeap * heap = new MedianHeap(arrSize);

    long number = 0;

    std::cout << std::fixed;
    std::cout.precision(1);

    for (int i = 0; i < arrSize; i++)
    {
        std::cin >> number;
        heap->insert(number);

        std::cout << heap->getMedian() << std::endl;
    }

    return 0;
}