
#!/bin/sh
rm -rf run.txt tracks.txt
for ener in 2
do
sed "s/ENERGY/$ener/g" my.mac.temp > $ener.mac
#for var in "$@"
for var in Fe Pb
do
echo $var
sed "s/MATERIAL/$var/g" $ener.mac > $var.mac
for varThickness in 1 2 3 4 5 10 15 20
do
sed "s/THICKNESS/$varThickness/g" $var.mac > $var$varThickness.mac
./main $var$varThickness.mac
rm -rf $var$varThickness.mac
done
rm -rf $var.mac
done
rm -rf $ener.mac
sed "s/nan/0/g" -i run.txt
sed "s/-nan/0/g" -i run.txt
done
