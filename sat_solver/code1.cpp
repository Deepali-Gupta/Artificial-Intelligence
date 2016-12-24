#include <iostream>
#include <sys/time.h>
#include <string>
#include <fstream>
#include <vector>
#include <deque>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <time.h>
#include <cstring>
#include <cstdlib>
using namespace std;

int n_g1;
int n_g2;
int m_g1;
int m_g2;
int no_of_clauses=0;
std::vector < std::vector <int> > edges_g1;
std::vector < std::vector <int> > edges_g2;
ofstream myfile;
int* degree1;
int* degree2;
int** edges1;
int** edges2;

void readfile1(string str)
{
	char* test = new char[str.length()+1];
	strcpy(test,str.c_str());
	//read first graph (larger)
	char* blah = ".graphs";
	strcat(test,blah);
	ifstream file(test);
	//read first graph (larger)
	vector< vector<int> > pairs;
	int i=0;
	m_g1 = 0;
	n_g1=0;
	int a,b;

	while(true)
	{
		file >> a >> b;
		if(!a)
			break;
		m_g1++;
		if(a > n_g1)
			n_g1 = a;
		if(b > n_g1)
			n_g1 = b;
		vector<int> pair;
		pair.push_back(a);
		pair.push_back(b);
		pairs.push_back(pair);
	}
	// cout <<" #Nodes = "<<n_g1<<"\t #Edges = "<<m_g1<<'\n';
	//convert into required format
	edges1=new int*[n_g1];
	for (i=0;i<n_g1;i++)
	{
		edges1[i]=new int[n_g1];
	}
	for (i=0;i<n_g1;i++)
	{
		for (int j=0;j<n_g1;j++)
		{
			edges1[i][j]=0;
		}
	}
	degree1= new int[n_g1];
	for(i=0;i<m_g1;i++)
	{
		edges1[pairs[i][0]-1][pairs[i][1]-1]=1;
	}
	int deg;
	for(int m=0;m<n_g1;m++)
	{
		vector<int> row;
		deg=0;
		for(int n=0;n<n_g1;n++)
		{
			if (edges1[m][n]==1) deg++;
			row.push_back(edges1[m][n]);
			// cout<<edges[m][n]<<"\t";
		}
		degree1[m]=deg;
		edges_g1.push_back(row);
		// cout<<'\n';
	}
	//read second graph (smaller)
	vector< vector<int> > pairs2;
	i=0;
	m_g2 = 0;
	n_g2=0;
	while(file>>a>>b)
	{
		if(!a)
			break;
		m_g2++;
		if(a > n_g2)
			n_g2 = a;
		if(b > n_g2)
			n_g2 = b;
		vector<int> pair;
		pair.push_back(a);
		pair.push_back(b);
		pairs2.push_back(pair);
	}
	// cout <<" #Nodes = "<<n_g2<<"\t #Edges = "<<m_g2<<'\n';
	//convert into required format
	edges2=new int*[n_g2];
	for (i=0;i<n_g2;i++)
	{
		edges2[i]=new int[n_g2];
	}
	// edges2={};
	for (i=0;i<n_g2;i++)
	{
		for (int j=0;j<n_g2;j++)
		{
			edges2[i][j]=0;
		}
	}
	degree2=new int[n_g2];
	for(i=0;i<m_g2;i++)
	{
		edges2[pairs2[i][0]-1][pairs2[i][1]-1]=1;
	}
	// int deg;
	for(int m=0;m<n_g2;m++)
	{
		deg=0;
		vector<int> row;
		for(int n=0;n<n_g2;n++)
		{
			if (edges2[m][n]==1) deg++;
			row.push_back(edges2[m][n]);
			// cout<<edges2[m][n]<<"\t";
		}
		edges_g2.push_back(row);
		degree2[m]=deg;
		// cout<<'\n';
	}
	return;
}

void edges()
{
	int num=n_g1*n_g2;
	for(int i=0;i<n_g2;i++)
	{
		for(int j=0;j<n_g2;j++)
		{
			if(edges_g2[i][j]==1)
			{
				num++;
				myfile<<num<<" 0"<<'\n';
				// no_of_clauses++;
			}
			else
			{
				num++;
				myfile<<"-"<<num<<" 0"<<'\n';
				// no_of_clauses++;
			}
		}
	}
	for(int i=0;i<n_g1;i++)
	{
		for(int j=0;j<n_g1;j++)
		{
			if(edges_g1[i][j]==1)
			{
				num++;
				myfile<<num<<" 0"<<'\n';
				// no_of_clauses++;
			}
			else
			{
				num++;
				myfile<<"-"<<num<<" 0"<<'\n';
				// no_of_clauses++;
			}
		}
	}
	return;
}

void edges_count()
{
	int num=n_g1*n_g2;
	for(int i=0;i<n_g2;i++)
	{
		for(int j=0;j<n_g2;j++)
		{
			if(edges_g2[i][j]==1)
			{
				num++;
				// myfile<<num<<" 0"<<'\n';
				no_of_clauses++;
			}
			else
			{
				num++;
				// myfile<<"-"<<num<<" 0"<<'\n';
				no_of_clauses++;
			}
		}
	}
	for(int i=0;i<n_g1;i++)
	{
		for(int j=0;j<n_g1;j++)
		{
			if(edges_g1[i][j]==1)
			{
				num++;
				// myfile<<num<<" 0"<<'\n';
				no_of_clauses++;
			}
			else
			{
				num++;
				// myfile<<"-"<<num<<" 0"<<'\n';
				no_of_clauses++;
			}
		}
	}
	return;
}


void one_val()
{
	for (int i=1;i<=n_g2;i++)
	{
		if (degree2[i-1]>0)
		{
			for (int j=1;j<=n_g1;j++)
			{
				myfile<<(i-1)*n_g1+j<<" ";
			}
			myfile<<'0'<<'\n';
			// no_of_clauses++;
		}
	}
}

void one_val_count()
{
	for (int i=1;i<=n_g2;i++)
	{
		if (degree2[i-1]>0)
		{
			for (int j=1;j<=n_g1;j++)
			{
				// myfile<<(i-1)*n_g1+j<<" ";
			}
			// myfile<<'0'<<'\n';
			no_of_clauses++;
		}
	}
}


void no_two_val()
{
	// int counter=0;
	for (int i=1;i<=n_g2;i++)
	{
		//for every edge in no G2 or GEmail I have to implement no two val
		for (int j=1;j<=n_g1;j++)
		{
			int k=n_g1;
			while (k>j)
			{
				myfile<<-((i-1)*n_g1+j)<<" "<<-((i-1)*n_g1+k)<<" "<<'0'<<'\n';
				// no_of_clauses++;
				k--;
				// counter++;
			}
		}
	}
	//cout<<"no_of_two_val_constraints are: "<<counter<<'\n';
}

void no_two_val_count()
{
	// int counter=0;
	for (int i=1;i<=n_g2;i++)
	{
		//for every edge in no G2 or GEmail I have to implement no two val
		for (int j=1;j<=n_g1;j++)
		{
			int k=n_g1;
			while (k>j)
			{
				// myfile<<-((i-1)*n_g1+j)<<" "<<-((i-1)*n_g1+k)<<" "<<'0'<<'\n';
				no_of_clauses++;
				k--;
				// counter++;
			}
		}
	}
	//cout<<"no_of_two_val_constraints are: "<<counter<<'\n';
}

void column_constraint()
{
	// int counter=0;
	for (int i=1;i<=n_g1;i++)
	{
		for (int j=1;j<=n_g2;j++)
		{
			int k=n_g2;
			while (k>j)
			{
				myfile<<-((j-1)*n_g1+i)<<" "<<-((k-1)*n_g1+i)<<" "<<'0'<<'\n';
				// no_of_clauses++;
				k--;
				// counter++;
			}
		}
	}
		//cout<<"no_of_column_constraints are: "<<counter<<'\n';
}

void column_constraint_count()
{
	// int counter=0;
	for (int i=1;i<=n_g1;i++)
	{
		for (int j=1;j<=n_g2;j++)
		{
			int k=n_g2;
			while (k>j)
			{
				// myfile<<-((j-1)*n_g1+i)<<" "<<-((k-1)*n_g1+i)<<" "<<'0'<<'\n';
				no_of_clauses++;
				k--;
				// counter++;
			}
		}
	}
		//cout<<"no_of_column_constraints are: "<<counter<<'\n';
}

void degree_constraint()
{
	// int counter=0;
	for (int i=1;i<=n_g2;i++)
	{
		for (int j=1;j<=n_g1;j++)
		{
			if (degree2[i-1]>degree1[j-1])
			{
				// counter++;
				myfile<<-((i-1)*n_g1+j)<<" 0"<<'\n';
				// no_of_clauses++;
			}
		}
	}
	//cout<<"no of degree_constraints are: "<<counter<<'\n';
}

void degree_constraint_count()
{
	// int counter=0;
	for (int i=1;i<=n_g2;i++)
	{
		for (int j=1;j<=n_g1;j++)
		{
			if (degree2[i-1]>degree1[j-1])
			{
				// counter++;
				// myfile<<-((i-1)*n_g1+j)<<" 0"<<'\n';
				no_of_clauses++;
			}
		}
	}
	//cout<<"no of degree_constraints are: "<<counter<<'\n';
}


void helper_constraint(int a, int b)
{
	//now define the mapping with i and j
	//a th node of G2 is mapped to i+1 th node of G1
	//b th node of G2 is mapped to j+1 th node of G1
	int initial_a=(a-1)*n_g1+1;
	int initial_b=(b-1)*n_g1+1;
	for (int i=0;i<n_g1;i++)
	{
		if (degree2[a-1]<=degree1[i])
		{
			for (int j=0;j<n_g1;j++)
			{
				//variable of mapping from a to i+1 => initial_a+i
				//variable of mapping from b to j+1 => initial_b+j
				//variable of edge between a and b node is n_g1*n_g2+(a-1)*n_g2+b
				//variable of edge between i+1 and j+1 node is n_g1*n_g2+n_g2*n_g2+i*n_g1+(j+1)
				if (degree2[b-1]<=degree1[j])
				{
					if (edges2[a-1][b-1]==edges1[i][j])
					{
						// myfile<<-(initial_a+i)<<" "<<-(initial_b+j)<<" "<<-(n_g1*n_g2+(a-1)*n_g2+b)<<" "<<(n_g1*n_g2+n_g2*n_g2+(i)*n_g1+(j+1))<<" "<<'0'<<'\n';
						// no_of_clauses++;
						// myfile<<-(initial_a+i)<<" "<<-(initial_b+j)<<" "<<(n_g1*n_g2+(a-1)*n_g2+b)<<" "<<-(n_g1*n_g2+n_g2*n_g2+(i)*n_g1+(j+1))<<" "<<'0'<<'\n';
						// no_of_clauses++;						
					}
					else 
					{
						myfile<<-(initial_a+i)<<" "<<-(initial_b+j)<<" 0"<<'\n';
						// no_of_clauses++;
					}
				}
			}

		}
		// else {myfile<<-(initial_a+i)<<" 0"<<'\n'; no_of_clauses++;}
	}
}

void helper_constraint_count(int a, int b)
{
	//now define the mapping with i and j
	//a th node of G2 is mapped to i+1 th node of G1
	//b th node of G2 is mapped to j+1 th node of G1
	int initial_a=(a-1)*n_g1+1;
	int initial_b=(b-1)*n_g1+1;
	for (int i=0;i<n_g1;i++)
	{
		if (degree2[a-1]<=degree1[i])
		{
			for (int j=0;j<n_g1;j++)
			{
				//variable of mapping from a to i+1 => initial_a+i
				//variable of mapping from b to j+1 => initial_b+j
				//variable of edge between a and b node is n_g1*n_g2+(a-1)*n_g2+b
				//variable of edge between i+1 and j+1 node is n_g1*n_g2+n_g2*n_g2+i*n_g1+(j+1)
				if (degree2[b-1]<=degree1[j])
				{
					if (edges2[a-1][b-1]==edges1[i][j])
					{
						// myfile<<-(initial_a+i)<<" "<<-(initial_b+j)<<" "<<-(n_g1*n_g2+(a-1)*n_g2+b)<<" "<<(n_g1*n_g2+n_g2*n_g2+(i)*n_g1+(j+1))<<" "<<'0'<<'\n';
						// no_of_clauses++;
						// myfile<<-(initial_a+i)<<" "<<-(initial_b+j)<<" "<<(n_g1*n_g2+(a-1)*n_g2+b)<<" "<<-(n_g1*n_g2+n_g2*n_g2+(i)*n_g1+(j+1))<<" "<<'0'<<'\n';
						// no_of_clauses++;						
					}
					else 
					{
						// myfile<<-(initial_a+i)<<" "<<-(initial_b+j)<<" 0"<<'\n';
						no_of_clauses++;
					}
				}
			}

		}
		// else {myfile<<-(initial_a+i)<<" 0"<<'\n'; no_of_clauses++;}
	}
}


void mapping_constraint()
{
	//constraint for each edge in GEmail or G2 or smaller graph
	for (int i=1;i<=n_g2;i++)
	{
		int j=1;
		while ((j<=n_g2))
		{
			// cout<<i<<" "<<j<<'\n';
			helper_constraint(i,j);
			j++;
		}
	}
}


void mapping_constraint_count()
{
	//constraint for each edge in GEmail or G2 or smaller graph
	for (int i=1;i<=n_g2;i++)
	{
		int j=1;
		while ((j<=n_g2))
		{
			// cout<<i<<" "<<j<<'\n';
			helper_constraint_count(i,j);
			j++;
		}
	}
}

//improvement 0: Use the degree heuristic to determine if that mapping is possible and then run this for mappings which are possible.
//revised improvement 1: run complete constraints if either both dont have edge or both have edge else just put in a constraint where that mapping is impossible
//improvement 2: now i know a to b edge hai so i can write a lesser constraint of that mapping variable ka negation if there is no edge between M(a) to M(b)
//improvement 3: self edges??
//improvement 4: no_two_val_constraint different implementation - about 2 million out 37 million are these constraints
//improvement 5: clean the representation of helper function - remove redudant if and maybe combine with mapping constraint
//improvement 6: remove all edge constraints - not required because my mapping is now based on that - this should ideally have done better but makes it worse
//time increased from 3.28 to 5.5 sec
//try for unsolvable graphs - by making it false


int main(int argc, char** argv)
{
	// ios_base::sync_with_stdio(false);
	string str(argv[1]);

	readfile1(str);

	edges_count();
	degree_constraint_count();
	mapping_constraint_count();
	no_two_val_count();
	one_val_count();
	column_constraint_count();
	//~ cout<<no_of_clauses<<endl;

	char* part1 = new char[str.length()+1];
	strcpy(part1,argv[1]);
	char* part2 = ".satinput";
	strcat(part1,part2);
	myfile.open(part1);
	// ofstream secfile(part1);
	
	//cout<<"readfile done: "<<'\n';
	myfile<<"p cnf "<<n_g1*n_g2+n_g2*n_g2+n_g1*n_g1<<" "<<no_of_clauses<<'\n';
	//~ myfile<<"p cnf "<<n_g1*n_g2<<" "<<no_of_clauses<<'\n';
	edges();
	//cout<<"edges done: "<<'\n';
	degree_constraint();
	//cout<<"degree constraint done: "<<'\n';
	mapping_constraint();
	//cout<<"mapping constraint done: "<<'\n';
	no_two_val();
	//cout<<"no two val done: "<<'\n';
	one_val();
	//cout<<"one val done: "<<'\n';
	column_constraint();
	//cout<<"column_constraint done: "<<'\n';
	// myfile.seekp(0,ios_base::app);
	// myfile<<"p cnf "<<n_g1*n_g2+n_g2*n_g2+n_g1*n_g1<<" "<<no_of_clauses<<'\n';
	//cout<<n_g2<<" "<<n_g1<<'\n';
	// secfile<<"p cnf "<<n_g1*n_g2+n_g2*n_g2+n_g1*n_g1<<" "<<no_of_clauses<<'\n';
	// char* com1 = "cat temp.txt >> ";
	// char* com = new char[25+str.length()];
	// strcpy(com,com1);
	// strcat(com,part1);
	// myfile.close();
	// secfile.close();
	// std::system(com);
    return 0;
}
//improvement 1: run helper_constraint only for edges which are there in Graph G2 - shayad isse galat solution aa sakta hai 
//(maybe iff ka ek part ka kat raha hai) - nah i think this is ok - because if there is no edge at Y - iff ko false hona padega unless that mapping is not there.
//nops improvement 1 is wrong
