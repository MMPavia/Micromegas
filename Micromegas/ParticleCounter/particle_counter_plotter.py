#!/usr/bin/python


import os
import math
from ROOT import *
import datetime

now = datetime.datetime.now()

data = []
finish = []
class DataInfo:
    def __init__(self, PathFile):

        File = open(PathFile,"r")
        lines = File.readlines()
        File.close()
        finish.append(len(data))

        for i in xrange(0,len(lines)):
            temp = lines[i].replace("\n","").split("\t")
            data.append(temp)
        
        
        



filenames = []
import glob
path = "/home/atlas/Micromegas/ParticleCounter/outfile/*.txt"
for fname in glob.glob(path):
    print(fname)
    filenames.append(fname)
    filenames.sort()
if (len(filenames)>7):
    for file_iterator in range (len(filenames)-7,len(filenames)):
        DataInfo(filenames[file_iterator])
	print filenames[file_iterator]
else:
    for file_iterator in range (len(filenames)):
         DataInfo(filenames[file_iterator])

gStyle.SetOptStat(0)
gStyle.SetPadTickX(1) 
gStyle.SetPadTickY(2) 

gr = TH2F("int_time","Particle Counter",len(data),0,len(data),200,0,200)
gr2 = TH2F("int_time","temperature ",len(data),0,len(data),200,0,200)


gr.SetMarkerStyle(22);
gr.SetMarkerSize(1.5);
gr.SetMarkerColor(2);


gr2.SetMarkerStyle(2);
gr2.SetMarkerSize(1.5);
gr2.SetMarkerColor(4);

LabelNamePos = 0.
if (len(filenames)>7):
	LabelNamePos = len(filenames)-7
	print filenames[LabelNamePos]
else:

	LabelNamePos = 0.

part05 = []
part07 = []    
for iterator in range (len(data)):
    print len(data)
    part05.append(float(data[iterator][2]))
    part07.append(float(data[iterator][4]))
    
    gr.Fill(iterator, float(data[iterator][2]))
    gr2.Fill(iterator, float(data[iterator][4]))

    print iterator

    if (iterator % 2) == 0:
        gr.GetXaxis().SetBinLabel(iterator+1,data[iterator][0]);
        
    if (iterator in finish):
        print filenames[LabelNamePos][-14:-4] 
        label=filenames[LabelNamePos][-14:-4] 
        del filenames[LabelNamePos]
        gr.GetXaxis().SetBinLabel(iterator+1,label);

c1 = TCanvas( 'c1', 'ParticleCounter', 200, 10, 700, 500 )


gStyle.SetOptTitle(0);
gr.Draw()
gr2.Draw("same")

leg = TLegend(0.1,0.7,0.48,0.9);
leg.SetHeader("Legend");
leg.AddEntry(gr,"0.5","p");
leg.AddEntry(gr2,"0.7","p");
leg.Draw();
gROOT.Reset()

taitle = "Plot/" + str(now.year) + "_" + str(now.month) + "_" + str(now.day)
c1.SaveAs("out.pdf")
c1.SaveAs(taitle)
mean05 = sum(part05) / float(len(part05))
mean07 = sum(part07) / float(len(part07))

ss = sum((x-mean05)**2 for x in part05)
devstad05 = math.sqrt(ss/float(len(part05)))

ss = sum((x-mean07)**2 for x in part07)
devstad07 = math.sqrt(ss/len(part07))

body = "\'Weekly report on particle counter \n" + "The average for 0.5 is " + str(mean05) + " with std: " + str(devstad05) + "\n"
body += "The average for 0.7 is " + str(mean07) + " with std: " + str(devstad07) + "\n"
body += "Regards.\'"
#send mail
message = "mail -s \"Subject\" -a \"out.pdf\" edoardo.farina@pv.infn.it <<<" + body
print message 

gSystem.Exec(message);



