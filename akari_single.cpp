#include <iostream>

#include <bits/stdc++.h>
#include "akari.h"
#include<sys/time.h>
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
struct black
{
    int  r,c;
};
vector<point>direction;
vector<point> all, lastLight;
vector<black> blackpoints;
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
bool canPlaceLight(vector<vector<int> > & g,int r, int c)
{
    int row=g.size()-1;
    int col=g[0].size()-1;
    if(r<0 || c<0 || r>row || c>col)
    {
        return false;
    }
    if(hasManyLights[r][c]>0) return false;
    else
        return g[r][c]==-2;

}
bool canPlaceLight(vector<vector<int> > & g, point &p)
{
    int row=g.size()-1;
    int col=g[0].size()-1;
    if(p.row<0 || p.col<0 || p.row>row || p.col>col)
    {
        return false;
    }
    if(hasManyLights[p.row][p.col]>0) return false;
    else
        return g[p.row][p.col]==-2;

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

void flipLight(vector<vector<int> > & g,int r, int c)
{
    //nearlights --;
    int flag=0,tempc=c,tempr=r;
    //--hasManyLights[r][c];
    while(tempr>=0&&((g[tempr][c]==-2) || (g[tempr][c]==5)))
    {
        if(hasManyLights[tempr][c]==1)
            presentLights--;
        --hasManyLights[tempr][c];
        --tempr;

    }
    tempr=r;
    while(++tempr<g.size()&&((g[tempr][c]==-2) || (g[tempr][c]==5)))
    {

        if(hasManyLights[tempr][c]==1)
            presentLights--;
        --hasManyLights[tempr][c];

    }
    while(++tempc<g[0].size()&&((g[r][tempc]==-2) || (g[r][tempc]==5)))
    {
        if(hasManyLights[r][tempc]==1)
            presentLights--;
        --hasManyLights[r][tempc];

    }
    tempc=c;
    while(--tempc>=0&&((g[r][tempc]==-2) || (g[r][tempc]==5)))
    {
        if(hasManyLights[r][tempc]==1)
            presentLights--;
        --hasManyLights[r][tempc];

    }

    if(isBlankOfLimitLight(g, r+1, c))
        nearlights[r+1][c]--;
    if(isBlankOfLimitLight(g, r-1, c))
        nearlights[r-1][c]--;

    if(isBlankOfLimitLight(g, r, c+1))
        nearlights[r][c+1]--;
    if(isBlankOfLimitLight(g, r, c-1))
        nearlights[r][c-1]--;
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
                    flipLight(g, des.row, des.col);
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
bool putlight(vector<vector<int> > & g, int r, int c, int pointIndex, int leftlights,int step=0);

//最好加上direction遍历到第几个的信息
bool putlight(vector<vector<int> > & g, int r, int c, int pointIndex, int leftlights, int step)//step=0表示摆放黑棋子。
{
    /*if(ans>=1)
        return true;*/
    if(g.size()>12 && ans>=MAX_ANS) return true;
    if(pointIndex>=all.size() || (step==1))
    {
        //而且全都被照亮
        if(presentLights>=totalShouldLights)
        {
            ansVec=g;
            ans++;
           /* Output(g);
            cout<<endl;*/
        }
        else
        {
            //cout<<"presentLights="<<presentLights<<"totalShouldLights="<<totalShouldLights<<endl;
            if(step==0)
            {
                lastLight.clear();
                for(int i=0; i<g.size(); ++i)
                {
                    for(int j=0; j<g[i].size(); ++j)
                    {
                        if(hasManyLights[i][j]==0 && (g[i][j]==-2)){
                            point temp={i,j,0};
                            lastLight.push_back(temp);
                        }
                    }
                }
                 putlight(g,r,c, 0 ,leftlights, 1);

            }
            else
            {
                 int put=0;
                 for(int i=pointIndex; i<lastLight.size(); ++i){
                     if(canPlaceLight(g,lastLight[i])){

                     if(placeLight(g, lastLight[i].row, lastLight[i].col)){
                        put=1;
                        putlight(g,r,c, i+1 ,leftlights, 1);
                     }
                     flipLight(g, lastLight[i].row, lastLight[i].col);

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
                putlight(g,all[pointIndex+1].row,all[pointIndex+1].col,pointIndex+1,all[pointIndex+1].lights-nearlights[r][c]);
            }
            else
                putlight(g,r,c,pointIndex+1,0);

        }
        else if(leftlights)
        {
            int put=0;
            for(int i=0; i<direction.size(); ++i)
            {
                point des=direction[i]+all[pointIndex];
                if(canPlaceLight(g, des))//==-2
                {
                    if(placeLight(g, des.row, des.col))
                    {
                        put=1;
                        putlight(g,r,c,pointIndex,leftlights-1);//回溯

                    }
                    flipLight(g, des.row, des.col);

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
vector<vector<int> > solveAkari(vector<vector<int> > & g)
{
    // 1.将黑色的数字加入
    direction.clear();
    nearlights.clear();
    hasManyLights.clear();
    all.clear();
    lastLight.clear();
    ans=0;
    direction.push_back(point(-1,0));
    direction.push_back(point(1,0));
    direction.push_back(point(0,1));
    direction.push_back(point(0,-1));
    ansVec.clear();
    presentLights=totalShouldLights=0;
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
            if(g[i][j]>=-1)
            {
                lightvec.push_back(1);
            }
            else
            {
                lightvec.push_back(0);

            }
            nearlightsvec.push_back(0);
        }
        islight.push_back(lightvec);
        nearlights.push_back(nearlightsvec);
        hasManyLights.push_back(hasManyLightsVec);
    }
    sort(all.begin(),all.end());
    if(putAll4(g))
    {
        int k=0;
        while(k<all.size() && all[k].lights==4)
            ++k;
        int r=0,c=0;

        if(k<all.size())
            putlight(g,all[k].row,all[k].col,k,all[k].lights);
        else
            putlight(g,r,c,k,0);
        return ansVec;
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
ifstream iff("test2.txt");
int main()
{
    int M,N;
    struct timeval first_time, second_time;
    int cnt=0;
    while(iff>>M>>N)
    {
        vector<vector<int> > g;
        vector<int> *temp;
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
        cout<<"Answer of "<<++cnt<<endl;
        gettimeofday(&first_time, NULL);
        aka::ansVec=aka::solveAkari(g);
        gettimeofday(&second_time, NULL);
        aka::Output(aka::ansVec);
        //accomplish time
        double time_val = (second_time.tv_sec - first_time.tv_sec) * 1000000 + second_time.tv_usec - first_time.tv_usec;
        printf("(%d * %d) time_val(us) = %lf, total answers=%d\n", M, N, time_val, aka::ans);
    }

    iff.close();
    //cout << "Hello world!" << endl;
    return 0;
}

