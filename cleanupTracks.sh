#!/bin/sh
sed -i.bak '/e+/d' ./tracks.txt
sed -i.bak '/e-/d' ./tracks.txt
sed -i.bak '/inf/d' ./tracks.txt
sed -i.bak '/nan/d' ./tracks.txt
