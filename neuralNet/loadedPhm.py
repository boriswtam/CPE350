from keras.models import model_from_json #to save and load net and weights
import numpy as np
INPUT_SIZE = 16
#num_lines = sum(1 for line in open('sampleRun.csv'))
#print num_lines
dataset = np.loadtxt("smallTest.csv", delimiter=",")  #might need to change to genfromtxt
#send dataset as array
jsonPhmFile = open('phmModel.json','r')
loadedPhmJsonModel = jsonPhmFile.read()
jsonPhmFile.close()
model = model_from_json(loadedPhmJsonModel)
#load weights into model
model.load_weights("phmModel.h5")
# Compile model
model.compile(loss='binary_crossentropy', 
              optimizer='adam',
              metrics=['accuracy'])

#keep this running in a loop
#dataset = data sent
X = dataset[:,0:INPUT_SIZE]
print X
Y = dataset[:,INPUT_SIZE]
X = np.expand_dims(X, axis=2) # reshape (10000, INPUT_SIZE) to (10000, INPUT_SIZE, 1) 
k = np.array(X)
predict = model.predict(k)
for x in np.nditer(predict):
   if x > 0.5:
      print("Error")
   else:
      print("No Error")

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

#scores = model.evaluate(X, Y)
#print("\n%s: %.2f%%" % (model.metrics_names[1], scores[1]*100))
