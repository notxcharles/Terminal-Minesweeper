#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <random>

class Board
{
private:
    std::vector<std::vector<bool>> m_bombGrid;
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
    }
    Board(int levelDifficulty)
    {
        m_level = levelDifficulty;
        //Beginner
        levels[0] = {8, 8, 10};
        //Intermediate
        levels[1] = {16, 16, 40};
        //Expert
        levels[2] = {30, 16, 99};

        m_gridRows = levels[m_level][0];
        m_gridCols = levels[m_level][1];
        std::vector<std::vector<bool>> newGrid(m_gridRows, std::vector<bool>(m_gridCols));
        m_bombGrid = newGrid;

        GenerateBombs(levels[m_level][2]);
    }


    void GenerateBombs(int numberOfBombs)
    {
        int gridTiles = m_gridRows * m_gridCols;
        int bombChance = (float)gridTiles/numberOfBombs;

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
        int sidedDice = (float)(100/bombChance);
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
private:
    bool GenerateRandomBomb(int diceSide)
    {
        std::random_device rd;
        std::uniform_int_distribution<int> dist(1, diceSide);
        int randomNumber = dist(rd);
        return randomNumber == 1;
    }
};

int main()
{
    Board ms(0);

    std::cout << "end" << std::endl;

}