#get data from dataset
attr = [0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1]
tot = len(attr)

attr_dex = []
for i in range(0, tot):
	attr_dex.append({})

fin = open("data\\adult.data", "r")
fout = open("data\\pre1.data", "w")
fout_attr = open("data\\attr.pre", "w");
cnt = 0;
for line in fin:
	attrs = line[0:-1].split(',');
	result = []
	out = 0
	if (attrs[tot] == "<=50K."):
		out = 0
	else:
		out = 1
		
	for i in range(0, tot):
		if (attrs[i] == '?'):
			cnt += 1;
		if (attr[i] == 0 or attrs[i] == '?'):
			result.append(attrs[i])
		else:
			if (not(attrs[i] in attr_dex[i])):
				attr_dex[i][attrs[i]] = len(attr_dex[i]) + 1;
			result.append(attr_dex[i][attrs[i]]);

	for i in range(0,tot):
		fout.write(str(result[i])+" ");
	fout.write(str(out)+'\n');
	
print(cnt);
fout2 = open("attr-num.data","w");
for i in range(0,tot):
	if (attr[i] != 0):
		fout2.write(str(len(attr_dex[i])));
		for j in attr_dex[i]:
			fout_attr.write(j + " ");
	fout_attr.write('\n');
	fout2.write('\n');
	
