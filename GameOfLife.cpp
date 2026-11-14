// Author: Mario Talevski
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sstream>

//Move OS defines up here to be used in different places
#if defined(_WIN32) || defined(WIN32) || defined(__MINGW32__) || defined(__BORLANDC__)
 #define OS_WIN
 //WINDOWS COLORS 
  #define COLOR_RED SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED)
  #define COLOR_WARNING SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN) 

  #define COLOR_BLUE SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE)

  #define COLOR_RESET SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15)

#elif defined(linux) || defined(__CYGWIN__)
  #define OS_LINUX

  #define COLOR_RED "\033[31m"
  #define COLOR_GREEN "\033[32m" 
  #define COLOR_BLUE "\033[34m"
  #define COLOR_RESET "\033[0m"

#elif (defined(__APPLE__) || defined(__OSX__) || defined(__MACOS__)) && defined(__MACH__)//To ensure that we are running on a mondern version of macOS
  #define OS_MAC

  #define COLOR_RED "\033[31m"
  #define COLOR_GREEN "\033[32m" 
  #define COLOR_BLUE "\033[34m"
  #define COLOR_RESET "\033[0m"

#endif

#if defined(OS_WIN)
  #include <windows.h> // Use for windows
#endif

using namespace std;

const int gridSize = 25;
void printGrid(bool gridOne[gridSize+1][gridSize+1]);
void determineState(bool gridOne[gridSize+1][gridSize+1]);
void clearScreen(void);


int main()
  {
    // system( "color A" );//LGT green
    cout << COLOR_GREEN;
    clearScreen();
    bool gridOne[gridSize+1][gridSize+1] = {};
    int x,y,n;
    string nc;
    string start;
    string filename;
    cout << "                         生命游戏（在C++中实现）" << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << "也被称为life, " << endl;
    cout << "是由英国数学家 John Horton Conway 在1970年设计的元胞自动机." << endl;
    cout << endl;
    cout << "规则如下" << endl;
    cout << "生命游戏的宇宙是一个无限的二维正交方形细胞网格" << endl;
    cout << "每个细胞只有生与死两种状态" << endl;
    cout << "每个细胞的状态由周围八个细胞所决定" << endl;
    cout << "随着时间流逝，细胞会发生如下转变：" << endl;
    cout << "1. 当一个细胞周围少于两个活细胞时，该细胞死亡，这是由人口不足导致" << endl;
    cout << "2. 当一个细胞周围有两到三个活细胞时，该细胞存活到下一次判定" << endl;
    cout << "3. 当一个细胞周围有三个以上活细胞时，该细胞死亡，这是由人口过多导致" << endl;
    cout << "4. 当一个细胞周围恰有三个活细胞时，该细胞由死亡转向复活，这是由繁殖导致" << endl;
    cout << endl;
    cout << "O - 活细胞" << endl;
    cout << ". - 死细胞" << endl;
    cout << endl;
    cout << "输入细胞数量或者输入“r”以从文件中读取细胞：";
    cin >> nc;
    cout << endl;

    if ( nc == "r" )
    {
	    while (true)
      {
	      
	      cout << "输入文件名："<<endl;
	      cin  >> filename;
	      
	      ifstream readfile(filename);
	      if ( readfile.is_open() )
        {
		      string fileline,xx,yy;
          
	      	while (getline(readfile,fileline)){
		        stringstream ss(fileline);
		        
		        getline(ss,xx,' ');
		        getline(ss,yy,' ');
            
  		      x = stoi(xx);
    		    y = stoi(yy);

	  	      gridOne[x][y] = true;
		      }
		      break;
	      } 
        else 
        {
	      cout << "没有这个文件夹，请再次输入" << endl;
	      }
      }
    }
    else
    {
	    
	    for(int i=0;i<stoi(nc);i++)
	    {
	      cout <<stoi(nc)<< "输入第 " << i+1 << "个细胞的坐标并用空格隔开（例：1 1） : ";
	      cin >> x >> y;
	      gridOne[x][y] = true;
	      printGrid(gridOne);
	    }
    }
    cout << "细胞已经就绪，是否开始游戏？（y/n）：" << endl;
    printGrid(gridOne);
    cin >> start;
    if( start == "y" || start == "Y" )
    {
      while (true)
      {
        printGrid(gridOne);
        determineState(gridOne);
        usleep(200000);
        clearScreen();
	    }
    }
    else
    {
      cout << COLOR_RESET;
      clearScreen();
      return 0;
    }   
  }

void clearScreen(void) {
  // Tested and working on Ubuntu and Cygwin
  #if defined(OS_WIN)
    HANDLE hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = { 0, 0 };

    if (hStdOut == INVALID_HANDLE_VALUE) return;

    /* Get the number of cells in the current buffer */
    GetConsoleScreenBufferInfo( hStdOut, &csbi );
    cellCount = csbi.dwSize.X *csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    FillConsoleOutputCharacter(hStdOut, (TCHAR) ' ', cellCount, homeCoords, &count);

    /* Fill the entire buffer with the current colors and attributes */
    FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count);

    SetConsoleCursorPosition( hStdOut, homeCoords );

  #elif defined(OS_LINUX) || defined(OS_MAC)
    cout << "\033[2J;" << "\033[1;1H"; // Clears screen and moves cursor to home pos on POSIX systems
  #endif

}


void printGrid(bool gridOne[gridSize+1][gridSize+1]){
    for(int a = 1; a < gridSize; a++)
        {
        for(int b = 1; b < gridSize; b++)
        {
            if(gridOne[a][b] == true)
            {
                cout << " O ";
            }
            else
            {
                cout << " . ";
            }
            if(b == gridSize-1)
            {
                cout << endl;
            }
        }
    }
}

void compareGrid (bool gridOne[gridSize+1][gridSize+1], bool gridTwo[gridSize+1][gridSize+1]){
    for(int a =0; a < gridSize; a++)
    {
        for(int b = 0; b < gridSize; b++)
        {
                gridTwo[a][b] = gridOne[a][b];
        }
    }
}

void determineState(bool gridOne[gridSize+1][gridSize+1]){
    bool gridTwo[gridSize+1][gridSize+1] = {};
    compareGrid(gridOne, gridTwo);

    for(int a = 1; a < gridSize; a++)
    {
        for(int b = 1; b < gridSize; b++)
        {
            int alive = 0;
            for(int c = -1; c < 2; c++)
            {
                for(int d = -1; d < 2; d++)
                {
                    if(!(c == 0 && d == 0))
                    {
                        if(gridTwo[a+c][b+d])
				{
					++alive;
				}
                    }
                }
            }
            if(alive < 2)
            {
                gridOne[a][b] = false;
            }
            else if(alive == 3)
            {
                gridOne[a][b] = true;
            }
            else if(alive > 3)
            {
                gridOne[a][b] = false;
            }
        }
    }
}
