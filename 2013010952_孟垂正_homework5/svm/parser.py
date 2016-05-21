# -*- coding: utf-8 -*-
# @Author: neozero
# @Date:   2016-05-20 11:31:07
# @Last Modified by:   mengcz13
# @Last Modified time: 2016-05-20 13:27:43
import re
import os
import sys
import math
import random
from collections import Counter


def word_parser(text):
	return [s.lower() for s in filter(lambda s: re.search(r'[^a-zA-Z]', s) == None, re.findall(r'\w+', text))]


def generate_dataset(filein, fileout):
	classlist = []
	filedir = []
	with open(filein, 'r') as f:
		for line in f.readlines():
			line = line.split()
			classlist.append(line[0])
			filedir.append(line[1])
	idfvocab = Counter()
	samplelist = []
	filenum = 0
	for classno in range(0, len(classlist)):
		filelist = os.listdir(filedir[classno])
		filenum += len(filelist)
		for file in filelist:
			with open(os.path.join(filedir[classno], file), 'r') as f:
				wordlistoffile = word_parser(f.read())
				samplelist.append((Counter(wordlistoffile), classno))
				idfvocab += Counter(set(wordlistoffile))
	for (key, value) in idfvocab.items():
		idfvocab[key] = math.log(float(filenum) / value)
	vocabulary = list(idfvocab.keys())
	vocabdict = dict()
	for rank in range(0, len(vocabulary)):
		vocabdict[vocabulary[rank]] = rank+1
	for sample in samplelist:
		for (key, value) in sample[0].items():
			sample[0][key] = (1 + math.log(value)) * idfvocab[key]
	random.shuffle(samplelist)
	with open(fileout, 'w') as f:
		for sample in samplelist:
			f.write('%d ' % (sample[1]))
			keylist = []
			for (key, value) in sample[0].items():
				keylist.append(vocabdict[key])
			keylist = sorted(keylist)
			for keyn in keylist:
				f.write('%d:%f ' % (keyn, sample[0][vocabulary[keyn-1]]))
			f.write('\n')




if __name__ == '__main__':
	generate_dataset(sys.argv[1], sys.argv[2])
	os.system('./svm-train -v %d %s' % (int(sys.argv[3]), sys.argv[2]))