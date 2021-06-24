'''
Small and simple code to demonstrate *How to get Model HyperParameters*
'''
import pandas as pd
from sklearn.datasets import load_diabetes
from sklearn.model_selection import train_test_split# load regression dataset
from sklearn.linear_model import LinearRegression# Train model
#lr = LinearRegression().fit([[3],[4],[5],[6],[7]],[2,3,4,5,6])
lr = LinearRegression().fit([[1],[2],[3],[4],[5]],[2,3,4,5,6])
params = lr.get_params()
print("=========================")
print(params)
print("======= Model HyperParameters ==========")
print(lr.coef_,lr.intercept_)
print("=========================")
print(lr.predict([[4.5]]))
print("=========================")
print(lr)

