import sys
import time
import numpy as np
from keras.models import load_model

def runNet(X, model):
   X = np.expand_dims(X, axis=2) 
   k = np.array(X)
   predicts = model.predict(k)
   y_proba = np.zeros(6)
   i = 0
   for singlePredict in np.nditer(predicts):
      y_proba[i % 6] += singlePredict
      i += 1
   y_proba = y_proba / 10
   y_class = y_proba.argmax(axis=-1) 
   return y_class 

def initNet():
   model = load_model('weightsAndModelWithClass.h5')
   model.compile(loss='categorical_crossentropy', 
                 optimizer='adam',
                 metrics=['accuracy'])
   return model
