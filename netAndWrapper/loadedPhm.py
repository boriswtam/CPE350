import sys
import time
import numpy as np
from keras.models import load_model

def runNet(X, model):
   X = np.expand_dims(X, axis=2) # reshape (10000, INPUT_SIZE) to (10000, INPUT_SIZE, 1) 
   k = np.array(X)
   predict = model.predict(k)
   return predict

def initNet():
   INPUT_SIZE = 8
   model = load_model('fullPhmModel.h5')
   model.compile(loss='binary_crossentropy', 
                 optimizer='adam',
                 metrics=['accuracy'])
   return model
