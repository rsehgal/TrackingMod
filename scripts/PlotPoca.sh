#!/bin/sh
sed '/nan/d' -i $1
./Reconstruct $1
