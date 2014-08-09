#include "Field.h"
#include "GameScene.h"
#include "cocos2d.h"

using namespace std;
USING_NS_CC;

void Field::initField()
{
    isExistDeletePanel = false;
    for (int x = 0; x < FIELD_WIDTH; x++)
    {
        for (int y = 0; y < FIELD_HEIGHT * 2; y++)
        {
            field[x][y] = (arc4random() % PANEL_TYPE_NUM) + 1;
        }
    }

    checkAdjoinSameNumField();
    createDeleteField();

    while(isExistDeletePanel)
    {
        deleteMainField();
        createDropField();
        dropMainField();
        checkAdjoinSameNumField();
        createDeleteField();
    }
/*
field[0][0] = 1;
field[1][0] = 3;
field[2][0] = 1;
field[3][0] = 2;
field[4][0] = 2;
field[0][1] = 1;
field[1][1] = 2;
field[2][1] = 1;
field[3][1] = 1;
field[4][1] = 2;
field[0][2] = 2;
field[1][2] = 2;
field[2][2] = 2;
field[3][2] = 2;
field[4][2] = 2;
field[0][3] = 1;
field[1][3] = 1;
field[2][3] = 2;
field[3][3] = 1;
field[4][3] = 2;
field[0][4] = 1;
field[1][4] = 1;
field[2][4] = 2;
field[3][4] = 2;
field[4][4] = 2;
*/
}


void Field::checkAdjoinSameNumField()
{
    adjoinSameNumCount = 0;


    for (int y = 0; y < FIELD_HEIGHT; y++)
    {
        for (int x = 0; x < FIELD_WIDTH; x++)
        {
            adjoinSameNumCount = 0;
            Field::initCheckedAdjoinSameNumField();
            Field::checkAdjoinSameNum(x, y);
            adjoinSameNumField[x][y] = adjoinSameNumCount;
            CCLog ("debug:adjoinSameNumField[%d][%d] = %d", x,y,adjoinSameNumCount);
        }
    }
}

void Field::initCheckedAdjoinSameNumField()
{
    for (int y = 0; y < FIELD_HEIGHT; y++)
    {
        for (int x = 0; x < FIELD_WIDTH; x++)
        {
            checkedAdjoinSameNumField[x][y] = 0;
        }
    }
}

void Field::checkAdjoinSameNum(int posX, int posY)
{
    if (checkedAdjoinSameNumField[posX][posY] != 0)
    {
        return;
    }

    checkedAdjoinSameNumField[posX][posY] = 1;
    adjoinSameNumCount++;

    if ( posX < FIELD_WIDTH - 1 && field[posX + 1][posY] == field[posX][posY])
    {
        Field::checkAdjoinSameNum(posX + 1, posY);
    }

    if (posY < FIELD_HEIGHT - 1 && field[posX][posY + 1] == field[posX][posY])
    {
        Field::checkAdjoinSameNum(posX, posY + 1);
    }

    if (posX > 0 && field[posX - 1][posY] == field[posX][posY])
    {
        Field::checkAdjoinSameNum(posX - 1, posY);
    }

    if (posY > 0 && field[posX][posY - 1] == field[posX][posY])
    {
        Field::checkAdjoinSameNum(posX, posY - 1);
    }

}

void Field::createDeleteField()
{
    isExistDeletePanel = false;
    deletePanelCount = 0;
    comboCount = 0;
    int deletePanelCounter[PANEL_TYPE_NUM + 1] = {};
    for (int y = 0; y < FIELD_HEIGHT; y++)
    {
        for (int x = 0; x < FIELD_WIDTH; x++)
        {
            if (field[x][y] == adjoinSameNumField[x][y])
            {
                isExistDeletePanel = true;
                deleteField[x][y] = 1;
                deletePanelCount++;
                deletePanelCounter[field[x][y]]++;
            } else {
                deleteField[x][y] = 0;
            }
            CCLog ("debug:deleteField[%d][%d] = %d", x,y,deleteField[x][y]);
        }
    }

    for (int i = 1; i <= PANEL_TYPE_NUM; i++)
    {
        if(deletePanelCounter[i] > 0)
        {
            comboCount += (int)floor(deletePanelCounter[i]/i);
        }
        deletePanelCounter[i] = 0;
    }

}

void Field::deleteMainField()
{
    for (int y = 0; y < FIELD_HEIGHT; y++)
    {
        for (int x = 0; x < FIELD_WIDTH; x++)
        {
            if (deleteField[x][y] == 1)
            {
                field[x][y] = 0;
            }
        }
    }
    isExistDeletePanel = false;
}

void Field::createDropField()
{
    for (int y = 0; y < FIELD_HEIGHT * 2; y++)
    {
        for (int x = 0; x < FIELD_WIDTH; x++)
        {
            dropField[x][y] = Field::getDropCount(x,y);
            CCLog ("debug:dropField[%d][%d] = %d", x,y,dropField[x][y]);
        }
    }
}

void Field::dropMainField()
{
    for (int y = 0; y < FIELD_HEIGHT * 2; y++)
    {
        for (int x = 0; x < FIELD_WIDTH; x++)
        {
            if (dropField[x][y] > 0)
            {
                field[x][y - dropField[x][y]] = field[x][y];
                CCLog ("debug:drop field [%d][%d] => [%d][%d] %d", x,y,x,y - dropField[x][y],field[x][y]);
                field[x][y] = (arc4random() % PANEL_TYPE_NUM) + 1;
            }
        }
    }
}

int Field::getDropCount(int posX, int posY)
{
    int dropCount = 0;
    for(int y = 0; y < posY; y++)
    {
        if(field[posX][y] == 0)
        {
            dropCount++;
        }
    }
    return dropCount;
}

int Field::getFieldValue(int x, int y)
{
    return field[x][y];
}

int Field::getDropFieldValue(int x, int y)
{
    return dropField[x][y];
}

void Field::swapFieldValue(int x1, int y1, int x2, int y2)
{
    int tmp;
    tmp = field[x1][y1];
    field[x1][y1] = field[x2][y2];
    field[x2][y2] = tmp;

}

