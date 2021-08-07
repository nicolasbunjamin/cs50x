#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>

// functions
int count_letters(string s);
int count_words(string s);
int count_sentences(string s);
float coleman_liau(int a, int b, int c);

int main(void)
{
    string text = get_string("Text: ");

    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    int index = round(coleman_liau(letters, words, sentences));
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int) round(index));
    }
}

int count_letters(string s)
{
    int x = 0;
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if ((s[i] >= 65 && s[i] <= 90) || (s[i] >= 97 && s[i] <= 122))
        {
            x ++;
        }
    }
    return x;
}

int count_words(string s)
{
    int y = 0;
    // first word
    if (s[0] != ' ')
    {
        y++;
    }
    for (int i = 0, n = strlen(s); i < n; i++) // following words
    {
        if ((s[i - 1] == ' ') && ((s[i] != ' ')))
        {
            y ++;
        }
    }
    return y;
}

int count_sentences(string s)
{
    int z = 0;
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if ((s[i] == '.') || (s[i] == '?') || (s[i] == '!'))
        {
            z ++;
        }
    }
    return z;
}

float coleman_liau(int a, int b, int c)
{
    float l = (float) a / (float) b * 100;
    float s = (float) c / (float) b * 100;
    return 0.0588 * l - 0.296 * s - 15.8;
}