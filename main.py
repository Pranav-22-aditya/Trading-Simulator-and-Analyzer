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
n=int(sys.argv[4])
id = sys.argv[5]
# date_str = '08/02/2024'

# Convert the string to a datetime object
st_date_obj = datetime.strptime(sd, '%d/%m/%Y')
st_date_obj=st_date_obj-timedelta(days=max(3*n,15))

end_date_obj = datetime.strptime(ed, '%d/%m/%Y')

from jugaad_data.nse import stock_df
df = stock_df(symbol=sym, from_date=st_date_obj, to_date=end_date_obj, series="EQ")
cols = [ '52W H', 'SYMBOL', 'SERIES', '52W L', 'VWAP']
df = df.drop(columns=cols)
output_file_csv = f"./{sym}"+id+".csv"
df.to_csv(output_file_csv, index=False,sep=',')