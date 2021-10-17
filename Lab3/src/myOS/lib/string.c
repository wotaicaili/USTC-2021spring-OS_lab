int myStrcmp(const char *str1, const char *str2) {
    // loop until str1 or str2 reach '\0' or meet different chars
    while (*str1 && *str2 && *str1++ == *str2++);   

    if (!*str1 || !*str2)
        return *str1 - *str2;
    else
        return *--str1 - *--str2;
}

int myStrlen(const char *str) {
    int len = 0;
    while (*str++)
        len++;

    return len;
}

int myStrnlen(const char *str, int maxlen) {
    int len = 0;
    while ((maxlen < 0 || len < maxlen) && *str++)
        len++;

    return len;
}