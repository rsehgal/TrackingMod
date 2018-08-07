#!/bin/sh
sed '/nan/d' -i StatsFromEventAction.txt
./GeneratorStats
