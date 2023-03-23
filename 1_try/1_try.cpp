#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <curses.h>

using namespace std;

float check(WINDOW* win, int x, int y, const char* prompt)
{
    float num;//верное число
    bool is_valid = false;//флаг

    while (!is_valid) {
        char* input_str = new char[256];// Выделяем память для массива символов, который будет хранить введенное пользователем значение
        echo(); // Включаем эхо-режим, чтобы введенные символы отображались на экране
        wgetstr(win, input_str); // Считываем строку ввода 
        noecho(); // Выключаем эхо-режим
        try
        {
            num = stof(input_str);// Преобразуем строку в число типа int
            is_valid = true;// Если преобразование прошло успешно, устанавливаем флаг is_valid в значение true
        }
        catch (invalid_argument&)
        {
            // Если не удалось преобразовать в число, выводим ошибку и повторяем ввод
            mvwprintw(win, x, y, "                                      ");
            mvwprintw(win, x, y, prompt);
            wrefresh(win);
        }
    }
    return num;
}

float f(float x, float y, float e)
{
    float z;
    z = ((3 * x) + (x * log10(sqrt(y)))) / (1 + pow(e, x + x));
    return z;
}

int main()
{
    initscr();

    // создаем окно для ввода данных
    WINDOW* inputwin = newwin(13, 40, 12, 10);
    // создаем окно для вывода данных
    WINDOW* outputwin = newwin(13, 40, 12, 70);

    WINDOW* menuwin = newwin(5, 20, 12, 50);
    // выводим рамки окон
    box(inputwin, 0, 0);
    box(outputwin, 0, 0);
    box(menuwin, 0, 0);
    // выводим заголовки окон
    mvwprintw(inputwin, 0, 1, "Input:");
    mvwprintw(outputwin, 0, 1, "Output:");
    mvwprintw(menuwin, 0, 1, "Choose:");
    // делаем окна цветными
    start_color();
    /*
        COLOR_BLACK
        COLOR_RED
        COLOR_GREEN
        COLOR_YELLOW
        COLOR_BLUE
        COLOR_MAGENTA
        COLOR_CYAN
        COLOR_WHITE
        */

    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);

    wbkgd(inputwin, COLOR_PAIR(1));
    wbkgd(outputwin, COLOR_PAIR(2));
    //делаем окна видимыми
    wrefresh(inputwin);
    wrefresh(outputwin);
    wrefresh(menuwin);

    float x_start, x_end, x_step;
    float y_start, y_end, y_step;
    float z;
    const float e = 2.71;

    mvwprintw(inputwin, 1, 1, "Enter first value for X: ");
    wrefresh(inputwin);
    x_start = check(inputwin, 1, 1, "Invalid X start, try again: ");

    mvwprintw(inputwin, 3, 1, "Enter max value for X: ");
    wrefresh(inputwin);
    x_end = check(inputwin, 3, 1, "Invalid X max, try again: ");

    mvwprintw(inputwin, 5, 1, "Enter step for X: ");
    wrefresh(inputwin);
    x_step = check(inputwin, 5, 1, "Invalid X step, try again: ");

    mvwprintw(inputwin, 6, 1, "______________________________________");

    mvwprintw(inputwin, 7, 1, "Enter first value for Y: ");
    wrefresh(inputwin);
    y_start = check(inputwin, 7, 1, "Invalid first Y, try again: ");

    mvwprintw(inputwin, 9, 1, "Enter max value for Y: ");
    wrefresh(inputwin);
    y_end = check(inputwin, 9, 1, "Invalid max Y, try again: ");

    mvwprintw(inputwin, 11, 1, "Enter step for Y: ");
    wrefresh(inputwin);
    y_step = check(inputwin, 11, 1, "Invalid step Y, try again: ");


    vector <float> dataX;
    vector <float> dataY;
    vector <float> dataZ;
    int calculation = 0;
/*
    float xx_end = x_end, xx_step = x_step, xx_start = x_start, yy_end = y_end, yy_start = y_start, yy_step = y_step;
    while (xx_start + xx_step <= xx_end)
    {
        calculation++;
        xx_start += xx_step;
    }
    while (yy_start + yy_step <= yy_end)
    {
        calculation++;
        yy_start += yy_step;
    }
    calculation *= 3;*/
    for (float x = x_start; x <= x_end; x += x_step) {
        // Внутренний цикл по Y
        float y = y_start;
        do
        {
            z = f(x, y, e);
            dataX.push_back(x);
            dataY.push_back(y);
            dataZ.push_back(z);
           
            y += y_step;
            
        } while (y <= y_end);
    }
   
    wrefresh(outputwin);

    wrefresh(menuwin);
    keypad(menuwin, true);

    string choices[3] = { "Next line", "Previous line", "Exit" };
    int choice;
    int highlight = 0;
    bool work = 1;
    int i = 0;
    while (work)
    {
            mvwprintw(outputwin, 1, 1, "Line number: %d", i + 1);
            mvwprintw(outputwin, 3, 1, "X: %f", dataX[i]);
            mvwprintw(outputwin, 5, 1, "Y: %f", dataY[i]);
            if (dataY[i] <= 0)
                mvwprintw(outputwin, 7, 1, "Z: ERROR");
            else
                
                mvwprintw(outputwin, 7, 1, "Z: %f", dataZ[i]);
            wrefresh(outputwin);
            for (int i = 0; i < 3; i++) {
                if (i == highlight)
                    wattron(menuwin, A_REVERSE);
                mvwprintw(menuwin, i + 1, 1, choices[i].c_str());
                wattroff(menuwin, A_REVERSE);
            }
            choice = wgetch(menuwin);
            switch (choice) {
            case KEY_UP:
                highlight--;
                if (highlight == -1)
                    highlight = 0;
                break;
            case KEY_DOWN:
                highlight++;
                if (highlight == 3)
                    highlight = 2;
                break;
            default:
                break;
            }

            bool work = false;

            if (choice == 10 && highlight == 0)
            {
                wclear(outputwin);
                box(outputwin, 0, 0);
                mvwprintw(outputwin, 0, 1, "Output:");
                wrefresh(outputwin);
                if (i+1 >= dataZ.size())
                    mvwprintw(outputwin, 1, 1, "That's all.");
                else
                {
                    i++;
                    mvwprintw(outputwin, 1, 1, "Line number: %d", i + 1);
                    mvwprintw(outputwin, 3, 1, "X: %f", dataX[i]);
                    mvwprintw(outputwin, 5, 1, "Y: %f", dataY[i]);
                    if (dataY[i] <= 0)
                        mvwprintw(outputwin, 7, 1, "Z: ERROR");
                    else
                        mvwprintw(outputwin, 7, 1, "Z: %f", dataZ[i]);
                    wrefresh(menuwin);
                    wrefresh(outputwin);
                }

            }
            if (choice == 10 && highlight == 1)
            {
                wclear(outputwin);
                box(outputwin, 0, 0);
                mvwprintw(outputwin, 0, 1, "Output:");
                wrefresh(outputwin);
                if (i - 1 < 0)
                    mvwprintw(outputwin, 1, 1, "That's all.");
                else
                {
                    i--;
                    mvwprintw(outputwin, 1, 1, "Line number: %d", i + 1);
                    mvwprintw(outputwin, 3, 1, "X: %f", dataX[i]);
                    mvwprintw(outputwin, 5, 1, "Y: %f", dataY[i]);
                    if (dataY[i] <= 0)
                        mvwprintw(outputwin, 7, 1, "Z: ERROR");
                    else
                        mvwprintw(outputwin, 7, 1, "Z: %f", dataZ[i]);
                    wrefresh(menuwin);
                    wrefresh(outputwin);
                }
            }
            if (choice == 10 && highlight == 2)
            {
                wclear(outputwin);
                box(outputwin, 0, 0);
                mvwprintw(outputwin, 0, 1, "Output:");
                mvwprintw(outputwin, 1, 1, "Have a good day!");
                wrefresh(outputwin);
                work = 0;
                break;
            }
    }
    wgetch(menuwin);

    endwin();
    return 0;
}