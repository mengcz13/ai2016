# -*- coding: utf-8 -*-
# @Author: neozero
# @Date:   2016-05-19 21:01:26
# @Last Modified by:   mengcz13
# @Last Modified time: 2016-05-20 13:19:30
import re
import os
import math
import random
from collections import Counter


def word_parser(text, tclass):
	return ([s.lower() for s in re.findall(r'\w+', text) if re.search(r'[^a-zA-Z]', s) == None], tclass)


# Naive Bayes Classifier for text.
# input: trainset, validateset
# input format: list[(<list of words>, <class>)]
class naive_bayes_classifier(object):
	def __init__(self, classlist, trainset, validateset):
		self.classlist = classlist
		self.trainset = trainset
		self.validset = validateset
		self.counterlist = dict()
		for tclass in classlist:
			self.counterlist[tclass] = Counter()
		self.tclasscounter = []
		self.defaultlist = dict()
	
	def train(self):
		for (wlist, tclass) in self.trainset:
			self.counterlist[tclass] += Counter(wlist)
		self.tclasscounter = Counter([x[1] for x in self.trainset])
		self.vocab = set(reduce(lambda lx,ly:lx+ly, [x[0] for x in self.trainset]))
		cnum = len(self.trainset)
		for (tclass, num) in self.tclasscounter.items():
			self.tclasscounter[tclass] = num * 1.0 / cnum
		self.vocabsize = len(self.vocab)
		for (tclass, wcount) in self.counterlist.items():
			wnum = sum(wcount.values())
			underdiv = wnum + self.vocabsize
			for (key, value) in wcount.items():
				wcount[key] = (value + 1) * 1.0 / underdiv
			self.counterlist[tclass] = wcount
			self.defaultlist[tclass] = 1.0 / underdiv


	def classify(self, wlist):
		estip = [math.log(self.tclasscounter[tclass]) + sum([math.log(self.cdp(w, tclass)) for w in wlist if w in self.vocab]) for tclass in self.classlist]
		return self.classlist[estip.index(max(estip))]

	def cdp(self, word, tclass):
		wcount = self.counterlist[tclass]
		if word in wcount:
			return wcount[word]
		elif word in self.vocab:
			return self.defaultlist[tclass]
		else:
			return 1

	def validate(self):
		tags = [x[1] for x in self.validset]
		classtags = [self.classify(x[0]) for x in self.validset]
		compare = map(lambda x, y:1 if x==y else 0, tags, classtags)
		return float(sum(compare)) / float(len(self.validset))


if __name__ == '__main__':
	pass