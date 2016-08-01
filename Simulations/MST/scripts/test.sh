#!/bin/sh
for var in "$@"
do
echo $var
sed "s/MATERIAL/$var/g" my.mac.temp > $var.mac
./main $var.mac
rm -rf $var.mac
done
