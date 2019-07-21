#include <bits/stdc++.h>
#include <pthread.h>
#include<sys/time.h>
#include "akari.h"
using namespace std;

namespace aka
{
int globalR, globalC;
struct point
{
    int row,col;
    int lights;
    point(int r, int c):row(r),col(c)
    {
        ;
    }
    point(int r, int c, int l):row(r),col(c),lights(l)
    {
        ;
    }
    int isOnBorder() const{
		return (row==0) ||(col==0) || (row==(globalR-1)) || (col==(globalC-1));;
	}
    bool operator <(const point &b) const //锟饺凤拷锟斤拷4锟斤拷锟斤拷
    {
		if(lights!=b.lights){
			if(b.lights==2){
			return false;
			}
			else return lights>b.lights; 
		}
		else
			return this->isOnBorder()<b.isOnBorder();

    }
    struct point operator+(const point &b)
    {
        return point(row+b.row, col+b.col);

    }

};
struct threadTask
{
    //int type;//1,2,3
    int seed;
    bool findAns, isAlive;
    int ansSum;
    int allIndex,presentLights;
    vector<vector<int> > test;
    vector<vector<int> >nearlights, hasManyLights;
    vector<vector<int> > ans;
    vector<point> lastLight;

    threadTask(int seed, int allIndex, vector<vector<int> > &test, vector<vector<int> > &nearlights, vector<vector<int> >& hasManyLights)
    {
        this->seed=seed;
        this->allIndex=allIndex;
        this->test=test;
        this->nearlights=nearlights;
        this->hasManyLights=hasManyLights;
        findAns=false;
        presentLights=ansSum=0;
    }

};
vector<threadTask> threads;
vector<point>direction;
vector<point> all, lastLight;
vector<vector<int> > islight,nearlights,hasManyLights,ansVec;
int ans,totalShouldLights, presentLights;



//锟斤拷锟斤拷锟斤拷锟斤拷锟秸硷拷锟节憋拷写锟斤拷锟诫，锟斤拷锟斤拷锟斤拷锟皆革拷
void Output(vector<vector<int> > & g)
{
    for(int i=0; i<g.size(); ++i)
    {
        for(int j=0; j<g[i].size(); ++j)
        {
            printf("%4d",g[i][j]);
        }
        cout<<endl;
    }

}


bool isBlankOfLimitLight(vector<vector<int> > & g,int r, int c)
{
    int row=g.size()-1;
    int col=g[0].size()-1;
    if(r<0 || c<0 || r>row || c>col)
    {
        return false;
    }
    else
        return (g[r][c]>=0 && g[r][c]<=4);

}
bool canPlaceLight(vector<vector<int> > & g,int r, int c)
{
    int row=g.size()-1;
    int col=g[0].size()-1;
    if(r<0 || c<0 || r>row || c>col)
    {
        return false;
    }
    else
        return g[r][c]==-2;

}

    bool placeLight(vector<vector<int> > & g,int r, int c)
{
    //nearlights ++;
    //锟斤拷锟斤拷前锟叫讹拷canPlaceLight
    int flag=0,tempc=c,tempr=r;
    g[r][c]=5;
    //++presentLights;
    //++hasManyLights[r][c];
    while(tempr>=0&&((g[tempr][c]==-2) || (g[tempr][c]==5))) //锟斤拷锟斤拷一锟斤拷g[r][c]  思锟斤拷锟斤拷while(tempr>=0&&(g[tempr][c]==-2 || (g[tempr--][c]==5))){
    {
        ++hasManyLights[tempr][c];
        if(hasManyLights[tempr][c]==1)
            presentLights++;
        tempr--;

    }
    tempr=r;
    while(++tempr<g.size()&&((g[tempr][c]==-2) || (g[tempr][c]==5)))
    {
        ++hasManyLights[tempr][c];
        if(hasManyLights[tempr][c]==1)
            presentLights++;

    }
    while(++tempc<g[0].size()&&((g[r][tempc]==-2) || (g[r][tempc]==5)))
    {
        ++hasManyLights[r][tempc];
        if(hasManyLights[r][tempc]==1)
            presentLights++;

    }
    tempc=c;
    while(--tempc>=0&&((g[r][tempc]==-2) || (g[r][tempc]==5)))
    {
        ++hasManyLights[r][tempc];
        if(hasManyLights[r][tempc]==1)
            presentLights++;

    }

    if(isBlankOfLimitLight(g, r+1, c))
    {
        nearlights[r+1][c]++;
        if(g[r+1][c] < nearlights[r+1][c])
            flag=1;
    }

    if(isBlankOfLimitLight(g, r-1, c))
    {
        nearlights[r-1][c]++;
        if(g[r-1][c] < nearlights[r-1][c])
            flag=1;
    }


    if(isBlankOfLimitLight(g, r, c+1))
    {
        nearlights[r][c+1]++;
        if(g[r][c+1] < nearlights[r][c+1])
            flag=1;
    }
    if(isBlankOfLimitLight(g, r, c-1))
    {
        nearlights[r][c-1]++;
        if(g[r][c-1] < nearlights[r][c-1])
            flag=1;
    }
    return flag==0;/**=1说锟斤拷锟缴癸拷锟斤拷锟斤拷锟斤拷*/

}

bool canPlaceLight(vector<vector<int> > & g, point &p, threadTask *th)
{
    int row=g.size()-1;
    int col=g[0].size()-1;
    if(p.row<0 || p.col<0 || p.row>row || p.col>col)
    {
        return false;
    }
    if(th->hasManyLights[p.row][p.col]>0) return false;
    else
        return g[p.row][p.col]==-2;

}
    //just call at beginning
bool canPlaceLight(vector<vector<int> > & g, point &p)
{
    int row=g.size()-1;
    int col=g[0].size()-1;
    if(p.row<0 || p.col<0 || p.row>row || p.col>col)
    {
        return false;
    }
    //if(th->hasManyLights[p.row][p.col]>0) return false;
    else
        return g[p.row][p.col]==-2;

}



bool placeLight(vector<vector<int> > & g,int r, int c, threadTask *th)
{
    //nearlights ++;
    //锟斤拷锟斤拷前锟叫讹拷canPlaceLight
    int flag=0,tempc=c,tempr=r;
    g[r][c]=5;
    //++presentLights;
    //++hasManyLights[r][c];
    while(tempr>=0&&((g[tempr][c]==-2) || (g[tempr][c]==5))) //锟斤拷锟斤拷一锟斤拷g[r][c]  思锟斤拷锟斤拷while(tempr>=0&&(g[tempr][c]==-2 || (g[tempr--][c]==5))){
    {
        ++(th->hasManyLights[tempr][c]);
        if(th->hasManyLights[tempr][c]==1)
            th->presentLights++;
        tempr--;

    }
    tempr=r;
    while(++tempr<g.size()&&((g[tempr][c]==-2) || (g[tempr][c]==5)))
    {
        ++th->hasManyLights[tempr][c];
        if(th->hasManyLights[tempr][c]==1)
            th->presentLights++;

    }
    while(++tempc<g[0].size()&&((g[r][tempc]==-2) || (g[r][tempc]==5)))
    {
        ++th->hasManyLights[r][tempc];
        if(th->hasManyLights[r][tempc]==1)
            th->presentLights++;

    }
    tempc=c;
    while(--tempc>=0&&((g[r][tempc]==-2) || (g[r][tempc]==5)))
    {
        ++th->hasManyLights[r][tempc];
        if(th->hasManyLights[r][tempc]==1)
            th->presentLights++;

    }

    if(isBlankOfLimitLight(g, r+1, c))
    {
        th->nearlights[r+1][c]++;
        if(g[r+1][c] < th->nearlights[r+1][c])
            flag=1;
    }

    if(isBlankOfLimitLight(g, r-1, c))
    {
        th->nearlights[r-1][c]++;
        if(g[r-1][c] < th->nearlights[r-1][c])
            flag=1;
    }


    if(isBlankOfLimitLight(g, r, c+1))
    {
        th->nearlights[r][c+1]++;
        if(g[r][c+1] < th->nearlights[r][c+1])
            flag=1;
    }
    if(isBlankOfLimitLight(g, r, c-1))
    {
        th->nearlights[r][c-1]++;
        if(g[r][c-1] < th->nearlights[r][c-1])
            flag=1;
    }
    return flag==0;/**=1说锟斤拷锟缴癸拷锟斤拷锟斤拷锟斤拷*/

}

void flipLight(vector<vector<int> > & g,int r, int c,  threadTask *th)
{
    //nearlights --;
    int flag=0,tempc=c,tempr=r;
    //--th->hasManyLights[r][c];
    while(tempr>=0&&((g[tempr][c]==-2) || (g[tempr][c]==5)))
    {
        if(th->hasManyLights[tempr][c]==1)
            th->presentLights--;
        --th->hasManyLights[tempr][c];
        --tempr;

    }
    tempr=r;
    while(++tempr<g.size()&&((g[tempr][c]==-2) || (g[tempr][c]==5)))
    {

        if(th->hasManyLights[tempr][c]==1)
            th->presentLights--;
        --th->hasManyLights[tempr][c];

    }
    while(++tempc<g[0].size()&&((g[r][tempc]==-2) || (g[r][tempc]==5)))
    {
        if(th->hasManyLights[r][tempc]==1)
            th->presentLights--;
        --th->hasManyLights[r][tempc];

    }
    tempc=c;
    while(--tempc>=0&&((g[r][tempc]==-2) || (g[r][tempc]==5)))
    {
        if(th->hasManyLights[r][tempc]==1)
            th->presentLights--;
        --th->hasManyLights[r][tempc];

    }

    if(isBlankOfLimitLight(g, r+1, c))
        th->nearlights[r+1][c]--;
    if(isBlankOfLimitLight(g, r-1, c))
        th->nearlights[r-1][c]--;

    if(isBlankOfLimitLight(g, r, c+1))
        th->nearlights[r][c+1]--;
    if(isBlankOfLimitLight(g, r, c-1))
        th->nearlights[r][c-1]--;
    g[r][c]=-2;

}

bool putAll4(vector<vector<int> > & g)
{
    int k=0;
    while(k<all.size() && all[k].lights==4)
    {

        for(int i=0; i<direction.size(); ++i)
        {
            point des=direction[i]+all[k];//error???
            if(canPlaceLight(g, des))
            {
                if(placeLight(g, des.row, des.col)) ;
                else
                {
                    //flipLight(g, des.row, des.col);
                    return false;
                }

            }
            else
                return false;

        }
        ++k;
    }
    return true;
}

int allAnsSum;
bool putlight(threadTask *mythread, int r, int c, int pointIndex, int leftlights,int step=0);

//锟斤拷眉锟斤拷锟絛irection锟斤拷锟斤拷锟斤拷锟节硷拷锟斤拷锟斤拷锟斤拷息
bool putlight(threadTask *mythread, int r, int c, int pointIndex, int leftlights, int step)//step=0锟斤拷示锟节放猴拷锟斤拷锟接★拷
{
    if(mythread->test.size()>12 && allAnsSum>=MAX_ANS)
        return true;

    if(pointIndex>=all.size() || (step==1))
    {
        //锟斤拷锟斤拷全锟斤拷锟斤拷锟斤拷锟斤拷
        //if(mythread->seed==10)
        //cout<<"presentLights="<<mythread->presentLights<<"totalShouldLights="<<totalShouldLights<<endl;
        if(mythread->presentLights>=totalShouldLights)
        {
           mythread->ans=mythread->test;
           mythread->findAns=true;
	  mythread->ansSum++;
		allAnsSum++;
            /* Output(g);
             cout<<endl;*/
        }
        else
        {

            if(step==0)
            {
                mythread->lastLight.clear();
                for(int i=0; i < mythread->test.size(); ++i)
                {
                    for(int j=0; j < mythread->test[i].size(); ++j)
                    {
                        if(mythread->hasManyLights[i][j]==0 && (mythread->test[i][j] == -2))
                        {
                            point temp= {i,j,0};
                            mythread->lastLight.push_back(temp);
                        }
                    }
                }
                putlight(mythread, r, c, 0,leftlights, 1);

            }
            else
            {
                int put=0;
                for(int i=pointIndex; i<mythread->lastLight.size(); ++i)
                {
                    if(canPlaceLight(mythread->test,mythread->lastLight[i], mythread))
                    {

                        if(placeLight(mythread->test, mythread->lastLight[i].row, mythread->lastLight[i].col, mythread))
                        {
                            put=1;
                            putlight(mythread,r,c, i+1,leftlights, 1);
                        }
                        flipLight(mythread->test, mythread->lastLight[i].row, mythread->lastLight[i].col, mythread);

                    }
                }
                if(!put) return false;
            }

            //putLightOfSpace()

        }
    }
    else
    {
        if(!leftlights)//锟斤拷前point锟斤拷围锟侥碉拷锟斤拷锟窖撅拷锟节凤拷锟斤拷锟?, 锟节凤拷锟斤拷一锟斤拷锟斤拷锟斤拷
        {
            if((pointIndex+1) < all.size())
            {
                r=all[pointIndex+1].row;
                c=all[pointIndex+1].col;
                putlight(mythread, all[pointIndex+1].row, all[pointIndex+1].col, pointIndex+1, all[pointIndex+1].lights-mythread->nearlights[r][c]);
            }
            else
                putlight(mythread,r,c,pointIndex+1,0);

        }
        else if(leftlights)
        {

            int put=0;
             /*if(mythread->seed==10)
            cout<<"step 0, seed="<<mythread->seed<<",presentLights="<<mythread->presentLights<<"totalShouldLights="<<totalShouldLights<<endl;
            */
            //cout<<"seed="<<mythread->seed<<",pointIndex="<<pointIndex<<"presentLights="<<mythread->presentLights<<"totalShouldLights="<<totalShouldLights<<endl;
            for(int i=0; i<direction.size(); ++i)
            {
                point des=direction[i]+all[pointIndex];
                if(canPlaceLight(mythread->test, des, mythread))//==-2
                {
                    if(placeLight(mythread->test, des.row, des.col, mythread))
                    {
                        put=1;
                        putlight(mythread,r,c,pointIndex,leftlights-1);//锟斤拷锟斤拷

                    }
                    flipLight(mythread->test, des.row, des.col, mythread);

                }
                /*else
                   return false;*/
            }
            if(!put)
                return false;

        }
    }
    return true;
}

void *putLightTask(void *s) {
    threadTask *p=(threadTask *)s;
    for(int i=0;i<4;++i){

        if(p->seed&(1<<i)){
            point ss=all[p->allIndex] + direction[i];
            if(canPlaceLight(p->test, ss, p)){
                    if(placeLight(p->test, ss.row, ss.col, p)) ;
                    else {
                        return NULL;
                    }
            }
            else {

                return NULL;
            }
        }
    }
    p->isAlive=true;
     //锟斤拷始锟斤拷锟矫递癸拷
    if((p->allIndex+1) < all.size())
        putlight(p, all[p->allIndex+1].row, all[p->allIndex+1].col, p->allIndex+1, all[p->allIndex+1].lights, 0);
    else
         putlight(p, 0, 0, p->allIndex+1,0, 0);

    return NULL;


}
vector<vector<int> > solveAkari(vector<vector<int> > & g)
{
    // 1.锟斤拷锟斤拷色锟斤拷锟斤拷锟街硷拷锟斤拷
    pthread_t ntid[100];
    int err;
    direction.clear();
    nearlights.clear();
    hasManyLights.clear();
    all.clear();
    lastLight.clear();
    totalShouldLights=presentLights=0;//锟斤拷要漏锟斤拷
    allAnsSum=0;
    direction.push_back(point(1,0));
    direction.push_back(point(-1,0));

    direction.push_back(point(0,-1));
    direction.push_back(point(0,1));
    for(int i=0;i<threads.size();++i){
         threads[i].hasManyLights.clear();
         threads[i].test.clear();
         threads[i].nearlights.clear();
    }
    threads.clear();
	globalR=g.size();
	globalC=g[0].size();
    // if(g.size()>10) return g;
    for(int i=0; i<g.size(); ++i)
    {
        vector<int>  lightvec,nearlightsvec,hasManyLightsVec;

        for(int j=0; j<g[i].size(); ++j)
        {
            hasManyLightsVec.push_back(0);
            if(g[i][j]==-2)
                totalShouldLights++;
            if(g[i][j]>=0)
            {
                point temp= {i,j,g[i][j]};
                all.push_back(temp);
            }
            nearlightsvec.push_back(0);
        }
        nearlights.push_back(nearlightsvec);
        hasManyLights.push_back(hasManyLightsVec);
    }
    sort(all.begin(),all.end());
	//把边界的点往后排放
    if(putAll4(g))
    {
        int k=0;
        while(k<all.size() && all[k].lights==4) ++k;
        if(k<all.size())
        {

            //if the point
            if(all[k].lights==3)
            {
                int ss=0xf;
                for(int i=0; i<4; ++i)
                {
                    int mask=~(1<<i);
                    //cout<<"seed="<<(ss&mask)<<endl;
                    threadTask temp(ss&mask, k, g, nearlights,hasManyLights);
                    temp.presentLights=presentLights;
                    threads.push_back(temp);
                }
            }
            //allocate task without thinking,  6 tasks
            else if(all[k].lights==2)
            {
                int ss=0xf;
                for(int i=0; i<4; ++i)
                {
                    int mask=~(1<<i);
                    for(int j=i+1; j<4; ++j)
                    {
                        int mask2=~(1<<j);
                        //cout<<"seed="<<(ss&mask&mask2)<<endl;
                        threadTask temp(ss&mask&mask2,k,g,nearlights,hasManyLights);
                        temp.presentLights=presentLights;
                        threads.push_back(temp);
                    }
                }
            }
             else if(all[k].lights==1)
            {

                for(int i=0; i<4; ++i)
                {

                        int mask2=(1<<i);
                        //cout<<"seed="<<(ss&mask&mask2)<<endl;
                        threadTask temp(mask2,k,g,nearlights,hasManyLights);
                        temp.presentLights=presentLights;
                        threads.push_back(temp);

                }
            }
	    printf("start point(%d, %d)\n", all[k].row, all[k].col) ;
            for(int i=0; i<threads.size(); ++i)
            {
                while(err=pthread_create(&ntid[i],NULL,putLightTask,(void *)&(threads[i])))
                {
                    printf("fail\n");
                }
            }
            for(int i=0; i<threads.size(); ++i)
            	pthread_join(ntid[i],NULL);
             for(int i=0; i<threads.size(); ++i){
                if(threads[i].findAns) return threads[i].ans;
            }
            //putlight(g,all[k].row,all[k].col,k,all[k].lights);
        }

        //put 3,2,1
    }
    return g;

}

}

/*

7 7
-2 1 -2 -2 -2 -2 -2
-2 -2 3 -2 -2 -2 0
-2 -2 -2 -2 -2 1 -2
-2 -2 -2 -1 -2 -2 -2
-2 1 -2 -2 -2 -2 -2
0 -2 -2 -2 2 -2 -2
-2 -2 -2 -2 -2 0 -2

*/
using namespace aka;
ifstream iff("test2.txt");//TODO:锟侥憋拷路锟斤拷
int main()
{
    int M,N;
    int cnt=0;

    vector<vector<int> > g,ansVec;
    vector<int> *temp;
    struct timeval first_time, second_time;
    if(!iff.is_open()) cout<<"FILE INVALID"<<endl;
    while(iff>>M>>N)
    {
        g.clear();
        for(int i=0; i<M; ++i)
        {
            int s;
            temp=new vector<int>();
            for(int j=0; j<N; ++j)
            {
                iff>>s;
                temp->push_back(s);
            }
            g.push_back(/*new vector<int>*/(*temp));
        }
        //aka::Output(g);
        //cout<<g[6][5]<<endl;

        cout<<"Answer of Matrix "<<++cnt<<endl;
        gettimeofday(&first_time, NULL);
        ansVec=aka::solveAkari(g);
        gettimeofday(&second_time, NULL);
        aka::Output(ansVec);
        //输出耗时
	int alives=0;
	for(int i=0;i<threads.size();++i){
		if(threads[i].isAlive) ++alives;
		cout<<threads[i].ansSum<<"\t"<<endl;
	}
	cout<<endl;
        double time_val = (second_time.tv_sec - first_time.tv_sec) * 1000000 + second_time.tv_usec - first_time.tv_usec;
        printf("seedNum=%d, alive seeds=%d", threads.size(), alives);
        printf("(%d * %d) time_val(us) = %lf\n", M, N, time_val);
        //return 0;

    }
    iff.close();

    //cout << "Hello world!" << endl;
    return 0;
}

/*

7 7
-2 1 -2 -2 -2 -2 -2
-2 -2 3 -2 -2 -2 0
-2 -2 -2 -2 -2 1 -2
-2 -2 -2 -1 -2 -2 -2
-2 1 -2 -2 -2 -2 -2
0 -2 -2 -2 2 -2 -2
-2 -2 -2 -2 -2 0 -2
*/



