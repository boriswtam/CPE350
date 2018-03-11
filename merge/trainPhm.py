import numpy as np
from keras.models import load_model

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

def initNet():
   INPUT_SIZE = 8
   model = load_model('fullPhmModel.h5')
   model.compile(loss='binary_crossentropy', 
                 optimizer='adam',
                 metrics=['accuracy'])
   return model

#main
fname, inpSize = handleFile()
dataset = np.loadtxt(fname, delimiter=",")  #might need to change to genfromtxt
K.set_image_dim_ordering('tf')

X = dataset[:,0:inpSize]
Y = dataset[:,inpSize]
X = np.expand_dims(X, axis=2) # reshape (10000, 7) to (10000, 7, 1) 

model = initNet()

#change this
model.fit(X, Y, epochs=10, batch_size=10)

scores = model.evaluate(X, Y)
print("\n%s: %.2f%%" % (model.metrics_names[1], scores[1]*100))
model.save("fullPhmModel.h5")
