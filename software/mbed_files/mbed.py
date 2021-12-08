from google.colab import drive
drive.mount('/content/Mydrive/')

cd Mydrive/MyDrive/project/

from sklearn.metrics import accuracy_score
from sklearn.naive_bayes import GaussianNB
import matplotlib.pyplot as plt
from joblib import dump
import pandas as pd
import tensorflow as tf

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score
import numpy as np

with tf.device('/device:GPU:0'):
      
  drop_terms =  ['a0c1','a0c2','a1c1','a1c2',	'a2c1',	'a2c2','a3c1','a3c2','a3c0'] # dropping these terms since our setup does not have four ble receivers per anchor
  df1 = pd.read_csv('lab7105_clean.csv', sep=';').drop(['Unnamed: 0'],axis= 'columns')
  df2 = pd.read_csv('lab7106_clean.csv', sep=';').drop(['Unnamed: 0'],axis= 'columns')
  df3 = pd.read_csv('corridor_clean.csv', sep=';').drop(['Unnamed: 0'],axis= 'columns')
  df1 = df1.iloc[0:500, :]
  df2 = df2.iloc[0:500, :]
  df3 = df1.iloc[0:500, :]
  df1.drop( columns = drop_terms, inplace = True) 
  df2.drop( columns = drop_terms, inplace = True) 
  df3.drop( columns = drop_terms, inplace = True) 
  
  df = pd.concat([df1, df2, df3])
  print(df.tail())
  process = 'yes'
  test_size=0.2
  if process=='yes':
      train = []
      test = []
      X_train = pd.DataFrame()
      for posx in df['dist_x'].unique():
          for posy in df['dist_y'].unique():
              chunk = df[(df['dist_x']==posx) & (df['dist_y']==posy)]
              if len(chunk) < 1:
                  continue 
              testitems = int(test_size*len(chunk))
              trainitems= len(chunk)-testitems
              train.append(chunk.iloc[:trainitems])
              test.append(chunk.iloc[-testitems:])
      # Merge components together
      train = pd.concat(train).reset_index(drop=True)
      test  = pd.concat(test).reset_index(drop=True)
     # Create train and test sets
      X_train = train.drop(axis=1, labels=['dist_x', 'dist_y', 'time'])
      y_train = train['dist_x']*10000+train['dist_y']*10
      X_test  = test.drop(axis=1, labels=['dist_x', 'dist_y', 'time'])
      y_test  = test['dist_x']*10000+test['dist_y']*10
      print('Samples train: {}'.format(len(train)))
      print('Samples test:  {}'.format(len(test)))
      
  
  # convert the targets to ordered categorical variables
  y_train = pd.Categorical(y_train,ordered=False)
  y_target = pd.Categorical(y_test,ordered=False)


  X_train = X_train.to_numpy()
  y_train = y_train.to_numpy()
  X_test = X_test.to_numpy()
  y_target = y_test.to_numpy()  
  # # Fit and predict 
  clf= GaussianNB()
  clf.fit(X_train, y_train)
  dump(clf,'model2D_svm_trained.joblib')
  y_predicted = clf.predict(X_test)

  #pp.makelog(clf,y_target, y_predicted, logfile)
  pred = pd.DataFrame(X_test)
  pred['Prediction'] = y_predicted
  pred['True'] = y_target
  pred.to_csv('gaussian.csv')

print(X_train.shape)
print(y_train.shape)

print(clf.class_count_)
print(len(clf.classes_))
print(len(clf.theta_))
print(len(clf.sigma_))
print(len(clf.class_prior_))

print("Parameters")
# Gaussian averages
print("Theta = ",list(np.reshape(clf.theta_,np.size(clf.theta_))))

# Gaussian variances
print("Sigma = ",list(np.reshape(clf.sigma_,np.size(clf.sigma_))))

# Class priors
print("Prior = ",list(np.reshape(clf.class_prior_,np.size(clf.class_prior_))))

print("Epsilon = ",clf.epsilon_)

accuracy_score(y_target, y_predicted)

print(clf.predict([[-0.8, -1, 1]]))

print(df1)
