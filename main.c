#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

//========声明区=========//
/** 自定义变量类型 **/
typedef enum loginResu {success = 1, fail = 0};// 登录结果类型
typedef enum readType{accountType = 0x0520, passwordType = 0x0521};// 信息读取类型（账户/密码）

/** 变量申明 **/
enum loginResu loginFlag = fail;// 记录登录是否成功

#define _MAX_USERNAME 20
#define _MAX_PASSWORD 16
char username[_MAX_USERNAME] = {0};// 用户输入的用户名
char password[_MAX_PASSWORD] = {0};// 用户输入的密码

#define ID_NONE_WND 0x0100
#define ID_GAMEINDEX_WND 0x0123
#define ID_EXPLAINRULE_WND 0x0124
#define ID_STARTGAME_WND 0x0125
#define ID_REVISEPASSWORD_WND 0x0126
unsigned int wndCode = ID_NONE_WND;

char magicFigure[12][12] = 
{
	"·﹏·  ",
	"$ _ $   ",
	"(/▽＼) ",
	"(O_o)   ",
	"(- o -) ",
	"(≧▽≦)",
	"(。·o·)",
	"X﹏X    ",
	"(＞﹏＜)",
	"(*'-')  ",
	"(ˉBˉ) ",
	"(－v＝) "
};

/** 逻辑函数声明 **/
enum loginResu login(const char* username, const char* password);// 登录验证函数，返回值是登录结果（成功/失败）
int gameIndex();// 游戏引导界面
void startGame();// 开始游戏
void explainRule();// 说明规则
void revisePassword();// 修改密码
void reviseUsername();// 修改昵称

/** 辅助函数声明 **/
void gotoxy(int x, int y)// 控制台光标控制函数
{
	COORD pos = { x,y };// 表示 pos 坐标位置为参数对应的(x，y)
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);// 创建一个hOut句柄来接收标准输出句柄
	SetConsoleCursorPosition(hOut, pos);// 该函数设置控制台(cmd)光标位置
}
void keyListening();// 键盘事件监听
//=======================//



int main(int argc, int *argv[])
{
START_HERE:
	// 登录主界面
	system("echo off && cls && (mode con:cols=32 lines=25) && (color 09) && (title Games登录)");
	printf("*************登  录*************\n\n");
	
	printf("账号:");
	scanf("%s", &username);// 输入账号
	putchar('\n');
	
	printf("密码:");
	scanf("%s", &password);// 输入密码
	printf("********************************");


	loginFlag = login(username, password);// 验证登录，并将登陆结果储存在 loginFlag 变量中
	if (loginFlag)
	{
		// 登录成功
		gameIndex(); keyListening();
	}
	else
	{
		// 登陆失败
		goto START_HERE;
	}

	//gameIndex(); keyListening();

	system("pause");
	return 0;
}

enum loginResu login(const char* username, const char* password)
{
	
	FILE *accFile;// 账户信息文件指针变量
	char usernameBuf[_MAX_USERNAME];// 储存账户信息文件中账户名
	char passwordBuf[_MAX_PASSWORD];// 储存账户信息文件中的密码
	char textBuf[1024];// 字符串缓冲区
	enum readType read = accountType;// 声明读取信息类型

	accFile = fopen("accountData.dat", "r");// 打开账户信息文件
	if(accFile == NULL)
	{
		// 储存账户信息的文件丢失
		perror("账户信息文件丢失！\n");// 错误输出
		exit(1);// 结束程序
	}
	else
	{
		// 成功读取账户文件信息
		while(fgets(textBuf, sizeof(textBuf), accFile))
		{
			(textBuf[strlen(textBuf)-1] == '\n')? (textBuf[strlen(textBuf)-1] = '\0') : (textBuf[strlen(textBuf)-1] = textBuf[strlen(textBuf)-1]);// 去掉结尾的换行符'\n'

			switch(read)
			{
			case accountType:// 读取账户类型的信息
				strcpy(usernameBuf, textBuf);
				read = passwordType;// 切换读取类型
				break;
			case passwordType:// 读取密码类型的信息
				strcpy(passwordBuf, textBuf);
				read = accountType;// 切换读取类型
				break;
			default:
				read = accountType;// 发生未知错误时，默认调回账户类型信息的读取
			}
		}
	}
	fclose(accFile);// 关闭文件指针

	// 验证登录信息
	if(stricmp(username, usernameBuf) == 0)
	{
		// 账户存在，验证密码是否正确
		if(stricmp(password, passwordBuf) == 0)
		{
			// 账号和密码均正确，返回登录成功的结果
			return success;
		}
		else
		{
			// 账号正确但是密码错误
			int i;
			system("cls && color 0C");
			for(i=0;i<32;i++){putchar('*');}
			printf("密码错误！\n请检查密码是否正确并重新登录！\n");
			for(i=0;i<32;i++){putchar('*');}
			system("pause 1>nul");
		}
	}
	else
	{
		// 账户不存在
		int i;
		system("cls && color 0C");
		for(i=0;i<32;i++){putchar('*');}
		printf("账户不存在!\n请检查账户名称拼写是否正确并重新登录！\n提示:账户名不区分大小写！\n");
		for(i=0;i<32;i++){putchar('*');}
		system("pause 1>nul");// 暂停显示信息
	}

	return fail;// 默认登录失败
}

int gameIndex()
{
	int i = 0;
	wndCode = ID_GAMEINDEX_WND;
	// 游戏引导界面
	system("cls && (mode con:cols=53 lines=25) && (color 0E) && (title MindReading)");
	for (i=0;i<52;i++)putchar('*');
	putchar('\n');
	printf("\n"
		"1. \033[1;37;44m开始游戏 (S)\t\033[1;33;40m\n\n"
		"2. 游戏规则 (R)\n\n"
		"3. 修改密码 (P)\n\n"
		"4. 修改昵称 (N)\n\n"
		"5. 退出游戏 (ESC)\n\n"
		);
	for (i=0;i<52;i++)putchar('*');
	putchar('\n');
	printf(
			"              ＿＿\n"
			"          ／＞　　 フ\n"
			"          |    _　_ |    回车确定哦~\n"
			"　 　　　／` ミ＿xノ\n"
			"　　 　 /　　　 　 |\n"
			"　　　 /　 ヽ　　 /\n"
			"　 　 │　　 |　|　|\n"
			"　／￣|　　 |　|　|\n"
			"　| (￣ヽ＿_ヽ_)__)\n"
			"　＼二つ\n"
	);
	return 0;
}

void keyListening()
{
	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);      //获得标准输入设备句柄
    INPUT_RECORD keyrec;        //定义输入事件结构体
    DWORD res;      //定义返回记录
	int line = 2; // 默认按键行

    while(1)
    {
        ReadConsoleInput(handle_in, &keyrec, 1, &res);      //读取输入事件
        if (keyrec.EventType == KEY_EVENT)      //如果当前事件是键盘事件
        {
			if(wndCode == ID_GAMEINDEX_WND)
			{
				gotoxy(3, line);// 原位置恢复原色（未选择状态）
				switch(line)
				{
				case 2:
					printf("\033[1;33;40m开始游戏 (S)\033[1;33;40m");
					break;
				case 4:
					printf("\033[1;33;40m游戏规则 (R)\033[1;33;40m");
					break;
				case 6:
					printf("\033[1;33;40m修改密码 (P)\033[1;33;40m");
					break;
				case 8:
					printf("\033[1;33;40m修改昵称 (N)\033[1;33;40m");
					break;
				case 10:
					printf("\033[1;33;40m退出游戏 (ESC)\033[1;33;40m");
					break;
				default:{}
				}
			}

            if (keyrec.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && keyrec.Event.KeyEvent.bKeyDown == 1) //当前事件的虚拟键为Esc键
            {
                exit(0);
            }
			else if (keyrec.Event.KeyEvent.wVirtualKeyCode == VK_UP && wndCode == ID_GAMEINDEX_WND && keyrec.Event.KeyEvent.bKeyDown == 1)// 上键
			{
				(line>2)?(line-=2):(line=2);
			}
			else if (keyrec.Event.KeyEvent.wVirtualKeyCode == VK_DOWN && wndCode == ID_GAMEINDEX_WND && keyrec.Event.KeyEvent.bKeyDown == 1)// 下键
			{
				(line<10)?(line+=2):(line=10);
			}
			else if (keyrec.Event.KeyEvent.wVirtualKeyCode == 'S' && wndCode == ID_GAMEINDEX_WND && keyrec.Event.KeyEvent.bKeyDown == 1)// 字母 S 键
			{
				line = 2;
			}
			else if (keyrec.Event.KeyEvent.wVirtualKeyCode == 'R' && wndCode == ID_GAMEINDEX_WND && keyrec.Event.KeyEvent.bKeyDown == 1)// 字母 R 键
			{
				line = 4;
			}
			else if (keyrec.Event.KeyEvent.wVirtualKeyCode == 'P' && wndCode == ID_GAMEINDEX_WND && keyrec.Event.KeyEvent.bKeyDown == 1)// 字母 P 键
			{
				line = 6;
			}
			else if (keyrec.Event.KeyEvent.wVirtualKeyCode == 'N' && wndCode == ID_GAMEINDEX_WND && keyrec.Event.KeyEvent.bKeyDown == 1)// 字母 N 键
			{
				line = 8;
			}
			else if (keyrec.Event.KeyEvent.wVirtualKeyCode == 'E' && wndCode == ID_GAMEINDEX_WND && keyrec.Event.KeyEvent.bKeyDown == 1)// 字母 E 键
			{
				line = 10;
			}
			else if (keyrec.Event.KeyEvent.wVirtualKeyCode == VK_RETURN && wndCode == ID_GAMEINDEX_WND && keyrec.Event.KeyEvent.bKeyDown == 1)// 回车键
			{
				switch(line)
				{
				case 2:
					startGame();
					break;
				case 4:
					explainRule();
					break;
				case 6:
					revisePassword();
					break;
				case 8:
					//reviseUsername();
					break;
				case 10:
					exit(0);
					break;
				default:{}
				}
			}
			else
			{}
			
			if (wndCode == ID_GAMEINDEX_WND)
			{
				gotoxy(3, line);// 目标位置修改颜色（选中状态）
				switch(line)
				{
				case 2:
					printf("\033[1;37;44m开始游戏 (S)\033[1;33;40m");
					break;
				case 4:
					printf("\033[1;37;44m游戏规则 (R)\033[1;33;40m");
					break;
				case 6:
					printf("\033[1;37;44m修改密码 (P)\033[1;33;40m");
					break;
				case 8:
					printf("\033[1;37;44m修改昵称 (N)\033[1;33;40m");
					break;
				case 10:
					printf("\033[1;37;44m退出游戏 (ESC)\033[1;33;40m");
					break;
				default:{}
				}
			}

			if(wndCode != ID_GAMEINDEX_WND)// 非常重要的一个归位
			{
				gameIndex();
				line = 2;
			}
        }
    }
}

void startGame()
{
	int randomNums[16] = {0};
	int i = 0, randIndex = 0, yourThoughts = -1;
	wndCode = ID_STARTGAME_WND;

PAGE_ONE:
	system("cls && (mode con:cols=50 lines=34) && (color 0F) && (title MindReading)");
	printf("欢迎来到读心术的世界！\n\n这里你将体验到不可思议的读心术哦~\n\n");
	printf("\033[0;32;47m"
"                                                  \n"
"                &                &                \n"
"                &&              &                 \n"
"                 &&&&&&&&&&&&&&&                  \n"
"               &&&&&&&&&&&&&&&&&&&                \n"
"             &&&&   &&&&&&&&&   &&&&              \n"
"            &&&&&   &&&&&&&&&   &&&&&             \n"
"           &&&&&&&&&&&&&&&&&&&&&&&&&&&            \n"
"           &&&&&&&&&&&&&&&&&&&&&&&&&&&            \n"
"                                                  \n"
"   &&&&&   &&&&&&&&&&&&&&&&&&&&&&&&&&&&   &&&&    \n"
"   &&&&&   &&&&&&&&&&&&&&&&&&&&&&&&&&&&  &&&&&&   \n"
"   &&&&&   &&&&&&&&&&&&&&&&&&&&&&&&&&&&  &&&&&&   \n"
"   &&&&&   &&&&&&&&&&&&&&&&&&&&&&&&&&&&  &&&&&&   \n"
"   &&&&&   &&&&&&&&&&&&&&&&&&&&&&&&&&&&  &&&&&&   \n"
"   &&&&&   &&&&&&&&&&&&&&&&&&&&&&&&&&&&  &&&&&&   \n"
"   &&&&&   &&&&&&&&&&&&&&&&&&&&&&&&&&&&  &&&&&&   \n"
"   &&&&&&  &&&&&&&&&&&&&&&&&&&&&&&&&&&&  &&&&&&   \n"
"   &&&&&   &&&&&&&&&&&&&&&&&&&&&&&&&&&&  &&&&&&   \n"
"    &&&    &&&&&&&&&&&&&&&&&&&&&&&&&&&&   &&&     \n"
"           &&&&&&&&&&&&&&&&&&&&&&&&&&&&           \n"
"           &&&&&&&&&&&&&&&&&&&&&&&&&&&&           \n"
"           &&&&&&&&&&&&&&&&&&&&&&&&&&&            \n"
"                &&&&&&      &&&&&                 \n"
"                &&&&&&      &&&&&                 \n"
"                &&&&&&      &&&&&                 \n"
"                &&&&&&      &&&&&                 \n"
"                &&&&&&      &&&&&                 \n"
"\n"
"\033[1;33;40m[任意键继续]\033[1;37;40m");
	system("pause 1>nul");

PAGE_TWO:
	system("cls");
	printf("请从下面 16 个数中任选一个数:\n\n\n");
	for (i=0; i<16 ; i++)
	{
		randomNums[i] = rand()%(99-10+1)+10;// 生成 10~99 的随机数
	}
	printf(// 看似很傻的操作，实际上降低了时间复杂度，属于优化代码
		"\033[1;37;42m[%d]\t[%d]\t[%d]\t[%d]\n\n\n"
		"[%d]\t[%d]\t[%d]\t[%d]\n\n\n"
		"[%d]\t[%d]\t[%d]\t[%d]\n\n\n"
		"[%d]\t[%d]\t[%d]\t[%d]\033[1;37;40m\n\n\n"
		"\033[0;31;40m将选中的数分别减去他的\033[1;31;40m个位数\033[0;31;40m和\033[1;31;40m十位数\033[1;37;40m，\n\n"
		"例如：71-7-1=63。\n\n"
		"一定要记清楚这个数字哦~\n\no(^▽^)o",
		randomNums[0], randomNums[1], randomNums[2], randomNums[3],
		randomNums[4], randomNums[5], randomNums[6], randomNums[7],
		randomNums[8], randomNums[9], randomNums[10], randomNums[11],
		randomNums[12], randomNums[13], randomNums[14], randomNums[15]
	);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\033[1;33;40m[任意键继续/R 键重新生成随机数]\033[1;37;40m");
	system("pause 1>nul");

PAGE_THREE:
	system("cls");
	printf("请在魔法图中找到计算结果对应图案，并牢记它的样子：\n");
	for (i=0; i<93; i++)
	{
		randIndex = rand()%(11-0+1)+0;// 生成 0~11 的随机数
		if (i==0) { yourThoughts = randIndex; }

		if(i%9 == 0)
		{
			printf("\033[1;36;40m%d\033[1;37;40m %s", i, magicFigure[yourThoughts]);
		}
		else
		{
			printf("\033[1;36;40m%d\033[1;37;40m %s", i, magicFigure[randIndex]);
		}

		if((i+1)%3 == 0){ putchar('\n'); } else { putchar('\t'); }
	}
	printf("\n\033[1;33;40m[任意键开始读心术]\033[1;37;40m");
	system("pause 1>nul");

PAGE_FOUR:
	system("cls");
	printf("哈哈哈哈哈，你看到的一定是：%s 对吧！", magicFigure[yourThoughts]);
	system("pause 1>nul");
}

void explainRule()
{
	wndCode = ID_EXPLAINRULE_WND;
	system("cls && (mode con:cols=40 lines=20) && (color 0B) && (title 游戏规则)");
	printf(
		"游戏规则如下：\n\n"
		"  首先系统会先生成 16 个随机数，你可以在心中\033[1;31;40m任意选择\033[1;36;40m其中一个两位数（10~99），把这个数字分别减去其\033[1;31;40m个位数\033[1;36;40m和\033[1;31;40m十位数\033[1;36;40m（例如你选择71，那就：71-7-1=63）。\n\n"
		"  记住计算所得数，然后\033[1;31;40m在魔法图中找到对应的图案\033[1;36;40m，最后牢牢记住这个图案！最后，我将神奇地将你心中想的那个图案显示出来！\n\n"
		);
	printf("\033[1;33;40m[任意键返回]\033[1;36;40m");
	system("pause 1>nul");
}

void revisePassword()
{
	FILE *accFile;
	char newPassword[_MAX_PASSWORD] = {0}, oldPassword[_MAX_PASSWORD] = {0};
	
	wndCode = ID_REVISEPASSWORD_WND;
	system("cls && (mode con:cols=40 lines=20) && (color 0B) && (title 修改密码)");
OLD_PASSWORD_INPUT:
	printf("请输入旧密码：");
	scanf("%s", oldPassword);
	


NEW_PASSWORD_INPUT_1:
	printf("\n\n请输入新密码:");
	scanf("%s", newPassword);
NEW_PASSWORD_INPUT_2:
	printf("\n\n请输入新密码:");
	scanf("%s", newPassword);
	system("pause 1>nul");
}