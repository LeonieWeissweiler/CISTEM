#!/usr/bin/python3
"""
CISTEM Stemmer for German

This is the official Perl implementation of the CISTEM stemmer.
It is based on the paper
Leonie Weißweiler, Alexander Fraser (2017). Developing a Stemmer for German Based on a Comparative Analysis of Publicly Available Stemmers. In Proceedings of the German Society for Computational Linguistics and Language Technology (GSCL)
which can be read here:
http://www.cis.lmu.de/~weissweiler/cistem/

In the paper, we conducted an analysis of publicly available stemmers, developed
two gold standards for German stemming and evaluated the stemmers based on the
two gold standards. We then proposed the stemmer implemented here and show
that it achieves slightly better f-measure than the other stemmers and is
thrice as fast as the Snowball stemmer for German while being about as fast as
most other stemmers.
"""
import re

stripge = re.compile(r"^ge(.{4,})")
replxx = re.compile(r"(.)\1")
replxxback = re.compile(r"(.)\*");
stripemr = re.compile(r"e[mr]$")
stripnd = re.compile(r"nd$")
stript = re.compile(r"t$")
stripesn = re.compile(r"[esn]$")

"""
This method takes the word to be stemmed and a boolean specifiying if case-insensitive stemming should be used and returns the stemmed word. If only the word
is passed to the method or the second parameter is 1, normal case-sensitive stemming is used, if the second parameter is 0, case-insensitive stemming is used.

Case sensitivity improves performance only if words in the text may be incorrectly upper case.
For all-lowercase and correctly cased text, best performance is achieved by
using the case-sensitive version.
"""

def stem(word, case_insensitive = False):
    if len(word) == 0:
        return word

    upper = word[0].isupper()
    word = word.lower()

    word = word.replace("ü","u")
    word = word.replace("ö","o")
    word = word.replace("ä","a")
    word = word.replace("ß","ss")

    word = stripge.sub(r"\1", word)
    word = word.replace("sch","$")
    word = word.replace("ei","%")
    word = word.replace("ie","&")
    word = replxx.sub(r"\1*", word)

    while len(word) > 3:
        if len(word) > 5:
            (word, success) = stripemr.subn("", word)
            if success != 0:
                continue

            (word, success) = stripnd.subn("", word)
            if success != 0:
                continue

        if not upper or case_insensitive:
            (word, success) = stript.subn("", word)
            if success != 0:
                continue

        (word, success) = stripesn.subn("", word)
        if success != 0:
            continue
        else:
            break

    word = replxxback.sub(r"\1\1", word)
    word = word.replace("%","ei")
    word = word.replace("&","ie")
    word = word.replace("$","sch")

    return word

"""
This method works very similarly to stem. The only difference is that in
addition to returning the stem, it also returns the rest that was removed at
the end. To be able to return the stem unchanged so the stem and the rest
can be concatenated to form the original word, all subsitutions that altered
the stem in any other way than by removing letters at the end were left out.
"""

def segment(word, case_insensitive = False):

    rest_length = 0

    if len(word) == 0:
        return ("", "")

    upper = word[0].isupper()
    word = word.lower()

    original = word[:]

    word = word.replace("sch","$")
    word = word.replace("ei","%")
    word = word.replace("ie","&")
    word = replxx.sub(r"\1*", word)

    while len(word) > 3:
        if len(word) > 5:
            (word, success) = stripemr.subn("", word)
            if success != 0:
                rest_length += 2
                continue

            (word, success) = stripnd.subn("", word)
            if success != 0:
                rest_length += 2
                continue

        if not upper or case_insensitive:
            (word, success) = stript.subn("", word)
            if success != 0:
                rest_length += 1
                continue

        (word, success) = stripesn.subn("", word)
        if success != 0:
            rest_length += 1
            continue
        else:
            break


    word = replxxback.sub(r"\1\1", word)
    word = word.replace("%","ei")
    word = word.replace("&","ie")
    word = word.replace("$","sch")

    if rest_length:
        rest = original[-rest_length:]
    else:
        rest = ""

    return (word,rest)
