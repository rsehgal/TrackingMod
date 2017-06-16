
#!/bin/sh
rm -rf Hodoscope.gdml run.txt tracks.txt
sed "s/NUMEVENTS/$1/g" my.mac.temp > my.mac.temp2
for ener in 1
do
sed "s/ENERGY/$ener/g" my.mac.temp2 > $ener.mac
#for var in "$@"
for var in Al Fe Pb
do
echo $var
sed "s/MATERIAL/$var/g" $ener.mac > $var.mac
for varThickness in  3 4 5 15 20 #1 2 3 4 5 10 15 20
do
sed "s/THICKNESS/$varThickness/g" $var.mac > $var$varThickness.mac
echo "========= Started Simulation for Energy = "$ener" , Material = "$var" , Thickness = " $varThickness" ==========" 
rm -rf Hodoscope.gdml
./main $var$varThickness.mac
rm -rf $var$varThickness.mac
done
rm -rf $var.mac
done
rm -rf $ener.mac
sed "s/nan/0/g" -i run.txt
sed "s/-nan/0/g" -i run.txt
done
