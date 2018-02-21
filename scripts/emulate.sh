#!/bin/sh
filepath="/home/user/re4data/TDC"
/bin/cp $filepath/run /var/www/html/Tomo/run
read runnum <$filepath/run
let runnum=runnum-1

/usr/bin/scp /home/user/re4data/TDC/$runnum.root user@192.168.0.3:/home/user/TrackingMod/build/$runnum.root
/usr/bin/scp /home/user/re4data/TDC/$runnum.info user@192.168.0.3:/home/user/TrackingMod/build/$runnum.info

#let runnum=runnum-1
/usr/bin/ssh -q user@192.168.0.3 'source /home/user/env.sh /home/user/BackBoneSoftwares/root-6.12.06/install/bin/thisroot.sh && cd /home/user/TrackingMod/build && ./htmlout'
/usr/bin/ssh -q user@192.168.0.3 'source /home/user/env.sh /home/user/BackBoneSoftwares/root-6.12.06/install/bin/thisroot.sh && cd /home/user/TrackingMod/build && ./TestHitPlotForRadiograph2'
