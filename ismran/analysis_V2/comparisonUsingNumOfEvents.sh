#!/bin/sh
pbfile=$1
nsfile=$2
./MuonTrackAnalysis_SimplestSetup_Pixels_Compare $pbfile $nsfile
./MuonTrackAnalysis_SimplestSetup_Pixels_Compare $nsfile $pbfile

