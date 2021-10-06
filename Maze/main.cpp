#include<iostream>
#include<cstring>
#include<cstdio>
#include<cstdlib>
#include<ctime>

using namespace std;

typedef pair<int, int> PII;

const int N = 110;

int n, m;
int stx, sty;
int edx, edy;
int g[N][N], G[N][N], d[N][N];
PII q[N * N], Prev[N][N];
void bfs()
{
    if (g[stx][sty] == 1)return;
    int hh = 0, tt = -1;
    q[++tt] = { stx,sty };
    memset(d, -1, sizeof d);
    d[stx][sty] = 0;
    int dx[8] = { -1,0,1,0,-1,-1,1,1 }, dy[8] = { 0,-1,0,1,-1,1,-1,1 };
    while (hh <= tt)
    {
        auto t = q[hh++];
        for (int i = 0; i < 8; i++)
        {
            int x = t.first + dx[i], y = t.second + dy[i];
            if (x >= 0 && x < n && y >= 0 && y < m && g[x][y] == 0 && d[x][y] == -1)
            {
                q[++tt] = { x,y };
                d[x][y] = d[t.first][t.second] + 1;
                Prev[x][y] = t;
            }
        }
    }
    int x = edx, y = edy;
    if (Prev[x][y].first == 0 && Prev[x][y].second == 0)return;
    while (x != stx || y != sty)
    {
        g[x][y] = 2;
        PII t = Prev[x][y];
        x = t.first, y = t.second;
    }
    g[x][y] = 2;
}
void printMazeg()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (g[i][j] == 0)cout << "��";
            else if (g[i][j] == 1)cout << "��";
            else if (g[i][j] == 2)cout << "��";
        }
        cout << endl;
    }
}
void printMazeG()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (G[i][j] == 0)cout << "��";
            else if (G[i][j] == 1)cout << "��";
            else if (G[i][j] == 2)cout << "��";
        }
        cout << endl;
    }
}
int main()
{
    //�����ʾ��Ϣ���û�ѡ���Թ���ͼ��Դ
    cout << "Welcome to the Nightmare of Maze!" << endl;
    cout << "The sources of maze we can get are as follows:" << endl;
    cout << "1.from file." << endl;
    cout << "2.auto create." << endl;
    cout << "Please input the corresponding numbers to choose the source of maze:";

    //�����û�ѡ��
    int op;
    cin >> op;

    switch (op)
    {
    case 1:
    {
        freopen("input.txt", "r", stdin);
        freopen("output.txt", "w", stdout);
        //ʹ���ļ���д
        //�����Թ���������������������n��������m
        cin >> n >> m;

        //�����Թ����
        cin >> stx >> sty;

        //�����Թ��յ�
        cin >> edx >> edy;

        //�����Թ���ͼ��0����ͨ·��1�����ϰ�
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)cin >> g[i][j];
        }

        //����һ��ԭ�Թ�
        memcpy(G, g, sizeof g);

        //���һ������ĵ�����
        cout << "The rows:" << n << endl;
        cout << "The columns:" << m << endl;
        cout << "The start:" << stx << "," << sty << endl;
        cout << "The end:" << edx << "," << edy << endl;
        cout << "The maze:" << endl;
        printMazeg();

        //���п����������������������
        bfs();
        cout << "The solution:" << endl;
        if (g[edx][edy] != 2)cout << "impossible!" << endl;
        else printMazeg();

        //fclose(stdout);
        //fclose(stdin);
        freopen("CON", "r", stdin);
        freopen("CON", "w", stdout);


        //��׼���һ����������ݼ��������

        cout << "The rows:" << n << endl;
        cout << "The columns:" << m << endl;
        cout << "The start:" << stx << "," << sty << endl;
        cout << "The end:" << edx << "," << edy << endl;
        cout << "The maze:" << endl;
        printMazeG();

        cout << "The solution:" << endl;
        if (g[edx][edy] != 2)cout << "impossible!" << endl;
        else printMazeg();
        system("pause");
    }
    case 2:
    {
        //��ʱ��Ϊ���Ӳ��������
        srand((unsigned)time(NULL));

        //���������������������Χ5-100
        n = (rand() % 96) + 5;
        m = (rand() % 96) + 5;

        //���������㣬��Χ0 - n-1��0 - m-1
        stx = (rand() % n);
        sty = (rand() % m);

        //��������յ㣬��Χ0 - n-1��0 - m-1����ͬʱҪ��֤�����յ㲻��ͬ
        int x = (rand() % n);
        while (x == stx)
        {
            x = (rand() % n);
        }
        edx = x;

        int y = (rand() % m);
        while (y == sty)
        {
            y = (rand() % m);
        }
        edy = y;

        //�������01��ͼ
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)g[i][j] = (rand() % 2);
        }

        //���һ��������ɵ�����
        cout << "The rows:" << n << endl;
        cout << "The columns:" << m << endl;
        cout << "The start:" << stx << "," << sty << endl;
        cout << "The end:" << edx << "," << edy << endl;
        cout << "The maze:" << endl;
        printMazeg();

        //����һ��ԭ�Թ�
        memcpy(G, g, sizeof g);
        printMazeG();

        //���п����������������������
        bfs();
        cout << "The solution:" << endl;
        if (g[edx][edy] != 2)cout << "impossible!" << endl;
        else printMazeg();

        //��������ɵ����ݼ��������������ļ���
        freopen("output.txt", "w", stdout);

        cout << "The rows:" << n << endl;
        cout << "The columns:" << m << endl;
        cout << "The start:" << stx << "," << sty << endl;
        cout << "The end:" << edx << "," << edy << endl;
        cout << "The maze:" << endl;
        printMazeG();
        cout << "The solution:" << endl;
        if (g[edx][edy] != 2)cout << "impossible!" << endl;
        else printMazeg();

        fclose(stdout);
    }
    }
    system("pause");
    return 0;
}
