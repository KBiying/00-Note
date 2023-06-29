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
int quickSelect(int arr[], int low, int high, int k)
{
    if (k > 0 && k <= high + 1)
    {
        int p = partition(arr, low, high);

        if (p == k - 1)
        {
            return arr[p];
        }
        if (p > k - 1)
        {
            return quickSelect(arr, low, p - 1, k);
        }
        return quickSelect(arr, p + 1, high, k);
    }

    return -1;
}
int main()
{
    int arr[] = {7, 10, 4, 3, 20, 15};
    int n = sizeof(arr) / sizeof(arr[0]); // 求数组元素个数 cool！
    int k = 6;                            // 要找的第k小元素的索引

    int result = quickSelect(arr, 0, n - 1, k);

    printf("第%d小元素是：%d\n", k, result);

    return 0;
}