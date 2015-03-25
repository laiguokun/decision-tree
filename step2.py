#eliminate "?" in the data base
import random;
attr = [0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1]
tot = len(attr)

fin = open("data\\pre1.data", "r")
fout = open("data\\pre2.data", "w")

att = []
attr_cnt = []
for i in range(0,tot):
	attr_cnt.append({});
	att.append(0);

for line in fin:
	attrs = line[0:-1].split(' ');

	for i in range(0, tot):
		if (not(attr[i] == 0 or attrs[i] == '?')):
			if (not(attrs[i] in attr_cnt[i])):
				attr_cnt[i][attrs[i]] = 0;
			else:
				attr_cnt[i][attrs[i]] += 1;
				att[i]+=1;

attr_sub = [];
for i in range(0,tot):
	attr_sub.append(0);
	if (attr[i] == 1):
		tmp = 0;
		for j in attr_cnt[i]:
			if (attr_cnt[i][j] > tmp):
				attr_sub[i] = j;
				tmp = attr_cnt[i][j];
		print(str(i)+" "+str(attr_sub[i]));

fin = open("data\\pre1.data", "r")
fout = open("data\\pre2.data", "w")
for line in fin:
	attrs = line[0:-1].split(' ');
	result = []
	out = 0
	if (attrs[tot] == "0"):
		out = 0
	else:
		out = 1
		
	for i in range(0, tot):
		if (attr[i] == 0 or attrs[i] == '?'):
			if (attr[i] == 0):
				result.append(attrs[i])
			else:
				result.append(attr_sub[i]);
#				x = random.random();
#				tmp = 0;
#				for j in attr_cnt[i]:
#					tmp += float(attr_cnt[i][j])/float(att[i]);
#					if (tmp > x):
#						result.append(j);
#						break;
		else:
			result.append(attrs[i]);
			
	for i in range(0,tot):
		fout.write(str(result[i])+" ");
	fout.write(str(out)+'\n');

fin = open("data\\pre1.test", "r")
fout = open("data\\pre2.test", "w")
for line in fin:
	attrs = line[0:-1].split(' ');
	result = []
	out = 0
	if (attrs[tot] == "0"):
		out = 0
	else:
		out = 1
		
	for i in range(0, tot):
		if (attr[i] == 0 or attrs[i] == '?'):
			if (attr[i] == 0):
				result.append(attrs[i])
			else:
				result.append(attr_sub[i]);
		else:
			result.append(attrs[i]);
			
	for i in range(0,tot):
		fout.write(str(result[i])+" ");
	fout.write(str(out)+'\n');