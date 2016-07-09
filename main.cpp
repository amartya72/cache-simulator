#include<iostream>
#include<fstream>
#include<cstdlib>
#include <math.h>
#include <cstring>

using namespace std;

int lru[500][20]; //Function to change the tag order in the lru algorithim
void setop(int set, int assoc, int x) //This functio takes in the set, associativity, position=x
{
    int i,pos=0;                //LRU Logic Begins
    for(i=0;i<assoc;i++)
        if(lru[set][i] == x)
            pos = i;
    for(i=pos;i<assoc-1;i++)
        lru[set][i] = lru[set][i+1];
    lru[set][assoc-1] = x;
}                                   //LRU Logic Ends

long int convert(string);

int main()
{
    int cache_size=0, asso=0, block_size=0,i,j, no_blocks,pos=0,lastitem; //
    long int address;
    float hitrate;
    string hex;
    int no_set;
    int check=0, hit=0;
    
    
    //INPUT PARAMETERS// Start Reading the Configuration file and extracting the values from it///
    
    string name, code,filename,addressfile;
    unsigned long m=0,n=0,o=0;
    cout<<"Enter the Configuration file (without .cfg extension)"<<endl;
    cin>>filename;
    cout<<"Enter the address file (without .trc extension)"<<endl;
    cin>>addressfile;
    filename=filename.append(".cfg");
    addressfile=addressfile.append(".trc");
    ifstream fin(filename); // opens the text file
    
    while(getline(fin,name)){
        
        if(name.find("Cache Size")!=-1){
            m=name.find("=");
            n=name.length();
            o=n-m;
            code=name.substr(m+1,o);
            cache_size = stoi(code,nullptr,10);
            cout <<"Cache Size--->" << cache_size << endl;
        }
        
        if(name.find("Block Size")!=-1){
            m=name.find("=");
            n=name.length();
            o=n-m;
            code=name.substr(m+1,o);
            block_size = stoi(code,nullptr,10);
            cout <<"Block Size--->" << block_size << endl;
        }
        if(name.find("Associativity")!=-1){
            m=name.find("=");
            n=name.length();
            o=n-m;
            code=name.substr(m+1,o);
            asso = stoi(code,nullptr,10);
            cout <<"Associativity--->" << asso<< endl;
        }
    }
    
				int a=(cache_size/block_size);
				int NumberOfSets = a/asso ;
				cout << "Number of Sets--->"  << NumberOfSets << endl;
    
				int offset = log2(block_size);
				cout << "Offset Length--->" << offset << endl;
    
				int index = log2(NumberOfSets);
				cout << "Index Length--->"  << index << endl;
    
				int tag1 = 32-index-offset;
				cout << "Tag Length--->"  << tag1 << endl;
    
				int Totaladdresses = (NumberOfSets)*(asso)*(block_size/32);
				cout << "Total number of address--->" << Totaladdresses<<endl;
    
    //End of Reading Configuration File//
    
    no_blocks = cache_size / block_size;
    no_set = cache_size / (asso * block_size);
    
    long int cache[no_set][asso];
    
    for(i=0;i<no_set;i++)
        for(j=0;j<asso;j++)
            cache[i][j] = -10; // Eliminating all garbage values in in the cache.
    
    for(i=0;i<no_set;i++)
        for(j=0;j<asso;j++)
            lru[i][j] = j;
    
    //READ ADDRESS FILE// The file reads the addresses//
    
    ifstream infile;
    
    infile.open(addressfile,ios::in);
    
    if(!infile)
    {
        cerr<<"Error! File not found...";
        exit(0);
    }
    int set, found;
    long int tag;
    string fun, num;//dummy variables as we separate the coloumns
    
    for(int z=0;z<Totaladdresses;z++) //reads till the cache is full
    {
        
        infile>>hex >> fun >> num;
        address = convert(hex);
        
        set = (address / block_size) % no_set;
        
        tag = address / (block_size * no_set);
        
        check++;
        found = 0;
        for(i=0;i<asso;i++)
            if(cache[set][i] == tag)
            {
                found = 1;
                pos = i;
            }
        
    }
    
    while(!infile.eof()) //Reads the lines after the cache is full and we check for HIT or MISS//
    {
        
        infile>>hex >> fun >> num;
        address = convert(hex);
        
        set = (address / block_size) % no_set;
        
        tag = address / (block_size * no_set);
        
        lastitem = set*asso;
        
        check++;
        found = 0;
        for(i=0;i<asso;i++)
            if(cache[set][i] == tag)
            {
                found = 1;
                pos = i;
            }
        
        
        if(found)
        {
            hit++;
            setop(set,asso,pos);
            
        }
        else
        {
			         
            i = lru[set][0];
            cache[set][i] = tag;
            
            setop(set,asso,i);
        }
        
    }
    infile.close();                             //Print the results on Terminal
    system("clear");
    cout<<"No: of checks : "<<check<<endl;
    int nhit = Totaladdresses + hit;
    cout<<" No: of hits : "<< nhit<<endl;
    cout<<" No of misses : "<<check-nhit<<endl;
    hitrate = (float(nhit)/float(check)*100);
    cout<<" Hit Rate : "<<hitrate<<"%"<<endl;
    cout<<" Miss Rate : "<<100-hitrate<<"%"<<endl;
    return 0;
    
}

//Coverting Hex to Decimal//

long int convert(string hex)
{
    long int dec;
    dec=stol(hex,nullptr,0);
    return dec;
    
}
