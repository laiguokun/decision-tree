#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#define datafile "data//vaild.data"
#define testfile "data//pre2.test"

//const int[] attr = {0, 8, 0, 16, 0, 7, 14, 6, 5, 2, 0, 0, 0, 41}
//const int attr_num = 14;
using namespace std;

struct User{
	int attr[attr_num+1];
};

struct Note{
	int son_num;
	note** sonList;
	int attr;
	int attr_cont;
	public Note(int num)
	{
		son_num = num;
		sonList = new Note*[num];
		for (int i = 0;i < num; i++)
			sonList[i] = new Note;
	}
};

vector<User>* user;
void get_input()
{
	ifstream fin(datafile);
	int age;
	user = new vector<User>;
	while (fin << age)
	{
		User new_user;
		new_user.attr[0] = age;
		for (int i = 1; i <= attr_num; i++)
			fin << new_user.attr[i];
		user->push_back(new_user);
	}
}

Note* root;

Note* buildTree(vector<User>* user, double entropy)
{
	double delta = 0;
	int choose = 0;
	double sum = (*user)->size();
	for (int i = 0; i < attr_num; i++)
	{
		//continue attribute
		if (attr[i] == 0)
		{
		}
		//discrete attribute
		if (attr[i] != 0)
		{
			int* cnt = new int[attr[i]];
			int* ss = new int[attr[i]];
			for (int j = 0; j < attr[j]; j++)
			{
				cnt[j] = 0;
				ss[j] = 0;
			}
			for (int j = 0; j < sum; j++)
			{
				ss[(*user)[j]->attr[i]] ++;
				if ((*user)[j]->attr[attr_num] == 0)
					cnt[(*user)[j]->attr[i]] ++;
			}
			double tmp = 0;
			for (int j = 0; j < attr[j]; j++)
			{
				double frac = (double)cnt[j]/ss[j];
				tmp += ((double)ss[j]/sum) * 
						(frac * (log(frac)/log(2)) + (double)(1-frac) * (log(1-frac)/log(2)));
			}
			if (entropy - tmp > delta)
			{
				delta = entropy - tmp;
				choose = i;
			}
		}
	}

	if (delta < 1e-3 or sum < threshold)
		return new Node(0);
	if (attr[choose]>0)
	{
		Node* result = new Node(attr[choose]);
		result->attr = choose;
		int* cnt = new int[attr[choose]];
		int* ss = new int[attr[choose]];
		vector<User>** separate = new vector<User>*[attr[choose]];
		for (int j = 0; j < attr[choose]; j++)
		{
			cnt[j] = 0;
			ss[j] = 0;
		}
		for (int j = 0; j < sum; j++)
		{
			separate[(*user)[j]->attr[choose]]->push_back((*user)[j]); 
			ss[(*user)[j]->attr[choose]] ++;
			if ((*user)[j]->attr[attr_num] == 0)
				cnt[(*user)[j]->attr[choose]] ++;
		}
		for (int j = 0; j < attr[choose]; j++)
		{
			double frac = (double)cnt[j]/ss[j];
			double next_entropy = frac * log(frac)/log(2) + (1-frac) * log(1-frac)/log(2);
			result->sonList[j] = buildTree(separate[j], next_entropy);
		}
		return result;
	}	
}

void train()
{
	root = buildTree(user)
}

int main()
{
	get_input();
	train();
//	test();
}