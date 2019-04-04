#this causes the master to run PathCalc on the slave nodes and then gets the resulting .png files
ssh other@192.168.1.100 << var
    cd Ocean-Sound-Waves;
    make all;
var
rm ./graphing/raypaths.png ./heatmap/heatMap.png
scp other@192.168.1.100:~/Ocean-Sound-Waves/graphing/raypaths.png ./graphing/raypaths.png
scp other@192.168.1.100:~/Ocean-Sound-Waves/heatmap/heatMap.png  ./heatmap/heatMap.png
