import csv
import sys

error = [0]
errorValue = int(sys.argv[2])
# error
if errorValue == 1:
   error[0] = 1
   
with open(sys.argv[1], "rb") as source:
   rdr= csv.reader( source )
   with open(sys.argv[1].replace(".csv", "_labeled.csv") ,"wb") as result:
      wtr= csv.writer( result )
      for r in rdr:
         wtr.writerow((r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7], error[0]))
