import numpy as np
np.random.seed(7)
from keras.models import Sequential #linear stack of layers
from keras.layers import Dense, Dropout, Activation, Flatten #typical layers used
from keras.layers import Convolution1D, MaxPooling1D #since our data is one dimensional
from keras import backend as K #using tensorflow backend
from keras.models import model_from_json #to save and load net and weights
K.set_image_dim_ordering('tf')

dataset = np.loadtxt("faults.csv", delimiter=",")  #might need to change to genfromtxt

#10,000 samples
X = dataset[:,0:7]
Y = dataset[:,7]
X = np.expand_dims(X, axis=2) # reshape (10000, 7) to (10000, 7, 1) 

#using sequential model so each layer feeds into the next
model = Sequential()

model.add(Dense(12, activation='relu', input_shape=(7,1)))

#Must provide an input shape if first layer in the model
#batch size only needs to be specified in "fit" as the net
#should be able to handle any batch size
model.add(Convolution1D(32, 2, padding="valid", activation='relu'))
#output of conv1D is 3 dimensional: batch_size, timesteps, features

model.add(MaxPooling1D())
model.add(Convolution1D(32, 2, padding="valid", activation='relu'))
model.add(MaxPooling1D())
model.add(Dropout(0.25))
model.add(Flatten())

#must be the last layer
model.add(Dense(1, activation='sigmoid'))
# Compile model
model.compile(loss='binary_crossentropy', 
              optimizer='adam',
              metrics=['accuracy'])
# Fit the model
#10k training samples, #batch_size=10 (number of samples to go thru net), will take 1000 iterations/steps per epoch for 1 epoch
model.fit(X, Y, epochs=10, batch_size=10)
# evaluate the model
scores = model.evaluate(X, Y)
print("\n%s: %.2f%%" % (model.metrics_names[1], scores[1]*100))

'''
#serialize model to json to save the weights
jsonModel = model.to_json()
with open('phmModel.json', 'w') as jsonPhmFile:
   jsonPhmFile.write(jsonModel)
#serialize weights to HDF5
model.save_weights("phmModel.h5")

#load json and create model
jsonPhmFile = open('phmModel.json','r')
loadedPhmJsonModel = jsonPhmFile.read()
jsonPhmFile.close()
loadedPhmModel = model_from_json(loadedPhmJsonModel)
#load weights into model
loadedPhmModel = load_weights("phmModel.h5")
'''
