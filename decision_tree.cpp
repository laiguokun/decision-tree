#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#include <cstdlib>
#include <ctime>
#define datafile "data//train.data"
#define vaildfile "data//vaild.data"
#define testfile "data//pre2.test"
using namespace std;

const int attr[] = {0, 8, 0, 16, 0, 7, 14, 6, 5, 2, 0, 0, 0, 41};
const int attr_num = 14;
//const int attr[] = {3, 0, 0, 2};
//const int attr_num = 4;
int threshold = 70;
int tot_id = 32561*0.6;
int tot_false = 0;
int tot_true = 0;
int node_tot =0;


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
	double rate;
	Node(int num, int res = 0, double r = 1)
	{
		son_num = num;
		sonList = new Node*[num];
		attr_num = 0;
		id = tot_id;
		tot_id ++;
		result = res;
		attr = 0;
		rate = r;
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
	if (abs(a) <= 1e-9) return 0;
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

Node* buildTree(vector<User>* user, double entropy, vector<bool> used)
{
	double delta = 0;
	int choose = 0;
	int choose_num = 0;
	double sum = user->size();
	double false_num = 0;
	double test_cnt = 0;
	node_tot ++;
	for (int i = 0; i < user->size(); i++)
	{
		if ((*user)[i].attr[attr_num] == 0) false_num ++;
/*		if ((*user)[i].attr[12] == 39) 
		{
			for (int t = 0; t < attr_num ; t++)
				cout<<(*user)[i].attr[t]<<" ";
			cout <<endl;
		}*/
	}
//	cout <<test_cnt <<endl;
	if (sum == 0) return new Node(0,0);
//	cout << attr[13] << endl;
	for (int i = 0; i < attr_num; i++)
	{
		if (used[i] == false) continue;
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
					double frac4 = -(frac2 * mylog(frac2) / log(2) + (1-frac2) * mylog(1-frac2) / log(2));
//					if (i == 12)
//						cout << entropy - tmp <<" "<<x<<" "<<f <<" "<<j<<" "<<frac<<" "<<frac3<<" "<<false_num<<(entropy-tmp)/frac4<< endl;
/*					if (i == 12 && x == 39)
					{
						for (int t = 0; t < attr_num; t++)
							cout << (*user)[j-1].attr[t]<<" ";
						cout <<endl;
					}*/
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
//			if (i == 12)
//			{
//				cout <<"12: " <<best_gain<<" "<<best_sep<<endl;
//			}
			if (best_rate > delta)
			{
				delta = best_rate;
				choose = i;
				choose_num = best_sep;
			}
		}
		//discrete attribute
//		if (i == 13) cout << "fucklbr"<< choose_num <<endl;
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
//	if (choose == 13) cout << choose_num <<endl;
	if (delta < 1e-9 or sum < threshold)
	{
//		cout <<"end:"<<false_num<<" "<<sum<<endl;
		tot_false+= false_num;tot_true+= sum-false_num;
		if (false_num > sum/2) return new Node(0,0,(double)false_num/sum);
		else return new Node(0,1,(double)false_num/sum);
	}
/*	cout << choose << endl;
	cout << choose_num <<endl;
	cout << delta << endl;
	cout << sum <<endl;
	cout << used[choose]<<endl;
	system("pause");*/
	used[choose] = false;
	if (attr[choose]>0)
	{
		Node* result = new Node(attr[choose]);
		result->attr = choose;
		result->rate = (double)false_num/sum;
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
/*			if (choose==5)
			{
				if (j == 0)
				{
			double frac = (double)cnt[j]/ss[j];
			double next_entropy = -frac * mylog(frac)/log(2) - (1-frac) * mylog(1-frac)/log(2);
			result->sonList[j] = buildTree(separate[j], next_entropy, used);
				}
			}
			else
			{*/
			double frac = (double)cnt[j]/ss[j];
			double next_entropy = -frac * mylog(frac)/log(2) - (1-frac) * mylog(1-frac)/log(2);
			result->sonList[j] = buildTree(separate[j], next_entropy, used);
//			}
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
		result->rate = (double)false_num/sum;
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
		double frac,next_entropy;
//		if (choose != 0)
//		{
		frac = (double)cnt[0]/c1->size();
		next_entropy = -frac * mylog(frac)/log(2) - (1-frac) * mylog(1-frac)/log(2);
		result->sonList[0] = buildTree(c1, next_entropy, used);
//		}
//		cout << "choose left over" <<endl;
		frac = (double)cnt[1]/c2->size();
		next_entropy = -frac * mylog(frac)/log(2) - (1-frac) * mylog(1-frac)/log(2);
		result->sonList[1] = buildTree(c2, next_entropy, used);
		delete[] cnt;
		delete c1;
		delete c2;
		return result;
	}
}

void train()
{
	vector<bool> used;
	used.clear();
	for (int i = 0; i < attr_num; i++)
		used.push_back(true);
	double tmp = 0;
	for (int i = 0; i < user->size(); i++)
		if ((*user)[i].attr[attr_num] == 0)
			tmp ++;
	double entropy = -(tmp/user->size())*mylog(tmp/user->size())/log(2) 
						-( 1-tmp/user->size() )*mylog( 1-tmp/user->size() )/log(2);
	root = buildTree(user, entropy, used);
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
	if (node->son_num == 0)
	{
	//	cout <<node->result<<endl;
	//	cout <<node->rate<<endl;
		return node->result;
	}
//	cout << node->attr <<endl;
//	cout << node->attr_num <<endl;
//	system("pause");
	if (attr[node->attr] == 0)
	{
		if (user.attr[node->attr] <= node->attr_num) return search(node->sonList[0], user);
		else return search(node->sonList[1], user);
	}
	else
	{
		return search(node->sonList[user.attr[node->attr]], user);
	}
}

double test(string filename)
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
		int ans = 0;
			//for (int i = 0; i<= attr_num; i++)
				//cout << new_user.attr[i]<<" ";
			//cout <<endl;
	    ans=search(root, new_user);
		if (ans == new_user.attr[attr_num]) acc++;
		else
		{
/*			for (int i = 0; i<= attr_num; i++)
				cout << new_user.attr[i]<<" ";
			cout <<endl;
			system("pause");*/
		}
		cnt ++;
	}
//	cout << (double)acc/cnt << endl;
	return (double)acc/cnt;
}


void prune_tree(Node* node, double &acc)
{
	if (node->son_num == 0) return ;
	for (int i = 0; i < node->son_num; i++)
	{
		prune_tree(node->sonList[i], acc);
		Node* tmp = node->sonList[i];
		node->sonList[i] = new Node(0);
		if (tmp->rate > 0.5) node->sonList[i]->result = 0;
		else node->sonList[i]->result = 1;
		double ff = test(vaildfile);
		cout << acc << " " << ff << " " << test(testfile)<<endl;
		if (acc > ff)
		{
			node->sonList[i] = tmp;
//			cout << test(vaildfile) << endl;
		}
		else
			acc = ff;
	}
}
void post_prune()
{
	double acc = test(vaildfile);
	prune_tree(root,acc);
}

int main()
{
	srand((unsigned)time(0));
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
//	cout  << test(vaildfile) <<endl;
	cout <<node_tot<<endl;
	cout  << test(testfile) <<endl;
//	cout << tot_true<<endl;
//	cout <<tot_false<<endl;
}