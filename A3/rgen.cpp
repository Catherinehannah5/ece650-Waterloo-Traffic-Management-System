#include <iostream>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

/*
 * 
 */



class Point
{
private:
    double xcoor;
    double ycoor;
public:
    Point(double xcoor, double ycoor)
    {
        this->xcoor = xcoor;
        this->ycoor = ycoor;
    }

    double getXcoor(){ return this->xcoor; }
    void setXcoor(double xcoor){ this->xcoor = xcoor; }
    double getYcoor(){ return this->ycoor; }
    void setYcoor(double ycoor){ this->ycoor = ycoor; }
    
    int min()
    {
        if(this->getXcoor() < this->getYcoor())
            return this->getXcoor();
        else 
            return this->getYcoor();
    }
    int max()
    {
        if(this->getXcoor() > this->getYcoor())
            return this->getXcoor();
        else 
            return this->getYcoor();
        
    }
};


class Line{
private:
    Point *sourcePoint;
    Point *destinPoint;
public:
    Line(Point *sourcePoint, Point* destinPoint)
    {
        this->sourcePoint = sourcePoint;
        this->destinPoint = destinPoint;
    }

    Point* getSourcePoint(){ return this->sourcePoint; }
    Point* getDestinPoint(){ return this->destinPoint; }
   


};

class Street
{
private:
    string streetName;
    vector<Line*> streetLines;
public:
    Street(string streetName)
    {
        this->streetName = streetName;
    }
    void addPath(Line* newLine){ this->streetLines.push_back(newLine); }
    string getStreetName(){ return this->streetName; }
    vector<Line*> getStreetLines(){ return this->streetLines; }
};

class StreetDatabase
{
private:
    vector<Street*> streets;
public:
    vector<Street*> getStreets()
    {
        return this->streets; 
    }
    void addStreet(Street* newStreet)
    {
        this->streets.push_back(newStreet); 
    }
};



string converttostring(char *a)
{
    string s="";
    for (int i =0;i<10;i++)
    {
        s=s+a[i];
    }
    return s;
}
bool check=false;


int get_gradient(Line* l)
{
	double x1 = l->getSourcePoint()->getXcoor();
	double y1 = l->getSourcePoint()->getYcoor();
	double x2 = l->getDestinPoint()->getXcoor();
	double y2 = l->getDestinPoint()->getYcoor();
        
        //cout<<"Gradient = x1:y1:x2:y2  " << x1<<":" <<y1<<":"<<x2<<":"<<y2 <<endl;
	
	int m = -5;
	if (x1 != x2) 
	{
		m = (1./(x1-x2)*(y1-y2));
		return m;
	}
        return m;
}



bool find_parallel(Line* l1, Line* l2)
{
	if (get_gradient(l1) != get_gradient( l2))
	{
        //    cout<<"In parallel, gradient(l1),gradient(l2)" << get_gradient(l1)<<get_gradient(l2)<<endl;
                return false;
	}
      //  cout<<"both gradient equal returning true" <<endl;
	return true;
}


double get_intersect(Line* l)
{
    //cout<<"In get_intersect" <<endl;
    //cout<<(l->getSourcePoint()->getYcoor() - (get_gradient(l)*l->getSourcePoint()->getXcoor()))<<endl;
	return l->getSourcePoint()->getYcoor() - (get_gradient(l)*l->getSourcePoint()->getXcoor());
}

Point* find_intersection(Line* l1, Line* l2)
{
    
            
	if (!find_parallel(l1, l2))
	{
            Point* p;
            double x,y;
		if ((get_gradient(l1) != -5) && (get_gradient( l2) != -5))
		{
			x=(1/(get_gradient( l1) - get_gradient( l2))) * (get_intersect( l2) - get_intersect( l1));
			y=(get_gradient( l1)*x) + get_intersect( l1) ;
                       // cout<<"first x and y"<<x<<":"<<y;
		}
		else
		{
                    
			if( get_gradient(l1) == -5)
			{
				x = l1->getSourcePoint()->getXcoor();
				y = (get_gradient( l2) * x) + get_intersect( l2);
                       //            cout<<"first x and y"<<x<<":"<<y;
			}
			if (get_gradient( l2) == -5)
			{
				x = l2->getSourcePoint()->getXcoor();
				y = (get_gradient( l1) * x) + get_intersect( l1);
			}
		}
            p=new Point(x,y);
	//	p->setXcoor(x);
	//	p->setYcoor(y);
		return p;
	}
}


double find_distance(double x1, double x2, double y1, double y2)
{
	double dist = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
	//double dist = sqrt(pow(a->getXcoor() - b->getXcoor(), 2) + pow(a->getYcoor() - b->getYcoor(), 2));
    return dist;
}

bool check_intersect(Line* l1,Line* l2)
{
    Point* intersect_pt;
    //line 1
    Point* l1_A =  l1->getSourcePoint(); //l1[0]
    //cout<<l1_A->getXcoor(); // l1[0][0]
    //cout<<l1_A->getYcoor(); // l1[0][1]
    
    Point* l1_B =  l1->getDestinPoint(); //l1[1]
    
    //cout<<l1_B->getXcoor(); // l1[1][0]
    //cout<<l1_B->getYcoor(); // l1[1][1]  
   
    // line 2
    Point* l2_A =  l2->getSourcePoint(); //l1[0]
    
    //cout<<l2_A->getXcoor(); // l1[0][0]
    //cout<<l2_A->getYcoor(); // l1[0][1]
    
    Point* l2_B =  l2->getDestinPoint(); //l1[1]
    
    //cout<<l2_B->getXcoor(); // l1[1][0]
    //cout<<l2_B->getYcoor(); // l1[1][1]  
    double x,y;
	if (!find_parallel(l1,l2))
	{
		intersect_pt = find_intersection( l1,  l2);
		x = intersect_pt->getXcoor();
		y = intersect_pt->getYcoor();
                
               // cout<<x<<":"<<y<<endl;
		
		//xranges = [max(min(l1[0][0],l1[1][0]),min(l2[0][0],l2[1][0])),min(max(l1[0][0],l1[1][0]),max(l2[0][0],l2[1][0]))]
		
                int min1 = min(l1_A->getXcoor(),l1_B->getXcoor());
                int min2 = min(l2_A->getXcoor(),l2_B->getXcoor());
                int max1 = max(l1_A->getXcoor(),l1_B->getXcoor());
                int max2 = max(l2_A->getXcoor(),l2_B->getXcoor());
                Point* xranges = new Point (max(min1,min2), min(max1,max2));
		
                if( (xranges->min() <=x) && (x<= xranges->max()))
		{
                    /*
			dist = find_distance(l1_A->getXcoor(), l1_A->getYcoor(), l1_B->getXcoor(), l1_B->getYcoor());
			dist1 = find_distance(l1_A->getXcoor(), l1_A->getYcoor(), x, y);
			dist2 = find_distance(x, y, l1_B->getXcoor(), l1_B->getYcoor());
			
			if dist1 > dist or dist2 > dist
			{
				//pass
				cout<<"distance pass" << endl;
			}
                     * */
			
			return true;
		} 
		else
		{
		//	cout<<"Lines don't intersect" << endl;
			return false;
		}
		
    }
	else
	{
		//cout<< "Lines are parallel, I think this part is for overlapping!!" << endl;
		double slope1 = 0;
		double slope2 = 0;
                double y_int1;
                double y_int2;
		try
		{
			slope1 = (l1_B->getYcoor()-l1_A->getYcoor() / l1_B->getXcoor()-l1_A->getXcoor());
			slope2 = (l2_B->getYcoor()-l2_A->getYcoor() / l2_B->getXcoor()-l2_A->getXcoor()); 
			y_int1 = l1_A->getYcoor() - (slope1 - l1_A->getXcoor());
			y_int2 = l2_A->getYcoor() - (slope2 - l2_A->getXcoor());
		}
		catch(...)
		{
			slope1 = 0;
			slope2 = 0;
			y_int1 = l1_A->getXcoor() - (slope1 - l1_A->getXcoor());
			y_int2 = l1_A->getXcoor() - (slope2 - l2_A->getXcoor());
		}
		
		if ((slope1 == slope2) && (y_int1 == y_int2))
		{
			//cout<<"Lines overlap";
		//temp code	
                    //return true;
			return false;
		}
		
		return false;
	}
    
}
bool check_two_street(Street* st ,  StreetDatabase* st_database)
{
//    if(i!=1)
//        return true; 
    for(Street* s : st_database->getStreets())
    {
        vector<Line*> s_lines = s->getStreetLines();
        vector<Line*> st_lines = st->getStreetLines();
        
       // cout<<"a";
        for(Line* l:st_lines)
        {
         //   cout<<"b";
            for(Line* inner_line :s_lines)
            {
           //     cout<<"c";
                if(check_intersect(l,inner_line))
                    return true;
            }
        }
        
    }
    return false;
    
    
    
}
void create_streets(int no_of_streets)
{
    srand(time(NULL));
    
    StreetDatabase *st_database=new StreetDatabase();
    int st_no=0;
    while(st_no<2)
            st_no=rand()%no_of_streets;
    for(int k=0;k<st_no;k++)
    {
        
        char alphabets[52] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
char sname[10];



int i=0;
while(i<10) {
int temp = rand() % 52;
sname[i] = alphabets[temp];
i++;
}
        int num_of_points ;
        do
        {
             num_of_points = rand()%5;
        }while(num_of_points<1);
        
        
       
        
       // cout<<"a"<<"  "<<"\"" <<converttostring(sname)<< "\""<<"  ";
            
      
           // cout<<"in do1"<<endl;
         Street *st;
        do
        {
          //  cout<<"do";
             st = new Street(converttostring(sname));
          Point *p1; 
          Point *p2;
          int limit=0;
        for(int j =0; j<num_of_points;j++)
        {
            limit++;
            int a,b;
            if(j==0)
            {
                a =(rand()-rand())%18;
                b =(rand()-rand())%18;
                //cout<<"("<<a<<","<<b<<") ";
                p1 = new Point(a,b);
            
            
            
            }
              Line *l;
           bool ch ;
           //int exit_count=0;
          
              // cout<<"in do2"<<endl;
              // exit_count++;
           do
           {
               ch = true;
               a =(rand()-rand())%18;
               b =(rand()-rand())%18;
           
           p2 = new Point(a,b);
           
           //cout<<"1"<<endl;
          l =new Line(p1,p2);
           //cout<<"2"<<endl;
           
          
          
          if(j>0)
          {
               vector<Line*> v_lines = st->getStreetLines();
              Line* curr = l;
           for(Line* f_l : v_lines)
           {
               Line* one  =f_l;
              // cout<<"check : "<<v_lines.size();
               if(check_intersect(curr,one))
               {
                   ch=false;
                   break;
               }
               
                
           }
          }
          if(limit>25)
          {
              cout<<"Error: Maximum limit 25 reached";
              exit(0);
              break;
          }
          
           }while(!ch);
            p1=p2;
           st->addPath(l);
           
            
           // cout<<"("<<a<<"!"<<b<<") ";
        }
          
          //cout<<"final";
          }while(k==st_no-1 && (!check_two_street(st,st_database)) );
         
          //to print
           cout<<"a"<<"  "<<"\"" <<st->getStreetName()<< "\""<<"  ";
           int t=0;
          for(Line* lin:st->getStreetLines())
          {
              if(t==0)
            {
               
                Point *pp1 = lin->getSourcePoint();
                Point *pp2 = lin->getDestinPoint();
                cout<<"("<<pp1->getXcoor()<<","<<pp1->getYcoor()<<") ";
                cout<<"("<<pp2->getXcoor()<<","<<pp2->getYcoor()<<") ";
                // temp code
              
                
            }
            else
            {
                Point *pp1 = lin->getDestinPoint();
                cout<<"("<<pp1->getXcoor()<<","<<pp1->getYcoor()<<") ";
            }
            t++;
          }
           //cout<<endl;
              
          st_database->addStreet(st);
        cout<<endl;
        
        
    }
    cout<<"g"<<endl;

    sleep(4);
    
    vector<Street*> temp_st = st_database->getStreets();
    for(Street* st:temp_st)
    {
        vector<Line*> lines = st->getStreetLines();
        int cool=0;
        cout<<"r"<<" "<<"\"" <<st->getStreetName() <<"\"";
        //for(Line* l : lines)
        //{
            //temp code
           
            //if(cool==0)
            //{
             
                //Point *pp1 = l->getSourcePoint();
                //Point *pp2 = l->getDestinPoint();
                //cout<<"("<<pp1->getXcoor()<<","<<pp1->getYcoor()<<") ";
                //cout<<"("<<pp2->getXcoor()<<","<<pp2->getYcoor()<<") ";
                // temp code
              
                
            //}
            //else
            //{
                //Point *pp1 = l->getDestinPoint();
                //cout<<"("<<pp1->getXcoor()<<","<<pp1->getYcoor()<<") ";
            //}
            //cool++;
        //}
        cout<<endl;
        
    }
    //cout<<endl;
    
    }
int main(int argc, char** argv) {
    srand(time(NULL));
     opterr = 0;
      string temp;
    int snum = 10, nnum = 5, lnum = 5, cnum = 20, c;
      while ((c = getopt (argc, argv, "s:n:l:c:")) != -1)
    {
          
          
          if(c=='s')
          {
               temp = optarg;
               snum = atoi(temp.c_str());
               if(snum < 2){
                    cerr << "Error: Number of streets can't be less than 2." << endl;
                    exit(0);
                } 
          }
          else if(c=='n')
          {
              temp = optarg;
                nnum = atoi(temp.c_str());
              
                if(nnum < 1){
                    cerr << "Error: Number of line segments in a street cant be less than 1." << endl;
                    exit(0);
                }
          }
          else if (c=='l')
          {
              temp = optarg;
                lnum = atoi(temp.c_str());
               
                if(lnum < 5){
                    cerr << "Error: Number of seconds less than 5." << endl;
                    exit(0);
                }
          }
          else if(c=='c')
          {
                    temp = optarg;
                cnum = atoi(temp.c_str());
               
                if(cnum < 1){
                    cerr << "Error: K value less than 1." << endl;
                    exit(0);
                }
          }
          else if(c=='?')
          {
              if (optopt == 's' || optopt == 'n' || optopt == 'l' || optopt == 'c')
                    cerr << "Error: option requires an argument." << endl;
                else
                    cerr << "Error: unknown option." << endl;
                return 1;
          }
          else
          {
              return 0;
          }
       
    }
    while(true)
    {
    
        create_streets(snum);
        
       // Line* L1=new Line(new Point(4,2),new Point(4,8));
        //Line* L2=new Line(new Point(1,4),new Point(5,8));
       
        //cout<<check_intersect(L1,L2);
        
       // break;
      
    
    }/*
    cout<<snum;
    cout<<nnum;
    cout<<lnum;
    cout<<cnum;
    */
    
   
    


    return 0;
}

