'''
Created on May 31, 2013

@author: mmrahman
'''
import sys

if len(sys.argv) != 2: #program_name and node edge outFile
    sys.exit("usage: terget_achived_plot.py inFile")
    
f_in=open(sys.argv[1],'r')
f_out=open(sys.argv[1]+'.plot','w')


terget = dict()
achived = dict()

f_in.readline()
f_in.readline()
f_in.readline()
f_in.readline()
f_in.readline()
f_in.readline()
f_in.readline()
f_in.readline()

totalweight = 0 
for line in f_in:
    splits = line.split('\t')
    
    if len(splits) !=2:
        break
    
    splits2 = splits[1].split('\n')
    if splits2[0] in terget:
        print('error! '+line)
    else:
        terget[splits2[0]] = float(splits[0])
        totalweight+=float(splits[0])
        achived[splits2[0]] = 0
print('totalweight '+str(totalweight))
        
sample = 0 
for line in f_in:
    splits = line.split()
    if len(splits) != 1:
        continue
    splits = line.split('\n')
    achived[splits[0]]+=1
    sample += 1
print('totalsample '+str(sample))

for x in terget:
    f_out.write(str(terget[x]/totalweight)+','+str(float(achived[x])/float(sample))+'\n')

    