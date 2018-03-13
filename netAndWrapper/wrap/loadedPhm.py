import sys
import time
import numpy as np
from keras.models import load_model

def runNet(X, model, lenDataChunk):
   X = np.expand_dims(X, axis=2) # reshape (10000, INPUT_SIZE) to (10000, INPUT_SIZE, 1) 
   k = np.array(X)
   predicts = model.predict(k)
   y_proba = 0
   for singlePredict in np.nditer(predicts):
      y_proba += singlePredict
   y_proba = y_proba / lenDataChunk
   return y_proba 

def initNet():
   model = load_model('weightsAndModel.h5')
   model.compile(loss='binary_crossentropy', 
                 optimizer='adam',
                 metrics=['accuracy'])
   return model
