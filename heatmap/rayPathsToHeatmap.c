#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define width 200000
#define height 2605
#define divisions 300


int main(){
	FILE * input, *output;
	double x, y;

	int ** table = (int**)malloc(divisions * sizeof(int*));
	for(int i = 0; i < divisions; i++)
		*(table + i) = (int*)malloc(divisions * sizeof(int));

	for(int i = 10; i < 250; i++){
		char buf[100];
		sprintf(buf, "./outputfiles/%03d.txt", i);

		input = fopen(buf, "r");

		while(fscanf(input, "%lf\t%lf", &x, &y) != EOF)
			table[(int)floor(x*divisions/width)][(int)floor(y*divisions/height)]++;

		fclose(input);
	}

	output = fopen("heatmap/HeatmapData.txt", "w");
	//int max = 0, current = 0;
	for(int i = 0; i < divisions; i++){
		for(int j = 0; j < divisions; j++){
			current = table[i][j];
			/*
			if(current > max){
				max = current;
				printf("%i\t%i\t%i\n", i, j, current);
			}
			*/
			fprintf(output, "%i\t%i\t%i\n", i, j, current);
		}
		fprintf(output, "\n");
	}
	fclose(output);

	return 0;
}
