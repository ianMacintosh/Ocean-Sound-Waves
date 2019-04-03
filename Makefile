build:
	mpicc -lm ./pathCalc.c -o PathCalc

run:
	mpiexec -n 20 ./PathCalc

multi:
	mpirun -n 1000 -hosts 192.168.1.100,192.168.1.101,192.168.1.102,192.168.1.105,192.168.1.106,192.168.1.108,192.168.1.109,192.168.1.110,192.168.1.112 ./PathCalc

clear:
	rm ./outputfiles/*.txt

rayGraph:
	gnuplot ./graphing/allGraph.pg > ./graphing/raypaths.png

heatGraph:
	gnuplot ./heatmap/createHeatmap.pg > ./heatmap/heatMap.png

heatData:
	gcc ./heatmap/rayPathsToHeatmap.c -lm -g -o ./heatmap/RayPathsToHeatmap.exe
	./heatmap/RayPathsToHeatmap.exe

all:
	make
	make clear
	make run
	make graph

