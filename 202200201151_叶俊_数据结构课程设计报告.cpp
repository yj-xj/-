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
int N = 10;//行
int M = 10;//列
int MINE = 10;//雷
#define SIZE 40//图片大小
int map[100][100];//地图
IMAGE image[13];//放图片的数组
int mine = MINE;//用于插旗时减少雷数
int flag = 0;//用来判断输赢
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
        settextstyle(height * 3 / 4, 0, "微软雅黑");

        outtextxy(x + 5, y + (height - textheight("微软雅黑")) / 2, text.c_str());

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
                            MessageBox(NULL, "输入字符过多", "输入错误", MB_OK);
                            text.erase(cursorPos - 1, 1);
                            cursorPos--;
                        }
                           
                    }
                }              
                else
                {
                    MessageBox(NULL, "只允许输入数字和回车，请重新输入！", "输入错误", MB_OK);
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
    int x; // 按钮左上角x坐标
    int y; // 按钮左上角y坐标
    int width; // 按钮宽度
    int height; // 按钮高度
    float scale; // 缩放比例，用于实现鼠标悬停效果
    bool isMouseOver; // 表示鼠标是否在按钮上方
    wstring text; // 按钮文本
    function<void()> onClick; // 点击按钮触发的函数

public:

    Button(int x, int y, int width, int height, const wstring& text, const function<void()>& onClick)
        : x(x), y(y), width(width), height(height), text(text), onClick(onClick), scale(1.0f), isMouseOver(false)
    {
    }

    // 检查鼠标是否在按钮上方
    void checkMouseOver(int mouseX, int mouseY)
    {
        isMouseOver = (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height);

        if (isMouseOver) {
            scale = 0.9f; // 鼠标悬停时缩放按钮
        }
        else {
            scale = 1.0f; // 恢复按钮原始大小
        }
    }

    // 检查鼠标点击是否在按钮内，并执行函数
    bool checkClick(int mouseX, int mouseY)
    {
        if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height)
        {

            onClick(); // 执行按钮点击时的函数
            isMouseOver = false;
            scale = 1.0f;
            return true;
        }
        return false;
    }
    // 绘制按钮
    void draw()
    {
        int scaledWidth = width * scale; // 缩放后的按钮宽度
        int scaledHeight = height * scale; // 缩放后的按钮高度
        int scaledX = x + (width - scaledWidth) / 2; // 缩放后的按钮x坐标
        int scaledY = y + (height - scaledHeight) / 2; // 缩放后的按钮y坐标

        if (isMouseOver)
        {
            setlinecolor(RGB(0, 120, 215)); // 鼠标悬停时按钮边框颜色
            setfillcolor(RGB(229, 241, 251)); // 鼠标悬停时按钮填充颜色

        }
        else
        {
            setlinecolor(RGB(173, 173, 173)); // 按钮边框颜色
            setfillcolor(RGB(225, 225, 225)); // 按钮填充颜色
        }

        fillrectangle(scaledX, scaledY, scaledX + scaledWidth, scaledY + scaledHeight); // 绘制按钮
        settextcolor(BLACK); // 设置文本颜色为黑色
        setbkmode(TRANSPARENT); // 设置文本背景透明
        settextstyle(20 * scale, 0, _T("微软雅黑")); // 设置文本大小和字体
        // 将宽字符转换为多字节字符
        int len = WideCharToMultiByte(CP_ACP, 0, text.c_str(), -1, NULL, 0, NULL, NULL);
        char* mbstr = new char[len];
        WideCharToMultiByte(CP_ACP, 0, text.c_str(), -1, mbstr, len, NULL, NULL);

        // 计算多字节字符文本的宽度
        int mbTextWidth = textwidth(mbstr);

        // 计算文本在按钮中央的x坐标
        int textX = scaledX + (scaledWidth - mbTextWidth) / 2;

        // 计算文本在按钮中央的y坐标
        int textY = scaledY + (scaledHeight - textheight("微软雅黑")) / 2;

        // 在按钮上绘制多字节字符文本
        outtextxy(textX, textY, mbstr);

        // 释放内存
        delete[] mbstr;

    }
};

// 定义Widget类，表示一个简单的图形用户界面
class Widget
{
private:
    int width; // 宽度
    int height; // 高度
    int currentIndex; // 当前页面索引
    vector<IMAGE*> pages; // 存储所有页面的图片指针
    vector<vector<Button*>> buttons; // 存储每个页面上的按钮
    vector<vector<TextBox*>>textBoxs;

    // 添加一个页面
    void addPage(IMAGE* page)
    {
        pages.push_back(page);
        buttons.push_back({});
        textBoxs.push_back({});
    }

    // 在指定页面上添加一个按钮
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

    // 设置当前显示的页面索引
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

    // 处理鼠标点击事件
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


    // 处理鼠标移动事件
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

    // 绘制当前页面的内容
    void draw1()
    {

        if (currentIndex >= 0 && currentIndex < pages.size())
        {

            putimage(0, 0, pages[currentIndex]); // 在窗口中绘制当前页面的图片

            if (currentIndex >= 0 && currentIndex < buttons.size())
            {
                
                for (Button* button : buttons[currentIndex])
                {
                    button->draw(); // 绘制当前页面上的所有按钮
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
    // 初始化控件，创建图形环境，设置页面和按钮
    void init()
    {
        initgraph(width, height,SHOWCONSOLE);
        IMAGE* page0 = new IMAGE(width, height);//可以直接用loadimage()函数加载图片
        setfillcolor(RGB(240, 240, 240)); // 设置页面1的背景颜色为浅灰色
        solidrectangle(0, 0, width, height); // 绘制页面1的背景矩形
        getimage(page0, 0, 0, width, height); // 将页面1的内容保存到图片中       
        Button* button0_1 = new Button(300, 325,200 , 50, L"自定义模式", [&]() {
            setCurrentIndex(1);
            });
        Button* button0_2 = new Button(300, 100, 200, 50, L"简单模式", [&]() {
            //setCurrentIndex(1);
            N = 10;
            M = 10;
            MINE = 10;
            start();
            });
        Button* button0_3 = new Button(300, 175, 200, 50, L"困难模式", [&]() {
            //setCurrentIndex(1);
            N = 16;
            M = 16;
            MINE = 45;
            start();
            });
        Button* button0_4 = new Button(300, 250, 200, 50, L"地狱难度", [&]() {
            int isok = MessageBox(GetHWnd(), "该模式难度过大，是否继续挑战？", "提示", MB_OKCANCEL);
            if (isok == IDOK) {
                N = 30;
                M = 16;
                MINE = 100;
                start();
            }
            //setCurrentIndex(1); 
            });
        Button* button0_5 = new Button(300, 500, 200, 50, L"退出", [&]() {
            exit(1);
            });
        Button* button0_6 = new Button(350, 425, 100, 50, L"游戏说明", [&]() {
            if (!ishit1) {
                int isok = MessageBox(NULL, "玩个扫雷还看说明是吧。", "无奈", MB_OKCANCEL);
                if (isok == IDOK)
                {
                    ishit1 = true;
                    MessageBox(NULL, "扫雷是一种经典的单人益智游戏，玩家需要根据数字提示来揭开地雷，避免踩中地雷。以下是扫雷的规则说明: 1. 游戏开始时，玩家会看到一个由方块组成的网格，其中一些方块下面隐藏有地雷，而其他方块下面则显示数字。                  2. 玩家需要逐个点击方块，如果点击的方块下面是数字，则表示该方块周围有几个地雷；如果点击的方块下面是空白，则表示该方块周围没有地雷。3. 根据数字提示，玩家可以推断出哪些方块下面是地雷，哪些方块是安全的。玩家需要谨慎思考，避免踩中地雷。4. 如果玩家踩中地雷，则游戏结束，玩家失败；如果玩家成功揭开所有不含地雷的方块，则游戏胜利。5. 玩家可以标记疑似地雷的方块，以帮助自己记忆。标记地雷的方式通常是右键点击方块，或者长按方块。6. 扫雷游戏通常有不同的难度等级，包括初级、中级和高级等，玩家可以根据自己的能力选择适合的难度来挑战。希望以上规则说明可以帮助您更好地了解扫雷游戏的玩法和规则。祝您玩得愉快！", "扫雷说明", MB_OKCANCEL);
                }
            }
            else {
                int isok = MessageBox(NULL, "看过了说明还不会是吧。", "无语", MB_OKCANCEL);
                if (isok == IDOK)
                {
                    MessageBox(NULL, "扫雷是一种经典的单人益智游戏，玩家需要根据数字提示来揭开地雷，避免踩中地雷。以下是扫雷的规则说明: 1. 游戏开始时，玩家会看到一个由方块组成的网格，其中一些方块下面隐藏有地雷，而其他方块下面则显示数字。                  2. 玩家需要逐个点击方块，如果点击的方块下面是数字，则表示该方块周围有几个地雷；如果点击的方块下面是空白，则表示该方块周围没有地雷。3. 根据数字提示，玩家可以推断出哪些方块下面是地雷，哪些方块是安全的。玩家需要谨慎思考，避免踩中地雷。4. 如果玩家踩中地雷，则游戏结束，玩家失败；如果玩家成功揭开所有不含地雷的方块，则游戏胜利。5. 玩家可以标记疑似地雷的方块，以帮助自己记忆。标记地雷的方式通常是右键点击方块，或者长按方块。6. 扫雷游戏通常有不同的难度等级，包括初级、中级和高级等，玩家可以根据自己的能力选择适合的难度来挑战。希望以上规则说明可以帮助您更好地了解扫雷游戏的玩法和规则。祝您玩得愉快！", "扫雷说明", MB_OKCANCEL);
                }
            }
            });

        addPage(page0); // 添加页面0
        addButton(0, button0_1); // 将按钮1添加到页面0
        addButton(0, button0_2);
        addButton(0, button0_3);
        addButton(0, button0_4);
        addButton(0, button0_5);
        addButton(0, button0_6);
        // 创建页面1
        IMAGE* page1 = new IMAGE(width, height);//可以直接用loadimage()函数加载图片
        setfillcolor(RGB(240, 240, 240)); // 设置页面1的背景颜色为浅灰色
        solidrectangle(0, 0, width, height); // 绘制页面1的背景矩形
        getimage(page1, 0, 0, width, height); // 将页面1的内容保存到图片中

        
        TextBox* textbox1_1 = new TextBox(300, 100, 120, 30, 10,1);
        TextBox* textbox1_2 = new TextBox(300, 175, 120, 30, 10, 2);
        TextBox* textbox1_3 = new TextBox(300, 250, 120, 30, 10, 3);
        // 在页面1创建按钮1
        Button* button1_1 = new Button(300, 300, 200, 50, L"开始游戏", [&]() {
            //setCurrentIndex(1); // 点击按钮1时，切换到页面2
            start();
            });
        Button* button1_2 = new Button(300, 500, 200, 50, L"返回主菜单", [&]() {
            setCurrentIndex(0); // 点击按钮1时，切换到页面2           
            });
        addPage(page1); // 添加页面1
        addButton(1, button1_1); // 将按钮1添加到页面1
        addButton(1, button1_2); // 将按钮1添加到页面1
        addTextBox(1, textbox1_1);
        addTextBox(1, textbox1_2);
        addTextBox(1, textbox1_3);
        // 建页面2
        IMAGE* page2 = new IMAGE(width, height);
        setfillcolor(LIGHTGRAY); 
        solidrectangle(0, 0, width, height); 
        getimage(page2, 0, 0, width, height); 
        //在页面2创建按钮1
        //Button* button2_1 = new Button(N * SIZE + 200, 50, 100, 50, L"返回到Page 1", [&]() {
           // setCurrentIndex(0); // 点击按钮1时，切换回页面1
          //  });

        addPage(page2);
        //addButton(1, button2_1); // 将按钮1添加到页面2

        setCurrentIndex(0); 
    }

    bool Safe() {
        if (MINE >=N * M)
        {
            int isok = MessageBox(GetHWnd(), "输入有误，已重置行，列和雷数，请保证地图能装下雷", "提示", MB_OK);
            if (isok == IDOK)
            {
                 N = 10;
                 M = 10;
                 MINE = 10;
            }
            return false;
        }
        else if (N >= 31) {
            int isok = MessageBox(GetHWnd(), "列数过大，已重置行，列和雷数，请重新输入", "提示", MB_OK);
            if (isok == IDOK)
            {
                 N = 10;
                 M = 10;
                 MINE = 10;
            }
            return false;
        }
        else if (M >= 25) {
            int isok = MessageBox(GetHWnd(), "行数过大，已重置行，列和雷数,请重新输入", "提示", MB_OK);
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
    // 运行，进入消息循环
    void run1()
    {
        Text();
        //run2();
        ExMessage msg;
        
        BeginBatchDraw(); // 开始批量绘制
        
        
        while (1)
        {
            if (Safe()) {
                if (peekmessage(&msg)) // 检查是否有消息
                {
                    int mouseX = msg.x; // 获取鼠标x坐标
                    int mouseY = msg.y; // 获取鼠标y坐标

                    switch (msg.message)
                    {
                    case WM_LBUTTONDOWN: // 鼠标左键按下事件  
                       
                        mouseClick(mouseX, mouseY); // 处理鼠标点击事件
                        break;
                    case WM_MOUSEMOVE: // 鼠标移动事件
                        mouseMove(mouseX, mouseY); // 处理鼠标移动事件
                        break;
                    case WM_CHAR:
                        keyInput(msg.ch);
                        break;
                    }
                }           
                draw1(); // 绘制当前页面内容   
                FlushBatchDraw();
                if (currentIndex == 1) {
                    outtextxy(275, 100, "行：");
                    outtextxy(275, 175, "列：");
                    outtextxy(260, 250, "雷数：");
                }
                FlushBatchDraw(); // 将缓冲区内容显示在屏幕上
                
                EndBatchDraw();// 结束批量绘制
            } 
            Sleep(2);
        }       
    }
    
    // 关闭
    void Game() {
        //设置随机数
        srand((unsigned)time(NULL));
        //初始化
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                map[i][j] = 0;
            }
        }
        //埋雷
        for (int i = 0; i < MINE;) {
            int r = rand() % N;
            int c = rand() % M;
            if (map[r][c] == 0) { //防止重复
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
        //读入图片
        for (int i = 0; i < 13; i++)
        {
            char file[50] = "";
            sprintf(file, "./image/%d.jpg", i);
            loadimage(&image[i], file, SIZE, SIZE);
        }
        //覆盖格子
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                map[i][j] += 20;
            }
        }
    }
    //绘制游戏界面
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
        //遍历地图
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                //如果当前位置是雷，则设置成显示雷的状态
                if (map[i][j] == 19 ) {
                    map[i][j] -= 20; 
                }
                else if(map[i][j] > 30)
                    map[i][j] -= 40;
            }
        }
        Drawgame(); //绘制游戏界面，显示所有的雷
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
                        Game(); // 重新生成地图直到第一次点击的位置不是雷
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
            int isok = MessageBox(GetHWnd(), "踩到雷了！再来一把？", "提示", MB_OKCANCEL);
            
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
            int isok = MessageBox(GetHWnd(), "你赢了！再来一把？", "提示", MB_OKCANCEL);
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
                int isok = MessageBox(GetHWnd(), "你自爆了！再来一把？", "提示", MB_OKCANCEL);
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
        SetWindowText(h, "扫雷");
    }
    void DrawTimer() {
        current_time = time(nullptr);
        int elapsed_time = current_time - start_time;

        int hours = elapsed_time / 3600;
        int minutes = (elapsed_time % 3600) / 60;
        int seconds = elapsed_time % 60;

        // 绘制计时器文本
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
