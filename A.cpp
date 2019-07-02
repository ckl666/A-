#include <iostream>
#include <queue>
#include <vector>
#include <math.h>

using namespace std;

#define TRAVERSE 2         //障碍物
#define TRANSITABLE 1      //可通行的
#define FOOT 3             //已经过的
#define PATH 0             //路径



//===================================================================================================================
struct Node
{
    Node(int x = 0,int y = 0):_x(x),_y(y),_prev(NULL){}
    int _x;
    int _y;
    int _data;
    int _mgn;    //dijkstra 计算出的G(N)值
    int _mfn;    //F(n) = G(n) + H(n)
    Node *_prev;   //前驱节点
};

//=================================================================================================================
class compare
{
public:
    bool operator()(const Node& left,const Node &right)
    {
        return left._mfn > right._mfn;
    }
};

//=====================================================================================================================
class CMap
{
public:
    vector<vector<Node> > &getMap() { return _map;}
    void setMap();
private:
    vector<vector<Node> > _map;
};

void CMap::setMap()
{
    cout << "输入地图的行数和列数:";
    int row,col;
    cin >> row >> col;
    for(int i = 0; i < row; ++i)
    {
        vector<Node> tmp;
        for(int j = 0; j < col; ++j)
        {
            Node node;
            int data;
            cin >> data;
            node._x = i;
            node._y = j;
            node._data = data;
            tmp.push_back(node);
        }
        _map.push_back(tmp);
        tmp.clear();
    }
};

//===================================================================================================================

class A
{
public:
    A()
    {
        H[0] = 1; V[0] = 0;
        H[1] = 0; V[1] = 1;
        H[2] = -1; V[2] = 0;
        H[3] = 0; V[3] = -1;
    }
    //获取H(n)的值
    int getHn(int x,int y,int endx,int endy);

    //查找路径
    void findPath(int startx,int starty,int endx,int endy);

    //打印路径
    void display(int x,int y);

    //设置地图
    void setMap() { _map.setMap();}
    
private:
    CMap _map;          
    priority_queue<Node,vector<Node>,compare> _prioque;      //优先级队列
    int H[4];
    int V[4];                   //H 与 V用来存储四个方向
};


void A::display(int x,int y)
{

    Node *node = &_map.getMap()[x][y];
    while(node != NULL)
    {
        node->_data = PATH;
        node = node->_prev;
    }
    int row = _map.getMap().size();
    for(int i = 0; i < row; i++)
    {
        int col = _map.getMap()[i].size();
        for(int j = 0; j < col; j++)
        {
            cout << _map.getMap()[i][j]._data << " ";
        }
        cout << endl;
    }
}
//
int A::getHn(int x,int y,int endx,int endy)
{
    return fabs(x-endx) + fabs(y-endy);
}

void A::findPath(int startx,int starty,int endx,int endy)
{
    if(startx == endx && starty == endy)
    {
        return ;
    }
    _map.getMap()[startx][starty]._mgn = 0; 
    _map.getMap()[startx][starty]._data = FOOT; 
    _map.getMap()[startx][starty]._mfn = getHn(startx,starty,endx,endy);
    _prioque.push(_map.getMap()[startx][starty]);

    while(!_prioque.empty())
    {
        int x = _prioque.top()._x;
        int y = _prioque.top()._y;
        //如果找到最短路径
        if(x == endx && y == endy)
        {
            //打印路径
            display(x,y);
            return ;
        }
        _prioque.pop();
        for(int i = 0; i < 4; i++)
        {
            if(x + H[i] >= 0 && x + H[i] < _map.getMap().size()
                    && y + V[i] >= 0 && y + V[i] < _map.getMap()[x].size())
            {
                if(_map.getMap()[x+H[i]][y+V[i]]._data == TRANSITABLE)
                {
                    //将下一个节点标记为FOOT
                    _map.getMap()[x+H[i]][y+V[i]]._data = FOOT;
                    //记录下一个节点的前驱节点
                    _map.getMap()[x+H[i]][y+V[i]]._prev = &_map.getMap()[x][y];
                    //计算下一个节点的G(N)
                    _map.getMap()[x+H[i]][y+V[i]]._mgn = _map.getMap()[x][y]._mgn + 1;
                    //计算下一个节点的F(N)
                    _map.getMap()[x+H[i]][y+V[i]]._mfn = _map.getMap()[x+H[i]][y+V[i]]._mgn + getHn(x+H[i],y+V[i],endx,endy);
                    //将下一个节点放入优先级队列
                    _prioque.push(_map.getMap()[x+H[i]][y+V[i]]);
                }
            }
        }
    }
    cout << " 无路径 " << endl;
}

int main()
{
    A a;
    a.setMap();
    int startx,starty,endx,endy;
    cout << "输入起始位置与终止位置:";
    cin >> startx >> starty >> endx >> endy;
    a.findPath(startx,starty,endx,endy);
    return 0;
}
