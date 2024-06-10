from datetime import date
from jugaad_data.nse import bhavcopy_save, bhavcopy_fo_save
import time
import os
import matplotlib.pyplot as plt
import pandas
from datetime import datetime,timedelta
import sys

sym=sys.argv[1]
sd=(sys.argv[2])
ed=(sys.argv[3])
id = sys.argv[4]
st_date_obj = datetime.strptime(sd, '%d/%m/%Y')
st_date_obj = st_date_obj - timedelta(days=3)
end_date_obj = datetime.strptime(ed, '%d/%m/%Y')
from jugaad_data.nse import stock_df
df = stock_df(symbol=sym, from_date=st_date_obj, to_date=end_date_obj, series="EQ")
cols = ['PREV. CLOSE', '52W H', 'SYMBOL', 'SERIES', '52W L']
df = df.drop(columns=cols)
output_file_csv = f"./{sym}" +id+".csv"
df.to_csv(output_file_csv, index=False,sep=',')