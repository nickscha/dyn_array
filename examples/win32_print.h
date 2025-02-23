#ifndef WIN32_PRINT_H
#define WIN32_PRINT_H

#define STD_OUTPUT_HANDLE ((unsigned long)-11)

void *GetStdHandle(unsigned long nStdHandle);
int WriteConsoleA(void *hConsoleOutput, const void *lpBuffer, unsigned long nNumberOfCharsToWrite, unsigned long *lpNumberOfCharsWritten, void *lpReserved);

unsigned long win32_lstrlen(const char *str)
{
    unsigned long length = 0;
    while (str[length] != '\0')
    {
        length++;
    }
    return length;
}

void win32_printf(const char *str)
{
    unsigned long written;
    void *hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteConsoleA(hConsole, str, win32_lstrlen(str), &written, (void *)0);
}

#endif /* WIN32_PRINT_H */
