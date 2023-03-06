import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import re

plt.rcParams["font.sans-serif"]=["SimHei"] 
plt.rcParams["axes.unicode_minus"]=False
# 使得图表可以正确显示中文

df = pd.read_csv('./user_data.csv')

def check_num(x):
    if pd.isnull(x):
        return np.nan
    elif re.fullmatch(r'\d+|\d+岁',x) is None:
        # 发现存在一组数据为 ‘36岁’ 故特判以下
        return np.nan
    elif re.fullmatch(r'\d+',x) is None:
        return int(x[:-1])
    else:
        return int(x)

df = df.applymap(check_num)

def age_check_legal(x):
    if pd.isnull(x):
        return np.nan
    elif x<=0 or x>125:
        return np.nan
    else:
        return x

def time_check_legal(x):
    if pd.isnull(x):
        return np.nan
    elif x<0 or x>24*60:
        return np.nan
    else:
        return x

df['年龄'] = df['年龄'].apply(age_check_legal)
df['平均每日使用时长(分钟)'] = df['平均每日使用时长(分钟)'].apply(time_check_legal)

'''
print(f'nan in age is {df["年龄"].isnull().sum()/df["年龄"].size}%')
# nan in age is 0.007%

df['年龄'].hist(bins = 120, stacked = True, density = True, alpha = 0.6, color = 'teal')
df['年龄'].plot(kind = 'density', color = 'teal')
plt.xlim(-10,120)
plt.xlabel('年龄')
plt.show()
'''

age_median = np.median(df['年龄'].dropna())

def age_fill(x):
    if pd.isnull(x):
        return age_median
    else:
        return x

df['年龄'] = df['年龄'].apply(age_fill)

'''
print(f'median of age is {age_median}')
# median of age is 25.0

df['年龄'].hist(bins = 120, stacked = True, density = True, alpha = 0.6, color = 'teal')
df['年龄'].plot(kind = 'density', color = 'teal')
plt.xlim(-10,120)
plt.xlabel('年龄')
plt.show()
'''

'''
print(f'nan in time is {df["平均每日使用时长(分钟)"].isnull().sum()/df["平均每日使用时长(分钟)"].size}%')
# nan in time is 0.007%

df['平均每日使用时长(分钟)'].hist(bins = 120, stacked = True, density = True, alpha = 0.6, color = 'teal')
df['平均每日使用时长(分钟)'].plot(kind = 'density', color = 'teal')
plt.xlim(-10,126)
plt.xlabel('平均每日使用时长(分钟)')
plt.show()
'''

time_median = np.median(df['平均每日使用时长(分钟)'].dropna())

def time_fill(x):
    if pd.isnull(x):
        return time_median
    else:
        return x

df['平均每日使用时长(分钟)'] = df['平均每日使用时长(分钟)'].apply(time_fill)

'''
print(f'median of age is {time_median}')
# median of age is 59.0

df['平均每日使用时长(分钟)'].hist(bins = 120, stacked = True, density = True, alpha = 0.6, color = 'teal')
df['平均每日使用时长(分钟)'].plot(kind = 'density', color = 'teal')
plt.xlim(-10,126)
plt.xlabel('平均每日使用时长(分钟)')
plt.show()
'''

'''
plt.scatter(df['年龄'],df['平均每日使用时长(分钟)'],s=7,c='teal')
plt.ylabel('平均每日使用时长(分钟)')
plt.xlabel('年龄')
plt.show()
'''

def outliers_Zscore(data):
    avg = np.mean(data)
    std = np.std(data)
    Zsocre = np.abs((data-avg)/data)
    return data[Zsocre > 3].index

df['temp'] = df['平均每日使用时长(分钟)']/df['年龄']
df_Zscore = df.drop(outliers_Zscore(df['temp']))

'''
print(f'left {df_Zscore.size//3} pairs of data')
print(f'pearson = {df_Zscore.corr()["年龄"]["平均每日使用时长(分钟)"]}')
# left 991 pairs of data
# pearson = 0.7615906197145329

plt.scatter(df['年龄'],df['平均每日使用时长(分钟)'],s=7,c='maroon')
plt.scatter(df_Zscore['年龄'],df_Zscore['平均每日使用时长(分钟)'],s=7,c='teal')
plt.ylabel('平均每日使用时长(分钟)')
plt.xlabel('年龄')
plt.show()
'''

def outliers_IQR(data):
    q1, q3 = np.quantile(data, [0.25, 0.75])
    iqr = q3 - q1
    lower, upper = q1 - 1.5 * iqr, q3 + 1.5 * iqr
    return data[(data < lower) | (data > upper)].index

df_IQR = df.drop(outliers_IQR(df['temp']))

print(f'left {df_IQR.size//3} pairs of data')
print(f'pearson = {df_IQR.corr()["年龄"]["平均每日使用时长(分钟)"]}')
#left 969 pairs of data
#pearson = 0.7792382078305893

plt.scatter(df['年龄'],df['平均每日使用时长(分钟)'],s=7,c='maroon')
plt.scatter(df_IQR['年龄'],df_IQR['平均每日使用时长(分钟)'],s=7,c='teal')
plt.ylabel('平均每日使用时长(分钟)')
plt.xlabel('年龄')
plt.show()