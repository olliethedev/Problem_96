#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cmath>


using namespace std;
string readFile(string fileName)
{
    string result ="failed";
    string line="";
    ifstream read;
    read.open(fileName.c_str());
    if(!read)
        cout<<"cant read file "<< fileName<<endl;
    else
        while (read >> line)
        {
            result+=line;
        }
    read.close();
    return result;
}
int countGrids(string text, string delim)
{
    int count=0;
    bool flag =false;
    for(int i =0; i<text.size(); i++)
    {
        if(text[i]==delim[0])
        {
            flag =true;
            for(int j=0; j<delim.size();j++)
            {
                if(text[i+j]!=delim[j])
                {
                    flag =false;
                }
            }
            if(flag)
            {
                count++;
            }
        }
    }
    return count;
}
string getGrid(int gridNum, string delim, string text)
{
    string result="";
    char numstr[21];
    string gridNumStr=itoa(gridNum, numstr, 10);
    delim+=(gridNum<10)?"0"+gridNumStr:gridNumStr;
    bool flag =true;
    for(int i =0;i<text.size();i++)
    {
        if(text[i]==delim[0])
        {
            flag =true;
            for(int j=0; j<delim.size();j++)
            {
                if(text[i+j]!=delim[j])
                {
                    flag =false;
                }
            }
            if(flag)
            {
                for(int k =i+delim.size(); text[k]!=delim[0]&&k<text.size();k++)
                {
                    result+=text[k];
                }
            }
        }
    }
    return result;
}
bool validStrGrid(string text)
{
    char count=0;
    for(int i =0; i<text.size(); i++)
    {
        if((text[i]-48)>9 ||(text[i]-48)<0)
            return false;
        else
            count++;
    }
    if(count<81 || count>81)
        return false;
    return true;
}

void toVectorGrid(vector<vector <int> > &grid, string text)
{
    for(int i = 0; i<text.size(); i++)
    {
        if((text[i]-48)!=0) // cell with one possible answer
        {
            grid.push_back(vector<int>(1,text[i]-48));
        }
        else //cell with 9 possibl answers
        {
            grid.push_back(vector<int>(9));
            for(int m =0;m<9;m++)
            {
                grid[grid.size()-1][m]=m+1;
            }
        }
    }
}
void printVectorGrid(vector<vector <int> > &grid)
{
    int posi =0;
    int width=sqrt(grid.size());
    int count=0;
    for(int i = 0; i<width; i++)
    {
        if(i%3==0)
        {
            cout<<endl;
            count=0;
        }
        cout<<endl;
        for(int j=0;j<width; j++)
        {
            if(j%3==0)
            {
                for(int l=0; l<(20-count)&&count>8; l++)
                {
                    if(j>1)
                    cout<<" ";
                }
                count =0;
                cout<<" | ";
                count+=3;
            }
            if(grid[i*width+j].size()==1)
            {
                cout<<grid[i*width+j][0]<<" ";
                count+=2;
            }

            else//count possibilities
            {
                cout<<"<";
                count++;
                for(int k =0; k<grid[i*width+j].size(); k++)
                {
                    posi++;
                    cout<<grid[i*width+j][k];
                    count++;
                }
                cout<<"> ";
                count++;
            }
        }
    }
    if(posi>0)
    cout<<"\nPossibility count: "<<posi<<endl;
}
bool isSolved(vector<vector <int> > &grid)
{
    int width=sqrt(grid.size());
    for(int i = 0; i<width; i++)
    {
        for(int j=0;j<width; j++)
        {
            if(grid[i*width+j].size()!=1)
                return false;
        }
    }
    return true;
}
bool contains(vector<vector<int>*> &arr, int num)
{

    for (int i=0; i<arr.size(); i++)
    {
        if((*arr[i]).size()==1 && (*arr[i])[0]==num) //checking cells that have one possible solution only
            return true;
    }
    return false;
}
bool updateAffected(vector<vector<vector<int>*> > &arr, vector<int> &paira, vector<int> &pairb, int num)//only pass rows or colums
{
    int count=0;
    bool updated = false;
    vector<vector<int>*> temp;
    for(int h=0;h<arr.size(); h++) //for every col or row
    {
        count=0;
        for (int i=0; i<arr[h].size(); i++) //for every cell in s col or row
        {
            if((*arr[h][i]).size()>1) //aka unsolved, cell with more than one possibility
            {
                if(&(*arr[h][i])==&paira || &(*arr[h][i])==&pairb) //find row or coll that has one of the cells from the pair
                {
                    count++;
                    temp=arr[h]; //save col or row reference for later use
                }
            }
        }
        if(count==2) // found a coll or row with both cells from pair
        {
            //cout<<" pair found in col or row #: "<< 1+h<<endl;
            //to do remove possibilities from this row where possibilities not in cells that contain the pair
            for(int j =0; j<temp.size(); j++) //for every cell in the row or coll containing our pair
            {
                if((*temp[j]).size()>1&& (&(*temp[j])!= &paira && &(*temp[j])!= &pairb))
                {
                    for(int k=0; k<(*temp[j]).size(); k++)
                    {
                        if((*temp[j])[k]==num)
                        {
                            //cout<<"deleted: "<< (*temp[j])[k]<<endl;
                            (*temp[j]).erase((*temp[j]).begin()+k);
                            updated=true;

                        }
                    }

                }
            }
        }
        if(count>2) //for debugging test only , remove after debugging
        {
            cout<<"something went wrong, noob!"<<endl;
        }
    }
    return updated;
}
bool hasPair(vector<vector<int>*> &arr, vector<vector<vector<int>*> > &rows,vector<vector<vector<int>*> > &cols)// check if we have a hidden pair
{

    int counter=0;
    bool updated=false;
    vector<int>* temp;
    int hiddenPair=0;
    int num =0;
        for (int i=0; i<arr.size(); i++) // for every cell
        {
            counter=0;
            if((*arr[i]).size()>1) // we want only unsolved cells
            {
            //cout<<"checking cell"<<i<<endl;
            for(int j=0; j<(*arr[i]).size(); j++) // for every possibility
            {
                counter=0;
                for(int k = 0; k<arr.size(); k++) // for every other cell
                {
                    //cout<<" cross checking with cell"<<k<<endl;
                    for(int l=0; l<(*arr[k]).size(); l++) //for every possibility in other cells
                    {
                        if(i!=k&&(*arr[i])[j]==(*arr[k])[l]) // not interested in possibilities in original cell
                        {
                            num=(*arr[i])[j];
                            temp=arr[k];
                            counter++;
                            //cout<< "pair found : " << (*arr[i])[j]<<endl;
                        }
                    }

                }
                if(counter==1)
                {
                    hiddenPair++;
                    //cout<<" pair found for value "<<num<<endl;
                    //cout<<"from: "<<endl;
                    for(int m=0; m<(*arr[i]).size(); m++)
                    {
                        //cout<<(*arr[i])[m]<<"-";
                    }
                    //cout<<"\nin :"<<endl;
                    for(int n=0; n<(*temp).size();n++)
                    {
                        //cout<<(*temp)[n]<<"-";
                    }
                    //cout<<endl;
                    if(updateAffected(rows,(*arr[i]),(*temp), num)||updateAffected(cols,(*arr[i]),(*temp), num))
                    {
                        updated=true;
                    }

                }
            }
        }
            //something here
        }
    return updated;
}
bool tryAdvanced(vector<vector<vector<int>*> > &sectors,vector<vector<vector<int>*> > &rows,vector<vector<vector<int>*> > &cols)
{
    bool updated =false;
    for(int h=0;h<sectors.size(); h++) //for  sector
    {
        //cout<<"checking sector"<<1+h<<endl;
            if(hasPair(sectors[h], rows, cols)) //do inner logic
            {
                updated=true;
            }
    }
    return updated;
}
bool updateInner(vector<vector<int>*> &arr) // inner logic only to be used by the update method
{
    int count=0;
    int innerCount =0;
    int pairCount=0;
    vector<int>** paira;
    vector<int>** pairb;
    for (int i=0; i<arr.size(); i++) // for every cell
    {
        if((*arr[i]).size()>1) //if unsolved cell
        {
            for(int j = 0; j<(*arr[i]).size(); j++) //for every possibility
            {
                innerCount=0;
                for(int k =0; k<arr.size(); k++) // we check all the OTHER cells for NOT having the same possible value
                {

                    if(i!=k) // we are interested in OTHER cells
                    {

                        for(int l =0; l<(*arr[k]).size(); l++) // for every possibility in OTHER cells
                        {
                            if((*arr[i])[j]==(*arr[k])[l]) // possible in another cell
                            {
                                innerCount++;//count all times possible
                            }
                        }
                    }
                }
                if(innerCount==0) //only possible in the origin cell
                {
                    int temp =(*arr[i])[j];
                    //clear all possiibilities
                    (*arr[i]).resize(1);
                    (*arr[i])[0]=temp;//set to the only possible possibility
                    //cout<<"Value changed "<<temp<<endl;
                    //cin.ignore();
                    return true;
                }
            }
        }
    }
    return false;
}
bool update(vector<vector<vector<int>*> > &arr)
{

    bool updated =false;
    for(int h=0;h<arr.size(); h++) //for every sector or col or row
    {
            if(updateInner(arr[h])) //do inner logic
                updated=true;
    }
    return updated;
}
bool check(vector<vector<vector<int>*> > &arr)
{
    bool updated = false;
    for(int h=0;h<arr.size(); h++) //for every sector or col or row
    {
        for (int i=0; i<arr[h].size(); i++) //for every cell in sector or col or row
        {
            for(int j =0; j <(*arr[h][i]).size();j++) //for every value in cell
            {
                if((*arr[h][i]).size()>1) //aka unsolved, cell with more than one possibility
                {
                    if(contains(arr[h],(*arr[h][i])[j]))
                    {
                        (*arr[h][i]).erase((*arr[h][i]).begin()+j); //delete possibility if it exists in same sector or col or row
                        updated=true;
                    }
                }
            }
        }
    }
    return updated;
}

bool solve(vector<vector <int> > &grid)
{
    bool solved= false;
    bool updated =true;
    int width = sqrt(grid.size());
    int height=sqrt(grid.size());
    int xstep=0;
    int ystep=0;
    vector<vector<vector<int>*> > sectors(9);
    vector<vector<vector<int>*> > cols(9);
    vector<vector<vector<int>*> > rows(9);
    for(int c =0; c<9;c++) //seting the nested vectors for convinience later
    {
        sectors[c]=vector<vector<int>*>(0); //size of zero at start later will be resized
        cols[c]=vector<vector<int>*>(9);
        rows[c]=vector<vector<int>*>(9);
    }
    for(int a = 0; a<height; a++) //fill rows and cols vectors
    {

        for(int b=0;b<width; b++)
        {
            rows[a][b]=&grid[a*width+b]; //add to the rows vector
            cols[b][a]=&grid[a*width+b]; //add to the the cols vector
        }
    }
    for(int sector=0; sector<9; sector++) //fill the sectors vector... algorithm could use optimization
    {
        for(int y = 0; y<height; y++)
        {
            for(int x=0;x<width; x++)
            {
                if(x>=xstep&&x<xstep+3 && y>=ystep&&y<ystep+3)
                {
                    //cout<<grid[y*width+x][0]<<"\n";
                    sectors[sector].push_back(&grid[y*width+x]);
                }
            }
        }
        //cout<<endl;
        if(xstep!=9)
        {
            xstep+=3;
            //cout<<endl;
        }
        else
        {
            xstep=0;
            ystep+=3;
            sector--;
        }
    }
    //update possibilities
    while(updated)
    {

            if(!check(sectors)&&!check(cols)&&!check(rows))
            {
                updated=false; //no possibilities were changed then we finish updating
                //printVectorGrid(grid);
                //cin.ignore();

                if(!updated &&!isSolved(grid))//try advanced
                {
                    updated=(!update(sectors)&&!update(cols)&&!update(rows))?false:true;
                    //printVectorGrid(grid);
                    //cin.ignore();
                    if(!updated)
                    {
                        updated=(!tryAdvanced(sectors, rows, cols)&&!tryAdvanced(rows, sectors, cols)&&!tryAdvanced(cols, rows, sectors))?false:true;
                    }
                }
            }
    }
    //check if solved
    solved =isSolved(grid);


    //examples of access
    //fourth row ninth value first possible answer
    //cout<<(*rows[3][8])[0]<<endl;
    //sevenths col ninth value first possible answer
    //cout<<(*cols[6][8])[0]<<endl;
    //sixth sector fourth value first possible answer
    //cout<<(*sectors[5][3])[0]<<endl;
    //cout<<contains(sectors[3],4) <<endl;
    return solved;
}
int main()
{
    cout<<"Note: this program reads from a text file that must be named sudoku.txt"<<endl;
    cout<<"Note: the text file must follow this format : http://projecteuler.net/project/sudoku.txt"<<endl;
    string fileData = readFile("sudoku.txt");
    while(fileData=="failed")
    {
        cout<<"Cant read the text file"<<endl;
        cin.ignore();
        fileData = readFile("sudoku.txt");
    }
    int solved=0;
    int toSolve=countGrids(fileData,"Grid");
    string gridStr;
    for(int i=1; i<=toSolve; i++)
    {
        cout<<"\n\nSolving Grid #"<<i<<endl;
        gridStr=getGrid(i, "Grid",fileData);
        if(gridStr=="" || !validStrGrid(gridStr))
        {
            cout<<"Cannot read grid number "<< i<<endl;
        }
        else
        {
            vector<vector <int> > grid;
            toVectorGrid(grid, gridStr);
            //printVectorGrid(grid);
            cout<<endl;
            if(solve(grid))
            {
                cout<<"\nSolved.\n";
                printVectorGrid(grid);
                solved++;
            }
            else
            {
                cout<<"\nCould Not Solve.\n";
                printVectorGrid(grid);
            }
        }


        cin.ignore();
    }
    cout<<"\n\nDONE!!!\n\n Solved "<<solved<<" out of " << toSolve<<endl;
    cin.ignore();
    return 0;
}
