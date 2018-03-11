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

'''
falsePositive = 0
falseCount = 0
for x in np.nditer(predict):
   if x > 0.5:
      falsePositive += 1
   else:
      falsePositive = 0

   if falsePositive == 10:
      falsePositive = 0
      falseCount += 1
print(falseCount)
'''
