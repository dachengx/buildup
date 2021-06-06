import os
import csv
import argparse

import numpy as np

psr = argparse.ArgumentParser()
psr.add_argument('-o', dest='opt', type=str, help='output file')
psr.add_argument('ipt', type=str, help='input file')
args = psr.parse_args()

filelist = os.listdir(args.ipt)
filelist = [f for f in filelist if f[0] != '.' and os.path.splitext(f)[-1] == '.csv' and f != args.opt]
text = [os.path.splitext(f)[0].split('-') for f in filelist]

csvf = open(args.opt, 'w')
wr = csv.writer(csvf)

for t, f in zip(text, filelist):
    fi = open(args.ipt + f, 'r')
    it = csv.reader(fi)
    l = [t + row for row in it]
    for li in l:
        wr.writerow(li)
    fi.close()

csvf.close()