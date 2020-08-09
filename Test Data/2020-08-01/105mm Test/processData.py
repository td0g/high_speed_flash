#Written by Tyler Gerritsen
#td0g.ca
#2020-08-05

#Processing CSV data from speedlight testing
#Data is filtered through a 5th order Butterworth filter

import os
import csv
import numpy as np
import matplotlib.pyplot as plt
from scipy import signal
import re

dpi = 300
pathName = os.getcwd()
testNames = []

fileList = []
fileNames = os.listdir(pathName)


#https://stackoverflow.com/questions/5967500/how-to-correctly-sort-a-string-with-a-number-inside
def atoi(text):
    return int(text) if text.isdigit() else text

def natural_keys(text):
    return [ atoi(c) for c in re.split(r'(\d+)', text) ]
    
def infoFromFileName(file):
  d = file.replace('.csv','').split('_')
  try:
    x = int(d[2])
    d[2] = '1/' + d[2]
  except:
    pass
  return d[0],d[1],d[2],d[3]
  
testName = []
modelColor = ['orange', 'purple', 'red', 'green','aqua','blue','black','pink','black','teal']
power = []
powerColor = ['green', 'blue', 'orange', 'red', 'violet','black','yellow']

for fileNames in fileNames:
    if fileNames.endswith(".csv"):
        fileList.append(fileNames)
        ID, model, p, seq = infoFromFileName(fileNames)
        if not model in testName:
          testName.append(model)
        if not p in power:
          power.append(p)
#power = sorted(power)
power.sort(key=natural_keys)
print ("Models:")
print (testName)
print ("Powers:")
print (power)

def readCSV(file):
  fileData = open(os.path.join(pathName, file), "r")
  reader = csv.reader(fileData, delimiter=',')
  time = []
  volt = []
  for column in reader:
    try:
      t = float(column[0]) * 1000000
      v = float(column[1]) 
      time.append(t)
      volt.append(v)
    except:
      pass
  fileData.close()
  dataTime = np.array(time)
  dataVolt = np.array(volt)  
  print("Data read complete, filtering")
  #https://dsp.stackexchange.com/questions/49460/apply-low-pass-butterworth-filter-in-python
  fs = dataTime[1]-dataTime[0]  # Sampling Period
  fc = 10  # Cut-off frequency of the filter in micros
  w = fc * fs / 2 # Normalize the frequency
  b, a = signal.butter(5, w, 'low')
  dataFiltered = signal.filtfilt(b, a, dataVolt)
  return dataTime, dataFiltered

def getMaxVolt(data):
  max = np.argmax(data)
  late = max + 1
  early = max - 1
  while late - early < 400:
    if data[late] > data[early]:
      late += 1
    else:
      early -= 1
  return (data[late] + data[early]) / 2

def plotSinglePulse(dataTime, dataFiltered, PNGname):
  print("Plotting")
  plt.plot(dataTime, dataFiltered)
  
  #find max signal
  maxVolt = getMaxVolt(dataFiltered)
  plt.axhline(y=maxVolt,color="red",linestyle="dashed",alpha=0.4)
  startRise = np.argmax(dataFiltered) - int(10 / (dataTime[1]-dataTime[0]))
  v1 = dataFiltered[startRise]
  v0 = dataFiltered[startRise-10000]
  while v0 < v1 and startRise > 0 or v0 > (maxVolt - 0.4) and startRise > 0:
    startRise -= 1
    v1 = dataFiltered[startRise]
    v0 = dataFiltered[max(startRise-10000,0)]
  plt.axvline(x=dataTime[startRise],color="red",linestyle="dashed",alpha=0.4)
  
  
  endRise = np.argmax(dataFiltered) + int(10 / (dataTime[1]-dataTime[0]))
  
  v1 = dataFiltered[endRise + 10000]
  v0 = dataFiltered[endRise + 5000]
  while v0 > v1 and endRise + 5001 < len(dataFiltered):
    endRise += 1
    v1 = dataFiltered[endRise + 5000]
    v0 = dataFiltered[endRise]
    

  plt.axvline(x=dataTime[endRise],color="red",linestyle="dashed",alpha=0.4)
  #http://resourcemagonline.com/2017/05/using-flash-duration-to-elevate-your-photography/78458/
  t10 = max(dataFiltered[startRise],dataFiltered[endRise])
  t50 = t10
  t10 = (maxVolt - t10) * 0.1 + t10
  t50 = (maxVolt - t50) * 0.5 + t50
  plt.axhline(y=t10,color="green",linestyle="dashed",alpha=0.4)
  plt.axhline(y=t50,color="green",linestyle="dashed",alpha=0.4)
  
  t10Start = startRise
  t10End = endRise
  while dataFiltered[t10Start] < t10:
    t10Start += 1
  while dataFiltered[t10End] < t10:
    t10End -= 1
  
  t50Start = startRise
  t50End = endRise
  while dataFiltered[t50Start] < t50:
    t50Start += 1
  while dataFiltered[t50End] < t50:
    t50End -= 1
  ID, model, power, sequence = infoFromFileName(PNGname)
  plt.title(model + " | " + power + " | " + sequence,fontsize=20)
  plt.text(dataTime[startRise]+150,t10+0.05, "T0.1 = " + str(int((dataTime[t10End]-dataTime[t10Start])))+ " \u03BCs",color="green",fontsize=10)
  plt.text(dataTime[startRise]+150,t50+0.05, "T0.5 = " + str(int((dataTime[t50End]-dataTime[t50Start])))+ " \u03BCs",color="green",fontsize=10)
  if maxVolt < -0.5:
    plt.text(dataTime[startRise]+150,maxVolt+0.05, "Max Voltage = +" + str(round(maxVolt-dataFiltered[startRise],2))+ " V",color="red",fontsize=10)
  else:
    plt.text(dataTime[startRise]+150,maxVolt-0.1, "Max Voltage = +" + str(round(maxVolt-dataFiltered[startRise],2))+ " V",color="red",fontsize=10)
 
  plt.xlabel("Time (\u03BCs)")
  plt.ylabel("Signal (V)")
  plt.xlim(dataTime[startRise]-50,dataTime[startRise]+250)
  plt.ylim(-1.5,0.5)
  plt.savefig(os.path.join(pathName, PNGname).replace(".csv",".png"), dpi=dpi)
  print("File Saved")
  plt.clf()

def savePNG(file, name):
  
  dataTime, dataVolt = readCSV(file)
  plotSinglePulse(dataTime, dataVolt, name)


def findStart(dataTime, dataVolt):  
  startRise = np.argmax(dataVolt) - int(10 / (dataTime[1]-dataTime[0]))
  v1 = dataVolt[startRise]
  v0 = dataVolt[startRise-10000]
  maxVolt = np.max(dataVolt)
  while v0 < v1 and startRise > 0 or v0 > (maxVolt - 0.2) and startRise > 0:
    startRise -= 1
    v1 = dataVolt[startRise]
    v0 = dataVolt[max(startRise-10000,0)]
  return dataTime[startRise]



if False:
    I = 0
    for thisName in fileList:
      print("Opening: " + fileList[I])
      savePNG(thisName,fileList[I])
      I += 1
    flashModels = []
I = 0


if True:
    J = 0
    I = 0
    for name in testName:
      for p in power:
        for f in fileList:
          ID, model, thisPower, sequence = infoFromFileName(f)
          if name == model:
    #      if testNames[I] != testNames[I].replace(name,""):
            if thisPower == p:
    #        if testNames[I] != testNames[I].replace(p, ""):
              dataTime, dataVolt = readCSV(f)
              startTime = findStart(dataTime, dataVolt)
              dataTime -= startTime
              plt.plot(dataTime, dataVolt, color=powerColor[J])
          I += 1
        I = 0
        J += 1
      J = 0
      plotname = name
      print ("Saving " + str(os.path.join(pathName, name + ".png"))) 
      for K in range(len(power)):    
        plt.text(-25,0.4 - 0.1 * K, power[K],color=powerColor[K],fontsize=10)
      plt.xlim(-50,250)
      plt.ylim(-1.5,0.5)
      plt.xlabel("Time (\u03BCs)")
      plt.ylabel("Signal (V)")
      plt.title(name,fontsize=20)
      plt.grid(True, 'major', 'both',color='lightgrey', linestyle='--')
      plt.savefig(os.path.join(pathName, "_MODEL " + name + ".png"), dpi=dpi)
      plt.clf()

if True:
    J = 0
    I = 0
    P = 0
    for p in power[::-1]:
      M = 0
      includeThisModel = []
      for name in testName:
        for f in fileList:
          ID, model, thisPower, sequence = infoFromFileName(f)
         #if testNames[I] != testNames[I].replace(name,""):
          if name == model:
            #if testNames[I] != testNames[I].replace(p, ""):
            if thisPower == p:
              if not model in includeThisModel:
                includeThisModel.append(model)
                print (model)
              dataTime, dataVolt = readCSV(f)
              startTime = findStart(dataTime, dataVolt)
              dataTime -= startTime
              plt.plot(dataTime, dataVolt, color=modelColor[J], linewidth=1,alpha=0.6)
          I += 1
        I = 0
        J += 1
        M += 1
      J = 0
      plotname = p
      #print("Models: " + str(includeThisModel))
      print ("Saving " + str(os.path.join(pathName, p.replace("/","|") + ".png"))) 
      includeThisModel = sorted(includeThisModel)
      for K in range(len(includeThisModel)):
        plt.text(-35, K * 0.06-0.2, includeThisModel[K],color=modelColor[testName.index(includeThisModel[K])],fontsize=5)  
      plt.xlim(-50,250)
      plt.ylim(-1.5,0.5)
      plt.xlabel("Time (\u03BCs)")
      plt.ylabel("Signal (V)")
      plt.title(p + " Power",fontsize=20)
      plt.grid(True, 'major', 'both',color='lightgrey', linestyle='--')
      plt.savefig(os.path.join(pathName, "_POWER " + str(P) + ".png"), dpi=dpi)
      P += 1
      plt.clf()

print("DONE")

