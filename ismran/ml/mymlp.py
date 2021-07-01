import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
import sys
from sklearn.neural_network import MLPRegressor
from sklearn.datasets import make_regression
import matplotlib.pyplot as plt
#model = MLPRegressor(random_state=1, max_iter=100)

def Inspect(estimator_reg,train_data,train_labels,numOfEpochs=10):
	train_data=train_data.to_numpy()
	train_labels=train_labels.to_numpy()
	""" Home-made mini-batch learning
	    -> not to be used in out-of-core setting!
	"""
	N_TRAIN_SAMPLES = train_data.shape[0]
	N_EPOCHS = int(numOfEpochs)
	N_BATCH = 128


	scores_train = []
	scores_test = []

	# EPOCH
	epoch = 0
	epochList=[]
	while epoch < N_EPOCHS:
		epochList.append(epoch)
		print('epoch: ', epoch)
		# SHUFFLING
		random_perm = np.random.permutation(train_data.shape[0])
		mini_batch_index = 0
		while True:
			# MINI-BATCH
			indices = random_perm[mini_batch_index:mini_batch_index + N_BATCH]
			estimator_reg.partial_fit(train_data[indices], train_labels[indices])
			mini_batch_index += N_BATCH

			if mini_batch_index >= N_TRAIN_SAMPLES:
				break

		# SCORE TRAIN
		score_val=estimator_reg.score(train_data, train_labels)
		print("Score Value for epoch : "+str(epoch)+" : "+str(score_val))
		scores_train.append(score_val)

		# SCORE TEST
		#scores_test.append(estimator_reg.score(test_data, test_labels))

		epoch += 1

	""" Plot """
	fig, ax = plt.subplots(2, sharex=True, sharey=True)
	ax[0].plot(epochList,scores_train,marker='o')
	ax[0].set_title('Train')
	ax[1].plot(scores_test)
	ax[1].set_title('Test')
	fig.suptitle("Accuracy over epochs", fontsize=14)
	plt.show()
