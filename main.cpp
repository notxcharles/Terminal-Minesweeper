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
    Board(int gridRows, int gridCols)
    {
        m_gridRows = gridRows;
        m_gridCols = gridCols;
        std::vector<std::vector<bool>> newGrid(m_gridRows, std::vector<bool>(m_gridCols));
        m_bombGrid = newGrid;
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
    Board ms(10, 4);
    ms.GenerateBombsRandom(25);

    std::cout << "end" << std::endl;

}