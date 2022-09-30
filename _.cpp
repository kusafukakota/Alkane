#include<bits/stdc++.h>
using namespace std;
using vi = vector<int>;
using vvi = vector<vi>;
using graph = vvi;
#define rep(i,n) for(int i=0;i<n;i++)

string prefix[4][10]={
  {"","hen","do","tri","tetra","penta","hexa","hepta","octa","nona"},
  {"","deca","icosa","triaconta","tetraconta","pentaconta","hexaconta","heptaconta","octaconta","nonaconta"},
  {"","hecta","dicta","tricta","tetracta","pentacta","hexacta","heptacta","octacta","nonacta"},
  {"","kilia","dilia","trilia","tetralia","pentalia","hexalia","heptalia","octalia","nonalia"}
};

vi BFS(graph &g,vi &dis,vi &pre,int start=0){
  int n=g.size();
  dis.assign(n,-1); // 始点からの距離
  pre.assign(n,-1); // 経路復元用
  dis[start]=0;
  queue<int> q;
  q.push(start);
  while(!q.empty()){
    int v=q.front();
    q.pop();
    for(auto nv:g[v]){
      if(dis[nv]==-1){
        dis[nv]=dis[v]+1;
        pre[nv]=v;
        q.push(nv);
      }
    }
  }
  vi ret; // 始点からの距離が最大となる全頂点を格納
  int m=*max_element(dis.begin(),dis.end()); // 最長距離
  rep(i,n) if(dis[i]==m) ret.push_back(i);
  return ret;
}

vvi diameter(graph &g){
  vvi ret;
  vi dis,pre;
  vi edge=BFS(g,dis,pre);
  for(auto x:edge){
    vi edge_=BFS(g,dis,pre,x);
    for(auto y:edge_){
      vi path;
      for(int p=y;p!=x;p=pre[p]) path.push_back(p); // 経路復元
      path.push_back(x);
      ret.push_back(path);
    }
  }
  return ret;
}

string main_name(int n){
  if(n==1) return "methane";
  if(n==2) return "ethane";
  if(n==3) return "propane";
  if(n==4) return "butane";
  string name="";
  int p0=n%10,p1=(n/10)%10,p2=(n/100)%10,p3=(n/1000)%10; // 桁ごとに分解
  if(p1==1&&p0==1) name+="undeca"; //例外処理 11
  else{
    name+=prefix[0][p0];
    if(p1==2&&p0>=2) name+="cosa"; // 例外処理 22-29
    else name+=prefix[1][p1];
  }
  name+=prefix[2][p2];
  name+=prefix[3][p3];
  name+="ne";
  return name;
}

string sub_name(int n){
  string name=main_name(n);
  rep(i,3) name.pop_back();
  name+="yl";
  return name;
}

string sub_prefix(int n){
  if(n==1) return "";
  if(n==2) return "di";
  string name="";
  int p0=n%10,p1=(n/10)%10,p2=(n/100)%10,p3=(n/1000)%10;
  if(p1==1&&p0==1) name+="undeca";
  else{
    name+=prefix[0][p0];
    if(p1==2&&p0>=2) name+="cosa";
    else name+=prefix[1][p1];
  }
  name+=prefix[2][p2];
  name+=prefix[3][p3];
  return name;
}

string sub_prefix_complex(int n){
  if(n==1) return "";
  if(n==2) return "bis";
  if(n==3) return "tris";
  string name=sub_prefix(n);
  name+="kis";
  return name;
}

string itos(int n){
  string ret="";
  while(n){
    ret+=n%10+'0';
    n/=10;
  }
  reverse(ret.begin(),ret.end());
  return ret;
}

string make_name(map<string,pair<pair<string,bool>,vi>> &m,int len){
  string name="";
  for(auto i:m){
    string sub_name=i.second.first.first;
    vi pos=i.second.second;
    int sub_cnt=pos.size();
    rep(j,sub_cnt){
      name+=itos(pos[j]);
      if(j!=sub_cnt-1) name+=',';
      else name+='-';
    }
    if(!i.second.first.second) name+=sub_prefix(sub_cnt)+sub_name+'-';
    else name+=sub_prefix_complex(sub_cnt)+"("+sub_name+")-";
  }
  if(!name.empty()) name.pop_back();
  name+=sub_name(len);
  return name;
}

string remove_pos(string s){
  reverse(s.begin(),s.end());
  while(s.back()>'z'||s.back()<'a') s.pop_back();
  reverse(s.begin(),s.end());
  return s;
}

pair<string,bool> sub_calc(graph &g,vi &dis,vi &pre,vi &visited,vi &deg,int start){
  int n=g.size();
  dis.assign(n,-1); // 分岐点からの距離
  pre.assign(n,-1); // 経路復元用
  dis[start]=1; // 探索は分岐点から１つずらす
  queue<int> q;
  q.push(start);
  while(!q.empty()){
    int v=q.front();
    q.pop();
    for(auto nv:g[v]){
      if(dis[nv]==-1&&!visited[nv]){ // 未訪問 かつ 何の主鎖でもない
        dis[nv]=dis[v]+1;
        pre[nv]=v;
        q.push(nv);
      }
    }
  }
  int cnt=0; // 主鎖候補数
  int len=*max_element(dis.begin(),dis.end()); // 置換基の長さの最大値
  vvi longests; // 主鎖候補
  rep(i,n) if(dis[i]==len){
    cnt++;
    vi longest;
    for(int p=i;p!=start;p=pre[p]) longest.push_back(p); // 経路復元
    longest.push_back(start);
    reverse(longest.begin(),longest.end());
    longests.push_back(longest);
  }
  int max_sub_cnt=-1; // 置換基数の最大値
  vi sub_cnt(cnt,0); // 各候補の置換基数
  vvi sub_pos(cnt); // 各候補の置換基の位置
  rep(i,cnt){
    rep(j,len){
      rep(k,deg[longests[i][j]]-2){
        sub_pos[i].push_back(j); // 置換基の位置を記録
        sub_cnt[i]++;
      }
    }
    if(max_sub_cnt<sub_cnt[i]) max_sub_cnt=sub_cnt[i]; // 最大値の更新
  }
  vi main_chain=longests[0]; // 主鎖
  vi min_sub_pos(max_sub_cnt,1<<30); // 置換基の位置番号の最小値
  rep(i,cnt){ // 始点が固定なので反転後は比較しない
    if(sub_cnt[i]==max_sub_cnt){
      if(min_sub_pos>sub_pos[i]){ // 最小値の更新
        main_chain=longests[i];
        min_sub_pos=sub_pos[i];
      }
    }
  }
  map<pair<string,bool>,vi> sub_info; // key : ｛置換基名，複合基かどうか｝, value : 置換基の位置番号
  rep(i,len) visited[main_chain[i]]=1; // 主鎖はこれ以上探索されないようにする
  rep(i,len){
    vi nex; // 分岐点から置換基側に１つずらしたところ
    for(auto j:g[main_chain[i]]) if(!visited[j]) nex.push_back(j);
    if(nex.size()>=1){
      auto sub=sub_calc(g,dis,pre,visited,deg,nex[0]); // 再帰
      sub_info[sub].push_back(i+1);
    }
    if(nex.size()==2){
      auto sub=sub_calc(g,dis,pre,visited,deg,nex[1]); // 再帰
      sub_info[sub].push_back(i+1);
    }
  }
  map<string,pair<pair<string,bool>,vi>> sub_info_sort;
  for(auto i:sub_info) sub_info_sort[remove_pos(i.first.first)]=i;
  string name=make_name(sub_info_sort,len);
  bool have_sub=0;
  if(max_sub_cnt>0) have_sub=1;
  auto ret=make_pair(name,have_sub);
  return ret;
}

void output_name(map<string,pair<pair<string,bool>,vi>> &m,int len){
  string name="";
  for(auto i:m){
    string sub_name=i.second.first.first;
    vi pos=i.second.second;
    int sub_cnt=pos.size();
    rep(j,sub_cnt){
      name+=itos(pos[j]);
      if(j!=sub_cnt-1) name+=',';
      else name+='-';
    }
    if(!i.second.first.second) name+=sub_prefix(sub_cnt)+sub_name+'-';
    else name+=sub_prefix_complex(sub_cnt)+"("+sub_name+")-";
  }
  if(!name.empty()) name.pop_back();
  name+=main_name(len);
  cout<<name<<'\n';
}

struct Unionfind{
  vi par,usiz;
  int e=0,v;
  Unionfind(int n):par(n),usiz(n),v(n){
    rep(i,n){
      par[i]=i;
      usiz[i]=1;
    }
  }
  int root(int x){
    if(par[x]==x) return x;
    return par[x]=root(par[x]);
  }
  void unite(int x,int y){
    x=root(x),y=root(y);
    if(x==y) return;
    if(usiz[x]>usiz[y]) swap(x,y);
    par[x]=y;
    usiz[y]+=usiz[x];
    e++;
  }
  bool same(int x,int y){return root(x)==root(y);}
};

int main(){
  int n; // 頂点数
  cin>>n;
  graph g(n);
  Unionfind u(n);
  vi deg(n,0); // 次数を記録
  rep(i,n-1){
    int x,y;
    cin>>x>>y;
    if(x>n||y>n||x<=0||y<=0){ // 入力エラー
      cout<<"!! INPUT ERROR !!"<<'\n';
      return 0; // 終了
    }
    x--,y--;
    g[x].push_back(y);
    g[y].push_back(x);
    if(u.same(x,y)){ // 環構造
      cout<<"!! RING STRUCTURE !!"<<'\n';
      return 0;
    }
    u.unite(x,y);
    deg[x]++,deg[y]++;
  }
  rep(i,n){
    if(deg[i]>4){ // 次数が４より大
      cout<<"!! TOO MANY BONDS !!"<<'\n';
      return 0;
    }
  }
  vvi dia=diameter(g); // 全主鎖候補
  int cnt=dia.size(); // 候補数
  int len=dia[0].size(); // 主鎖の長さ
  int max_sub_cnt=-1; // 置換基数の最大値
  vi sub_cnt(cnt,0); // 各候補の置換基数
  vvi sub_pos(cnt); // 各候補の置換基の位置
  rep(i,cnt){
    rep(j,len){
      rep(k,deg[dia[i][j]]-2){
        sub_pos[i].push_back(j); // 置換基の位置を記録
        sub_cnt[i]++;
      }
    }
    if(max_sub_cnt<sub_cnt[i]) max_sub_cnt=sub_cnt[i];  // 最大値の更新
  }
  vi main_chain=dia[0]; // 主鎖
  vi min_sub_pos(max_sub_cnt,1<<30); // 置換基の位置番号の最小値
  bool flag=0; // 始点が決定しているかどうか
  rep(i,cnt){
    if(sub_cnt[i]==max_sub_cnt){
      vi sub_pos_rev=sub_pos[i]; // 反転したものも比較
      reverse(sub_pos_rev.begin(),sub_pos_rev.end());
      rep(j,max_sub_cnt) sub_pos_rev[j]=len-1-sub_pos_rev[j];
      if(min_sub_pos>sub_pos[i]||min_sub_pos>sub_pos_rev){ // 最小値の更新
        main_chain=dia[i];
        min_sub_pos=sub_pos[i];
        if(sub_pos[i]==sub_pos_rev) flag=0; // 始点未決定
        else{
          flag=1; // 始点決定
          if(sub_pos_rev<sub_pos[i]){ // 反転後が勝ちの場合
            reverse(main_chain.begin(),main_chain.end());
            reverse(min_sub_pos.begin(),min_sub_pos.end());
          }
        }
      }
    }
  }
  cout<<flag<<endl;
  if(flag){
    map<pair<string,bool>,vi> sub_info;
    vi dis,pre,visited(n,0);
    rep(i,len) visited[main_chain[i]]=1;
    rep(i,len){
      vi nex;
      for(auto j:g[main_chain[i]]) if(!visited[j]) nex.push_back(j);
      if(nex.size()>=1){
        auto sub=sub_calc(g,dis,pre,visited,deg,nex[0]);
        sub_info[sub].push_back(i+1);
      }
      if(nex.size()==2){
        auto sub=sub_calc(g,dis,pre,visited,deg,nex[1]);
        sub_info[sub].push_back(i+1);
      }
    }
    map<string,pair<pair<string,bool>,vi>> sub_info_sort;
    for(auto i:sub_info) sub_info_sort[remove_pos(i.first.first)]=i;
    output_name(sub_info_sort,len);
  }
  else{
    vi main_chain_rev=main_chain;
    reverse(main_chain_rev.begin(),main_chain_rev.end());
    map<pair<string,bool>,vi> sub_info,sub_info_rev;
    vi dis,pre,visited(n,0),visited_rev(n,0);
    rep(i,len){
      visited[main_chain[i]]=1;
      visited_rev[main_chain_rev[i]]=1;
    }
    rep(i,len){
      vi nex,nex_rev;
      for(auto j:g[main_chain[i]]) if(!visited[j]) nex.push_back(j);
      if(nex.size()>=1){
        auto sub=sub_calc(g,dis,pre,visited,deg,nex[0]);
        sub_info[sub].push_back(i+1);
      }
      if(nex.size()==2){
        auto sub=sub_calc(g,dis,pre,visited,deg,nex[1]);
        sub_info[sub].push_back(i+1);
      }
      for(auto j:g[main_chain_rev[i]]) if(!visited_rev[j]) nex_rev.push_back(j);
      if(nex.size()>=1){
        auto sub=sub_calc(g,dis,pre,visited_rev,deg,nex_rev[0]);
        sub_info_rev[sub].push_back(i+1);
      }
      if(nex.size()==2){
        auto sub=sub_calc(g,dis,pre,visited_rev,deg,nex_rev[1]);
        sub_info_rev[sub].push_back(i+1);
      }
    }
    map<string,pair<pair<string,bool>,vi>> sub_info_sort,sub_info_sort_rev;
    for(auto i:sub_info) sub_info_sort[remove_pos(i.first.first)]=i;
    for(auto i:sub_info_rev) sub_info_sort_rev[remove_pos(i.first.first)]=i;
    vi pos,pos_rev;
    rep(i,sub_info.size()){
      pos=next(sub_info_sort.begin(),i)->second.second;
      pos_rev=next(sub_info_sort_rev.begin(),i)->second.second;
      if(pos<pos_rev){
        flag=1;
        break;
      }
      if(pos>pos_rev){
        flag=0;
        break;
      }
    }
    if(flag) output_name(sub_info_sort,len);
    else output_name(sub_info_sort_rev,len);
  }
}
