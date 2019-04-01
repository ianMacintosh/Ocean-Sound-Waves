build:
	mpicc -lm ./pathCalc.c -o PathCalc

run:
	mpiexec -n 10 ./PathCalc

multi:
	mpirun -n 300 -hosts host,10.69.106.25,10.69.106.27 ./PathCalc

clear:
	rm ./outputfiles/*.txt

rayGraph:
	gnuplot ./graphing/allGraph.pg > ./graphing/raypaths.png
	sxiv -f ./graphing/raypaths.png

heatGraph:
	gnuplot ./heatmap/createHeatmap.pg > ./heatmap/heatMap.png
	sxiv -f ./heatmap/heatMap.png

heatData:
	gcc ./heatmap/rayPathsToHeatmap.c -lm -g -o ./heatmap/RayPathsToHeatmap.exe
	./heatmap/RayPathsToHeatmap.exe

all:
	make
	make clear
	make run
	make graph
