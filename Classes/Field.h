#ifndef __Field_H__
#define __Field_H__
#include "Config.h"


class Field
{
public:

    void checkAdjoinSameNumField();
    void checkAdjoinSameNum(int posX, int posY);
    void initCheckedAdjoinSameNumField();
    void initField();
    void createDeleteField();
    void createDropField();
    void deleteMainField();
    void dropMainField();
    int getFieldValue(int x, int y);
    int getDropFieldValue(int x, int y);
    int getDropCount(int x, int y);
    void swapFieldValue(int x1, int y1, int x2, int y2);

    int checkedAdjoinSameNumField[FIELD_WIDTH][FIELD_HEIGHT]; //同じ数字が隣り合ったマスのチェックが終わった座標を管理するフィールド
    int adjoinSameNumField[FIELD_WIDTH][FIELD_HEIGHT]; //同じ数字と隣り合っている数が入るフィールド
    int deleteField[FIELD_WIDTH][FIELD_HEIGHT]; //削除する座標が入るフィールド
    int dropField[FIELD_WIDTH][FIELD_HEIGHT * 2]; //そのブロックの下にある消えたブロックの数が入るフィールド
    int adjoinSameNumCount;
    int field[FIELD_WIDTH][FIELD_HEIGHT * 2];
    int deletePanelCount;
    int deletePanelCounter[6];
    int comboCount;

    bool isExistDeletePanel;

};

#endif // __Field_H__
