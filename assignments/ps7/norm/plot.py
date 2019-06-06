# 
#  This file is part of the course materials for AMATH483/583 at the University of Washington,
#  Spring 2017
# 
#  Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
#  https://creativecommons.org/licenses/by-nc-sa/4.0/
# 
#  Author: Andrew Lumsdaine
# 

import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

import csv
import sys
import os
names = []

plt.figure(figsize=(9,4))

for name in sys.argv[1:]:

    infile = name

    names.append(os.path.basename(name).replace(".txt",""))

    print ('plotting ' + infile)

    with open(infile, 'r') as csvfile:
        Ns = []
        Times = []
        
        reader = csv.DictReader(csvfile, delimiter='\t')
    
        for row in reader:
            Ns.append(int(row['N']))
            Times.append(float(row['time']))
        

        plt.semilogy(Ns, Times)


pp = PdfPages('time.pdf')


#plt.axis([0, 2048, .01, 24])
plt.legend(names, loc='upper left', fontsize='small')
plt.title('Euclidean Norm Computation')
plt.xlabel('Problem Size')
plt.ylabel('Execution Time')
plt.xticks([22, 23, 24, 25, 26, 27],
           [22, 23, 24, 25, 26, 27])


pp.savefig()
pp.close()
plt.show()
