#include "TXLib.h"
#include <iostream>
#include <conio.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

struct Button
{
    int x;
    int y;
    const char* text;
    string category;
};

void drawButton(Button btn)
{
    txSetColor (TX_BLACK);
    txSetFillColor (TX_GRAY);
    Win32::RoundRect  (txDC(), btn.x+2, btn.y+2, btn.x+100+2, btn.y+30+2, 20, 20);
    txSetFillColor (TX_YELLOW);
    Win32::RoundRect  (txDC(), btn.x, btn.y, btn.x+100, btn.y+30, 20, 20);
    txSelectFont("Times", 20);
    txDrawText(btn.x, btn.y, btn.x+100, btn.y+30, btn.text);
}

bool Click(Button btn)
{
  if (txMouseButtons() == 1 &&
        txMouseX() >= btn.x &&
        txMouseX() <= btn.x+100 &&
        txMouseY() >= btn.y &&
        txMouseY() <= btn.y+60)
        {
        return 1;
        }
        else
        {
        return 0;
        }
}

struct Picture
{
    int x;
    int y;
    string adress;
    HDC image;
    string category;
    int w;
    int h;
    bool visible;
};

int get_w(string adress)
{
    FILE *f1 = fopen(adress.c_str(), "rb");

    unsigned char headerinfo[54];
    fread(headerinfo, sizeof(unsigned char),54,f1);

    int w = *(int*)&headerinfo[18];

    return w;
}

int get_h(string adress)
{
    FILE *f1 = fopen(adress.c_str(), "rb");

    unsigned char headerinfo[54];
    fread(headerinfo, sizeof(unsigned char),54,f1);

    int h = *(int*)&headerinfo[22];

    return h;
}

void drawPicture(Picture pct)
{
    if (pct.visible)
    {
    Win32::TransparentBlt(txDC(), pct.x, pct.y, pct.w, pct.h, pct.image, 0, 0, pct.w, pct.h, TX_WHITE );
    }
}


int main()
{
    txCreateWindow (900, 800);
    const int COUNT_BTNS = 4;
    const int COUNT_PICS = 10;
    int nCentrPic = 0;
    HDC fon = txLoadImage("��������/���.bmp");
    HDC avto = txLoadImage("��������/����/����.bmp");


    //������ ��������-����
    Picture menuPicture[COUNT_PICS];
    menuPicture[0] = {70,80, "��������/�����/�����.bmp"};
    menuPicture[1] = {70,130, "��������/�����/�����2.bmp"};
    menuPicture[2] = {70,180, "��������/�����/�����3.bmp"};
    menuPicture[3] = {70,230, "��������/�����/�����4.bmp"};
    menuPicture[4] = {180,70, "��������/����������/���������1.bmp"};
    menuPicture[5] = {180,130, "��������/����������/���������2.bmp"};
    menuPicture[6] = {290,80, "��������/���������/����������.bmp"};
    menuPicture[7] = {290,140, "��������/���������/�������.bmp"};
    menuPicture[8] = {290,200, "��������/���������/�������.bmp"};
    menuPicture[9] = {390,90, "��������/������/������.bmp"};



    //������ ����������� ��������
    Picture centralPicture[100];

    //������ ������
    Button btn[COUNT_BTNS];
    btn[0] = {50, 30, "�����", "�����"};
    btn[1] = {160, 30, "����������", "����������"};
    btn[2] = {270, 30, "���������", "���������"};
    btn[3] = {380, 30, "������", "������"};

    int vybor=-1;
    bool mouse_release=false;

    for (int npic=0; npic<COUNT_PICS; npic++)
    {
        menuPicture[npic].image = txLoadImage(menuPicture[npic].adress.c_str());

        menuPicture[npic].w=get_w(menuPicture[npic].adress);
        menuPicture[npic].h=get_h(menuPicture[npic].adress);
        menuPicture[npic].visible=false;

        string stroka = menuPicture[npic].adress;
        int pos_1 = stroka.find("/");
        int pos_2 = stroka.find("/", pos_1+1);
        menuPicture[npic].category = stroka.substr(pos_1+1, pos_2-pos_1-1);
    }

    while (!GetAsyncKeyState (VK_ESCAPE))
    {
    txBegin();
    txSetColor (TX_WHITE);
    txSetFillColor (TX_WHITE);

    txClear();

      txBitBlt(txDC(),0, 0, 900, 800, fon);
      txTransparentBlt(txDC(), 70, 280, 500, 280, avto, 0,0, TX_WHITE);

        //��������� ������
        for (int nk=0; nk<COUNT_BTNS; nk++)
        {
        drawButton (btn[nk]);
        }
        //��������� ��������-����
        for (int npic=0; npic<COUNT_PICS; npic++)
        {
        drawPicture(menuPicture[npic]);
        }
        //��������� ����������� ��������

        for (int npic=0; npic<COUNT_PICS; npic++)
        {
        drawPicture(centralPicture[npic]);
        }


        //��������� ��������-���� �� ��������� ������
        for (int nk=0; nk <COUNT_BTNS; nk++)
        {
            if (Click(btn[nk]))
                {
                for (int npic=0; npic<COUNT_PICS; npic++)
                    {
                        menuPicture[npic].visible=false;
                        if (menuPicture[npic].category == btn[nk].category)
                          {
                            menuPicture[npic].visible=true;
                          }
                    }

                }
         }


        //������������ �������� ������� ����������� ��������
        for (int npic=0; npic<COUNT_PICS; npic++)
        {
         if (txMouseButtons() == 1 &&
            menuPicture[npic].visible &&
            txMouseX() >= menuPicture[npic].x &&
            txMouseX() <= menuPicture[npic].x+menuPicture[npic].w &&
            txMouseY() >= menuPicture[npic].y &&
            txMouseY() <= menuPicture[npic].y+menuPicture[npic].h)
            {
                //�������� ������� ����� ������ ����
                /*    while(txMouseButtons() == 1)
                    {
                    txSleep(10);
                    }             */

                centralPicture[nCentrPic] = {300,
                                             100,
                                             menuPicture[npic].adress,
                                             menuPicture[npic].image,
                                             menuPicture[npic].category,
                                             menuPicture[npic].w,
                                             menuPicture[npic].h,
                                             menuPicture[npic].visible,
                                             };

                nCentrPic++;
            }

        }

        //����� ����������� ��������
        for (int npic=0; npic<nCentrPic; npic++)
        {
            if (txMouseButtons() == 1 &&
                centralPicture[npic].visible &&
                txMouseX() >= centralPicture[npic].x &&
                txMouseX() <= centralPicture[npic].x+centralPicture[npic].w &&
                txMouseY() >= centralPicture[npic].y &&
                txMouseY() <= centralPicture[npic].y+centralPicture[npic].h)
                {
                vybor=npic;
                mouse_release=false;
                }
        }

        //������������ ����������� ��������
        if (vybor>=0 && (txMouseButtons() == 1) && !mouse_release)
            {
                centralPicture[vybor].x = txMouseX() - centralPicture[vybor].w/2;
                centralPicture[vybor].y = txMouseY() - centralPicture[vybor].h/2;
            }

        if (vybor>=0 && !(txMouseButtons() == 1) && !mouse_release)
                {
                mouse_release=true;
                }

        char str[100];
        sprintf(str, "vybor = %d nCentrPic = %d", vybor , nCentrPic);
        txTextOut(1000,500, str);

        //�������� ����������� ��������
        if (vybor>=0 && GetAsyncKeyState(VK_DELETE))
        {
            centralPicture[vybor] = centralPicture[nCentrPic - 1];
            nCentrPic--;
            vybor=-1;
            mouse_release = true;
        }




    txSleep(150);
    txEnd();
    }

    for (int npic=0; npic<COUNT_PICS; npic++)
    {
      txDeleteDC(menuPicture[npic].image);
    }



    return 0;
}


