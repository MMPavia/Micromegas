#!/usr/bin/python


import os
import math
from ROOT import *


matches = []
directory = []

filenames = []
import glob
path = "*.root"
for fname in glob.glob(path):
    print(fname)
    filenames.append(fname)

hs = THStack("ThicknessUPAverageALL","ThicknessUPAverageALL")
hs1 = THStack("BendingALL","BendingALL")
hs2= THStack("TorsionALL","TorsionALL")
hs3 = THStack("ThicknessDOWNAverageALL","ThicknessDOWNAverageALL")

c1 = TCanvas ("stack","stack",900,800)


for itr in range(len(filenames)):
	file1 = TFile(filenames[itr],"read")
	print filenames[itr]
	evhist = file1.Get("ThicknessUPAverageALL")
	evhist1 = file1.Get("BendingALL")
	evhist2 = file1.Get("TorsionALL")
	evhist3 = file1.Get("ThicknessDOWNAverageALL")

	gROOT.cd()
	hnew = evhist.Clone()
	hnew1 = evhist1.Clone()
	hnew2 = evhist2.Clone()
	hnew3 = evhist3.Clone()


	hs.Add(hnew)
	hs1.Add(hnew1)
	hs2.Add(hnew2)
	hs3.Add(hnew3)



	print "g1"
	file1.Close()


file2 = TFile("title.root1","recreate")


hs.Draw("")
hs.Print()
hs.Write()
hs1.Print()
hs1.Write()
hs2.Print()
hs2.Write()
hs3.Print()
hs3.Write()

c1.Update()
c1.Print("plot.png")
file2.Close()
