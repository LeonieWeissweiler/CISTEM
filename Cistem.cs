using System;
using System.Text.RegularExpressions;

namespace Cistem
{
  public static class Cistem
  {
    private const string GE_PATTERN = "^ge(.{4,})";
    private const string DOLLAR1_PATTERN = "(.)\\1";
    private const string ND_PATTERN = "nd$";
    private const string EMR_PATTERN = "e[mr]$";
    private const string T_PATTERN = "t$";
    private const string ESN_PATTERN = "[esn]$";
    private const string STAR_PATTERN = "(.)\\*";

    public static String Stem(String word)
    {
      return Stem(word, false);
    }

    public static String Stem(String word, bool case_insensitive)
    {
      if (word.Length == 0) return word;

      word = word.Replace("Ü", "U");
      word = word.Replace("Ö", "O");
      word = word.Replace("Ä", "A");
      word = word.Replace("ü", "u");
      word = word.Replace("ö", "o");
      word = word.Replace("ä", "a");

      var uppercase = char.IsUpper(word[0]);

      word = word.ToLower();

      word = word.Replace("ß", "ss");
      word = Regex.Replace(word, GE_PATTERN, "$1");
      word = word.Replace("sch", "$");
      word = word.Replace("ei", "%");
      word = word.Replace("ie", "&");

      word = Regex.Replace(word, DOLLAR1_PATTERN, "$1*");

      string newWord;

      while (word.Length > 3)
      {
        if (word.Length > 5)
        {
          newWord = Regex.Replace(word, EMR_PATTERN, "");
          if (word != newWord)
          {
            word = newWord;
            continue;
          }

          newWord = Regex.Replace(word, ND_PATTERN, "");
          if (word != newWord)
          {
            word = newWord;
            continue;
          }
        }

        if (!uppercase || case_insensitive)
        {
          newWord = Regex.Replace(word, T_PATTERN, "");
          if (word != newWord)
          {
            word = newWord;
            continue;
          }
        }

        newWord = Regex.Replace(word, ESN_PATTERN, "");

        if (word != newWord)
          word = newWord;
        else
          break;
      }

      word = Regex.Replace(word, STAR_PATTERN, "$1$1");
      word = word.Replace("&", "ie");
      word = word.Replace("%", "ei");
      word = word.Replace("$", "sch");

      return word;
    }

    public static String[] Segment(String word)
    {
      return Segment(word, false);
    }

    public static String[] Segment(String word, bool case_insensitive)
    {
      if (word.Length == 0)
        return new string[] { string.Empty, string.Empty };

      var restLength = 0;
      var uppercase = char.IsUpper(word[0]);
      word = word.ToLower();
      var original = word;

      word = word.Replace("sch", "$");
      word = word.Replace("ei", "%");
      word = word.Replace("ie", "&");

      word = Regex.Replace(word, DOLLAR1_PATTERN, "$1*");

      string newWord;

      while (word.Length > 3)
      {
        if (word.Length > 5)
        {
          newWord = Regex.Replace(word, EMR_PATTERN, "");
          if (word != newWord)
          {
            restLength += 2;
            word = newWord;
            continue;
          }

          newWord = Regex.Replace(word, ND_PATTERN, "");
          if (word != newWord)
          {
            restLength += 2;
            word = newWord;
            continue;
          }
        }

        if (!uppercase || case_insensitive)
        {
          newWord = Regex.Replace(word, T_PATTERN, "");
          if (word != newWord)
          {
            restLength += 1;
            word = newWord;
            continue;
          }
        }

        newWord = Regex.Replace(word, ESN_PATTERN, "");
        if (word != newWord)
        {
          restLength += 1;
          word = newWord;
        }
        else
          break;
      }

      word = Regex.Replace(word, STAR_PATTERN, "$1$1");
      word = word.Replace("&", "ie");
      word = word.Replace("%", "ei");
      word = word.Replace("$", "sch");

      var rest = string.Empty;

      if (restLength != 0)
        rest = original.Substring(original.Length - restLength);

      return new string[] { word, rest };
    }
  }
}