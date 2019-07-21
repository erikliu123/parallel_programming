
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <cuda_runtime.h>
long int array[100];
int N;

__device__ 
double  power1(int n)
{
    double temp=1.0;
    for(int i=0; i<n; ++i)
        temp=temp*(1.0+sqrt(5))/2.0;

    return temp;
}
__device__ 
double  power2(int n)
{
    double temp=1.0;
    for(int i=0; i<n; ++i)
        temp=temp*(1.0-sqrt(5))/2.0;

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
        C[i] = (long int)((power1(i+1)-power2(i+1))/sqrt(5));
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
    cudaMalloc((void**)&device_C, N * sizeof(int));

    //定义block中thread的分布

    dim3 threadsPerBlock(4, 4);
    //根据输入图片的宽高定义block的大小
    dim3 blocksPerGrid((N + threadsPerBlock.x - 1) / threadsPerBlock.x);
    //算子大小
    //Size Element(3, 5);
    //CUDA erosion

    gettimeofday(&kernel_begin,NULL);
    fib <<<blocksPerGrid, threadsPerBlock >>>(device_C,N);
    gettimeofday(&kernel_end,NULL);
    //double kenerl_time=1000000*(kernel_end.tv_sec-kernel_begin.tv_sec)+kernel_end.tv_usec-kernel_begin.tv_usec;
    //cout<<"this kernel operation consumes "<<kenerl_time<<"us\n";


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
    //double timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec;
    //cout<<"this operation consumes "<<timeuse<<"us\n";

    //CUDA dilate
    //dilateInCuda << <blocksPerGrid, threadsPerBlock >> >(d_in, d_out2, Element, imgWidth, imgHeight);

    //将结果传回CPU
    //cudaMemcpy(dstImg2->imageData, d_out2, imgWidth * imgHeight * sizeof(unsigned char), cudaMemcpyDeviceToHost);

    //CPU内膨胀（OpenCV实现）

    return 0;
}


/***** End *****/

