#include <iostream>
#include <malloc.h>
#include <string>
#include <string.h>
#include <cstdlib>
#include<cstdio>
#include<vector>
#include <ctime>
#define max 10000
using namespace std;

struct node // bid node structure
{
	int cid; //company
	double price;
	int norc;  // no. of regions in one bid
	int region[max];
};

float tim;
int nor;
int nob;
int noc;
struct node tob[max]; // total no. of bids

bool com[max],reg[max]; //keeps record of which companies and regions can be selected in the remaining unprocessed bids
bool bid[max]; // final bids
vector<int> compbidmap[max];
/////
vector<int> soln;
vector<int> curr;
double bestprofit;
double profit;
///////

//function to take input - read from console by redirection
void readFile()
{

	string g;
	scanf("%f\n\n",&tim);
	scanf("%d\n\n",&nor);
	scanf("%d\n\n",&nob);
	scanf("%d\n\n",&noc);
	for(int i=0;i<nob;i++)
	{
		cout<<flush;
		string ch;
		getline(cin,ch);
		int t=0;int j=0;
		char ch1[max];
		while(ch[t]!=' ')
		{
			ch1[j]=ch[t];
			j++;t++;
		}

		ch1[j]='\0';
		tob[i].cid=atoi(ch1);
        compbidmap[tob[i].cid].push_back(i);
		ch1[0]='\0';j=0;t++;
		while(ch[t]!=' ')
		{
			ch1[j]=ch[t];
			j++;t++;
		}
		ch1[j]='\0';
		tob[i].price=strtod (ch1, NULL);
		t++;

		int x=0;
		int w=t;
		while(ch[t]!='#')
		{
			if(ch[t]==' ')
			{	x++;}
			t++;
		}
		tob[i].norc=x;
		t=w;
		for(int qq=0;qq<x;qq++)
		{
			ch1[0]='\0';j=0;
			while(ch[t]!=' ')
			{
				ch1[j]=ch[t];
				j++;t++;
			}
			t++;
			ch1[j]='\0';
			tob[i].region[qq]=atoi(ch1);
		}
		getline(cin,g);
	}
}

void fill(int);
void unfill(int);
bool checkReg(int);
double calpro(vector<int>);
void print(vector<int>);

//helper function of getRandom function
void fill(int bidno)
{
	com[tob[bidno].cid]=true;
	bid[bidno]=true;
	for(int i=0;i<tob[bidno].norc;i++)
	{
		reg[tob[bidno].region[i]]=true;
	}
}

/////
void unfill(int bidno)
{
    com[tob[bidno].cid]=false;
	bid[bidno]=false;
	for(int i=0;i<tob[bidno].norc;i++)
	{
		reg[tob[bidno].region[i]]=false;
	}
}
/////

//helper function of getRandom function
bool checkReg(int bidno)
{
	for(int i=0;i<tob[bidno].norc;i++)
	{
		if(reg[tob[bidno].region[i]]==true)
			return true;
	}
	return false;
}

//|||||||||

bool bestnbr()
{
    bool ret = 0;
    int i=0;
    vector<int> temp = curr;
    double temppro = profit;
    int prebid,sucbid;
//    //cout<<"vector easy"<<endl;
//    print(curr);
    //cout<<"in nbr "<<profit<<endl;
    for(;i<noc;i++)
    {
        ////cout<<"in for bn";
        double temppro2 = calpro(curr);
        vector<int> temp2 = curr;
        int currbid = curr[i]; //bid no. accepted for ith company
        if(currbid>=0)
        {
            unfill(currbid);
            ////cout<<"unfill done";
        }
        int j=0;

        for(;j<compbidmap[i].size();j++)//find best for changing bid of ith company
        {
            bool jugad = 1;

            if(compbidmap[i][j]!=currbid)
            {
                if(checkReg(compbidmap[i][j]))
                    continue;
                double pro;
                if(currbid>=0)
					pro = profit - tob[currbid].price + tob[compbidmap[i][j]].price;
				else
					pro = profit + tob[compbidmap[i][j]].price;
                if(pro>temppro2)
                {
                    temppro2 = pro;
                    temp2[i] = compbidmap[i][j];
                    ////cout<<"temp2 info  "<<temppro2<<endl;
                    //print(temp2);
                }

            }
        }
        if(temppro2>temppro)
        {
		   prebid = curr[i];
           temp = temp2;
           sucbid = temp[i];
           temppro = temppro2;
        }
        if(currbid>=0)
			fill(currbid);
    }
    if(calpro(temp)>profit)
    {
		if(prebid>=0)
			unfill(prebid);
        if(sucbid>=0)
            fill(sucbid);
        profit = calpro(temp);
        curr = temp;
        ret = 1;
    }
    //~ //cout<<"current best state and profit";
    //~ print(curr);
    //cout<<profit<<endl;
    return ret;
}
double calpro(vector<int> state)
{
    int i = 0;
    double sum = 0;
    for(;i<noc;i++)
    {
        if(state[i]>=0)
        {
                sum += tob[state[i]].price;
        }

    }
    return sum;
}

void print(vector<int> v)
{
    int i;
    for(i=0;i<v.size();i++)
    {
        cout<<v[i]<< " ";
    }
    cout<<endl;
}
vector<int> random_init(int seed)
{
    vector<int> rand_init;

    int q;
    for(q=0;q<noc;q++){
        rand_init.push_back(-1);
    }
    srand (seed);
    int num1=(rand())%noc;
    ////cout<<"random index for 1st company  "<<num1<<endl;
    vector<int> current_arr = compbidmap[num1];
    ////cout<<"compbidmap for 1st comp  ";
    //print(compbidmap[num1]);
    int index = rand()%current_arr.size();
    rand_init[num1]= current_arr[index];
    fill(current_arr[index]);


    int ab = 1;
    int ac = 50;          //ac is for seeding in random no. generation
    int i;
	while (ab < noc)    //ab keeps track of no. of companies for which bid is accepted whenever possible
    {
        srand(ac);
        i = rand()%noc;
        if(com[i] == 1)
        {
            ac += 1;
            continue;
        }
        else
        {
            bool assign = 0;
            current_arr = compbidmap[i];
            //print(current_arr);
            int k = rand()%current_arr.size();
            ////cout<<reg[0]<<reg[1]<<reg[2]<<reg[3]<<reg[4]<<reg[5]<<endl;
            for(int ind=0;ind<current_arr.size();ind++)
            {
                ////cout<<"infor"<<endl;
                bool m = checkReg(current_arr[k]);
                if(!checkReg(current_arr[k]))
                {
                    ////cout<<"inif"<<endl;
                    rand_init[i] = current_arr[k];
                    fill(current_arr[k]);
                    ab += 1;
                    ac += 1;
                    assign = 1;
                    break;
                }
                k = (k+1)%current_arr.size();
            }
               if (!assign)
                {
                    ////cout<<"iniff  "<<i<< endl;
                    rand_init[i]= -1;
                    ab += 1;
                    ac += 1;
                }



        }

    }
    //print(rand_init);
    return rand_init;

}
///|||||||||||

void getRandom(int seed) //modify this function to produce the best output(following the conditions mentioned in the assignment)
{
    curr = random_init(seed);
    profit = calpro(curr);
    //cout<<"get random for each iter info  " << profit<<endl;
    //print(curr);

}

int main()
{
    clock_t begin = clock();

    readFile();
    //cout<<tim<<endl;
    double alloted_time = (tim*60)-10;
    //cout<<"alloted time  "<<alloted_time<<endl;
    clock_t end;
    double elapsed_secs=0;
    int iter=1;
    getRandom(0);
    soln = curr;
    bestprofit = profit;
    //print(curr);
    //cout<<"initial profit"<<endl;
    //cout << profit << endl;
    //for(;iter<noc;iter++)
    while(elapsed_secs<alloted_time)
    {
        for(int r = 0;r<nor;r++)
			reg[r] =0;
        for(int r = 0;r<noc;r++)
			com[r] =0;
		getRandom(iter);
		//print(curr);
		while(1)
		{
		   if (!(bestnbr()))
				break;
		}
		if(profit>bestprofit)
		{
            bestprofit = profit;
            soln = curr;
		}
		//cout<<"overall best profit";
		//print(soln);
		 //cout<<bestprofit<<endl;
		 //cout<<"next loop starts"<<endl;
		 iter = iter+1;
         end = clock();
         elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	}
		for(int i=0;i<noc;i++)
		{
		if(soln[i]>=0) {
			cout<<soln[i]<<" ";
			}
		}
	cout<<"#"<<endl;

	//cout<<"BEST PROFIT AND SOLUTION BASED ON SEARCH"<<endl<<bestprofit<<endl;
	//print(soln);





	return 0;
}
