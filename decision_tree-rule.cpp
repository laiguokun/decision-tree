#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#define datafile "data//pre2.data"
#define vaildfile "data//vaild.data"
#define testfile "data//pre2.test"
using namespace std;

const int attr[] = {0, 8, 0, 16, 0, 7, 14, 6, 5, 2, 0, 0, 0, 41};
const int attr_num = 14;
//const int attr[] = {3, 0, 0, 2};
//const int attr_num = 4;
int threshold = 100;
int tot_id = 0;



struct User{
	int attr[attr_num+1];
};

struct Node{
public:
	int son_num;
	Node** sonList;
	int attr;
	int attr_num;
	int id;
	int result;
	Node(int num, int res = 0)
	{
		son_num = num;
		sonList = new Node*[num];
		id = tot_id;
		tot_id ++;
		result = res;
		attr = 0;
	}
};

struct Rule{
	int attr;
	int attr_select;
	int attr_num;
};

vector<User>* user;
void get_input()
{
	ifstream fin(datafile);
	int age;
	user = new vector<User>;
	while (fin >> age)
	{
		User new_user;
		new_user.attr[0] = age;
		for (int i = 1; i <= attr_num; i++)
		{
			fin >> new_user.attr[i];
		}
		user->push_back(new_user);
	}
	cout<<datafile<<endl;
}

double mylog(double a)
{
	if (a == 0) return 0;
	else return log(a);
}

void quickSort(vector<User>* arr, int head, int tail, int attr)
{
    int h = head, t = tail;
    int mid = (*arr)[(h+t)/2].attr[attr];
    while(h <= t)
    {
        while((*arr)[h].attr[attr] < mid) h++;
        while((*arr)[t].attr[attr] > mid) t--;
        if(h <= t)
        {
            User tmp = (*arr)[h];
            (*arr)[h] = (*arr)[t];
            (*arr)[t] = tmp;
            h++; t--;
         }
    }
    if(head < t) quickSort(arr, head, t, attr);
    if(h < tail) quickSort(arr, h, tail, attr);
}

Node* root;

Node* buildTree(vector<User>* user, double entropy)
{
	double delta = 0;
	int choose = 0;
	int choose_num = 0;
	double sum = user->size();
	double false_num = 0;
	for (int i = 0; i < user->size(); i++)
	{
		if ((*user)[i].attr[attr_num] == 0) false_num ++;
	}
	if (sum == 0) return new Node(0,0);
	for (int i = 0; i < attr_num; i++)
	{
//		cout << i << " ";
		if (attr[i] == 0)
		{
			double best_gain = 0;
			double best_sep = 0;
			double best_rate = 0;
			quickSort(user, 0, sum-1, i);
			int x = (*user)[0].attr[i];
			int f = 0;
			for (int j = 0; j < sum; j++)
			{
				if ((*user)[j].attr[i] != x)
				{
					double frac = (double)(f)/(j);
					double frac2 = (double)(j)/sum;
					double frac3 = (double)(false_num-f)/(sum-j);
					double tmp = - frac2 * (frac * mylog(frac) / log(2) + (1-frac) * mylog(1-frac) / log(2))
									- (1-frac2) * (frac3 * mylog(frac3) / log(2) + (1-frac3) * mylog(1-frac3) / log(2)); 
					if (entropy - tmp > best_gain)
					{
						best_gain = entropy - tmp;
						best_sep = x;
						best_rate = best_gain / -(frac2 * mylog(frac2) / log(2) + (1-frac2) * mylog(1-frac2) / log(2));
//						best_rate = best_gain;
					}
					x = (*user)[j].attr[i];
				}
				if ((*user)[j].attr[attr_num] == 0) f++;
			}
			if (best_rate > delta)
			{
				delta = best_rate;
				choose = i;
				choose_num = best_sep;
			}
		}
		//discrete attribute
		if (attr[i] != 0)
		{
			int* cnt = new int[attr[i]];
			int* ss = new int[attr[i]];
			for (int j = 0; j < attr[i]; j++)
			{
				cnt[j] = 0;
				ss[j] = 0;
			}
			for (int j = 0; j < sum; j++)
			{
				ss[(*user)[j].attr[i]] ++;
				if ((*user)[j].attr[attr_num] == 0)
					cnt[(*user)[j].attr[i]] ++;
			}
			double tmp = 0;
			double rate = 0;
			for (int j = 0; j < attr[i]; j++)
			{
				if (ss[j] == 0) continue;
				double frac = (double)cnt[j]/ss[j];
				double frac2 = (double)ss[j]/sum;
				rate -= frac2*mylog(frac2)/log(2);
				tmp -= ((double)frac2) * 
						(frac * (mylog(frac)/log(2)) + (1-frac) * (mylog(1-frac)/log(2)));
			}
			if (rate == 0) continue;
//			rate = 1;
			if ((entropy - tmp)/rate > delta)
			{
				delta = (entropy - tmp)/rate;
				choose = i;
			}
			delete[] cnt;
			delete[] ss;
		}
	}
	if (delta < 1e-3 or sum < threshold)
	{
		if (false_num > sum/2) return new Node(0,0);
		else return new Node(0,1);
	}
	if (attr[choose]>0)
	{
		Node* result = new Node(attr[choose]);
		result->attr = choose;
		int* cnt = new int[attr[choose]];
		int* ss = new int[attr[choose]];
		vector<User>** separate = new vector<User>* [attr[choose]];
		for (int j = 0; j < attr[choose]; j++)
		{
			cnt[j] = 0;
			ss[j] = 0;
			separate[j] = new vector<User>;
			separate[j]->clear();
		}
		for (int j = 0; j < sum; j++)
		{
			separate[(*user)[j].attr[choose]]->push_back((*user)[j]);
			ss[(*user)[j].attr[choose]] ++;
			if ((*user)[j].attr[attr_num] == 0)
				cnt[(*user)[j].attr[choose]] ++;
		}
		for (int j = 0; j < attr[choose]; j++)
		{
			double frac = (double)cnt[j]/ss[j];
			double next_entropy = -frac * mylog(frac)/log(2) - (1-frac) * mylog(1-frac)/log(2);
			result->sonList[j] = buildTree(separate[j], next_entropy);
		}
		delete[] cnt;
		delete[] ss;
		for (int j = 0; j < attr[choose]; j++)
			delete separate[j];
		delete[] separate;
		return result;
	}	
	else
	{
		Node* result = new Node(2);	
		result->attr = choose;
		result->attr_num = choose_num;
		vector<User>* c1 = new vector<User>;
		vector<User>* c2 = new vector<User>;
		c1->clear(); c2->clear();
		int* cnt = new int[2];
		cnt[0] = 0; cnt[1] = 0;
		for (int i = 0; i < sum; i++)
			if ((*user)[i].attr[choose] <= choose_num) 
			{
				c1->push_back((*user)[i]);
				if ((*user)[i].attr[attr_num] == 0) cnt[0] ++;
			}
			else 
			{
				c2->push_back((*user)[i]);
				if ((*user)[i].attr[attr_num] == 0) cnt[1] ++;
			}
		double frac = (double)cnt[0]/c1->size();
		double next_entropy = -frac * mylog(frac)/log(2) - (1-frac) * mylog(1-frac)/log(2);
//		cout << frac << endl;
		result->sonList[0] = buildTree(c1, next_entropy);
		frac = (double)cnt[1]/c2->size();
		next_entropy = -frac * mylog(frac)/log(2) - (1-frac) * mylog(1-frac)/log(2);
		result->sonList[1] = buildTree(c2, next_entropy);
		delete[] cnt;
		delete c1;
		delete c2;
		return result;
	}
}

void train()
{
	double tmp = 0;
	for (int i = 0; i < user->size(); i++)
		if ((*user)[i].attr[attr_num] == 0)
			tmp ++;
	double entropy = -(tmp/user->size())*mylog(tmp/user->size())/log(2) 
						-( 1-tmp/user->size() )*mylog( 1-tmp/user->size() )/log(2);
	root = buildTree(user, entropy);
}

void printTree(Node* node)
{
	cout << node->id;
	cout << " attr: "<<node->attr;
	if (attr[node->attr] == 0) cout << " sep : " << node->attr_num;
	cout << " son: ";
	for (int i = 0; i < node->son_num; i++)
	{
		cout<<node->sonList[i]->id;
		cout<<" ";
	}
	cout << " result: "<< node -> result << endl;
	cout<<endl;
	for (int i = 0; i < node->son_num; i++)
		printTree(node->sonList[i]);
}

int search(Node* node, User user)
{
	if (node->son_num == 0) return node->result;
	if (attr[node->attr] == 0)
	{
		if (user.attr[node->attr] <= node->attr_num) return search(node->sonList[0], user);
		else return search(node->sonList[1], user);
	}
	else
		return search(node->sonList[user.attr[node->attr]], user);
}

double test()
{
	ifstream fin(vaildfile);
	int age;
	int cnt = 0;
	int acc = 0;
	while (fin >> age)
	{
		User new_user;
		new_user.attr[0] = age;
		for (int i = 1; i <= attr_num; i++)
			fin >> new_user.attr[i];
		int ans = search(root, new_user);
		if (ans == new_user.attr[attr_num]) acc++;
		cnt ++;
	}
	cout << (double)acc/cnt << endl;
	return (double)acc/cnt;
}

vector< vector<Rule> > rules;
void get_rule(Node* node, vector<Rule> rule)
{
	if (node->son_num == 0)
	{
		Rule fin;
		fin.attr_num = node->result;
		rule.push_back(fin);
		rules.push_back(rule);
		return;
	}
	Rule new_rule;
	new_rule.attr = node->attr;
	rule.push_back(new_rule);
	if (attr[node->attr] == 0)
	{
		rule[rule.size()-1].attr_num = node->attr_num;
		rule[rule.size()-1].attr_select = 0;
		get_rule(node->sonList[0], rule);
		rule[rule.size()-1].attr_select = 1;
		get_rule(node->sonList[1], rule);
	}
	else
	{
		for (int i = 0; i < attr[node->attr]; i++)
		{
			rule[rule.size()-1].attr_select = i;
			get_rule(node->sonList[i], rule);
		}
	}
}

double rule_test(string filename, int x, int y)
{
	ifstream fin(filename.c_str());
	int age;
	int cnt = 0;
	int acc = 0;
	while (fin >> age)
	{
		User new_user;
		new_user.attr[0] = age;
		for (int i = 1; i <= attr_num; i++)
			fin >> new_user.attr[i];
		int result = 0;
		for (int i = 0; i < rules.size(); i++)
		{
			double ok = true;
			for (int j = 0 ; j < rules[i].size()-1; j++)
			{
				if (i == x && j == y)
					continue;
				if (attr[rules[i][j].attr] == 0)
				{
					if (rules[i][j].attr_select == 0 && new_user.attr[rules[i][j].attr] > rules[i][j].attr_num) ok = false;
					if (rules[i][j].attr_select == 1 && new_user.attr[rules[i][j].attr] <= rules[i][j].attr_num) ok = false;
				}
				else
					if (new_user.attr[rules[i][j].attr] != rules[i][j].attr_select) ok = false;
				if (!ok) break;
			}
			if (ok) 
			{
				result = rules[i][rules[i].size()-1].attr_num;
			}
//			if (i == 0 && ok)
//				cout <<"fuck"<<endl;
		}
		if (result == new_user.attr[attr_num])
			acc ++;
		cnt ++;
//		break;
	}
	return (double)acc/cnt;
}

void post_prune()
{
	rules.clear();
	vector<Rule> rule;
	rule.clear();
	get_rule(root, rule);
			cout << rules.size() << endl;
	double acc = rule_test(vaildfile, -1, -1);
	cout << acc << endl;
//	cout << test() << endl;
	for (int i = 0; i < rules.size(); i++)
	{
		bool ok = true;
		while (ok) 
		{
			ok = false;
			for (int j = 0; j < rules[i].size()-1; j++)
			{
				double tmp = rule_test(vaildfile, i, j);
				cout << tmp << endl;
				if (tmp >= acc)
				{
					acc = tmp;
					rules[i].erase(rules[i].begin() + j);
					ok = true;
					break;
				}
			}
		}
		cout <<"test:"<<rule_test(testfile, -1, -1)<<" "<<rules[i].size()<<endl;
	}
}

int main()
{
	get_input();
/*	ofstream fout("threshold.txt");
	for (threshold = 0; threshold < 1000; threshold +=10)
	{
		train();
		double res = test();
		fout << threshold << " " << res << endl;
	}*/
	train();
	post_prune();
	rule_test(testfile, -1, -1);
//	test();
}