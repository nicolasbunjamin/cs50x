#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// functions
void input_check(int i); // check if an argument is inputted at all
void char_check(string s); // check if string contains exactly 26 characters
void alpha_check(string s); // check if string contains only alphabetic characters
void repetition_check(string s); // check if string contains repeated characters
string lowercase(string s); // use loop to convert all characters in a string to lowercase

int main(int argc, string argv[])
{
    string key = argv[1];
    int argument = argc - 1;

    input_check(argument);
    char_check(key);
    alpha_check(key);
    repetition_check(key);
    key = lowercase(key);
    string text = get_string("plaintext: ");

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        string ciphertext;
        if (isupper(text[i]))
        {
            text[i] = toupper(key[text[i] - 65]);
            // substracting the char by 65 turns it into the index number of the cipher alphabet
        }
        if (islower(text[i]))
        {
            text[i] = tolower(key[text[i] - 97]);
            // substracting the char by 97 turns it into the index number of the cipher alphabet
        }
    }
    printf("ciphertext: %s\n", text);
}

void input_check(int i)
{
    if (i != 1)
    {
        printf("Usage: ./substitution KEY\n");
        exit(1);
    }
}

void char_check(string s)
{
    if (strlen(s) != 26)
    {
        printf("Key must contain 26 characters\n");
        exit(1);
    }
}

void alpha_check(string s)
{
    for (int i = 0; i < 26; i++)
    {
        if ((s[i] < 65) || (s[i] > 90 && s[i] < 97) || (s[i] > 122))
        {
            printf("Key must only contain alphabetic characters\n");
            exit(1);
        }
    }
}

void repetition_check(string s)
{
    bool repeat = false;
    for (int i = 0; i < 26; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (s[i] == s[j])
            {
                repeat = true;
            }
        }
        for (int k = i + 1; k < 26; k++)
        {
            if (s[i] == s[k])
            {
                repeat = true;
            }
        }
    }
    if (repeat == true)
    {
        printf("Key must not contain repeated characters\n");
        exit(1);
    }
}

string lowercase(string s)
{
    string text = s;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        s[i] = tolower(s[i]);
    }
    return text;
}