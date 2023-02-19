#include "cuda_runtime.h"  
#include "device_launch_parameters.h" 

__global__ void positionCalculateExecution(float* coordinate, float* distance, float* relativePos)
{
	const int tid = threadIdx.x;

	if (tid == 0)
		relativePos[tid] = distance[0];

	else
	{
		for (int i = tid; i >= 0; i--)
		{
			relativePos[tid] = distance[tid];
		}
	}
}


//coordinate:	coordinate of leading truck.two elements(x,y)
//distance:		distance with front truck sorted by following sequence
//relativeDis:	relative distance based on leading truck
//size:			number of following truck
extern "C" void PositionCalculation(float* coordinate, float* distance, float* relativePos, int size)
{
	float* dev_coordinate;
	float* dev_dis;
	float* dev_rPos;

	//allocate GPU memory
	cudaMalloc((void**)&dev_coordinate, 2 * sizeof(float));
	//cudaMalloc((void**)&dev_dis, size * sizeof(float));
	positionCalculateExecution << <1, size >> > (dev_coordinate, dev_dis, dev_rPos);
	cudaMalloc((void**)&dev_rPos, size * sizeof(float));

	//Copy input arry from host memory to GPU buffers. 
	cudaMemcpy(dev_coordinate, coordinate, 2 * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_dis, coordinate, size * sizeof(float), cudaMemcpyHostToDevice);

	//call kernel function
	positionCalculateExecution << <1, size >> > (dev_coordinate, dev_dis, dev_rPos);

	//Copy output arry from GPU device to Host
	cudaMemcpy(relativePos, dev_rPos, size * sizeof(float), cudaMemcpyDeviceToHost);// Not Working 

	//Free device memory
	cudaFree(dev_coordinate);
	cudaFree(dev_dis);
	cudaFree(dev_rPos);
}


