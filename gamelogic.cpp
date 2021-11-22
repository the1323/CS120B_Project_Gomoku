
#include <iostream>

using namespace std;

int checkwin(int board[10][10],int cx, int cy ,int xoffset,int yoffset){
    int turn = board[cx][cy];
    int counta=0,countb=0;

    for (int i = 1; i < 6; i++){// check right,down
        if(board[cx+(xoffset*i)][cy+(yoffset*i)]==turn) counta++;
        else break;
    }
    for (int i = 1; i < 6; i++){// check left,up
        if(board[cx+(xoffset*i*(-1))][cy+(yoffset*i*(-1))]==turn) countb++;
        else break;
    }
    return 1+counta+countb;

}
int main()
{
    int n=10;

    int board[10][10];
    for(int i =0;i<n;i++){
        for(int j =0;j<n;j++){
            board[i][j]=0; //init board
        }
    }
    board[3][4]=1;
    //test cases
    //    board[2][4]=1;
    //    board[1][4]=1; x
    //    board[4][4]=1;
    //    board[5][4]=1;
//    board[3][3]=1;
//    board[3][2]=1;y
//    board[3][1]=1;
//    board[3][5]=1;
//    board[4][5]=1;
//    board[5][6]=1; /
//    board[2][3]=1;
//    board[6][7]=1;

//    board[3][4]=1;
//    board[2][5]=1;
//    board[1][6]=1;\
//    board[0][7]=1;
    //0=none, 1= black, 2 =white

    int cx=3,cy=4;
    int xwin = checkwin(board,cx,cy,1,0);//x-axis
    int ywin = checkwin(board,cx,cy,0,1);//y-axis
    int lrwin= checkwin(board,cx,cy,-1,1);// '\'-axis
    int rlwin= checkwin(board,cx,cy,1,1);// '/'-axis

    int final = max(max(xwin,ywin),max(lrwin,rlwin)); //max of the 4

    cout<<"x: "<<xwin<<endl;
    cout<<"y: "<<ywin<<endl;
    cout<<"\: "<<lrwin<<endl;
    cout<<"/: "<<rlwin<<endl;
    cout<<"f: "<<final<<endl;
    int row= sizeof(board)/sizeof(board[0]);
    int col= sizeof(board[0])/sizeof(board[0][0]);
    cout<<"r size "<<row<<endl;
    cout<<"c size "<<col<<endl;


    cout << "Hello World!" << endl;
    return 0;
}
