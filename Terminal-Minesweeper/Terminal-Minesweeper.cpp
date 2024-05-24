// Terminal-Minesweeper.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <random>

class Board
{
private:
    std::vector<std::vector<bool>> m_bombGrid;
    //m_bombGrid is a vector of vectors containing locations of the bombs
    std::vector<std::vector<char>> m_completeGrid;
    //m_gameGrid is a vector of vectors containing locations of the bombs and clues
    int m_level = -1;
    int m_bombCount = -1;
    int m_gridRows = 0;
    int m_gridCols = 0;

    std::map<int, std::vector<int>> levels;

public:
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
                    m_completeGrid[row][col] = 'x';
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
                m_completeGrid[row][col] = bombsInRadius;
            }
        }
    }
};

int main()
{
    Board minesweeper(0);
    //minesweeper.DisplayCompleteGrid();
    minesweeper.DisplayCompleteGridWithCoordinates();
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
