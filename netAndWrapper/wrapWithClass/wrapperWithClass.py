import time
import signal
import numpy as np
import loadedPhmWithClass
import sys
import os.path
def exit_gracefully(signal, frame):
   sys.exit("")

def follow(thefile):
   thefile.seek(0,2)
   while True:
      lines = thefile.readlines()
      if not lines:
         time.sleep(1)
         continue
      yield lines #return the generator

def handleFile():
   if(len(sys.argv) < 2):
      sys.exit("Error: Specify file name\nthat data will be written to as second argument")
   fname = sys.argv[1]
   if(os.path.isfile(fname) != True):
      print("File not found. Creating new file with specified name")
      if ".csv" not in fname: 
         fname = fname + '.csv'
      open(fname,"w+")
   return fname

def initializeNet():
   model = loadedPhmWithClass.initNet()
   os.system('clear')
   print("Ready to begin data analysis")
   return model


if __name__ == '__main__':
   signal.signal(signal.SIGINT, exit_gracefully)

   fname = handleFile()
   model = initializeNet()

   #get data from file
   logfile = open(fname,"r")
   loglines = follow(logfile)

   i = 0
   for dataChunk in loglines:
      dataset = []
      for line in dataChunk:
         dataset.append(np.fromstring(line, sep=","))
      results = loadedPhmWithClass.runNet(dataset, model)
      print (results)
      if i == 10:
         os.system('clear')
         i = 0
      i+=1
