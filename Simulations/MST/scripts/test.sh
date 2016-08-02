#!/bin/sh
rm -rf run.txt
for ener in 2 3
do
sed "s/ENERGY/$ener/g" my.mac.temp > $ener.mac
for var in "$@"
do
echo $var
sed "s/MATERIAL/$var/g" $ener.mac > $var.mac
./main $var.mac
rm -rf $var.mac
done
rm -rf $ener.mac
done
