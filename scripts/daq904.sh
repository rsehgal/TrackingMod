#!/bin/sh
#Get run number
read runnum </home/user/re4data/TDC/run
/bin/cp /home/user/re4data/TDC/6928.root /home/user/re4data/TDC/$runnum.root
/bin/cp /home/user/re4data/TDC/daqinfo /home/user/re4data/TDC/$runnum.info
let "runnum++"
echo "Next run num : "$runnum
echo $runnum > /home/user/re4data/TDC/run

