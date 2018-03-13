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
   #predict = model.predict_classes(k)
   predicts = model.predict(k)
   noErrAve = currAve = tempAve = offBalanceAve = frictionAve = soundAve = 0
   y_proba = np.zeros(6)

   '''
   for singlePredict in np.nditer(predicts):
      for i in singlePredict:
         y_proba[i] += singlePredict
   '''

   print y_proba

   y_class = y_proba.argmax(axis=-1) 
   return y_class

def initNet():
   model = load_model('weightsAndModelWithClass.h5')
   model.compile(loss='categorical_crossentropy', 
                 optimizer='adam',
                 metrics=['accuracy'])
   return model
