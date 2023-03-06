import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
import re

def OutliersIQR(data):
    q1, q3 = np.quantile(data, [0.25, 0.75])
    iqr = q3 - q1
    lower, upper = q1 - 1.5 * iqr, q3 + 1.5 * iqr
    return data[(data < lower) | (data > upper)].index

df = pd.read_csv('./user_data.csv')

df = df.dropna()
df = df.applymap(lambda x: "0" if (re.fullmatch(r'\d+|\d+岁',x) is None) else x)
df = df.applymap(lambda x: int(x[:-1]) if (re.fullmatch(r'\d+',x) is None) else int(x))

df = df.drop(df[ (df['年龄'] <= 0) | (df['年龄']>125) ].index)
df = df.drop(df[ df['平均每日使用时长(分钟)']>24*60 ].index)

df['temp'] = df['平均每日使用时长(分钟)']/df['年龄']
df_IQR = df.drop(OutliersIQR(df['temp']))
#df = df.drop(OutliersZscore(df['temp']))

print(df_IQR.size//3, 'person =', df_IQR.corr()['年龄']['平均每日使用时长(分钟)'])

x = np.array(df_IQR['年龄']).reshape(-1,1)
y = np.array(df_IQR['平均每日使用时长(分钟)']).reshape(-1,1)

modelLR = LinearRegression()
modelLR.fit(x,y)
yFit = modelLR.predict(x)

ax = plt.subplot()
ax.plot(x, y, 'o', label='raw')
ax.plot(x, yFit, 'r-', label='OLS')
ax.legend(loc='best')

plt.show()

#plt.figure(dpi=120)

#plt.scatter(x,y,s=7)
#plt.scatter(x,yFit,s=7,r='r ')