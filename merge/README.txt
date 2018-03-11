Created by Mitchell Myjak 3/11/18

Instructions for training neural net:

Store all acquired csv files in a folder in the current directory

Run the concatenation of files shell script, ./catFlights.sh
To do this type:
./catFiles.sh <folder made in first step> <destination csv>
If this does not execute, type:
chmod +x catFiles.sh
and run again

Now run the training neural net, phm.py
To do this type:
python phm.py <destination csv from previous step> <number of inputs>
