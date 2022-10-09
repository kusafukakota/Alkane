#include<bits/stdc++.h>
using namespace std;
using vi = vector<int>;
using vvi = vector<vi>;
#define fi first
#define se second
#define pb push_back
#define rep(i,n) for(int i=0;i<n;i++)
#define all(v) (v).begin(),(v).end()

string prefix[4][10]={
  {"","hen","do","tri","tetra","penta","hexa","hepta","octa","nona"},
  {"","deca","icosa","triaconta","tetraconta","pentaconta","hexaconta","heptaconta","octaconta","nonaconta"},
  {"","hecta","dicta","tricta","tetracta","pentacta","hexacta","heptacta","octacta","nonacta"},
  {"","kilia","dilia","trilia","tetralia","pentalia","hexalia","heptalia","octalia","nonalia"}
};

vi BFS(vvi &graph,vi &dis,vi &pre,int start=0){
  int n=graph.size();
  dis.assign(n,-1); // 始点からの距離
  pre.assign(n,-1); // 経路復元用
  dis[start]=0;
  queue<int> q;
  q.push(start);
  while(!q.empty()){
    int v=q.front();
    q.pop();
    for(auto nv:graph[v]){
      if(dis[nv]==-1){
        dis[nv]=dis[v]+1;
        pre[nv]=v;
        q.push(nv);
      }
    }
  }
  vi ret; // 始点からの距離が最大となる全頂点を格納
  int m=*max_element(all(dis)); // 最長距離
  rep(i,n) if(dis[i]==m) ret.pb(i);
  return ret;
}

vvi diameter(vvi &graph){
  vvi ret;
  vi dis,pre;
  vi edge=BFS(graph,dis,pre);
  for(auto x:edge){
    vi edge_=BFS(graph,dis,pre,x);
    for(auto y:edge_){
      vi path;
      for(int p=y;p!=x;p=pre[p]) path.pb(p); // 経路復元
      path.pb(x);
      ret.pb(path);
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
  reverse(all(ret));
  return ret;
}

string make_name(map<string,vector<pair<pair<string,bool>,vi>>> &m,int len){
  string name="";
  for(auto i:m){
    sort(all(i.se));
    for(auto j:i.se){
      string sub_name=j.fi.fi; // 置換基名
      vi pos=j.se; // 置換基の位置番号
      int sub_cnt=pos.size(); // 置換基の個数
      rep(k,sub_cnt){
        name+=itos(pos[k]);
        if(k!=sub_cnt-1) name+=','; // 位置番号はカンマで区切る
        else name+='-'; // 位置番号と名前の間はハイフンで区切る
      }
      // 置換基の個数 ＋（位置番号を含む）名前＋ハイフン（置換基同士を区切る）
      if(!j.fi.se) name+=sub_prefix(sub_cnt)+sub_name+'-';
      else name+=sub_prefix_complex(sub_cnt)+"("+sub_name+")-"; // 複合基は括弧で括る
    }
  }
  if(!name.empty()) name.pop_back();
  name+=sub_name(len); // 置換基の母体名
  return name;
}

string remove_pos(string s){
  reverse(all(s));
  while(s.back()>'z'||s.back()<'a') s.pop_back(); // 初めて小文字アルファベットが現れるまで削除
  reverse(all(s));
  return s;
}

pair<string,bool> sub_calc(vvi &graph,vi &dis,vi &pre,vi &is_main,vi &deg,int start){
  int n=graph.size();
  dis.assign(n,-1); // 分岐点からの距離
  pre.assign(n,-1); // 経路復元用
  dis[start]=1; // 探索は分岐点から１つずらす
  queue<int> q;
  q.push(start);
  while(!q.empty()){
    int v=q.front();
    q.pop();
    for(auto nv:graph[v]){
      if(dis[nv]==-1&&!is_main[nv]){ // 未訪問 かつ 何の主鎖でもない
        dis[nv]=dis[v]+1;
        pre[nv]=v;
        q.push(nv);
      }
    }
  }
  int cnt=0; // 主鎖候補数
  int len=*max_element(all(dis)); // 置換基の長さの最大値
  vvi longests; // 主鎖候補
  rep(i,n) if(dis[i]==len){
    cnt++;
    vi longest;
    for(int p=i;p!=start;p=pre[p]) longest.pb(p); // 経路復元
    longest.pb(start);
    reverse(all(longest));
    longests.pb(longest);
  }
  int max_sub_cnt=-1; // 置換基数の最大値
  vi sub_cnt(cnt,0); // 各候補の置換基数
  vvi sub_pos(cnt); // 各候補の置換基の位置
  rep(i,cnt){
    rep(j,len){
      rep(k,deg[longests[i][j]]-2){
        sub_pos[i].pb(j); // 置換基の位置を記録
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
  rep(i,len) is_main[main_chain[i]]=1; // 主鎖はこれ以上探索されないようにする
  rep(i,len){
    vi nex; // 分岐点から置換基側に１つずらしたところ
    for(auto j:graph[main_chain[i]]) if(!is_main[j]) nex.pb(j);
    if(nex.size()>=1){
      auto sub=sub_calc(graph,dis,pre,is_main,deg,nex[0]); // 再帰
      sub_info[sub].pb(i+1);
    }
    if(nex.size()==2){
      auto sub=sub_calc(graph,dis,pre,is_main,deg,nex[1]); // 再帰
      sub_info[sub].pb(i+1);
    }
  }
  map<string,vector<pair<pair<string,bool>,vi>>> sub_info_sort;
  for(auto i:sub_info) sub_info_sort[remove_pos(i.fi.fi)].pb(i);
  string name=make_name(sub_info_sort,len);
  bool is_complex=0; // 複合基かどうか
  if(max_sub_cnt>0) is_complex=1; // 置換基を持つなら複合基
  auto ret=make_pair(name,is_complex);
  return ret;
}

void output_name(map<string,vector<pair<pair<string,bool>,vi>>> &m,int len){
  string name="";
  for(auto i:m){
    sort(all(i.se));
    for(auto j:i.se){
      string sub_name=j.fi.fi;
      vi pos=j.se;
      int sub_cnt=pos.size();
      rep(k,sub_cnt){
        name+=itos(pos[k]);
        if(k!=sub_cnt-1) name+=',';
        else name+='-';
      }
      if(!j.fi.se) name+=sub_prefix(sub_cnt)+sub_name+'-';
      else name+=sub_prefix_complex(sub_cnt)+"("+sub_name+")-";
    }
  }
  if(!name.empty()) name.pop_back();
  name+=main_name(len); // sub_name(len) -> main_name(len)に
  cout<<name<<'\n'; // return name; -> cout<<name<<'\n';（出力）に
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
  vvi graph(n);
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
    graph[x].pb(y); graph[y].pb(x);
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
  vvi dia=diameter(graph); // 全主鎖候補
  int cnt=dia.size(); // 候補数
  int len=dia[0].size(); // 主鎖の長さ
  int max_sub_cnt=-1; // 置換基数の最大値
  vi sub_cnt(cnt,0); // 各候補の置換基数
  vvi sub_pos(cnt); // 各候補の置換基の位置
  rep(i,cnt){
    rep(j,len){
      rep(k,deg[dia[i][j]]-2){
        sub_pos[i].pb(j); // 置換基の位置を記録
        sub_cnt[i]++;
      }
    }
    if(max_sub_cnt<sub_cnt[i]) max_sub_cnt=sub_cnt[i];  // 最大値の更新
  }
  vi main_chain=dia[0]; // 主鎖
  vi min_sub_pos(max_sub_cnt,1<<30); // 置換基の位置番号の最小値
  int flag=0; // 始点が決定しているかどうか
  rep(i,cnt){
    if(sub_cnt[i]==max_sub_cnt){
      vi sub_pos_rev=sub_pos[i]; // 反転したものも比較
      reverse(all(sub_pos_rev));
      rep(j,max_sub_cnt) sub_pos_rev[j]=len-1-sub_pos_rev[j];
      if(min_sub_pos>sub_pos[i]||min_sub_pos>sub_pos_rev){ // 最小値の更新
        main_chain=dia[i];
        min_sub_pos=sub_pos[i];
        if(sub_pos[i]==sub_pos_rev) flag=0; // 始点未決定
        else{
          flag=1; // 始点決定
          if(sub_pos_rev<sub_pos[i]){ // 反転後が勝ちの場合
            reverse(all(main_chain));
            reverse(all(min_sub_pos));
          }
        }
      }
    }
  }
  if(flag){
    map<pair<string,bool>,vi> sub_info;
    vi dis,pre,is_main(n,0);
    rep(i,len) is_main[main_chain[i]]=1;
    rep(i,len){
      vi nex;
      for(auto j:graph[main_chain[i]]) if(!is_main[j]) nex.pb(j);
      if(nex.size()>=1){
        auto sub=sub_calc(graph,dis,pre,is_main,deg,nex[0]);
        sub_info[sub].pb(i+1);
      }
      if(nex.size()==2){
        auto sub=sub_calc(graph,dis,pre,is_main,deg,nex[1]);
        sub_info[sub].pb(i+1);
      }
    }
    map<string,vector<pair<pair<string,bool>,vi>>> sub_info_sort;
    for(auto i:sub_info) sub_info_sort[remove_pos(i.fi.fi)].pb(i);
    output_name(sub_info_sort,len);
  }
  else{
    vi main_chain_rev=main_chain;
    reverse(all(main_chain_rev));
    map<pair<string,bool>,vi> sub_info,sub_info_rev;
    vi dis,pre,is_main(n,0),is_main_rev(n,0);
    rep(i,len){
      is_main[main_chain[i]]=1;
      is_main_rev[main_chain_rev[i]]=1;
    }
    rep(i,len){
      vi nex,nex_rev;
      for(auto j:graph[main_chain[i]]) if(!is_main[j]) nex.pb(j);
      if(nex.size()>=1){
        auto sub=sub_calc(graph,dis,pre,is_main,deg,nex[0]);
        sub_info[sub].pb(i+1);
      }
      if(nex.size()==2){
        auto sub=sub_calc(graph,dis,pre,is_main,deg,nex[1]);
        sub_info[sub].pb(i+1);
      }
      for(auto j:graph[main_chain_rev[i]]) if(!is_main_rev[j]) nex_rev.pb(j);
      if(nex.size()>=1){
        auto sub=sub_calc(graph,dis,pre,is_main_rev,deg,nex_rev[0]);
        sub_info_rev[sub].pb(i+1);
      }
      if(nex.size()==2){
        auto sub=sub_calc(graph,dis,pre,is_main_rev,deg,nex_rev[1]);
        sub_info_rev[sub].pb(i+1);
      }
    }
    map<string,vector<pair<pair<string,bool>,vi>>> sub_info_sort,sub_info_sort_rev;
    for(auto i:sub_info) sub_info_sort[remove_pos(i.fi.fi)].pb(i);
    for(auto i:sub_info_rev) sub_info_sort_rev[remove_pos(i.fi.fi)].pb(i);

    vector<vector<pair<string,string>>> name_compare(len),name_compare_rev(len);
    for(auto i:sub_info){
      for(auto j:i.se){
        name_compare[j-1].pb(make_pair(remove_pos(i.fi.fi),i.fi.fi));
      }
    }
    for(auto i:sub_info_rev){
      for(auto j:i.se){
        name_compare_rev[j-1].pb(make_pair(remove_pos(i.fi.fi),i.fi.fi));
      }
    }
    rep(i,len){
      sort(all(name_compare[i]));
      sort(all(name_compare_rev[i]));
    }
    rep(i,len){
      rep(j,name_compare[i].size()){
        if(name_compare[i][j].fi<name_compare_rev[i][j].fi){
          flag=1;
          break;
        }
        else if(name_compare[i][j].fi>name_compare_rev[i][j].fi){
          flag=-1;
          break;
        }
        else{
          if(name_compare[i][j].se<name_compare_rev[i][j].se){
            flag=1;
            break;
          }
          else if(name_compare[i][j].se>name_compare_rev[i][j].se){
            flag=-1;
            break;
          }
        }
      }
      if(flag!=0) break;
    }
    if(flag>=0) output_name(sub_info_sort,len);
    else output_name(sub_info_sort_rev,len);
  }
}
