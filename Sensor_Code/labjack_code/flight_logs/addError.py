import csv
import sys

error = [0, 0, 0, 0, 0, 0]
errorValue = int(sys.argv[2])
# No error
if errorValue == 0:
   error[0] = 1
# Low Current
elif errorValue == 1:
   error[1] = 1
# High Temp
elif errorValue == 2:
   error[2] = 1
# Off Balance
elif errorValue == 3:
   error[3] = 1
# Friction
elif errorValue == 4:
   error[4] = 1
# Loud Sound
elif errorValue == 5:
   error[5] = 1
# Loud Sound Low Current
elif errorValue == 6:
   error[5] = 1
   error[1] = 1
# Loud Sound High Temp
elif errorValue == 7:
   error[5] = 1
   error[2] = 1
   



with open(sys.argv[1], "rb") as source:
   rdr= csv.reader( source )
   with open(sys.argv[1].replace(".csv", "_error.csv") ,"wb") as result:
      wtr= csv.writer( result )
      for r in rdr:
         wtr.writerow( (r[0], r[1], r[2], r[3], r[4], r[5], r[6], error[0], error[1], error[2], error[3], error[4], error[5]) )
