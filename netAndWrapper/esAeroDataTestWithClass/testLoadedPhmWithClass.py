import sys
import time
import numpy as np
from keras.models import load_model

def runNet(X, model):
   X = np.expand_dims(X, axis=2) # reshape (10000, INPUT_SIZE) to (10000, INPUT_SIZE, 1) 
   k = np.array(X)
   #predict = model.predict(k)
   #y_proba = model.predict(k)
   #y_classes = y_proba.argmax(axis=-1) 
   predict = model.predict_classes(k)
   return predict

def initNet():
   model = load_model('esAeroDataNetWithClass.h5')
   model.compile(loss='categorical_crossentropy', 
                 optimizer='adam',
                 metrics=['accuracy'])
   return model
