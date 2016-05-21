# -*- coding: utf-8 -*-
# @Author: mengcz13
# @Date:   2016-05-20 01:48:46
# @Last Modified by:   mengcz13
# @Last Modified time: 2016-05-20 10:01:27

import naive_bayes_classifier as nbc
import os
import random
import sys

def build_set(setfile):
	allset = []
	classlist = []
	with open(setfile, 'r') as f:
		lines = f.readlines()
		for line in lines:
			line = line.split()
			classlist.append(line[0])
			filelist = os.listdir(line[1])
			for fn in filelist:
				with open(os.path.join(line[1], fn), 'r') as fin:
					allset.append(nbc.word_parser(fin.read(), line[0]))
	random.shuffle(allset)
	return (allset, classlist)


def cross_validation_n_1(setbuilt, n):
	allset = setbuilt[0]
	classlist = setbuilt[1]
	vasize = len(allset) // n;
	for i in range(0, len(allset), vasize):
		validset = allset[i:i+vasize-1]
		trainset = [sam for sam in allset if sam not in validset]
		nbcc = nbc.naive_bayes_classifier(classlist, trainset, validset)
		nbcc.train()
		print 'rate of %d patch: %f' % (i // vasize, nbcc.validate())


if __name__ == '__main__':
	cross_validation_n_1(build_set(sys.argv[1]), int(sys.argv[2]))