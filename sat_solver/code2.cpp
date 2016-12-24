#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <cstring>

int n_g1;
int n_g2;
int m_g1;
int m_g2;
std::vector < std::vector <int> > edges_g1;
std::vector < std::vector <int> > edges_g2;
using namespace std;
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

void map(string str)
{
	char* test1 = new char[str.length()+1];
	strcpy(test1,str.c_str());
	char* test2 = new char[str.length()+1];
	strcpy(test2,str.c_str());
	
	char* blah = ".satoutput";
	strcat(test1,blah);
	
	ifstream satout(test1);
	blah = ".mapping";
	strcat(test2,blah);
	ofstream output(test2);
	string sat;
	satout>>sat;
	if(sat=="UNSAT")
	{
		output<<"0";
		satout.close();
		output.close();
		return;
	}
	for(int i=1;i<=n_g2;i++)
	{
		for(int j=1;j<=n_g1;j++)
		{
			int val;
			satout >> val;
			if((val>0)&&(degree2[i-1]>0))
				output<<i<< " "<<j<<endl;
		}
	}
	satout.close();
	output.close();
	return;
}

int main(int argc, char** argv)
{
	//read input file
	string str(argv[1]);
	readfile1(str);
	map(str);
	return 0;
}
