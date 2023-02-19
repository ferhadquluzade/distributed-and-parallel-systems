#include "cuda_runtime.h"  
#include "device_launch_parameters.h" 

#define MINIMUN_SPACING 3
#define MAX_SIZE 10

__global__ void positionCalculateExecution(float* coordinate, float* distance, 
										   float relativePos[MAX_SIZE][2], bool* distanceCheck)
{
	const int tid = threadIdx.x;

	//calculate relative distance
	if (tid == 0)
		relativePos[tid][0] = distance[0];
	else
	{
		for (int i = tid; i >= 0; i--)
		{
			relativePos[tid][0] = distance[tid];
		}
	}
	// check reasonability of distance
	if (distance[tid] > MINIMUN_SPACING)
		distanceCheck[tid] = true;
	else
		distanceCheck[tid] = false;

}


//coordinate[2]:		coordinate of leading truck.two elements(x,y)
//distance[size]:		distance with front truck sorted by following sequence
//relativePos[size][2]:	relative distance based on leading truck
//size:					number of following truck
//distanceCheck:		check distance is reasonanle or not.true:reasonable false:not reasonable
extern "C" void PositionCalculation(float* coordinate, float* distance, float relativePos[MAX_SIZE][2], bool *distanceCheck, int size)
{
	float* dev_coordinate;
	float* dev_dis;
	float dev_rPos[MAX_SIZE][2];
	bool* dev_check;

	//allocate GPU memory
	cudaMalloc((void**)&dev_coordinate, 2 * sizeof(float));
	cudaMalloc((void**)&dev_dis, size * sizeof(float));
	cudaMalloc((void**)&dev_rPos, 2 * size * sizeof(float));
	cudaMalloc((void**)&dev_check, size * sizeof(bool));

	//Copy input arry from host memory to GPU buffers. 
	cudaMemcpy(dev_coordinate, coordinate, 2 * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_dis, coordinate, size * sizeof(float), cudaMemcpyHostToDevice);

	//call kernel function
	positionCalculateExecution << <1, size >> > (dev_coordinate, dev_dis, dev_rPos, dev_check);

	//Copy output arry from GPU device to Host
	cudaMemcpy(relativePos, dev_rPos, size * sizeof(float), cudaMemcpyDeviceToHost);
	cudaMemcpy(distanceCheck, dev_check, size * sizeof(float), cudaMemcpyDeviceToHost);

	//Free device memory
	cudaFree(dev_coordinate);
	cudaFree(dev_dis);
	cudaFree(dev_rPos);
	cudaFree(dev_check);
}


