from datetime import date
from jugaad_data.nse import bhavcopy_save, bhavcopy_fo_save
import time
import os
import matplotlib.pyplot as plt
import pandas
from datetime import datetime,timedelta
import sys
sym=sys.argv[1]
train_sd=(sys.argv[2])
train_ed=(sys.argv[3])

# date_str = '08/02/2024'

# Convert the string to a datetime object
# print(train_sd)
# print(train_ed)
st_date_obj = datetime.strptime(train_sd, '%d/%m/%Y')
st_date_obj = st_date_obj - timedelta(days=3)

end_date_obj = datetime.strptime(train_ed, '%d/%m/%Y')

from jugaad_data.nse import stock_df
df = stock_df(symbol=sym, from_date=st_date_obj, to_date=end_date_obj, series="EQ")
cols = ['PREV. CLOSE', '52W H', 'SYMBOL', 'SERIES', '52W L']
df = df.drop(columns=cols)
output_file_csv = sym+"LR.csv"
df.to_csv(output_file_csv, index=False,sep=',')