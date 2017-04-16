#include <iostream>
#include <cstdlib>
#include <fstream>
#include <bitset>
#include <vector>
#include <queue>
    
using namespace std;
const int ITEM_NUM=500; //物品個數
const float FLOAT_ERR1=-0.0001; //浮點數誤差

    
vector<bitset<ITEM_NUM> > m,f,o,u; //歷史分錄
string history; 
string wanted; //讀檔的字串
string input,in; //做輸入的字串
int totalm=0,totalf=0,totalo=0,totalu=0;
queue<bitset<ITEM_NUM>  > vq; //一疊01字串的資料序列(紀錄有包含假想子集的歷史分錄)
int ans;
float maxs,maxc; // 紀錄答案
int item[500]; // 紀錄有購買的物品(假想分錄)
int icount=0; //紀錄item的尺寸
int minsupport; //support門檻
    
inline void dfs(bitset<ITEM_NUM>, bitset<ITEM_NUM>   , int  , int , int); //搜尋每一個子集合

        
int main(){
    ifstream fin;
    int idnum,pos,tmp;
    totalm=0;totalf=0;totalo=0;totalu=0;
    string gender;
    int type;
    bitset<ITEM_NUM> trans; //紀錄分錄
    for (int i = 0; i < 24; ++i)
    {
       cin>>history;
        fin.open(history.c_str());
        fin>>idnum;
        type=0;
            
        while (getline(fin,input)){
            icount=0;
            pos=input.find(',');
            input=input.substr(pos+1,input.size()-pos-1);
            pos=input.find(',');
            input=input.substr(pos+1,input.size()-pos-1);
            pos=input.find(',');
            input=input.substr(pos+1,input.size()-pos-1);
            pos=input.find(',');
            gender=input.substr(0,pos);
            input=input.substr(pos+1,input.size()-pos-1);
            pos=input.find(',');
            input=input.substr(pos+1,input.size()-pos-1);
            tmp=0;//把沒用的東西剪掉
            fin>>tmp;
            while (tmp==idnum){
                getline(fin,in);
                input=input+in.substr(in.rfind(','),in.size()-in.rfind(','));
                tmp=0;
                fin>>tmp;
            }
            /*
            這裡有點複雜，大致上是while開始時有一個分錄編號，讀下一個分錄看他編號是否一樣(type 3)，若
            一樣(type 3)           則把下一個買的東西接上上一個字串(type 3 -> type 2)，直到讀到不一樣
            時，處理開始的字串，讀到的不一樣的字串當下一次while的開始字串
            */
            idnum=tmp;
            trans.reset();
            if (type==0){
                if (input.size()==ITEM_NUM*2-1 && input.find(",0")!=input.npos)type=1;
                else type=2;
            }
            if (type==1){
                for (int i = 0; i < ITEM_NUM; ++i)
                {
                    if (input[i*2]=='1'){trans.set(i);
                        item[icount]=i;
                        icount++;
                    }
                }
                     
                
            }//type1的讀法
            else {
                int k;
                pos=input.find(',');
                while (pos!=input.npos){
                    k=atoi(input.substr(0,pos).c_str())-1;
                    input=input.substr(pos+1,input.size()-pos-1);
                    trans.set(k);
                    item[icount]=k;
                    icount++;
                    pos=input.find(',');
                }
                k=atoi(input.c_str())-1;
                trans.set(k);
                item[icount]=k;
                icount++;
            }
            //type2的讀法
            //type3上面已轉成type2
            if (gender=="M"){m.push_back(trans);totalm++;}
            else if (gender=="F"){f.push_back(trans);totalf++;}
            else if (gender=="O"){o.push_back(trans);totalo++;}
            else {u.push_back(trans);totalu++;}
            //依性別加到相對應的vector(陣列)
        }
        fin.close();
    }
    //input
    cin>>wanted;
    cin>>wanted;
    fin.open(wanted.c_str());
    //recommend
    while (getline(fin,input)){
        ans=501;maxs=0;maxc=0;
        pos=input.find(',');
        input=input.substr(pos+1,input.size()-pos-1);
        pos=input.find(',');
        input=input.substr(pos+1,input.size()-pos-1);
        pos=input.find(',');
        gender=input.substr(0,pos);
        input=input.substr(pos+1,input.size()-pos-1);
        pos=input.find(',');//剪掉不要的東西
        input=input.substr(pos+1,input.size()-pos-1);
        trans.reset();
        int k;
        bitset<ITEM_NUM > bs; //暫存用
        pos=input.find(',');
        icount=0;
        while (pos!=input.npos){
            k=atoi(input.substr(0,pos).c_str())-1;
            input=input.substr(pos+1,input.size()-pos-1);
            pos=input.find(',');
            trans.set(k);
            item[icount]=k;
            icount++;
        }
        pos=input.find(';');
        k=atoi(input.substr(0,pos).c_str())-1;
        trans.set(k);
        item[icount]=k;
        icount++;
        //讀假想分錄的購買物品
        input=input.substr(pos+1,input.size()-pos-1);
        minsupport=atoi(input.c_str());
        int vcount;
        int sum;
        if (gender=="M")sum=totalm;
        else if (gender=="F")sum=totalf;
        else if (gender=="O")sum=totalo;
        else sum=totalm+totalf+totalo+totalu;
        //求需要參考的總數
        for (int i = 0; i < icount; ++i)
        {
            bs.reset();
            bs.set(item[i]);
            vcount=0;
            if (gender=="M"){
                for (vector<bitset<ITEM_NUM> >::iterator it = m.begin(); it != m.end(); ++it)
                {
                    if ( ((*it)&(bs))==bs && (*it).count()>=bs.count()){vq.push((*it));vcount++;}
                }
            }
            else if (gender=="F"){
                for (vector<bitset<ITEM_NUM> >::iterator it = f.begin(); it != f.end(); ++it)
                {
                    if ( ((*it)&(bs))==bs && (*it).count()>=bs.count()){vq.push((*it));vcount++;}
                }
            }
            else if (gender=="O"){
                for (vector<bitset<ITEM_NUM> >::iterator it = o.begin(); it != o.end(); ++it)
                {
                    if ( ((*it)&(bs))==bs && (*it).count()>=bs.count()){vq.push((*it));vcount++;}
                }
            }
            else {
                for (vector<bitset<ITEM_NUM> >::iterator it = m.begin(); it != m.end(); ++it)
                {
                    if ( ((*it)&(bs))==bs && (*it).count()>=bs.count()) {vq.push((*it));vcount++;}
                }
                for (vector<bitset<ITEM_NUM> >::iterator it = f.begin(); it != f.end(); ++it)
                {
                    if ( ((*it)&(bs))==bs && (*it).count()>=bs.count()){vq.push((*it));vcount++;}
                }
                for (vector<bitset<ITEM_NUM> >::iterator it = o.begin(); it != o.end(); ++it)
                {
                    if ( ((*it)&(bs))==bs && (*it).count()>=bs.count()){vq.push((*it));vcount++;}
                }
                for (vector<bitset<ITEM_NUM> >::iterator it = u.begin(); it != u.end(); ++it)
                {
                    if ( ((*it)&(bs))==bs && (*it).count()>=bs.count()){vq.push((*it));vcount++;}
                }
            }//依性別去找相對應性別內有包含的歷史分錄
            dfs( bs , trans , vcount , sum, i);//看下面
            while (!vq.empty()){
                vq.pop();
            }//清空vq
    
        }
        if (maxs-minsupport > FLOAT_ERR1) 
            cout<<ans+1<<' '<<int(maxs+0.5)<<' '<<int(maxc+0.5)<<endl;
        else cout<<-1<<endl;
        //輸出答案
    }
    fin.close();
    //cin>>wanted;
  
    return 0;
}       
    
    
inline void dfs(bitset<ITEM_NUM> bs , bitset<ITEM_NUM> trans  , int vcount , int sum, int index){
    bitset<ITEM_NUM> tmp,tm;
    queue<bitset<ITEM_NUM>  > vqc; //暫存用
    float p,s,c;
    int tcount;
    p= float(vcount)/sum*100;
    if ( p - minsupport > FLOAT_ERR1 ){//因為電腦計算小數時會對太小的小數位數做進位，可能有只差一點點的被進位變差很多的情況，所以要容許一點誤差
        for ( int i = 0; i < ITEM_NUM; ++i){
            tmp.reset();
            if (!trans.test(i)){
                tcount=vcount;
                vq.push(tmp);
                tmp=bs;
                tmp.set(i);
                while (1){
                    tm=vq.front();
                    if (tm.none()){vq.pop();break;}
                    if ( (tm & tmp) ==tmp && tm.count()>=tmp.count());
                    else tcount--;
                    vq.pop();
                    vq.push(tm);
                }//在已包含子集合的歷史分錄中找有包含(子集合+i)的(i是想推薦的東西)
                s= float(tcount)/sum*100;
                if (s - minsupport >= FLOAT_ERR1 ){//如果嘗試的答案有過門檻
                    c=(s/p)*100;//求confidence
                    if (c-maxc>-1*FLOAT_ERR1){
                        maxc=c;
                        maxs=s;
                        ans=i;
                    }
                    else if (c-maxc > FLOAT_ERR1){
                        if (s -maxs > -1*FLOAT_ERR1){
                            maxs=s;
                            ans=i;
                        }
                        else if (s-maxs > FLOAT_ERR1){
                            ans=min(ans,i);
                        }
                    }//更新答案
                }
                    
            }
                
        }
        for (int j = index + 1; j < icount; ++j)
        {
            tmp.reset();
            vq.push(tmp);
            tmp=bs;
            tmp.set(item[j]);
            while (1){
                tm=vq.front();
                if (tm.none()){vq.pop();break;}
                if ( (tm&tmp)==tmp && tm.count()>=tmp.count()){
                    vq.push(tm);
                }
                else {
                    vcount--;
                    vqc.push(tm);                    
                }
                vq.pop();
            }//在已包含子集合的歷史分錄中找有包含(子集合+j)的(j是新加進集合的東西)
            dfs(tmp , trans, vcount , sum, j);//遞迴
            while (!vqc.empty()){
                vq.push(vqc.front());
                vqc.pop();
                vcount++;
            }//換原成只包含子集合的
        }//如果這個子集合有超過門檻，就在加一個物品到子集合中，遞迴搜尋答案
    }
    return ;
}
    
