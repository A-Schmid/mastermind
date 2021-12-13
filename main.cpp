#include <stdio.h>
#include <windows.h>
#include "commctrl.h"
#include "uxtheme.h"
#include <time.h>

const char g_szClassName[] = "MasterMind";
HINSTANCE g_hinst;

const UINT idTimer1 = 1;
UINT nTimerDelay = 0;

int i;
int j;

int level;
int solution[4];
int choice[4];

int numBlack;
int numWhite;

char *complaint;

int msgbox1;



#define BTN_OK				100
#define BTN_QUIT			101
#define CO_COLOR_0          200
#define CO_COLOR_1          201
#define CO_COLOR_2          202
#define CO_COLOR_3          203

HWND CON_BTN_OK;
HWND CON_BTN_QUIT;
HWND CON_CO_COLOR[4];

HFONT hFont;

TCHAR *color[] =
{
    TEXT("Schwarz"),
    TEXT("Weiß"),
    TEXT("Rot"),
    TEXT("Blau"),
    TEXT("Grün"),
    TEXT("Gelb"),
    TEXT("Lila"),
    TEXT("Orange")    
};

HBRUSH backgroundBrush = CreateSolidBrush(0x0099AACC);
HBRUSH greyBrush = CreateSolidBrush(0x00CCCCCC);

HBRUSH brush[] = 
{
    CreateSolidBrush(0x00000000),
    CreateSolidBrush(0x00FFFFFF),
    CreateSolidBrush(0x000000FF),
    CreateSolidBrush(0x00FF0000),
    CreateSolidBrush(0x0000FF00),
    CreateSolidBrush(0x0000FFFF),
    CreateSolidBrush(0x00FF00CC),
    CreateSolidBrush(0x000099FF)
};

RECT colorfield[10][4];
RECT checkfield[10][4];

void CreateTooltip (HWND hwnd, char *tooltip)
{
    HWND hwndTT;                

    TOOLINFO ti;
    RECT rect;

    hwndTT = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
        WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,		
        0, 0, 0, 0, hwnd, NULL, NULL, NULL );

    SetWindowPos(hwndTT, HWND_TOPMOST, 0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
   
    GetClientRect (hwnd, &rect);

    ti.cbSize = sizeof(TOOLINFO);
    ti.uFlags = TTF_SUBCLASS;
    ti.hwnd = hwnd;
    ti.hinst = NULL;
    ti.uId = 0;
    ti.lpszText = tooltip;
    ti.rect.left = rect.left;    
    ti.rect.top = rect.top;
    ti.rect.right = rect.right;
    ti.rect.bottom = rect.bottom;

    SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);	
}

void CreateRects(HDC hdc, HWND hwnd)
{
    for(i = 0; i < 10; i++)
    {
        for(j = 0; j < 4; j++)
        {
            colorfield[i][j].left = 165 + (j * 120);
            colorfield[i][j].right = 165 + (j * 120) + 30;
                    
            colorfield[i][j].top = 430 - (i * 40);
            colorfield[i][j].bottom = 430 - (i * 40) + 30;
                    
            FillRect(hdc, &colorfield[i][j], greyBrush);
            
            
            checkfield[i][j].left = 35 + (j * 20);
            checkfield[i][j].right = 35 + (j * 20) + 10;
                    
            checkfield[i][j].top = 440 - (i * 40);
            checkfield[i][j].bottom = 440 - (i * 40) + 10;
                    
            FillRect(hdc, &checkfield[i][j], greyBrush);
        }
    }
}

char *intToString(int num) 
{
	int i=0;
	int j=0;
	int k=0;
	int ones=0;
	char temp[5];
	char ans[5];
	while (num!=0) 
	{
		ones=num%10;
		temp[i]=(char)(ones+48);
		num=num/10;
		i++;
	}
	for(j=i-1;j>=0;j--) 
	{
		ans[k]=temp[j];
		k++;
	}
	ans[i]='\0';
	return (char *)ans;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case BTN_OK:
                    HDC hdc;
                    hdc = GetDC(hwnd);
                    
                    numBlack = 0;
                    numWhite = 0;
                    
                    choice[0] = SendMessage(CON_CO_COLOR[0], CB_GETCURSEL, 0, 0);
                    choice[1] = SendMessage(CON_CO_COLOR[1], CB_GETCURSEL, 0, 0);
                    choice[2] = SendMessage(CON_CO_COLOR[2], CB_GETCURSEL, 0, 0);
                    choice[3] = SendMessage(CON_CO_COLOR[3], CB_GETCURSEL, 0, 0);
                    
                    FillRect(hdc, &colorfield[level][0], brush[choice[0]]);
                    FillRect(hdc, &colorfield[level][1], brush[choice[1]]);
                    FillRect(hdc, &colorfield[level][2], brush[choice[2]]);
                    FillRect(hdc, &colorfield[level][3], brush[choice[3]]);
                    
                    if(choice[0] == solution[0])
                    {
                        numBlack++;
                    }
                    if(choice[1] == solution[1])
                    {
                        numBlack++;
                    }
                    if(choice[2] == solution[2])
                    {
                        numBlack++;
                    }
                    if(choice[3] == solution[3])
                    {
                        numBlack++;
                    }
                    
                    if( choice[0] == solution[1] || 
                        choice[0] == solution[2] || 
                        choice[0] == solution[3])
                    {
                        numWhite++;                        
                    }
                    if( choice[1] == solution[0] || 
                        choice[1] == solution[2] || 
                        choice[1] == solution[3])
                    {
                        numWhite++;                        
                    }
                    if( choice[2] == solution[0] || 
                        choice[2] == solution[1] || 
                        choice[2] == solution[3])
                    {
                        numWhite++;                        
                    }
                    if( choice[3] == solution[0] || 
                        choice[3] == solution[1] || 
                        choice[3] == solution[2] )
                    {
                        numWhite++;                        
                    }
                    
                    for(i = 0; i < numBlack; i++)
                    {
                        FillRect(hdc, &checkfield[level][i], brush[0]);
                    }
                    
                    for(i = 0; i < numWhite; i++)
                    {
                        FillRect(hdc, &checkfield[level][i + numBlack], brush[1]);
                    }
                    
                    
                    level++;
                    
                    if(level >= 10)
                    {
                        MessageBox(hwnd, "Du hast Verloren!", "Verloren!", MB_OK);
                    }
                    if(numBlack == 4)
                    {
                        MessageBox(hwnd, "Du hast Gewonnen!", "Gewonnen!", MB_OK);
                    }
                    ReleaseDC(hwnd, hdc);
					break;
				case BTN_QUIT:
					PostQuitMessage(0);
					break;
			}
			break;
		case WM_CREATE:
             //----- Font ------
             hFont = CreateFont(18,
             0,
             0,
             0,
             0,
             0,
             0,
             0,
             ANSI_CHARSET,
             OUT_DEFAULT_PRECIS,
             CLIP_DEFAULT_PRECIS,
             DEFAULT_QUALITY,
             DEFAULT_PITCH | FF_DONTCARE,
             "Segoe UI");
             
             //----- Buttons ------
			 CON_BTN_OK = CreateWindow(TEXT("button"), TEXT("Ok"),
		             WS_VISIBLE | WS_CHILD,
		             10, 530, 80, 25,        
		             hwnd, (HMENU) BTN_OK, NULL, NULL);
		             

			 CON_BTN_QUIT = CreateWindow(TEXT("button"), TEXT("Verlassen"),    
		             WS_VISIBLE | WS_CHILD ,
		             100, 530, 80, 25,        
		             hwnd, (HMENU) BTN_QUIT, NULL, NULL);
		             
		    //----- Comboboxes ------
		    CON_CO_COLOR[0] = CreateWindow(TEXT("combobox"), NULL, 
								WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL,
								100, 480, 100, 250, hwnd, (HMENU) CO_COLOR_0, g_hinst, NULL);
								
			CON_CO_COLOR[1] = CreateWindow(TEXT("combobox"), NULL, 
								WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL,
								220, 480, 100, 250, hwnd, (HMENU) CO_COLOR_1, g_hinst, NULL);
								
			CON_CO_COLOR[2] = CreateWindow(TEXT("combobox"), NULL, 
								WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL,
								340, 480, 100, 250, hwnd, (HMENU) CO_COLOR_2, g_hinst, NULL);
								
			CON_CO_COLOR[3] = CreateWindow(TEXT("combobox"), NULL, 
								WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL,
								460, 480, 100, 250, hwnd, (HMENU) CO_COLOR_3, g_hinst, NULL);
								
			//----- Combobox-Content ------
			for(i = 0; i < 4; i++)
			{
			    for(j = 0; j < 8; j++)
			    {
			        SendMessage(CON_CO_COLOR[i], CB_ADDSTRING, 0, (LPARAM) color[j]);
			    }
            }
            
            //----------------            
            
            srand( (unsigned)time( NULL ) );
            solution[0] = rand() % 7;
            do
            {
                solution[1] = rand() % 7;
            } while(solution[1] == solution[0]);
            do
            {
                solution[2] = rand() % 7;
            } while(solution[2] == solution[0] || solution[2] == solution[1]);
            do
            {
                solution[3] = rand() % 7;
            } while(solution[3] == solution[0] || solution[3] == solution[1] || solution[3] == solution[2]);            

                       
			break;
		case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdcWindow;
            hdcWindow = BeginPaint(hwnd, &ps);

            CreateRects(hdcWindow, hwnd);
                          
            EndPaint(hwnd, &ps);
            break;
        case WM_CLOSE:
            DeleteObject(hFont);
            DestroyWindow(hwnd);
			break;
        case WM_DESTROY:
            PostQuitMessage(0);
			break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(hInstance, NULL);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = backgroundBrush;
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(hInstance, NULL);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

	g_hinst = hInstance;

    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "Mastermind",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 630, 600,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
