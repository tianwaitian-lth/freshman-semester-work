#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <io.h>
#include <stdbool.h>
#include <windows.h>

// 颜色宏定义
#define HIGHLIGHT_RED "\033[1;31m"
#define HIGHLIGHT_GREEN "\033[1;32m"
#define HIGHLIGHT_YELLOW "\033[1;33m"
#define HIGHLIGHT_BLUE "\033[1;34m"
#define HIGHLIGHT_PURPLE "\033[1;35m"
#define RESET "\033[0m"

// 分隔符宏定义
#define DIVIDER_LINE "==========================================================\n"
#define SUB_DIVIDER "----------------------------------------------------------\n"
#define MENU_DIVIDER "----------------------------------------------------------\n"

int reloadFileByReopen(const char *filename, const char *mode, FILE **fp) {
    // 先关闭原文件
    if (*fp != NULL) {
        fclose(*fp);
        *fp = NULL; // 避免野指针
    }
    // 重新打开文件
    *fp = fopen(filename, mode);
    if (*fp == NULL) {
        perror("文件重新打开失败");
        return -1; // 返回错误码
    }
    return 0;
}
typedef struct {
    int lineCount;
    int charCount;
    int num;
    int chara;
    long long size;
    char lastmodtime[40];
} fileinfo;

#define BUFFER_SIZE 1024

void choosefile(int, char*, FILE**, FILE*, FILE*);
void printfile(FILE*);
void writefile(FILE*);
void clearfile(FILE*);
void englishfile(FILE*, const char*);
void comparefile(FILE*, FILE*);
fileinfo analyzefile(FILE*, const char*);

int main() {
    // 设置Windows控制台编码为UTF-8
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    // 打开文件
    FILE* file1, * file2;
    file1 = fopen("test1.txt", "r+");
    file2 = fopen("test2.txt", "r+");
    
    printf(DIVIDER_LINE);
    if (file1 == NULL || file2 == NULL) {
        printf(HIGHLIGHT_RED "文件打开失败！\n" RESET);
        perror("");
        printf(DIVIDER_LINE);
        return 1;
    }
    printf(HIGHLIGHT_GREEN "✓ 文件打开成功！\n" RESET);
    printf(DIVIDER_LINE);
    
    // 主要功能
    while (1) {
        printf(HIGHLIGHT_BLUE "文件管理系统 - 功能菜单\n" RESET);
        printf(MENU_DIVIDER);
        printf("%s0.%s 退出系统\n", HIGHLIGHT_YELLOW, RESET);
        printf("%s1.%s 读取test1.txt文件内容\n", HIGHLIGHT_YELLOW, RESET);
        printf("%s2.%s 读取test2.txt文件内容\n", HIGHLIGHT_YELLOW, RESET);
        printf("%s3.%s 重写test1.txt文件内容\n", HIGHLIGHT_YELLOW, RESET);
        printf("%s4.%s 重写test2.txt文件内容\n", HIGHLIGHT_YELLOW, RESET);
        printf("%s5.%s 分析文件内容\n", HIGHLIGHT_YELLOW, RESET);
        printf("%s6.%s 英文文件分析\n", HIGHLIGHT_YELLOW, RESET);
        printf("%s7.%s 比较2个文件内容\n", HIGHLIGHT_YELLOW, RESET);
        printf(MENU_DIVIDER);
        
        printf(HIGHLIGHT_BLUE "请输入你的选择：(0-7): " RESET);
        int choice;
        scanf("%d", &choice);
        
        printf(DIVIDER_LINE);
        
        fileinfo result;  // 移到switch开头声明
        switch (choice) {
            case 0:
                // 关闭文件
                fclose(file1);
                fclose(file2);
                printf(HIGHLIGHT_GREEN "✓ 成功退出系统！\n" RESET);
                printf(DIVIDER_LINE);
                return 0;
                
            case 1:
                printf(HIGHLIGHT_BLUE "test1.txt 文件内容：\n" RESET);
                printf(SUB_DIVIDER);
                printfile(file1);
                printf(SUB_DIVIDER);
                break;
                
            case 2:
                printf(HIGHLIGHT_BLUE "test2.txt 文件内容：\n" RESET);
                printf(SUB_DIVIDER);
                printfile(file2);
                printf(SUB_DIVIDER);
                break;
                
            case 3:
                printf(HIGHLIGHT_BLUE "重写 test1.txt 文件内容\n" RESET);
                printf(SUB_DIVIDER);
                writefile(file1);
                printf(SUB_DIVIDER);
                break;
                
            case 4:
                printf(HIGHLIGHT_BLUE "重写 test2.txt 文件内容\n" RESET);
                printf(SUB_DIVIDER);
                writefile(file2);
                printf(SUB_DIVIDER);
                break;
                
            case 5: {
                printf(HIGHLIGHT_BLUE "文件分析功能\n" RESET);
                printf(SUB_DIVIDER);
                printf("请选择要分析的文件：%s(1)test1.txt %s或 %s(2)test2.txt%s: ", 
                       HIGHLIGHT_YELLOW, RESET, HIGHLIGHT_YELLOW, RESET);
                int i = 0;
                scanf("%d", &i);
                char testname[20];
                FILE* targetFile;
                choosefile(i, testname, &targetFile, file1, file2);
                
                printf("\n正在分析 %s%s%s 文件...\n", HIGHLIGHT_YELLOW, testname, RESET);
                result = analyzefile(targetFile, testname);
                
                printf(SUB_DIVIDER);
                printf(HIGHLIGHT_BLUE "文件分析结果：\n" RESET);
                printf("行数：%s%d%s\n", HIGHLIGHT_GREEN, result.lineCount, RESET);
                printf("字符数：%s%d%s\n", HIGHLIGHT_GREEN, result.charCount, RESET);
                printf("数字数：%s%d%s\n", HIGHLIGHT_GREEN, result.num, RESET);
                printf("英文字符数：%s%d%s\n", HIGHLIGHT_GREEN, result.chara, RESET);
                printf("文件大小: %s%lld%s 字节\n", HIGHLIGHT_GREEN, result.size, RESET);
                printf("最后修改时间: %s%s%s\n", HIGHLIGHT_GREEN, result.lastmodtime, RESET);
                printf(SUB_DIVIDER);
                break;
            }
                
            case 6: {
                printf(HIGHLIGHT_BLUE "英文文件分析功能\n" RESET);
                printf(SUB_DIVIDER);
                printf("请选择要分析的文件：%s(1)test1.txt %s或 %s(2)test2.txt%s: ", 
                       HIGHLIGHT_YELLOW, RESET, HIGHLIGHT_YELLOW, RESET);
                int i = 0;
                scanf("%d", &i);
                char testname[20];
                FILE* targetFile;
                choosefile(i, testname, &targetFile, file1, file2);
                
                printf("\n正在分析 %s%s%s 文件...\n", HIGHLIGHT_YELLOW, testname, RESET);
                printf(SUB_DIVIDER);
                englishfile(targetFile, testname);
                printf(SUB_DIVIDER);
                break;
            }
                
            case 7:
                printf(HIGHLIGHT_BLUE "文件比较功能\n" RESET);
                printf(SUB_DIVIDER);
                comparefile(file1, file2);
                printf(SUB_DIVIDER);
                break;
                
            default:
                printf(HIGHLIGHT_RED "输入错误！请输入0-7之间的数字。\n" RESET);
                break;
        }
        
        // 添加功能完成提示和分隔符
        printf(DIVIDER_LINE);
        printf("功能执行完成！\n");
        printf(DIVIDER_LINE);
    }
    
    return 0;
}

void choosefile(int i, char* testname, FILE** targetFile, FILE* file1, FILE* file2) {
    if (i == 1) {
        strcpy(testname, "test1.txt");
        *targetFile = file1;
        printf("已选择：%s%s%s\n", HIGHLIGHT_YELLOW, testname, RESET);
    }
    else if (i == 2) {
        strcpy(testname, "test2.txt");
        *targetFile = file2;
        printf("已选择：%s%s%s\n", HIGHLIGHT_YELLOW, testname, RESET);
    }
    else {
        printf("%s输入错误！默认选择test1.txt\n" RESET, HIGHLIGHT_RED);
        strcpy(testname, "test1.txt");
        *targetFile = file1;
    }
    return;
}

void printfile(FILE* file) {
    char lineBuffer[BUFFER_SIZE];
    int lineNum = 1;
    rewind(file);
    
    while (fgets(lineBuffer, BUFFER_SIZE, file) != NULL) {
        printf("%s%03d:%s %s", HIGHLIGHT_YELLOW, lineNum++, RESET, lineBuffer);
    }
    
    if (lineNum == 1) {
        printf("%s文件为空！\n" RESET, HIGHLIGHT_RED);
    }
    return;
}

void writefile(FILE* file) {
    char lineBuffer[BUFFER_SIZE];
    int c;
    
    while ((c = getchar()) != '\n' && c != EOF); // 清空输入缓冲区
    clearfile(file);
    
    printf("请逐行输入要写入的内容(会覆盖原内容！)，输入空行结束：\n");
    printf("%s提示：输入空行（直接按Enter键）即可结束写入%s\n", HIGHLIGHT_YELLOW, RESET);
    
    while (1) {
        printf("%s>%s ", HIGHLIGHT_GREEN, RESET);
        if (fgets(lineBuffer, BUFFER_SIZE, stdin) == NULL) {
            perror("输入失败");
            return;
        }
        
        // 如果输入的是空行（只有换行符），则结束写入
        if (strcmp(lineBuffer, "\n") == 0 || strcmp(lineBuffer, "\r\n") == 0) {
            break;
        }
        
        if (fputs(lineBuffer, file) == EOF) {
            perror("写入失败");
            return;
        }
    }
    
    printf("%s写入完成！\n" RESET, HIGHLIGHT_GREEN);
    return;
}

void clearfile(FILE* file) {
    rewind(file);
    // 写入空字符串（这会覆盖文件内容）
    if (fputs("", file) == EOF) {
        perror("文件清空失败");
        return;
    }
    // 截断文件到当前位置（确保文件大小为0）
    if (_chsize(fileno(file), 0) != 0) {
        perror("文件截断失败");
        return;
    }
    printf("%s文件已清空！\n" RESET, HIGHLIGHT_GREEN);
}

fileinfo analyzefile(FILE* file, const char* filename) {
    int original_position = ftell(file);
    if (original_position == -1) {
        perror("获取文件位置失败");
        return (fileinfo){0,0,0,0,0,{0}};
    }
    fclose(file);
    file = fopen(filename, "r+");
    if (file == NULL) {
        perror("重新打开文件失败");
        return (fileinfo){0,0,0,0,0,{0}};
    }
    struct stat file_stat;
    if (stat(filename, &file_stat) != 0) {
        perror("获取文件信息失败");
        return (fileinfo){0,0,0,0,0,{0}};
    }
    
    long long infosize = file_stat.st_size;
    struct tm* timeinfo;
    char time_buffer[80];
    timeinfo = localtime(&file_stat.st_mtime);
    // 简化时间格式
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    // 使用传入的file指针读取文件内容
    char* lineBuffer = malloc(BUFFER_SIZE);
    if (lineBuffer == NULL) {
        perror("内存分配失败");
        return (fileinfo){0,0,0,0,0,{0}};
    }
    
    // 保存当前文件指针位置
    long currentPos = ftell(file);
    if (currentPos == -1) {
        perror("获取文件指针位置失败");
        free(lineBuffer);
        return (fileinfo){0,0,0,0,0,{0}};
    }
    
    int lineCount = 0, charCount = 0, num = 0, chara = 0;
    rewind(file); // 将文件指针移到开头
    while (fgets(lineBuffer, BUFFER_SIZE, file) != NULL) {
        lineCount++;
        charCount += strlen(lineBuffer);
        for (int i = 0; i < strlen(lineBuffer); i++) {
            if (lineBuffer[i] <= '9' && lineBuffer[i] >= '0') {
                num++;
            }
            else if ((lineBuffer[i] <= 'z' && lineBuffer[i] >= 'a') || 
                     (lineBuffer[i] <= 'Z' && lineBuffer[i] >= 'A')) {
                chara++;
            }
        }
    }
    
    // 恢复文件指针位置
    fseek(file, currentPos, SEEK_SET);
    
    free(lineBuffer);
    
    fileinfo info;
    info.lineCount = lineCount;
    info.charCount = charCount - lineCount;
    info.num = num;
    info.chara = chara;
    info.size = infosize;
    // 安全复制时间字符串
    strncpy(info.lastmodtime, time_buffer, sizeof(info.lastmodtime) - 1);
    info.lastmodtime[sizeof(info.lastmodtime) - 1] = '\0'; 
    
    return info;
}

void englishfile(FILE* file, const char* filename) {
    char lineBuffer[BUFFER_SIZE];
    int wordcnt = 0;
    char word[1024][BUFFER_SIZE] = {0};
    int inWord = 0;
    int maxWordsReached = 0; // 添加标志位表示是否达到最大单词数
    
    rewind(file);
    while (fgets(lineBuffer, BUFFER_SIZE, file) != NULL && !maxWordsReached) {
        for (int i = 0; i < strlen(lineBuffer); i++) {
            if ((lineBuffer[i] >='A' && lineBuffer[i] <='Z') || 
                (lineBuffer[i] >= 'a' && lineBuffer[i] <= 'z') || 
                (lineBuffer[i] == '\'')) {
                if (!inWord)
                    inWord = 1;
                int currentLen = strlen(word[wordcnt]);
                if (currentLen < BUFFER_SIZE - 1) {
                    word[wordcnt][currentLen] = lineBuffer[i];
                    word[wordcnt][currentLen + 1] = '\0';
                }
            }
            else if (lineBuffer[i] == ' ' || lineBuffer[i] == '\n' || lineBuffer[i] == '\t') {
                if (inWord) {
                    inWord = 0;
                    wordcnt++;
                    if (wordcnt >= 1024) {
                        maxWordsReached = 1;
                        break; 
                    }
                }
            }
        }
        if (maxWordsReached) // 如果已达到最大单词数，跳出外层while循环
            break;
    }
    
    // 分析词频
    int wordfreq[1024] = {0};
    for (int i = 0; i < wordcnt; i++) {
        for (int j = 0; j < wordcnt; j++) {
            if (strcmp(word[i], word[j]) == 0)
                wordfreq[i]++;
        }
    }
    
    printf("%s英语文件分析结果：%s\n", HIGHLIGHT_BLUE, RESET);
    printf("总单词数: %s%d%s\n", HIGHLIGHT_GREEN, wordcnt, RESET);
    if (maxWordsReached) {
        printf("%s注意：文件包含超过1024个单词，仅显示前1024个单词的统计结果%s\n", HIGHLIGHT_RED, RESET);
    }
    
    // 找出高频词
    printf("\n%s高频词统计：%s\n", HIGHLIGHT_PURPLE, RESET);
    printf("------------------------\n");
    for (int i = 0; i < wordcnt; i++) {
        int printed = 0;
        for (int j = 0; j < i; j++) {
            if (strcmp(word[i], word[j]) == 0) {
                printed = 1;
                break;
            }
        }
        if (!printed) {
            printf("%-15s: %s%d%s次\n", word[i], HIGHLIGHT_GREEN, wordfreq[i], RESET);
        }
    }
    return;
}

void comparefile(FILE* file1, FILE* file2) {
    char lineBuffer1[BUFFER_SIZE], lineBuffer2[BUFFER_SIZE];
    int lineNum = 0;
    bool filesAreSame = true;
    size_t len1, len2;
    
    rewind(file1);
    rewind(file2);
    
    printf("%s文件比较结果：%s\n", HIGHLIGHT_BLUE, RESET);
    printf("==========================================================\n");
    
    while (1) {
        lineNum++;
        bool eof1 = (fgets(lineBuffer1, BUFFER_SIZE, file1) == NULL);
        bool eof2 = (fgets(lineBuffer2, BUFFER_SIZE, file2) == NULL);
        
        if (eof1 && eof2) break; // 两个文件都结束了
        
        // 移除换行符
        if (!eof1) {
            len1 = strlen(lineBuffer1);
            if (len1 > 0 && lineBuffer1[len1-1] == '\n') {
                lineBuffer1[len1-1] = '\0';
                len1--;
            }
        }
        
        if (!eof2) {
            len2 = strlen(lineBuffer2);
            if (len2 > 0 && lineBuffer2[len2-1] == '\n') {
                lineBuffer2[len2-1] = '\0';
                len2--;
            }
        }
        
        printf("第%d行：\n", lineNum);
        
        if (eof1) {
            printf("%s  文件1：(结束)%s\n", HIGHLIGHT_RED, RESET);
            printf("%s  文件2：%s%s\n", HIGHLIGHT_GREEN, lineBuffer2, RESET);
            filesAreSame = false;
        } 
        else if (eof2) {
            printf("%s  文件1：%s%s\n", HIGHLIGHT_GREEN, lineBuffer1, RESET);
            printf("%s  文件2：(结束)%s\n", HIGHLIGHT_RED, RESET);
            filesAreSame = false;
        } 
        else {
            // 比较两行内容
            if (strcmp(lineBuffer1, lineBuffer2) != 0) {
                printf("%s  文件1：%s%s\n", HIGHLIGHT_RED, lineBuffer1, RESET);
                printf("%s  文件2：%s%s\n", HIGHLIGHT_RED, lineBuffer2, RESET);
                filesAreSame = false;
            } else {
                printf("%s  文件1：%s%s\n", HIGHLIGHT_GREEN, lineBuffer1, RESET);
                printf("%s  文件2：%s%s\n", HIGHLIGHT_GREEN, lineBuffer2, RESET);
            }
        }
        
        printf("----------------------------------------------------------\n");
    }
    
    printf("==========================================================\n");
    if (filesAreSame) {
        printf("%s结论：两个文件内容完全相同！%s\n", HIGHLIGHT_GREEN, RESET);
    } else {
        printf("%s结论：两个文件内容存在差异！%s\n", HIGHLIGHT_RED, RESET);
    }
}