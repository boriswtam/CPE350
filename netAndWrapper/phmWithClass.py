import numpy as np
import sys
import os.path
from keras.models import Sequential #linear stack of layers
from keras.layers import Dense, Dropout, Activation, Flatten #typical layers used
from keras.layers import Convolution1D, MaxPooling1D #since our data is one dimensional
from keras import backend as K #using tensorflow backend
from keras.utils import np_utils
np.random.seed(7)

def handleFile():
   if(len(sys.argv) < 3):
      sys.exit("Format: python phm.py [filename] [input size]")
   fname = sys.argv[1]
   inpSize = int(sys.argv[2])
   if ".csv" not in fname: 
      sys.exit("Specify a filename with .csv at the end")
   if(os.path.isfile(fname) != True):
      sys.exit("Cannot find file")
   return fname, inpSize

#main
fname, inpSize = handleFile()
K.set_image_dim_ordering('tf')

dataset = np.loadtxt(fname, delimiter=",")  #might need to change to genfromtxt

#16 inputs, 3 outputs (output catagories specified below)
#No Error: 0,0,1
#Unbalanced: 0,1,0
#Increased load: 1,0,0

#for some reason, the .csv file is not matching up with the 
#expected output (Y)
X = dataset[:,0:inpSize]
#probably just taking the last column
Y = dataset[:,[inpSize,inpSize+1,inpSize+2,inpSize+3,inpSize+4,inpSize+5]] 

X = np.expand_dims(X, axis=2) # e.g. reshape (10000, 7) to (10000, 7, 1) 
#Y = np.expand_dims(Y, axis=2)  
#Y = np_utils.to_categorical(Y, 3)
'''
print X.shape
print Y.shape 
print "Increased Load"
print Y[1]
print "Unbalanced"
print Y[45000]
print "No error"
print Y[145000]
'''

#using sequential model so each layer feeds into the next
model = Sequential()

#Must provide an input shape if first layer in the model
#batch size only needs to be specified in "fit" as the net
#should be able to handle any batch size
model.add(Dense(8, activation='relu', input_shape=(inpSize,1)))

model.add(Convolution1D(16, 2, padding="same", activation='relu'))
#output of conv1D is 3 dimensional: batch_size, timesteps, features

model.add(MaxPooling1D())
model.add(Convolution1D(16, 2, padding="valid", activation='relu'))
model.add(MaxPooling1D())
model.add(Dropout(0.25))
model.add(Flatten())

#must be the last layer
model.add(Dense(6, activation='softmax'))
# Compile model
model.compile(loss='categorical_crossentropy', 
              optimizer='adam',
              metrics=['accuracy'])
# Fit the model
#10k training samples, batch_size=10 (number of samples to go thru net)`
#will take 1000 iterations/steps per epoch
model.fit(X, Y, epochs=5, batch_size=10)
# evaluate the model
scores = model.evaluate(X, Y)
print("\n%s: %.2f%%" % (model.metrics_names[1], scores[1]*100))

#save model and weights
model.save("fullPhmModel.h5")

