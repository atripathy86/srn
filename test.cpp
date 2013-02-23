#include <stdio.h>
#include <string.h>


#define RANGE 16


int main(int argc, char *argv[])
{
	int* weight;
	int *weight_ptr, *weight_ptr0, *weight_ptr1, *weight_ptr2;

	printf("Hello, world\n");
	
	weight = new int[RANGE];
	memset(weight,0,RANGE);
	int &val0 = weight[0], &val1= weight[1], &val2 = weight[2];

	//Loads the memory
	for (int i=0;i<RANGE ;i++ )
	{
		weight[i]=i;
	}


	for (int i=0;i<RANGE ;i++ )
	{
		
		
		//To generate corruption

		if ( i%4 == 0)
		{
			weight[0] = weight[i]; printf("Corruption :");
		} else  printf("           :");


		
		
		
		weight_ptr = &(weight[i]);

		switch (i)
		{
		case 0:
			weight_ptr0 = &(weight[i]);
		break;

		case 1:
			weight_ptr1 = &(weight[i]);
		break;

		case 2:
			weight_ptr2 = &(weight[i]);

		}

		printf("position %d : %d, pos 0: %d, pos 1: %d, pos 2: %d, \n", i, *weight_ptr, *weight_ptr0,*weight_ptr1, *weight_ptr2);
		printf("       copy:position %d : %d, pos 0: %d, pos 1: %d, pos 2: %d, \n", i, *weight_ptr, val0,val1, val2);



	}

	

	return 0;
}
