#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <cuda_runtime.h>
#include <iostream>
using namespace std;
long int array[100];
int N;
__device__ 
/*Newton method to caculate zero point y=x*x-a */
double mysqrt(int s){
    //double s=1.0;
    double before=1.0,after;
    while(1){
        after=(before*before+s)/(2.0*before); 
        if(fabs(before-after)<0.0000001) break;
         before=after;
        
    }
    return before;
    
}
__device__ 
double  power1(int n)
{
    double temp=1.0;
    double a5=mysqrt(5);
    for(int i=0; i<n; ++i)
        temp=temp*(1.0+a5)/2.0;//sqrt

    return temp;
}
__device__ 
double  power2(int n)
{
    double temp=1.0;
    double a5=mysqrt(5);
    for(int i=0; i<n; ++i)
        temp=temp*(1.0-a5)/2.0;//sqrt

    return temp;
}


__global__ void fib(long int *C, int N)
{
    //Grid中x方向上的索引
    //int xIndex = threadIdx.x + blockIdx.x * blockDim.x;

    //Grid中y方向上的索引
    //int yIndex = threadIdx.y + blockIdx.y * blockDim.y;
    //cout<<"threadIdx.x"=<<threadIdx.x<<" xIndex="<<xIndex<<endl;

    int i = (blockIdx.x * gridDim.x + blockIdx.y) * blockDim.x * blockDim.y + threadIdx.x * blockDim.x + threadIdx.y;
    if (i < N)
    {
        C[i] = (long int)((power1(i+1)-power2(i+1))/mysqrt(5));//sqrt
    }

}



int main(int argc, char **agrv)
{

    long int *device_C;

    scanf("%d",&N);
    // N=atoi(agrv[1]);

    struct timeval tpstart,tpend;
    struct timeval kernel_begin,kernel_end;
    //开始计时
    gettimeofday(&tpstart,NULL);


    //在GPU中开辟内存
    cudaMalloc((void**)&device_C, N * sizeof(long int));

    //定义block中thread的分布

    dim3 threadsPerBlock(4, 4);
    //根据输入图片的宽高定义block的大小
    dim3 blocksPerGrid(1,(N + 15) / 16,1);

    gettimeofday(&kernel_begin,NULL);
    fib <<<blocksPerGrid, threadsPerBlock >>>(device_C,N);
    gettimeofday(&kernel_end,NULL);
    double kenerl_time=1000000*(kernel_end.tv_sec-kernel_begin.tv_sec)+kernel_end.tv_usec-kernel_begin.tv_usec;
    cout<<"this kernel operation consumes "<<kenerl_time<<"us\n";


    //将结果传回CPU
    cudaMemcpy(array, device_C, N * sizeof(long int), cudaMemcpyDeviceToHost);
    for(int i = 0 ; i<N ; i++)
    {
        if(i==0) printf("%ld",array[i]);
        else printf(" %ld",array[i]);
    }
    putchar('\n');

    //结束计时
    gettimeofday(&tpend,NULL);
    double timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec;
    cout<<"this operation consumes "<<timeuse/1000<<"ms\n";

    return 0;
}


/***** End *****/

