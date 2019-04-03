ssh other@slave01 rm -r /home/other/Ocean-Sound-Waves/*; scp OceanData.txt pathCalc.c Makefile other@slave01:~/Ocean-Sound-Waves/; ssh other@slave01 << var01
	cd Ocean-Sound-Waves; 
	make;
var01
ssh other@slave02 rm -r /home/other/Ocean-Sound-Waves/*; scp OceanData.txt pathCalc.c Makefile other@slave02:~/Ocean-Sound-Waves/; ssh other@slave02 << var02
	cd Ocean-Sound-Waves; 
	make;
var02
ssh other@slave05 rm -r /home/other/Ocean-Sound-Waves/*; scp OceanData.txt pathCalc.c Makefile other@slave05:~/Ocean-Sound-Waves/; ssh other@slave05 << var05
	cd Ocean-Sound-Waves; 
	make;
var05
ssh other@slave06 rm -r /home/other/Ocean-Sound-Waves/*; scp OceanData.txt pathCalc.c Makefile other@slave06:~/Ocean-Sound-Waves/; ssh other@slave06 << var06
	cd Ocean-Sound-Waves; 
	make;
var06
ssh other@slave08 rm -r /home/other/Ocean-Sound-Waves/*; scp OceanData.txt pathCalc.c Makefile other@slave08:~/Ocean-Sound-Waves/; ssh other@slave08 << var08
	cd Ocean-Sound-Waves; 
	make;
var08
ssh other@slave09 rm -r /home/other/Ocean-Sound-Waves/*; scp OceanData.txt pathCalc.c Makefile other@slave09:~/Ocean-Sound-Waves/; ssh other@slave09 << var09
	cd Ocean-Sound-Waves; 
	make;
var09
ssh other@slave10 rm -r /home/other/Ocean-Sound-Waves/*; scp OceanData.txt pathCalc.c Makefile other@slave10:~/Ocean-Sound-Waves/; ssh other@slave10 << var10
	cd Ocean-Sound-Waves; 
	make;
var10
ssh other@slave12 rm -r /home/other/Ocean-Sound-Waves/*; scp OceanData.txt pathCalc.c Makefile other@slave12:~/Ocean-Sound-Waves/; ssh other@slave12 << var12
	cd Ocean-Sound-Waves; 
	make;
var12
