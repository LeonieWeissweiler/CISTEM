#include "cistem.hpp"
#include <regex>
#include <algorithm>

using namespace std;

namespace Cistem {
	wregex replacess(L"ß");
	
	wregex replaceSch(L"sch");
	wregex replaceSchBack(L"\\$");
	wregex replaceEi(L"ei");
	wregex replaceEiBack(L"%");
	wregex replaceIe(L"ie");
	wregex replaceIeBack(L"&");
	wregex replacexx(L"(.)\\1");
	wregex replacexxback(L"(.)\\*");
	
	wregex stripge(L"^ge(.{4,})");
	
	wregex stripemr(L"e[mr]$");
	wregex stripnd(L"nd$");
	wregex stript(L"t$");
	wregex stripesn(L"[esn]$");
	
	wstring stem(const wstring word, bool caseInsensitive) {
		if (word.size() == 0) {
			return L"";
		}
		
		bool uppercase = iswupper(word[0]);
		
		wstring stem = word;
		setlocale(LC_ALL, "de_DE.UTF-8");
		transform(stem.begin(), stem.end(), stem.begin(), towlower);
		
		replace(stem.begin(), stem.end(), L'ä', L'a');
		replace(stem.begin(), stem.end(), L'ö', L'o');
		replace(stem.begin(), stem.end(), L'ü', L'u');
		stem = regex_replace(stem, replacess, L"ss");
		
		stem = regex_replace(stem, stripge, L"$1");
		stem = regex_replace(stem, replaceSch, L"$$");
		stem = regex_replace(stem, replaceEi, L"%");
		stem = regex_replace(stem, replaceIe, L"&");
		stem = regex_replace(stem, replacexx, L"$1*");
		
		bool match;
		while (stem.size() > 3) {
			if (stem.size() > 5) {
				match = regex_search(stem, stripemr);
				if (match) {
					stem = regex_replace(stem, stripemr, L"");
					continue;
				}
				match = regex_search(stem, stripnd);
				if (match) {
					stem = regex_replace(stem, stripnd, L"");
					continue;
				}
			}
			
			if (!uppercase || caseInsensitive) {
				match = regex_search(stem, stript);
				if (match) {
					stem = regex_replace(stem, stript, L"");
					continue;
				}
			}
			
			match = regex_search(stem, stripesn);
			if (match) {
				stem = regex_replace(stem, stripesn, L"");
				continue;
			}
			
			break;
		}
		
		stem = regex_replace(stem, replacexxback, L"$1$1");
		stem = regex_replace(stem, replaceEiBack, L"ei");
		stem = regex_replace(stem, replaceIeBack, L"ie");
		stem = regex_replace(stem, replaceSchBack, L"sch");
		
		return stem;
	}
	
	
	array<wstring,2> segment(const wstring word, bool caseInsensitive) {
		array<wstring,2> result;
		
		if (word.size() == 0) {
			result[0] = L"";
			result[1] = L"";
			return result;
		}
		
		bool uppercase = iswupper(word[0]);
		
		wstring stem = word;
		setlocale(LC_ALL, "de_DE.UTF-8");
		transform(stem.begin(), stem.end(), stem.begin(), towlower);
		wstring original(stem);
		
		stem = regex_replace(stem, replaceSch, L"$$");
		stem = regex_replace(stem, replaceEi, L"%");
		stem = regex_replace(stem, replaceIe, L"&");
		stem = regex_replace(stem, replacexx, L"$1*");
		
		bool match;
		unsigned int restLength = 0;
		while (stem.size() > 3) {
			if (stem.size() > 5) {
				match = regex_search(stem, stripemr);
				if (match) {
					stem = regex_replace(stem, stripemr, L"");
					restLength += 2;
					continue;
				}
				match = regex_search(stem, stripnd);
				if (match) {
					stem = regex_replace(stem, stripnd, L"");
					restLength += 2;
					continue;
				}
			}
			
			if (!uppercase || caseInsensitive) {
				match = regex_search(stem, stript);
				if (match) {
					stem = regex_replace(stem, stript, L"");
					restLength += 1;
					continue;
				}
			}
			
			match = regex_search(stem, stripesn);
			if (match) {
				stem = regex_replace(stem, stripesn, L"");
				restLength += 1;
				continue;
			}
			
			break;
		}
		
		stem = regex_replace(stem, replacexxback, L"$1$1");
		stem = regex_replace(stem, replaceEiBack, L"ei");
		stem = regex_replace(stem, replaceIeBack, L"ie");
		stem = regex_replace(stem, replaceSchBack, L"sch");
		
		result[1] = original.substr(original.length() - restLength,
									original.length());
		
		result[0] = stem;
		return result;
	}
	
	
	wstring stem(const wstring word) {
		return stem(word, false);
	}
	array<wstring,2> segment(const wstring word) {
		return segment(word, false);
	}
	
}
