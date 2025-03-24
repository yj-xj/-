#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <conio.h>
#include<time.h>
#include<stdlib.h>
#include<graphics.h>
#include<mmsystem.h>
#include <functional>
#include <string>
#include <vector>
#include <cctype>
#include <locale>
#include <codecvt>
#pragma comment(lib,"winmm.lib")
using namespace std;
int N = 10;//��
int M = 10;//��
int MINE = 10;//��
#define SIZE 40//ͼƬ��С
int map[100][100];//��ͼ
IMAGE image[13];//��ͼƬ������
int mine = MINE;//���ڲ���ʱ��������
int flag = 0;//�����ж���Ӯ
bool ishit1 = false;
bool ishit2 = true;

time_t start_time = time(nullptr);
time_t current_time = start_time;

class TextBox
{
private:
    int x;
    int y;
    int width;
    int height;
    int maxWord;
    std::string text;
    bool isSelected;
    bool showCursor;
    int cursorPos;
    int mun;

public:
    TextBox(int x, int y, int width, int height, int maxWord, int mun)
        : x(x), y(y), width(width), height(height), maxWord(maxWord), mun(mun), isSelected(false), showCursor(false), cursorPos(0)
    {
        text = "";
    }


    bool getSelect() const
    {
        return isSelected;
    }

    void draw()
    {
        setfillcolor(WHITE);
        if (isSelected)
        {
            setlinecolor(RGB(0, 120, 215));
            fillrectangle(x, y, x + width, y + height);
        }
        else
        {
            setlinecolor(RGB(122, 122, 122));
            fillrectangle(x, y, x + width, y + height);
        }

        settextcolor(BLACK);
        setbkmode(TRANSPARENT);
        settextstyle(height * 3 / 4, 0, "΢���ź�");

        outtextxy(x + 5, y + (height - textheight("΢���ź�")) / 2, text.c_str());

        setlinecolor(BLACK);
        if (isSelected && showCursor)
        {
            int cursorX = x + 5 + textwidth(text.c_str());
            line(cursorX, y + 2 + height / 8, cursorX, y + height * 7 / 8 - 2);
        }
    }

    bool checkClick(int mouseX, int mouseY)
    {
        if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height)
        {
            isSelected = true;
            return true;
        }
        else
        {
            isSelected = false;
            cursorPos = text.length();
            return false;
        }
    }

    void keyInput(wchar_t ch)
    {
        if (isSelected)
        {
            switch (ch)
            {
            case '\b':
                if (!text.empty() && cursorPos > 0)
                {
                    text.erase(cursorPos - 1, 1);
                    cursorPos--;
                }
                break;
            case '\r':
            case '\n':
                if (mun == 1)
                    M = std::stoi(text);
                else if (mun == 2)
                    N = std::stoi(text);
                else if (mun == 3)
                    MINE = std::stoi(text);
                cursorPos = text.length();
                isSelected = false;
                break;
            default:
                if ((ch >= 1 && ch <= 127)) {
                    if (std::isdigit(ch) || ch == '\r')
                    {
                        text.insert(cursorPos, 1, ch);
                        cursorPos++;
                        if (text.length() > maxWord) {
                            MessageBox(NULL, "�����ַ�����", "�������", MB_OK);
                            text.erase(cursorPos - 1, 1);
                            cursorPos--;
                        }
                           
                    }
                }              
                else
                {
                    MessageBox(NULL, "ֻ�����������ֺͻس������������룡", "�������", MB_OK);
                }
            }
        }
    }

    void updateCursor()
    {
        static DWORD lastTick = GetTickCount();
        DWORD currentTick = GetTickCount();
        if (currentTick - lastTick >= 500)
        {
            showCursor = !showCursor;
            lastTick = currentTick;
        }
    }
};

class Button
{
private:
    int x; // ��ť���Ͻ�x����
    int y; // ��ť���Ͻ�y����
    int width; // ��ť���
    int height; // ��ť�߶�
    float scale; // ���ű���������ʵ�������ͣЧ��
    bool isMouseOver; // ��ʾ����Ƿ��ڰ�ť�Ϸ�
    wstring text; // ��ť�ı�
    function<void()> onClick; // �����ť�����ĺ���

public:

    Button(int x, int y, int width, int height, const wstring& text, const function<void()>& onClick)
        : x(x), y(y), width(width), height(height), text(text), onClick(onClick), scale(1.0f), isMouseOver(false)
    {
    }

    // �������Ƿ��ڰ�ť�Ϸ�
    void checkMouseOver(int mouseX, int mouseY)
    {
        isMouseOver = (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height);

        if (isMouseOver) {
            scale = 0.9f; // �����ͣʱ���Ű�ť
        }
        else {
            scale = 1.0f; // �ָ���ťԭʼ��С
        }
    }

    // ���������Ƿ��ڰ�ť�ڣ���ִ�к���
    bool checkClick(int mouseX, int mouseY)
    {
        if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height)
        {

            onClick(); // ִ�а�ť���ʱ�ĺ���
            isMouseOver = false;
            scale = 1.0f;
            return true;
        }
        return false;
    }
    // ���ư�ť
    void draw()
    {
        int scaledWidth = width * scale; // ���ź�İ�ť���
        int scaledHeight = height * scale; // ���ź�İ�ť�߶�
        int scaledX = x + (width - scaledWidth) / 2; // ���ź�İ�ťx����
        int scaledY = y + (height - scaledHeight) / 2; // ���ź�İ�ťy����

        if (isMouseOver)
        {
            setlinecolor(RGB(0, 120, 215)); // �����ͣʱ��ť�߿���ɫ
            setfillcolor(RGB(229, 241, 251)); // �����ͣʱ��ť�����ɫ

        }
        else
        {
            setlinecolor(RGB(173, 173, 173)); // ��ť�߿���ɫ
            setfillcolor(RGB(225, 225, 225)); // ��ť�����ɫ
        }

        fillrectangle(scaledX, scaledY, scaledX + scaledWidth, scaledY + scaledHeight); // ���ư�ť
        settextcolor(BLACK); // �����ı���ɫΪ��ɫ
        setbkmode(TRANSPARENT); // �����ı�����͸��
        settextstyle(20 * scale, 0, _T("΢���ź�")); // �����ı���С������
        // �����ַ�ת��Ϊ���ֽ��ַ�
        int len = WideCharToMultiByte(CP_ACP, 0, text.c_str(), -1, NULL, 0, NULL, NULL);
        char* mbstr = new char[len];
        WideCharToMultiByte(CP_ACP, 0, text.c_str(), -1, mbstr, len, NULL, NULL);

        // ������ֽ��ַ��ı��Ŀ��
        int mbTextWidth = textwidth(mbstr);

        // �����ı��ڰ�ť�����x����
        int textX = scaledX + (scaledWidth - mbTextWidth) / 2;

        // �����ı��ڰ�ť�����y����
        int textY = scaledY + (scaledHeight - textheight("΢���ź�")) / 2;

        // �ڰ�ť�ϻ��ƶ��ֽ��ַ��ı�
        outtextxy(textX, textY, mbstr);

        // �ͷ��ڴ�
        delete[] mbstr;

    }
};

// ����Widget�࣬��ʾһ���򵥵�ͼ���û�����
class Widget
{
private:
    int width; // ���
    int height; // �߶�
    int currentIndex; // ��ǰҳ������
    vector<IMAGE*> pages; // �洢����ҳ���ͼƬָ��
    vector<vector<Button*>> buttons; // �洢ÿ��ҳ���ϵİ�ť
    vector<vector<TextBox*>>textBoxs;

    // ���һ��ҳ��
    void addPage(IMAGE* page)
    {
        pages.push_back(page);
        buttons.push_back({});
        textBoxs.push_back({});
    }

    // ��ָ��ҳ�������һ����ť
    void addButton(int index, Button* button)
    {
        if (index >= 0 && index < buttons.size())
        {
            buttons[index].push_back(button);
        }
    }

    void addTextBox(int index, TextBox* textBox)
    {
        if (index >= 0 && index < textBoxs.size())
        {
            textBoxs[index].push_back(textBox);
        }
    }

    // ���õ�ǰ��ʾ��ҳ������
    void setCurrentIndex(int index)
    {
        if (index >= 0 && index < pages.size())
        {
            currentIndex = index;
        }
    }

    void keyInput(wchar_t ch)
    {
        if (currentIndex >= 0 && currentIndex < textBoxs.size())
        {
            for (auto textbox : textBoxs[currentIndex])
            {
                if (textbox->getSelect())
                {
                    textbox->keyInput(ch);
                }
            }
        }
    }

    // ����������¼�
    void mouseClick(int mouseX, int mouseY)
    {
        if (currentIndex >= 0 && currentIndex < buttons.size())
        {
            for (Button* button : buttons[currentIndex])
            {
                if (button->checkClick(mouseX, mouseY))
                {
                    break;
                }
            }
        }
        if (currentIndex >= 0 && currentIndex < textBoxs.size())
        {
            for (auto textbox : textBoxs[currentIndex])
            {
                textbox->checkClick(mouseX, mouseY);
            }
        }
    }


    // ��������ƶ��¼�
    void mouseMove(int mouseX, int mouseY)
    {
        if (currentIndex >= 0 && currentIndex < buttons.size())
        {
            for (Button* button : buttons[currentIndex])
            {
                button->checkMouseOver(mouseX, mouseY);
            }
        }
    }

    // ���Ƶ�ǰҳ�������
    void draw1()
    {

        if (currentIndex >= 0 && currentIndex < pages.size())
        {

            putimage(0, 0, pages[currentIndex]); // �ڴ����л��Ƶ�ǰҳ���ͼƬ

            if (currentIndex >= 0 && currentIndex < buttons.size())
            {
                
                for (Button* button : buttons[currentIndex])
                {
                    button->draw(); // ���Ƶ�ǰҳ���ϵ����а�ť
                }
            }
            if (currentIndex >= 0 && currentIndex < textBoxs.size())
            {
                for (auto textbox : textBoxs[currentIndex])
                {
                    if (textbox->getSelect())
                    {
                        textbox->updateCursor();
                    }
                    textbox->draw();
                }
            }
        }
    }
public:
    Widget(int width, int height)
        :width(width), height(height), currentIndex(-1)
    {
    }
    ~Widget() {}
    void start()
    {
        mine = MINE;
        initgraph(N * SIZE, M * SIZE + 50, SHOWCONSOLE);
        setbkcolor(LIGHTGRAY);
        //cleardevice();
        Text();
            Game();
        BeginBatchDraw();
        start_time = time(nullptr);
        while (1) {
            Drawgame();
            DrawTimer();
            FlushBatchDraw();
            IsWin();
            Boom();
        }
        EndBatchDraw();
        closegraph();
    }
    // ��ʼ���ؼ�������ͼ�λ���������ҳ��Ͱ�ť
    void init()
    {
        initgraph(width, height,SHOWCONSOLE);
        IMAGE* page0 = new IMAGE(width, height);//����ֱ����loadimage()��������ͼƬ
        setfillcolor(RGB(240, 240, 240)); // ����ҳ��1�ı�����ɫΪǳ��ɫ
        solidrectangle(0, 0, width, height); // ����ҳ��1�ı�������
        getimage(page0, 0, 0, width, height); // ��ҳ��1�����ݱ��浽ͼƬ��       
        Button* button0_1 = new Button(300, 325,200 , 50, L"�Զ���ģʽ", [&]() {
            setCurrentIndex(1);
            });
        Button* button0_2 = new Button(300, 100, 200, 50, L"��ģʽ", [&]() {
            //setCurrentIndex(1);
            N = 10;
            M = 10;
            MINE = 10;
            start();
            });
        Button* button0_3 = new Button(300, 175, 200, 50, L"����ģʽ", [&]() {
            //setCurrentIndex(1);
            N = 16;
            M = 16;
            MINE = 45;
            start();
            });
        Button* button0_4 = new Button(300, 250, 200, 50, L"�����Ѷ�", [&]() {
            int isok = MessageBox(GetHWnd(), "��ģʽ�Ѷȹ����Ƿ������ս��", "��ʾ", MB_OKCANCEL);
            if (isok == IDOK) {
                N = 30;
                M = 16;
                MINE = 100;
                start();
            }
            //setCurrentIndex(1); 
            });
        Button* button0_5 = new Button(300, 500, 200, 50, L"�˳�", [&]() {
            exit(1);
            });
        Button* button0_6 = new Button(350, 425, 100, 50, L"��Ϸ˵��", [&]() {
            if (!ishit1) {
                int isok = MessageBox(NULL, "���ɨ�׻���˵���ǰɡ�", "����", MB_OKCANCEL);
                if (isok == IDOK)
                {
                    ishit1 = true;
                    MessageBox(NULL, "ɨ����һ�־���ĵ���������Ϸ�������Ҫ����������ʾ���ҿ����ף�������е��ס�������ɨ�׵Ĺ���˵��: 1. ��Ϸ��ʼʱ����һῴ��һ���ɷ�����ɵ���������һЩ�������������е��ף�������������������ʾ���֡�                  2. �����Ҫ���������飬�������ķ������������֣����ʾ�÷�����Χ�м������ף��������ķ��������ǿհף����ʾ�÷�����Χû�е��ס�3. ����������ʾ����ҿ����ƶϳ���Щ���������ǵ��ף���Щ�����ǰ�ȫ�ġ������Ҫ����˼����������е��ס�4. �����Ҳ��е��ף�����Ϸ���������ʧ�ܣ������ҳɹ��ҿ����в������׵ķ��飬����Ϸʤ����5. ��ҿ��Ա�����Ƶ��׵ķ��飬�԰����Լ����䡣��ǵ��׵ķ�ʽͨ�����Ҽ�������飬���߳������顣6. ɨ����Ϸͨ���в�ͬ���Ѷȵȼ��������������м��͸߼��ȣ���ҿ��Ը����Լ�������ѡ���ʺϵ��Ѷ�����ս��ϣ�����Ϲ���˵�����԰��������õ��˽�ɨ����Ϸ���淨�͹���ף�������죡", "ɨ��˵��", MB_OKCANCEL);
                }
            }
            else {
                int isok = MessageBox(NULL, "������˵���������ǰɡ�", "����", MB_OKCANCEL);
                if (isok == IDOK)
                {
                    MessageBox(NULL, "ɨ����һ�־���ĵ���������Ϸ�������Ҫ����������ʾ���ҿ����ף�������е��ס�������ɨ�׵Ĺ���˵��: 1. ��Ϸ��ʼʱ����һῴ��һ���ɷ�����ɵ���������һЩ�������������е��ף�������������������ʾ���֡�                  2. �����Ҫ���������飬�������ķ������������֣����ʾ�÷�����Χ�м������ף��������ķ��������ǿհף����ʾ�÷�����Χû�е��ס�3. ����������ʾ����ҿ����ƶϳ���Щ���������ǵ��ף���Щ�����ǰ�ȫ�ġ������Ҫ����˼����������е��ס�4. �����Ҳ��е��ף�����Ϸ���������ʧ�ܣ������ҳɹ��ҿ����в������׵ķ��飬����Ϸʤ����5. ��ҿ��Ա�����Ƶ��׵ķ��飬�԰����Լ����䡣��ǵ��׵ķ�ʽͨ�����Ҽ�������飬���߳������顣6. ɨ����Ϸͨ���в�ͬ���Ѷȵȼ��������������м��͸߼��ȣ���ҿ��Ը����Լ�������ѡ���ʺϵ��Ѷ�����ս��ϣ�����Ϲ���˵�����԰��������õ��˽�ɨ����Ϸ���淨�͹���ף�������죡", "ɨ��˵��", MB_OKCANCEL);
                }
            }
            });

        addPage(page0); // ���ҳ��0
        addButton(0, button0_1); // ����ť1��ӵ�ҳ��0
        addButton(0, button0_2);
        addButton(0, button0_3);
        addButton(0, button0_4);
        addButton(0, button0_5);
        addButton(0, button0_6);
        // ����ҳ��1
        IMAGE* page1 = new IMAGE(width, height);//����ֱ����loadimage()��������ͼƬ
        setfillcolor(RGB(240, 240, 240)); // ����ҳ��1�ı�����ɫΪǳ��ɫ
        solidrectangle(0, 0, width, height); // ����ҳ��1�ı�������
        getimage(page1, 0, 0, width, height); // ��ҳ��1�����ݱ��浽ͼƬ��

        
        TextBox* textbox1_1 = new TextBox(300, 100, 120, 30, 10,1);
        TextBox* textbox1_2 = new TextBox(300, 175, 120, 30, 10, 2);
        TextBox* textbox1_3 = new TextBox(300, 250, 120, 30, 10, 3);
        // ��ҳ��1������ť1
        Button* button1_1 = new Button(300, 300, 200, 50, L"��ʼ��Ϸ", [&]() {
            //setCurrentIndex(1); // �����ť1ʱ���л���ҳ��2
            start();
            });
        Button* button1_2 = new Button(300, 500, 200, 50, L"�������˵�", [&]() {
            setCurrentIndex(0); // �����ť1ʱ���л���ҳ��2           
            });
        addPage(page1); // ���ҳ��1
        addButton(1, button1_1); // ����ť1��ӵ�ҳ��1
        addButton(1, button1_2); // ����ť1��ӵ�ҳ��1
        addTextBox(1, textbox1_1);
        addTextBox(1, textbox1_2);
        addTextBox(1, textbox1_3);
        // ��ҳ��2
        IMAGE* page2 = new IMAGE(width, height);
        setfillcolor(LIGHTGRAY); 
        solidrectangle(0, 0, width, height); 
        getimage(page2, 0, 0, width, height); 
        //��ҳ��2������ť1
        //Button* button2_1 = new Button(N * SIZE + 200, 50, 100, 50, L"���ص�Page 1", [&]() {
           // setCurrentIndex(0); // �����ť1ʱ���л���ҳ��1
          //  });

        addPage(page2);
        //addButton(1, button2_1); // ����ť1��ӵ�ҳ��2

        setCurrentIndex(0); 
    }

    bool Safe() {
        if (MINE >=N * M)
        {
            int isok = MessageBox(GetHWnd(), "���������������У��к��������뱣֤��ͼ��װ����", "��ʾ", MB_OK);
            if (isok == IDOK)
            {
                 N = 10;
                 M = 10;
                 MINE = 10;
            }
            return false;
        }
        else if (N >= 31) {
            int isok = MessageBox(GetHWnd(), "���������������У��к�����������������", "��ʾ", MB_OK);
            if (isok == IDOK)
            {
                 N = 10;
                 M = 10;
                 MINE = 10;
            }
            return false;
        }
        else if (M >= 25) {
            int isok = MessageBox(GetHWnd(), "���������������У��к�����,����������", "��ʾ", MB_OK);
            if (isok == IDOK)
            {
                 N = 10;
                 M = 10;
                MINE = 10;               
            }
            return false;
        }
        return true;
    }
    // ���У�������Ϣѭ��
    void run1()
    {
        Text();
        //run2();
        ExMessage msg;
        
        BeginBatchDraw(); // ��ʼ��������
        
        
        while (1)
        {
            if (Safe()) {
                if (peekmessage(&msg)) // ����Ƿ�����Ϣ
                {
                    int mouseX = msg.x; // ��ȡ���x����
                    int mouseY = msg.y; // ��ȡ���y����

                    switch (msg.message)
                    {
                    case WM_LBUTTONDOWN: // �����������¼�  
                       
                        mouseClick(mouseX, mouseY); // ����������¼�
                        break;
                    case WM_MOUSEMOVE: // ����ƶ��¼�
                        mouseMove(mouseX, mouseY); // ��������ƶ��¼�
                        break;
                    case WM_CHAR:
                        keyInput(msg.ch);
                        break;
                    }
                }           
                draw1(); // ���Ƶ�ǰҳ������   
                FlushBatchDraw();
                if (currentIndex == 1) {
                    outtextxy(275, 100, "�У�");
                    outtextxy(275, 175, "�У�");
                    outtextxy(260, 250, "������");
                }
                FlushBatchDraw(); // ��������������ʾ����Ļ��
                
                EndBatchDraw();// ������������
            } 
            Sleep(2);
        }       
    }
    
    // �ر�
    void Game() {
        //���������
        srand((unsigned)time(NULL));
        //��ʼ��
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                map[i][j] = 0;
            }
        }
        //����
        for (int i = 0; i < MINE;) {
            int r = rand() % N;
            int c = rand() % M;
            if (map[r][c] == 0) { //��ֹ�ظ�
                map[r][c] = -1;
                i++;
            }
        }
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                if (map[i][j] == -1) {
                    for (int a = i - 1; a < i + 2; a++) {
                        for (int b = j - 1; b < j + 2; b++) {
                            if ((a >= 0 && a < N && b >= 0 && b < M) && map[a][b] != -1) {
                                map[a][b] += 1;
                            }
                        }
                    }
                }
            }
        }
        //����ͼƬ
        for (int i = 0; i < 13; i++)
        {
            char file[50] = "";
            sprintf(file, "./image/%d.jpg", i);
            loadimage(&image[i], file, SIZE, SIZE);
        }
        //���Ǹ���
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                map[i][j] += 20;
            }
        }
    }
    //������Ϸ����
    void Drawgame() {
        cleardevice();
        putimage(0, 0, &image[9]);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                if (map[i][j] == -1) {
                    putimage(i * SIZE, j * SIZE + 50, &image[9]);
                }
                switch (map[i][j])
                {
                case 0:putimage(i * SIZE, j * SIZE + 50, &image[0]);
                    break;
                case 1:putimage(i * SIZE, j * SIZE + 50, &image[1]);
                    break;
                case 2:putimage(i * SIZE, j * SIZE + 50, &image[2]);
                    break;
                case 3:putimage(i * SIZE, j * SIZE + 50, &image[3]);
                    break;
                case 4:putimage(i * SIZE, j * SIZE + 50, &image[4]);
                    break;
                case 5:putimage(i * SIZE, j * SIZE + 50, &image[5]);
                    break;
                case 6:putimage(i * SIZE, j * SIZE + 50, &image[6]);
                    break;
                case 7:putimage(i * SIZE, j * SIZE + 50, &image[7]);
                    break;
                case 8:putimage(i * SIZE, j * SIZE + 50, &image[8]);
                    break;
                default:
                    break;
                }
                if (map[i][j] >= 19 && map[i][j] < 28)
                    putimage(i * SIZE, j * SIZE + 50, &image[10]);
                else if (map[i][j] > 30)
                    putimage(i * SIZE, j * SIZE + 50, &image[11]);               
            }
        }
    }
    void RevealMines() {
        //������ͼ
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                //�����ǰλ�����ף������ó���ʾ�׵�״̬
                if (map[i][j] == 19 ) {
                    map[i][j] -= 20; 
                }
                else if(map[i][j] > 30)
                    map[i][j] -= 40;
            }
        }
        Drawgame(); //������Ϸ���棬��ʾ���е���
    }

    void Open(int r, int c) {
        if (map[r][c] == 0)
        {
            for (int a = r - 1; a < r + 2; a++) {
                for (int b = c - 1; b < c + 2; b++) {
                    if ((a >= 0 && a < N && b >= 0 && b < M) && (map[a][b] >= 20 && map[a][b] <= 28) && map[a][b] > 8) {
                        map[a][b] -= 20;
                        flag++;
                        Open(a, b);
                    }
                }
            }
        }
    }
    int Mousehit() {
        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            int n = msg.x / SIZE;
            int m = (msg.y - 50) / SIZE;
            switch (msg.uMsg)
            {
            case WM_LBUTTONDOWN:
                if (ishit2) {
                    while (map[n][m] == -1) {
                        Game(); // �������ɵ�ͼֱ����һ�ε����λ�ò�����
                    }
                    ishit2 = false;
                }
                if (map[n][m] > 8 && map[n][m] != 20) {
                    mciSendString("close c", 0, 0, 0);
                    mciSendString("open ./image/click.wav alias c", 0, 0, 0);
                    mciSendString("play c", 0, 0, 0);
                    map[n][m] -= 20;
                    flag++;
                }
                else if (map[n][m] == 20) {
                    mciSendString("close c", 0, 0, 0);
                    mciSendString("open ./image/click.wav alias c", 0, 0, 0);
                    mciSendString("play c", 0, 0, 0);
                    map[n][m] -= 20;
                    flag++;
                    Open(n, m);
                }
               
                break;
            case WM_RBUTTONDOWN:
                if (map[n][m] > 8 && map[n][m] < 28 && mine!=0) {
                    map[n][m] += 20;
                    if (mine > 0)
                        mine--;
                }                   
                else if (map[n][m] > 28) {
                    map[n][m] -= 20;
                    if (mine < MINE)
                        mine++;
                    break;
                }
                    
            }
            return map[n][m];
        }
    }
    //void Out() {
       // for (int i = 0; i < N; i++) {
            //for (int j = 0; j < M; j++) {
                //cout << map[i][j] << " ";
            //}
           // cout << endl;
       // }
   // }
    void IsWin() {
        if (Mousehit() == -1)
        {
            mciSendString("close e", 0, 0, 0);
            mciSendString("open ./image/end.wav alias e", 0, 0, 0);
            mciSendString("play e", 0, 0, 0);
           RevealMines();
            FlushBatchDraw();
            int isok = MessageBox(GetHWnd(), "�ȵ����ˣ�����һ�ѣ�", "��ʾ", MB_OKCANCEL);
            
            if (IDOK == isok)
            {
                mine = MINE;
                flag = 0;
                Widget(800, 600);
                init();
                run1();
                //run2();
                closegraph();            
            }
            else {
                exit(1);
            }
        }
        else if (flag == N * M - MINE) {
            RevealMines();
            FlushBatchDraw();
            int isok = MessageBox(GetHWnd(), "��Ӯ�ˣ�����һ�ѣ�", "��ʾ", MB_OKCANCEL);
            if (IDOK == isok)
            {
                mine = MINE;
                flag = 0;
                Widget(800, 600);
                init();
                run1();
               // run2();
                closegraph();               
            }
            else {
                exit(1);
            }
        }
    }
    void Boom() {
        if (_kbhit()) {
            char key = _getch();
            switch (key)
            {
            case 72:
            case'w':
            case'W':
                RevealMines();
                FlushBatchDraw();
                int isok = MessageBox(GetHWnd(), "���Ա��ˣ�����һ�ѣ�", "��ʾ", MB_OKCANCEL);
                if (IDOK == isok)
                {
                    mine = MINE;
                    flag = 0;
                    Widget(800, 600);
                    init();
                    run1();
                 //   run2();
                    closegraph();                   
                }
                else {
                    exit(1);
                }
                break;
            }

        }
    }
    void Text() {
        HWND h = GetHWnd();
        SetWindowText(h, "ɨ��");
    }
    void DrawTimer() {
        current_time = time(nullptr);
        int elapsed_time = current_time - start_time;

        int hours = elapsed_time / 3600;
        int minutes = (elapsed_time % 3600) / 60;
        int seconds = elapsed_time % 60;

        // ���Ƽ�ʱ���ı�
        settextcolor(WHITE);
        settextstyle(50, 0, _T("Consolas"));
        char timer_text[20];
        sprintf(timer_text, "%02d:%02d:%02d", hours, minutes, seconds);
        outtextxy(N*SIZE-200, 0, timer_text);
        char mine_text[20];
        
        sprintf(mine_text, "%d", mine);
        outtextxy(60, 0, mine_text);
    }
};


int main() {
    Widget widget(800, 600);
    widget.init();
    widget.run1();
    closegraph();
    return 0;
}
