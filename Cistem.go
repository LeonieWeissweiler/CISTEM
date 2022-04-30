package main

import (
	"regexp"
	"strings"
	"unicode"
)

var revertXX = regexp.MustCompile("(.)\\*")
var stripEmr = regexp.MustCompile("e[mr]$")
var stripGe = regexp.MustCompile("^ge(.{4,})")
var stripNd = regexp.MustCompile("nd$")
var stripT = regexp.MustCompile("t$")
var stripEsn = regexp.MustCompile("[esn]$")

var replacerUOAß = strings.NewReplacer(
	"ü", "u",
	"ö", "o",
	"ä", "a",
	"Ü", "U",
	"Ö", "O",
	"Ä", "A",
	"ß", "ss",
)

var replacerSchEiIe = strings.NewReplacer(
	"sch", "$",
	"ei", "%",
	"ie", "&",
)

var revertSchEiIe = strings.NewReplacer(
	"$", "sch",
	"%", "ei",
	"&", "ie",
)

func Stem(term string, caseInsensitive bool) string {

	if len(term) == 0 {
		return term
	}

	term = replacerUOAß.Replace(term)

	isUppercase := unicode.IsUpper(rune(term[0]))

	term = strings.ToLower(term)
	term = stripGe.ReplaceAllString(term, "$1")
	term = replacerSchEiIe.Replace(term)
	term = replaceXX(term)

	for len(term) > 3 {

		if len(term) > 5 {
			newWord := stripEmr.ReplaceAllString(term, "")
			if term != newWord {
				term = newWord
				continue
			}

			newWord = stripNd.ReplaceAllString(term, "")
			if term != newWord {
				term = newWord
				continue
			}
		}

		if !isUppercase || caseInsensitive {
			newWord := stripT.ReplaceAllString(term, "")
			if term != newWord {
				term = newWord
				continue
			}
		}

		newWord := stripEsn.ReplaceAllString(term, "")
		if term != newWord {
			term = newWord
		} else {
			break
		}
	}

	term = revertXX.ReplaceAllString(term, "$1$1")
	term = revertSchEiIe.Replace(term)

	return term
}

// go's regex package doesn't provide backreferences
func replaceXX(term string) string {

	lastChar := int32(-1)
	str := ""
	for _, char := range term {

		if char == lastChar {
			// don't insert second character, append '*' instead
			str += "*"
			lastChar = '*'
			continue
		}

		str += string(char)
		lastChar = char
	}

	return str
}
