/*****************************************************************************************************
 *  Name:               Reza Rostamy
 *  Description:        Generate Randomly Maze Using Eller's Algorithm,This Algorithm no Need Mor Memory-
 *                      And Take Short Time to Create Maze. 
 *  Date:               09/02/2020
 *  Data structures:    2 axis cells class as map table,a cell class for single cell     
 * ***************************************************************************************************/

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

//define constants
#define row 25 //number of cells in a table is row*colon
#define colon 25
#define delay 7
#define bombNumber (row * colon * 0.025)
#define playerChar 'P'
#define endChar 'E'
#define bombChar 'B'
#define wallChar (char)254

using std::cin;
using std::cout;
using std::endl;

class Cell
{
public:
    int setNumber = 0; //every cell belong to a set
    bool type = true;  //resserv 1 for ordinary type cell and 0 for bomb cell
    bool upWall = true;
    bool downWall = true;
    bool rightWall = true;
    bool leftWall = true;
};

//build a tow axis array of cells as map
class Map
{
    class Set
    {
    public:
        int startPos, endPos;
        int value;  //any set has set specifise with unique number
        int member; //every set have many member
    };

    //create a class for every row sets
    class RowSet
    {
    public:
        Set sets[colon];
        int rowSetsNumber; //number of sets in a row

        RowSet()
        {
            rowSetsNumber = 0; //initialize row sets numbers to 0
        }
    } rowSets[row];

    //fill every cell with unique set number
    void assingCells(int thisRow)
    {
        static int setCounter = 0;
        for (int k = 0; k < colon; k++)
        {
            if (!cells[thisRow][k].setNumber)
                cells[thisRow][k].setNumber = ++setCounter;
        }
    }

    //randomly decide for side walls beetwin cells
    void setSideWalls(int thisRow)
    {
        for (int i = 0; i < colon - 1; i++)
        {
            if (thisRow == row - 1)
            {
                if (cells[thisRow][i].setNumber != cells[thisRow][i + 1].setNumber)
                {
                    cells[thisRow][i].rightWall = false;
                    cells[thisRow][i + 1].leftWall = false;
                }
            }
            else
            {

                if (cells[thisRow][i].setNumber != cells[thisRow][i + 1].setNumber)
                {
                    if (rand() % 2)
                    {
                        cells[thisRow][i + 1].setNumber = cells[thisRow][i].setNumber;
                        cells[thisRow][i].rightWall = false;
                        cells[thisRow][i + 1].leftWall = false;
                    }
                }
            }
        }
    }

    //find number of sets in a row with their number of member
    void findSets(int thisRow)
    {
        int counter = 0;
        rowSets[thisRow].sets[0].value = cells[thisRow][0].setNumber;
        rowSets[thisRow].sets[0].member = 1;
        rowSets[thisRow].sets[0].startPos = 0;

        for (int i = 1; i < colon; i++)
        {
            if (rowSets[thisRow].sets[counter].value == cells[thisRow][i].setNumber)
            {
                rowSets[thisRow].sets[counter].member++;
            }
            else
            {
                counter++;
                rowSets[thisRow].sets[counter].value = cells[thisRow][i].setNumber;
                rowSets[thisRow].sets[counter].member = 1;

                rowSets[thisRow].sets[counter].startPos = i;
                rowSets[thisRow].sets[counter - 1].endPos = i - 1;
            }
        }
        rowSets[thisRow].sets[counter].endPos = rowSets[thisRow].sets[counter].startPos + rowSets[thisRow].sets[counter].member - 1;
        rowSets[thisRow].rowSetsNumber = ++counter;
    }

    //at a set at least should be a passage to down row
    void setDownWalls(int thisRow)
    {
        int counter = rowSets[thisRow].rowSetsNumber;
        int randLimit;
        int randCell;

        for (int i = 0; i < counter; i++)
        {
            int check = 0;

            while (check < 1)
            {
                for (int j = 0; j < rowSets[thisRow].sets[i].member; j++)
                {
                    if (rand() % 2)
                    {
                        randCell = rowSets[thisRow].sets[i].startPos + j;
                        cells[thisRow][randCell].downWall = false;
                        cells[thisRow + 1][randCell].upWall = false;
                        cells[thisRow + 1][randCell].setNumber = cells[thisRow][randCell].setNumber;
                        check++;
                    }
                }
            }
        }
    }

public:
    Cell cells[row][colon];

    Map()
    {
        srand(time(0)); //set seed for rand fuction

        SetConsoleCP(437);
        SetConsoleOutputCP(437); //for winodos consol to show ascii character
    }

    void generateMap()
    {

        for (int i = 0; i < row; i++)
        {
            if (i == row - 1)
            {
                assingCells(i); //last row have deferent method
                findSets(i);
                setSideWalls(i);
            }
            else
            {
                assingCells(i);
                setSideWalls(i);
                findSets(i);
                setDownWalls(i);
            }
        }
    }

    void showConsoleCursor(bool showFlag)
    {
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(out, &cursorInfo);
        cursorInfo.bVisible = showFlag;
        SetConsoleCursorInfo(out, &cursorInfo);
    }

    //whit print up-walls and side-walls for each row the map is completely shown.
    void printMap()
    {
        for (int i = 0; i < row; i++)
        {
            showConsoleCursor(false);

            Sleep(delay);
            cout << wallChar;
            Sleep(delay);
            cout << " ";

            for (int j = 0; j < colon; j++) //print row up-walls
            {

                if (cells[i][j].upWall)
                {
                    Sleep(delay);
                    cout << wallChar;
                    Sleep(delay);
                    cout << " ";
                    Sleep(delay);
                    cout << wallChar;
                    Sleep(delay);
                    cout << " ";
                }
                else
                {
                    Sleep(delay);
                    cout << " ";
                    Sleep(delay);
                    cout << " ";
                    Sleep(delay);
                    cout << wallChar;
                    Sleep(delay);
                    cout << " ";
                }
            }

            cout << endl;
            Sleep(delay);
            cout << wallChar;
            Sleep(delay);
            cout << " ";

            for (int j = 0; j < colon; j++) //print row side-walls
            {
                if (cells[i][j].rightWall)
                {
                    Sleep(delay);
                    cout << " ";
                    Sleep(delay);
                    cout << " ";
                    Sleep(delay);
                    cout << wallChar;
                    Sleep(delay);
                    cout << " ";
                }
                else
                {
                    Sleep(delay);
                    cout << " ";
                    Sleep(delay);
                    cout << " ";
                    Sleep(delay);
                    cout << " ";
                    Sleep(delay);
                    cout << " ";
                }
            }
            cout << endl;
        }
        Sleep(delay);
        cout << wallChar;
        Sleep(delay);
        cout << " ";
        for (int j = 0; j < colon; j++) //print last down-walls
        {
            Sleep(delay);
            cout << wallChar;
            Sleep(delay);
            cout << " ";
            Sleep(delay);
            cout << wallChar;
            Sleep(delay);
            cout << " ";
        }
    }
};

class Game
{
    short playerXPos, playerYPos;
    short endCellX, endCellY;
    int playerRow, playerColon, endRow, endColon;
    Map *gameMap;

    //move to a corespandig element in array cordinate on screen and show a character
    void show(short XPos, short YPos, char ch)
    {
        short screenX = (XPos - 1) * 4 + 2;
        short screenY = (YPos - 1) * 2 + 1;
        gotoxy(screenX, screenY, ch);
    }

    //move to a screen position and show a character
    void gotoxy(short x, short y, char ch)
    {
        static HANDLE h = NULL;
        if (!h)
        {
            h = GetStdHandle(STD_OUTPUT_HANDLE);
        }
        COORD c = {x, y};
        SetConsoleCursorPosition(h, c);
        cout << ch;
    }

    //add bomb to the cells
    void insertBomb(int bombNum)
    {
        int bombRow, bombColon;
        for (int i = 0; i < bombNum; i++)
        {
            while (1)
            {
                bombRow = rand() % row;
                bombColon = rand() % colon;
                if ((*gameMap).cells[bombRow][bombColon].type)
                {
                    (*gameMap).cells[bombRow][bombColon].type = false;
                    show(bombColon + 1, bombRow + 1, bombChar);
                    break;
                }
            }
        }
    }

public:
    Game(Map *map)
    {
        gameMap = map;

        playerXPos = (rand() % row) + 1; //specefi the player start postition
        playerYPos = (rand() % colon) + 1;

        endCellX = (rand() % row) + 1; //specefi end cell position
        endCellY = (rand() % colon) + 1;

        playerRow = playerYPos - 1;   //corespond player screen position and cells array position-
        playerColon = playerXPos - 1; //cells array start from 0 but screen position start from 1
        endRow = endCellY - 1;
        endColon = endCellX - 1;

        insertBomb(bombNumber);

        show(playerXPos, playerYPos, playerChar);
        show(endCellX, endCellY, endChar);
    }

    void play()
    {
        char input = '0';

        while (!(playerRow == endRow && playerColon == endColon))
        {
            input = getch();
            fflush(stdin);
            switch (input)
            {
            case 'w':
                if (!((*gameMap).cells[playerRow][playerColon].upWall))
                {
                    show(playerXPos, playerYPos, ' ');
                    show(playerXPos, --playerYPos, playerChar);
                    playerRow--;
                    if (!(*gameMap).cells[playerRow][playerColon].type)
                    {
                        system("cls");
                        cout << "Game Over!!!!!!";
                        Sleep(500);
                        exit(0);
                    }
                }

                break;

            case 's':
                if (!((*gameMap).cells[playerRow][playerColon].downWall))
                {
                    show(playerXPos, playerYPos, ' ');
                    show(playerXPos, ++playerYPos, playerChar);
                    playerRow++;
                    if (!(*gameMap).cells[playerRow][playerColon].type)
                    {
                        system("cls");
                        cout << "Game Over!!!!!!";
                        Sleep(500);
                        exit(0);
                    }
                }

                break;
            case 'd':
                if (!((*gameMap).cells[playerRow][playerColon].rightWall))
                {
                    show(playerXPos, playerYPos, ' ');
                    show(++playerXPos, playerYPos, playerChar);
                    ++playerColon;
                    if (!(*gameMap).cells[playerRow][playerColon].type)
                    {
                        system("cls");
                        cout << "Game Over!!!!!!";
                        Sleep(500);
                        exit(0);
                    }
                }

                break;
            case 'a':
                if (!((*gameMap).cells[playerRow][playerColon].leftWall))
                {
                    show(playerXPos, playerYPos, ' ');
                    show(--playerXPos, playerYPos, playerChar);
                    --playerColon;
                    if (!(*gameMap).cells[playerRow][playerColon].type)
                    {
                        system("cls");
                        cout << "Game Over!!!!!!";
                        Sleep(500);
                        exit(0);
                    }
                }

                break;
            }
        }
        system("cls");
        cout << "<<<<<<<< YOU WIN >>>>>>>>>";
        Sleep(500);
        exit(0);
    }
};

int main()
{
    Map map;
    map.generateMap();
    map.printMap();

    Game game(&map);
    game.play();

    return 0;
}
