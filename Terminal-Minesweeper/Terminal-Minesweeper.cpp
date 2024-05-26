// Terminal-Minesweeper.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include<limits>
#include <vector>
#include <string>
#include <map>
#include <random>
#include <queue>
#include <set>

class Board
{
private:
    //m_bombGrid is a vector of vectors containing locations of the bombs
    std::vector<std::vector<bool>> m_bombGrid;
    
    //m_gameGrid is a vector of vectors containing locations of the bombs and clues
    std::vector<std::vector<char>> m_completeGrid;
    
    //m_blankGrid is a vector of vectors containing nothing but a blank grid. this is the board that the user sees
    std::vector<std::vector<char>> m_playerGrid;
    
    int m_level = -1;
    int m_bombCount = -1;
    int m_gridRows = 0;
    int m_gridCols = 0;

    std::map<int, std::vector<int>> levels;

public:
    enum InformationFound {
        Bomb,
        Clue,
        BlankSpace,
        None
    };

    Board(int gridRows, int gridCols, int bombChance)
    {
        m_gridRows = gridRows;
        m_gridCols = gridCols;
        std::vector<std::vector<bool>> newGrid(m_gridRows, std::vector<bool>(m_gridCols));
        m_bombGrid = newGrid;
        GenerateBombsRandom(bombChance);

        std::vector<std::vector<char>> newCompleteGrid(m_gridRows, std::vector<char>(m_gridCols));
        m_completeGrid = newCompleteGrid;
        GenerateCompleteGrid();
    }
    Board(int levelDifficulty)
    {
        m_level = levelDifficulty;
        //Beginner
        levels[0] = { 8, 8, 10 };
        //Intermediate
        levels[1] = { 16, 16, 40 };
        //Expert
        levels[2] = { 30, 16, 99 };

        m_gridRows = levels[m_level][0];
        m_gridCols = levels[m_level][1];
        std::vector<std::vector<bool>> newGrid(m_gridRows, std::vector<bool>(m_gridCols));
        m_bombGrid = newGrid;

        GenerateBombs(levels[m_level][2]);

        std::vector<std::vector<char>> newCompleteGrid(m_gridRows, std::vector<char>(m_gridCols));
        m_completeGrid = newCompleteGrid;
        GenerateCompleteGrid();

        std::vector<std::vector<char>> newBlankGrid(m_gridRows, std::vector<char>(m_gridCols));
        m_playerGrid = newBlankGrid;
    }


    void GenerateBombs(int numberOfBombs)
    {
        int gridTiles = m_gridRows * m_gridCols;
        int bombChance = (float)gridTiles / numberOfBombs;

        while (m_bombCount < numberOfBombs)
        {
            for (int row = 0; row < m_bombGrid.size(); row++)
            {
                for (int col = 0; col < m_bombGrid[0].size(); col++)
                {

                    if (m_bombGrid[row][col] == 1) continue;

                    bool isBomb = GenerateRandomBomb(bombChance);
                    m_bombGrid[row][col] = isBomb;
                    if (isBomb)
                    {
                        m_bombCount += 1;
                    }
                    if (m_bombCount == numberOfBombs)
                    {
                        return;
                    }

                }
            }
        }
    }
    void GenerateBombsRandom(int bombChance)
    {
        int sidedDice = (float)(100 / bombChance);
        m_bombCount = 0;

        for (int row = 0; row < m_bombGrid.size(); row++)
        {
            for (int col = 0; col < m_bombGrid[0].size(); col++)
            {
                bool isBomb = GenerateRandomBomb(sidedDice);
                m_bombGrid[row][col] = isBomb;

                if (isBomb)
                {
                    m_bombCount += 1;
                }
            }
        }
    }

    void DisplayBombGrid()
    {
        for (int row = 0; row < m_bombGrid.size(); row++)
        {
            for (int col = 0; col < m_bombGrid[0].size(); col++)
            {
                std::cout << "[" << m_bombGrid[row][col] << "] ";
            }
            std::cout << "\n";
        }
    }
    void DisplayCompleteGrid()
    {
        for (int row = 0; row < m_completeGrid.size(); row++)
        {
            for (int col = 0; col < m_completeGrid[0].size(); col++)
            {
                std::string current = "";
                current += m_completeGrid[row][col];
                char currentC = m_completeGrid[row][col];
                std::cout << "[" << m_completeGrid[row][col] << "] ";
            }
            std::cout << "\n";
        }
    }
    void DisplayCompleteGridWithCoordinates()
    {
        std::cout << "    1   2   3   4   5   6   7   8" << std::endl;
        for (int row = 0; row < m_completeGrid.size(); row++)
        {
            std::cout << row + 1 << "  ";
            for (int col = 0; col < m_completeGrid[0].size(); col++)
            {
                std::string current = "";
                current += m_completeGrid[row][col];
                char currentC = m_completeGrid[row][col];
                std::cout << "[" << m_completeGrid[row][col] << "] ";
            }
            std::cout << "\n";
        }
    }

    InformationFound GuessPosition(int row, int col)
    {
        row = row - 1;
        col = col - 1;
        if (IsOutsideOfGrid(row, col))
        {
            return None;
        }
        char result = m_completeGrid[row][col];
        if (result == ' ')
        {
            return BlankSpace;
        }
        else if (result == 'B')
        {
            return Bomb;
        }
        else
        {
            return Clue;
        }
        
    }

    void ExploreArea(int row, int col)
    {
        //
        std::vector<std::vector<char>> grid = m_completeGrid;

        std::set<std::vector<int>> seen;
        std::queue<std::vector<int>> exploreQueue;
        exploreQueue.push(std::vector<int>{row, col});
        seen.insert(std::vector<int>{row, col});

        while (!exploreQueue.empty())
        {
            int n = exploreQueue.size();
            
            for (int i = 0; i < n; i++)
            {
                std::vector<int> currentCoords = exploreQueue.front();
                exploreQueue.pop();

                int currentRow = currentCoords[0];
                int currentCol = currentCoords[1];
                seen.insert(std::vector<int>{currentRow, currentCol});

                if (IsOutsideOfGrid(currentRow - 1, currentCol - 1))
                {
                    std::cout << currentRow << "," << currentCol << " is out of bounds" << std::endl;
                    continue;
                }
                else if (grid[currentRow - 1][currentCol - 1] == '1' ||
                    grid[currentRow - 1][currentCol - 1] == '2' ||
                    grid[currentRow - 1][currentCol - 1] == '3' ||
                    grid[currentRow - 1][currentCol - 1] == '4' ||
                    grid[currentRow - 1][currentCol - 1] == '5' ||
                    grid[currentRow - 1][currentCol - 1] == '6' ||
                    grid[currentRow - 1][currentCol - 1] == '7' ||
                    grid[currentRow - 1][currentCol - 1] == '8' ||
                    grid[currentRow - 1][currentCol - 1] == '9')
                {
                    std::cout << currentRow << "," << currentCol << " is digit" << std::endl;
                    continue;
                }
                else
                {
                    std::cout << currentRow << "," << currentCol << " is blank" << std::endl;

                    std::vector<std::vector<int>> directions = { {1,0}, {-1, 0}, {0, 1}, {0, -1} };
                    for (std::vector<int> direction : directions)
                    {
                        int r = direction[0];
                        int c = direction[1];
                        std::vector<int> coords{ currentRow + r, currentCol + c };
                        if (seen.count(coords) > 0)
                        {
                            continue;
                        }

                        exploreQueue.push(coords);

                    }
                }
                
                
            }
        }
    }
    
private:
    bool GenerateRandomBomb(int diceSide)
    {
        std::random_device rd;
        std::uniform_int_distribution<int> dist(1, diceSide);
        int randomNumber = dist(rd);
        return randomNumber == 1;
    }

    bool IsBombInLocation(int row, int col)
    {
        //Return true if there is a bomb in m_bombGrid[row][col]
        if (IsOutsideOfGrid(row, col))
        {
            return false;
        }
        return m_bombGrid[row][col];
    }

    bool IsOutsideOfGrid(int row, int col)
    {
        //Returns true if outside of grid
        return row < 0 || row >= m_gridRows || col < 0 || col >= m_gridCols;
    }

    void GenerateCompleteGrid()
    {
        //Generate the complete grid - this includes bombs and clues to their location
        for (int row = 0; row < m_gridRows; row++)
        {
            for (int col = 0; col < m_gridCols; col++)
            {
                //std::cout << row << " " << col << std::endl;
                if (IsBombInLocation(row, col))
                {
                    m_completeGrid[row][col] = 'B';
                    continue;
                }
                int bombsInRadius = 48;
                std::vector<std::vector<int>> directions = { {1, 0}, {1, 1}, {0, 1}, {0, -1}, {-1, 0}, {-1, -1}, {1, -1}, {-1, 1} };
                for (auto direction : directions)
                {
                    //std::cout << "row: " << row + direction[0] << " col:" << col + direction[1] << std::endl;
                    if (IsBombInLocation(row + direction[0], col + direction[1]))
                    {
                        bombsInRadius += 1;
                    }
                }
                if (bombsInRadius == 48)
                {
                    m_completeGrid[row][col] = ' ';
                }
                else
                {
                    m_completeGrid[row][col] = bombsInRadius;
                }
                
            }
        }
    }
};

void ClearTerminal()
{
    std::cout << "\033[2J\033[1;1H";
    return;
}

int main()
{
    enum GameState {
        mainMenu,
        playing,
        win,
        lose
    };
    GameState currentState = playing;
    
    Board minesweeper(0);
    while (currentState == playing)
    {
        minesweeper.DisplayCompleteGridWithCoordinates();
        std::cout << "\n\n";
        int option = 0;
        std::cout << "(1) Flag Bomb\n(2) Guess Position\n";
        std::cin >> option;
        if (option == 1)
        {
            ClearTerminal();
            minesweeper.DisplayCompleteGridWithCoordinates();
            std::cout << "\n\n";
            std::cout << "Flag Bomb" << std::endl;
            int row = 0;
            int col = 0;
            std::cout << "Row: ";
            std::cin >> row;
            std::cout << "Column: ";
            std::cin >> col;
            
        }
        else if (option == 2)
        {
            std::cout << "2";
            ClearTerminal();
            minesweeper.DisplayCompleteGridWithCoordinates();
            std::cout << "\n\n";
            std::cout << "Guess Position" << std::endl;
            int row = 0;
            int col = 0;
            std::cout << "Row: ";
            std::cin >> row;
            std::cout << "Column: ";
            std::cin >> col;

           
            Board::InformationFound result = minesweeper.GuessPosition(row, col);
            if (result == Board::InformationFound::Bomb)
            {
                std::cout << "Game over" << std::endl;
            }
            else if (result == Board::InformationFound::Clue)
            {
                std::cout << "Clue" << std::endl;
            }
            else if (result == Board::InformationFound::BlankSpace)
            {
                std::cout << "Blank space" << std::endl;
                minesweeper.ExploreArea(row, col);
            }
            else
            {
                std::cout << "Error" << std::endl;
            }
        }
        Sleep(1000);
        std::cout << std::endl;
        ClearTerminal();
    }
    
    //minesweeper.DisplayCompleteGrid();
    
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
