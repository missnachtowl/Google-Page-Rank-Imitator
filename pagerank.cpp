#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include<stdio.h> 
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <cmath>
using namespace std;

int getSpaceLoc(string line)
{
    int spacePos=0;
    for(int i=0;i<line.length();i++) 
    {
        if(spacePos==0)
        {
            if(line.at(i)==' ')
            {
                spacePos=i;
            }
        }
    } 
    return spacePos;
}

/*sorts list first, and then removes duplicates */
void removeDuplicates(vector<string>& pgs) 
{
    sort(pgs.begin(), pgs.end());
    pgs.erase(unique(pgs.begin(), pgs.end()), pgs.end());
}

vector<string> getPages(char* input)
{
    vector<string> pages;
    stringstream fileName;
	fileName << input;
    ifstream myfile (fileName.str());
    string line;
    if (myfile.is_open())
    {
        /*Getting the web URLs */
        getline (myfile,line);
        while ( getline (myfile,line))
        {
            int spacePos=getSpaceLoc(line);
            pages.push_back(line.substr(0,spacePos));
            pages.push_back(line.substr(spacePos+1));
        }
        myfile.close();
    }
    removeDuplicates(pages);
    return pages;
}

double** getAdjMatrix(char* input, vector<string> pgs)
{
    double** adjM =0;
    adjM = new double*[pgs.size()];
    for(int i =0;i<pgs.size();i++)
    {
        adjM[i] = new double[pgs.size()];

        for(int j =0;j<pgs.size();j++)
            adjM[i][j]=0;
    }

    stringstream fileName;
	fileName << input;
    ifstream myfile (fileName.str());
    string line;
    if (myfile.is_open())
    {
        /*Getting the web URLs */
        getline (myfile,line);
        while ( getline (myfile,line))
        {
            int spacePos=getSpaceLoc(line);
            string page1 = line.substr(0,spacePos);
            string page2 = line.substr(spacePos+1);

            int from, to=0;
            for(int i =0; i<pgs.size();i++)
            {
                if(pgs.at(i)==page1)
                    from = i;

                if(pgs.at(i)==page2)
                    to = i;
            }
            adjM[to][from] = 1;
        }
        myfile.close();
    }

    for(int i=0;i<pgs.size();i++)
    {
        int count=0;
        for(int j=0;j<pgs.size();j++)
        {
            if(adjM[j][i]!=0)
                count++;
        }

        for(int k=0;k<pgs.size();k++)
        {
            if(count!=0)
                adjM[k][i] /= (double)count;
        }   
    }
    return adjM;
}

int getPower(char* input)
{
    int power =0;
    stringstream fileName;
	fileName << input;
    ifstream myfile (fileName.str());
    string line;
    if (myfile.is_open())
    {
        /*Getting the numbers */
        getline (myfile,line);
        int spacePos=getSpaceLoc(line);
        power=stoi(line.substr(spacePos+1));
        myfile.close();
    }
    return power;
}

void getRank(int power, double** adjM,vector<double>& pgRank,bool created)
{
    if(power==0)
        return;
    if(!created)
    {
        for(int i=0;i<pgRank.size();i++)
            pgRank[i]/=(double)pgRank.size();
        getRank(power-1,adjM,pgRank,true);
    }
    else
    {
        vector<double> newRank;
        for(int i=0;i<pgRank.size();i++)
        {
            double sum =0.0;
            for(int j=0;j<pgRank.size();j++)
            {
                sum+=(double)(adjM[i][j]*pgRank.at(j));
            }
            newRank.push_back(sum);
        }
        pgRank=newRank;
        getRank(power-1,adjM,pgRank,true);
    }
}

void printRank(vector<double> pgRank,vector<string> pages)
{
    for(int i=0;i<pgRank.size();i++)
    {
        cout<<fixed;
        cout<<setprecision(2);
        if(isnan(pgRank.at(i)))
        {
            pgRank[i]=0;
            cout<<pages.at(i)<<" "<<pgRank.at(i)<<endl;
        }
        else
        {
            cout<<pages.at(i)<<" "<<pgRank.at(i)<<endl;
        }
        
    }
}

int main(int argc,char* argv[])
{
    char* input = argv[1];
    int power = getPower(input);
    vector<string> pages = getPages(input);
    double** adjM = getAdjMatrix(input,pages);
    vector<double> pgRank(pages.size(),1);
    getRank(power,adjM,pgRank,false);
    printRank(pgRank,pages);
}