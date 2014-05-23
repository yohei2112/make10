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

    int checkedAdjoinSameNumField[FIELD_WIDTH][FIELD_HEIGHT]; //�����������ׂ荇�����}�X�̃`�F�b�N���I��������W���Ǘ�����t�B�[���h
    int adjoinSameNumField[FIELD_WIDTH][FIELD_HEIGHT]; //���������Ɨׂ荇���Ă��鐔������t�B�[���h
    int deleteField[FIELD_WIDTH][FIELD_HEIGHT]; //�폜������W������t�B�[���h
    int dropField[FIELD_WIDTH][FIELD_HEIGHT * 2]; //���̃u���b�N�̉��ɂ���������u���b�N�̐�������t�B�[���h
    int adjoinSameNumCount;
    int field[FIELD_WIDTH][FIELD_HEIGHT * 2];
};

#endif // __Field_H__
