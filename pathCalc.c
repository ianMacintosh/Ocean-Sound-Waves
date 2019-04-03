#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>
#define PI 3.14159265358979323846

#define readfile "OceanData.txt"//file containing layer depths and soundspeeds
#define startDepth 500//500 for middle of soundspeed curve, where it should theoretically oscilate best
#define stopDistance 200000
#define MaxMessage 50000
#define maxAngle 9.4
#define minAngle -maxAngle
#define printJobs 8

int getNodeCount();
int getFileLength();
void populateArrays(double*, double*);
double * createPath(double, int*);
int jobsToPrint(int, int, int);

int world_rank;

void main(int argc, char ** argv)
{
	// Initialize the MPI environment
	MPI_Init(NULL, NULL);
	// Find out rank, size
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);

	//process to recieve data and write to text files
	int finalLength;
	int machineCount = getNodeCount();
	if(world_rank/machineCount < printJobs && world_rank % machineCount == 0){
		int jobsToReceive = jobsToPrint(world_rank, machineCount, world_size);
		printf("%03i is printer, looking for %03i jobs\n", world_rank, jobsToReceive);
		for(int i = 0; i < jobsToReceive; i++){
			double * list = malloc(sizeof(double) * MaxMessage);
			MPI_Status status;
			printf("receiving");
			MPI_Recv(list, MaxMessage, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			//printf("\treceived\t");
			MPI_Get_count(&status, MPI_DOUBLE, &finalLength);

			//output file title
			char buf[0x100];
			snprintf(buf, sizeof(buf), "./outputfiles/%03i.txt", status.MPI_SOURCE);

			//create write file
			FILE* write = fopen(buf, "w");
			for(int i = 0; i < finalLength; i+=2){
				fprintf(write, "%lf\t%lf\n", *(list + i), *(list + i + 1));
			}
			fclose(write);
			free(list);
			//printf("printed %3i, %i of %i\n", status.MPI_SOURCE, i+1, jobsToReceive);
			printf("printer: %03i\tprocess: %03i\t%03i remaining\n", world_rank, status.MPI_SOURCE, jobsToReceive - i);
		}
		printf("COMPLETED %i\n", world_rank);
	}
	//processes to generate data
	else{
		double divisionSize = (double)(maxAngle - minAngle) / (world_size - printJobs - 1);
		double startingAngle = minAngle + (world_rank - printJobs) * divisionSize;//space different processes for different start angles from minAngle to maxAngle degrees

		double * list = createPath(startingAngle, &finalLength);

		//printf("sending %3i\n", world_rank);
		MPI_Send(list, finalLength, MPI_DOUBLE, (world_rank%printJobs) * machineCount, 0, MPI_COMM_WORLD);
		free(list);
	}
	MPI_Finalize();
}

int jobsToPrint(int rank, int machines, int processes){
	int jobs = 0;

	for(int i = 0; i < processes; i++){
		if(!(i/machines < printJobs && i % machines == 0))//this process value of i is not a printer
 			if((i%printJobs) * machines == rank)//this process value of i will be sent to this printer
				jobs++;
	}

	return jobs;
}

int getNodeCount() {//return total number of machines maybe??
	int rank, is_rank0, nodes;
	MPI_Comm shmcomm;

	MPI_Comm_split_type(MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, 0, MPI_INFO_NULL, &shmcomm);
	MPI_Comm_rank(shmcomm, &rank);
	is_rank0 = (rank == 0) ? 1 : 0;
	MPI_Allreduce(&is_rank0, &nodes, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	MPI_Comm_free(&shmcomm);
	return nodes;
}

double * createPath(double startAngle, int * final){
	int length = 0;
	//open file, get length, possible error. does not leave file open for reading, simply counts lines of input data
	if ((length = getFileLength()) == -1){
		printf("Error opening file \"%s\"\n", readfile);
		system("pause");
		exit(0);
	}

	//create arrays of correct length, then read data into arrays
	double *depths = malloc(sizeof(double)*length);
	double *speeds = malloc(sizeof(double)*length);
	populateArrays(depths, speeds);

	//set initial parameters for loop
	int decending = -1;
	if (startAngle < 0) decending = 1;

	double angle = (90-fabs(startAngle)) * PI / 180;//angle in radians
	double depth = startDepth, distance = 0;//present coordinate points
	double speedratio = 0;//ratio of (current/next) layers soundspeeds. Used in checking for a bounce
	int layer = 0;//holds address location in arrays of current ocean layer and position in the arrays

	//set layer counter to being perfectly on or one layer before the user specified starting point
	while (depths[layer] < depth)
		layer++;
	
	double * results = malloc(sizeof(double) * MaxMessage);

	//loop
	int i;
	for (i = 0; 1; i+=2){
		//print current coordinates to file and console
		//fprintf(write, "%lf\t%lf\n", distance, depth);
		//printf("\ncoordinates: (%lf, %lf)\tangle: %lf(decending: %d)\tlayer: %d\n", distance, depth, angle, decending, layer);//	
		
		results[i] = distance;
		results[i+1] = depth;

		//calculate new coordinates
		distance += fabs(depth - depths[layer + decending]) * tan(angle);
		depth = depths[layer + decending];

		//ensure next point in current direction is not out of bounds
		if(layer + 2 == length && decending == 1){
			//printf("\nHit Bottom\n");
			break;
		}
		else if(layer == 0 && decending == -1){
			//printf("\nHit Surface\n");
			break;
		}
		if(distance > stopDistance){
			//printf("\nPast Stop Distance\n");
			break;
		}

		//calculate new angle
		//bounce
		if(angle > asin(speeds[layer]/speeds[layer+decending])){ 
			decending = 0 - decending;
			//printf("\nbounce\n");
		}

		//refract	
		else{
			//old angle reference: angle = PI/2 - asin(sin(PI/2 - angle) * speeds[layer + decending] / speeds[layer]);
			angle = asin( speeds[layer+decending] * sin(angle) / speeds[layer]); //A2 = asin( V2 sin(A1) / V1)
			layer += decending;
		}
	}
	*final = i;

	//print final coordinates to file and console
	//fprintf(write, "%lf\t%lf\n", distance, depth);
	//printf("final coordinates: (%lf, %lf)\tangle: %lf(decending: %d)\tlayer: %d\n", distance, depth, angle, decending, layer);//	

	//close write file
	//fclose(write);
	free(depths);
	free(speeds);
	//*results = startAngle;
	return results;
}

void populateArrays(double *depths, double *speeds){
	FILE* read = fopen(readfile, "r");

	for (int i = 0; fscanf(read, "%lf %lf", depths + i, speeds + i) != EOF; i++);

	fclose(read);
}

int getFileLength(){
	char current;
	int newlines = 0;
	FILE* read = fopen(readfile, "r");
	if (read == NULL)
		return -1;

	while ((current = fgetc(read)) != EOF){
		if (current == '\n')
			newlines++;
	}

	fclose(read);
	//printf("%d lines\n", newlines);
	return newlines;
}
