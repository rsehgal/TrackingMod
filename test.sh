#!/bin/sh

## Scrip to remove lines containing nan, inf
sed -i '/inf/d' $1 
sed -i '/nan/d' $1
sed -i '/-nan/d' $1
sed -i '/e+/d' $1
