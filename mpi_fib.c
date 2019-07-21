#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <string.h>

/***** Begin *****/
long int array[100];
int n;
 double  power1(int n)
{
    double temp=1.0;
    for(int i=0; i<n; ++i)
        temp=temp*(1.0+sqrt(5))/2.0;

    return temp;
}
 double  power2(int n)
{
    double temp=1.0;
    for(int i=0; i<n; ++i)
        temp=temp*(1.0-sqrt(5))/2.0;

    return temp;
}


int main (int argc, char* argv[])
{
    int rank, size;
    MPI_Status status;

    MPI_Init (&argc, &argv);      /* starts MPI*/
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);        /* get current process id*/
    MPI_Comm_size (MPI_COMM_WORLD, &size);        /* get number of processes*/
    //printf( "Hello world from process %d of %d\n", rank, size );
    if(rank==0)
    {
        scanf("%d",&n);

         int gap=(n+size-2)/(size-1);
        for(int i=1; i<size; ++i)
            MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        for(int i=1; (i<=size-1) && (i<=n); ++i)
            MPI_Recv(array+(i-1)*gap, gap, MPI_LONG_INT, i, 0,
                     MPI_COMM_WORLD, &status);
        /*MPI_Recv(array+(size-2)*gap, gap+(n%(size-1)), MPI_LONG_INT, size-1, 0,
                 MPI_COMM_WORLD, &status);*/
		// array[1]=array[2]=1;
        //
        for(int i = 0 ; i<n ; i++)
        {
            if(i==0) printf("%ld",array[i]);
            else printf(" %ld",array[i]);
        }
        putchar('\n');


    }
    else //if(rank!=size-1)
    {
        int gap=(n)/(size-1);
        int begin=(rank-1)*gap;
        int k=0;
        MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        
        //gap=(n)/(size-1);//不应当定义时赋值，此时还不知道n
         gap=(n+size-2)/(size-1);
        begin=(rank-1)*gap;
        if(rank<=n) {
        for(int i=begin+1; i<=begin+gap; ++i)
        {
            array[k++]=(long int)((power1(i)-power2(i))/sqrt(5));

        }
        
        MPI_Send( array, gap, MPI_LONG_INT, 0, 0, MPI_COMM_WORLD);
        }


    }
    /*else
    {
        int gap=(n)/(size-1);
        int begin=(rank-1)*gap;
        int k=0;
        MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        gap=(n)/(size-1);//不应当定义时赋值，此时还不知道n
        begin=(rank-1)*gap;
        for(int i=(rank-1)*gap+1; i<=n; ++i)
        {
            array[k++]=(long int)((power1(i)-power2(i))/sqrt(5));

        }
        MPI_Send( array, gap+(n%(size-1)), MPI_LONG_INT, 0, 0, MPI_COMM_WORLD);

    }*/



    MPI_Finalize();
    return 0;
}

/***** End *****/

