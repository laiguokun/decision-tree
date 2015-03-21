#get the data from test
attr = [0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1]
tot = len(attr)

attr_dex = []
for i in range(0, tot):
	attr_dex.append({})

fin_attr = open("data\\attr.pre", "r");
cnt = 0;
for line in fin_attr:
	attrs = line[0:-2].split(' ');
	tmp = 0;
	for word in attrs:
		attr_dex[cnt][word] = tmp;
		tmp += 1;
	cnt += 1;

fin = open("data\\adult.test", "r")
fout = open("data\\pre1.test", "w")

for line in fin:
	attrs = line[0:-1].split(',');
	result = []
	out = 0
	if (attrs[tot] == "<=50K."):
		out = 0
	else:
		out = 1
		
	for i in range(0, tot):
		if (attr[i] == 0 or attrs[i] == '?'):
			result.append(attrs[i])
		else:
			if (not(attrs[i] in attr_dex[i])):
				print ('fuck');
				result.append('?');
			else:
				result.append(attr_dex[i][attrs[i]]);
			

	for i in range(0,tot):
		fout.write(str(result[i])+" ");
	fout.write(str(out)+'\n');
	
