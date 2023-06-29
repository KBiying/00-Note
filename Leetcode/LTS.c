#include <stdio.h>

// 交换两个元素的值
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 将序列划分为两部分并返回划分点的索引
int partition(int arr[], int low, int high)
{
    int pivot = arr[high]; // 选取最后一个元素作为划分点
    int i = (low - 1);     // 为什么记住low-1??

    for (int j = low; j <= high - 1; j++) // 找到第一个比pivot小的数,
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]); // 将小的数和大的数调换位置
        }
    }

    swap(&arr[i + 1], &arr[high]); // 最后交换pivot和大的数
    return (i + 1);                // 返回划分点
}

// 使用快速排序找到第k小元素
int quickSelect(int arr[], int low, int high, int k)
{
    if (k > 0 && k <= high - low + 1) // 当k在界内时
    {
        int pivotIndex = partition(arr, low, high); // 对[low, high]范围内的数值进行排序

        if (pivotIndex - low == k - 1) // 如果划分点恰好是第k个元素,则找到啦~
        {
            return arr[pivotIndex];
        }

        if (pivotIndex - low > k - 1) // 如果不是,则判断划分点与k的位置左右
        {
            return quickSelect(arr, low, pivotIndex - 1, k); // 当大于k的时候,则说明要在下游[low, pivot-1]中找第k个元素
        }

        return quickSelect(arr, pivotIndex + 1, high, k - pivotIndex + low - 1); // 当小于k时,说明在上游[pivot+1, high]中找第(k - pivotIndex + low - 1)个元素
        // return quickSelect(arr, pivotIndex + 1, high, k); // 当小于k时,说明在上游[pivot+1, high]中找第(k - pivotIndex + low - 1)个元素
    }

    return -1; // 如果k的值无效，则返回-1
}

int main()
{
    int arr[] = {7, 10, 4, 3, 20, 15};
    int n = sizeof(arr) / sizeof(arr[0]); // 求数组元素个数 cool！
    int k = 5;                            // 要找的第k小元素的索引

    int result = quickSelect(arr, 0, n - 1, k);

    printf("第%d小元素是：%d\n", k, result);

    return 0;
}
