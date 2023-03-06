from operator import invert
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import re

a = np.array([ [1,2,1],[2,4,1],[3,6,1] ])

print(np.linalg.inv(a))