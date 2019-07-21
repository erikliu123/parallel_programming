#include <bits/stdc++.h>
#include <pthread.h>
//#include "akari.h"
using namespace std;

namespace aka
{

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
    bool operator <(const point &b) const //先放置4个的
    {
        return lights>b.lights;
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
    bool findAns;
    int allIndex,presentLights;
    vector<vector<int> > test;
    vector<vector<int> >nearlights, hasManyLights;
    vector<vector<int> > ans;
    vector<point> lastLight;

    threadTask(int seed, int allIndex, vector<vector<int> > &test, vector<vector<int> > nearlights, vector<vector<int> >& hasManyLights)
    {
        this->seed=seed;
        this->allIndex=allIndex;
        this->nearlights=nearlights;
        this->hasManyLights=hasManyLights;
        findAns=false;
        presentLights=0;
    }

};
vector<threadTask> threads;
vector<point>direction;
vector<point> all, lastLight;
vector<vector<int> > islight,nearlights,hasManyLights,ansVec;
int ans,totalShouldLights, presentLights;



//请在命名空间内编写代码，否则后果自负
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
    //调用前判断canPlaceLight
    int flag=0,tempc=c,tempr=r;
    g[r][c]=5;
    //++presentLights;
    //++hasManyLights[r][c];
    while(tempr>=0&&((g[tempr][c]==-2) || (g[tempr][c]==5))) //包括一次g[r][c]  思考：while(tempr>=0&&(g[tempr][c]==-2 || (g[tempr--][c]==5))){
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
    return flag==0;/**=1说明成功放置了*/

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
    //调用前判断canPlaceLight
    int flag=0,tempc=c,tempr=r;
    g[r][c]=5;
    //++presentLights;
    //++hasManyLights[r][c];
    while(tempr>=0&&((g[tempr][c]==-2) || (g[tempr][c]==5))) //包括一次g[r][c]  思考：while(tempr>=0&&(g[tempr][c]==-2 || (g[tempr--][c]==5))){
    {
        ++th->hasManyLights[tempr][c];
        if(th->hasManyLights[tempr][c]==1)
            presentLights++;
        tempr--;

    }
    tempr=r;
    while(++tempr<g.size()&&((g[tempr][c]==-2) || (g[tempr][c]==5)))
    {
        ++th->hasManyLights[tempr][c];
        if(th->hasManyLights[tempr][c]==1)
            presentLights++;

    }
    while(++tempc<g[0].size()&&((g[r][tempc]==-2) || (g[r][tempc]==5)))
    {
        ++th->hasManyLights[r][tempc];
        if(th->hasManyLights[r][tempc]==1)
            presentLights++;

    }
    tempc=c;
    while(--tempc>=0&&((g[r][tempc]==-2) || (g[r][tempc]==5)))
    {
        ++th->hasManyLights[r][tempc];
        if(th->hasManyLights[r][tempc]==1)
            presentLights++;

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
    return flag==0;/**=1说明成功放置了*/

}

void flipLight(vector<vector<int> > & g,int r, int c,  threadTask *th)
{
    //nearlights --;
    int flag=0,tempc=c,tempr=r;
    //--th->hasManyLights[r][c];
    while(tempr>=0&&((g[tempr][c]==-2) || (g[tempr][c]==5)))
    {
        if(th->hasManyLights[tempr][c]==1)
            presentLights--;
        --th->hasManyLights[tempr][c];
        --tempr;

    }
    tempr=r;
    while(++tempr<g.size()&&((g[tempr][c]==-2) || (g[tempr][c]==5)))
    {

        if(th->hasManyLights[tempr][c]==1)
            presentLights--;
        --th->hasManyLights[tempr][c];

    }
    while(++tempc<g[0].size()&&((g[r][tempc]==-2) || (g[r][tempc]==5)))
    {
        if(th->hasManyLights[r][tempc]==1)
            presentLights--;
        --th->hasManyLights[r][tempc];

    }
    tempc=c;
    while(--tempc>=0&&((g[r][tempc]==-2) || (g[r][tempc]==5)))
    {
        if(th->hasManyLights[r][tempc]==1)
            presentLights--;
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
bool putlight(threadTask *mythread, int r, int c, int pointIndex, int leftlights,int step=0);

//最好加上direction遍历到第几个的信息
bool putlight(threadTask *mythread, int r, int c, int pointIndex, int leftlights, int step)//step=0表示摆放黑棋子。
{
    if(mythread->findAns)
        return true;

    if(pointIndex>=all.size() || (step==1))
    {
        //而且全都被照亮
        if(mythread->presentLights>=totalShouldLights)
        {
           mythread->ans=mythread->test;
           mythread->findAns=true;
            /* Output(g);
             cout<<endl;*/
        }
        else
        {
            //cout<<"presentLights="<<mythread->presentLights<<"totalShouldLights="<<totalShouldLights<<endl;
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

                        if(placeLight(mythread->test, mythread->lastLight[i].row, lastLight[i].col, mythread))
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
        if(!leftlights)//当前point周围的灯泡已经摆放完毕, 摆放下一粒灯泡
        {
            if((pointIndex+1) < all.size())
            {
                r=all[pointIndex+1].row;
                c=all[pointIndex+1].col;
                putlight(mythread, all[pointIndex+1].row, all[pointIndex+1].col,pointIndex+1, all[pointIndex+1].lights-mythread->nearlights[r][c]);
            }
            else
                putlight(mythread,r,c,pointIndex+1,0);

        }
        else if(leftlights)
        {

            int put=0;
            for(int i=0; i<direction.size(); ++i)
            {
                point des=direction[i]+all[pointIndex];
                if(canPlaceLight(mythread->test, des, mythread))//==-2
                {
                    if(placeLight(mythread->test, des.row, des.col, mythread))
                    {
                        put=1;
                        putlight(mythread,r,c,pointIndex,leftlights-1);//回溯

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
            if(canPlaceLight(p->test, all[p->allIndex])){
                    placeLight(p->test, all[p->allIndex].row, all[p->allIndex].col);
            }
            else return NULL;
        }
    }
     //开始调用递归
    if((p->allIndex+1) < all.size())
        putlight(p, all[p->allIndex+1].row, all[p->allIndex+1].col, p->allIndex, all[p->allIndex+1].lights, 0);
    else
         putlight(p, 0, 0, p->allIndex+1,0, 0);

    return NULL;


}
vector<vector<int> > solveAkari(vector<vector<int> > & g)
{
    // 1.将黑色的数字加入
    pthread_t ntid;
    int err;
    direction.clear();
    nearlights.clear();
    hasManyLights.clear();
    all.clear();
    lastLight.clear();
    totalShouldLights=presentLights=0;//不要漏掉！
    direction.push_back(point(-1,0));
    direction.push_back(point(1,0));
    direction.push_back(point(0,1));
    direction.push_back(point(0,-1));
    threads.clear();

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
    if(putAll4(g))
    {
        int k=0;
        while(k<all.size() && all[k++].lights==4);
        if(k<all.size())
        {
            //分工
            //可以摆放三颗灯泡，但是有可能周围有一个障碍物。导致无法合理地平均分配任务
            if(all[k].lights==3)
            {
                int ss=0xf;
                for(int i=0; i<4; ++i)
                {
                    int mask=~(1<<i);
                    cout<<"seed="<<(ss&mask)<<endl;
                    threadTask temp(ss&mask,k,g,nearlights,hasManyLights);
                    threads.push_back(temp);
                }
            }
            //可以摆放两个灯泡，无脑；六个task
            else if(all[k].lights==2)
            {
                int ss=0xf;
                for(int i=0; i<4; ++i)
                {
                    int mask=~(1<<i);
                    for(int j=i+1; j<4; ++j)
                    {
                        int mask2=~(1<<j);
                        cout<<"seed="<<(ss&mask&mask2)<<endl;
                        threadTask temp(ss&mask&mask2,k,g,nearlights,hasManyLights);
                        threads.push_back(temp);
                    }
                }
            }

           /* for(int i=0; i<threads.size(); ++i)
            {
                while(err=pthread_create(&ntid,NULL,putLightTask,(void *)&(threads[i])))
                {
                    printf("fail\n");
                }
            }
            pthread_join(ntid,NULL);
             for(int i=0; i<threads.size(); ++i){
                if(threads[i].findAns) return threads[i].ans;
            }*/
            //putlight(g,all[k].row,all[k].col,k,all[k].lights);
        }

        //put 3,2,1
    }
    return g;

}

}

ifstream iff("test2.txt");//TODO:改变路径
int main()
{
    int M,N;
    cout << "Hello world!" << endl;

    vector<vector<int> > g,ansVec;
    vector<int> *temp;
    while(!iff.eof())
    {
        iff>>M>>N;
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
        cout<<"Answer of G:"<<endl;
        ansVec=aka::solveAkari(g);
        aka::Output(ansVec);

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

