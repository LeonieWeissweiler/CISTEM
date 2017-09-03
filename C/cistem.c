
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>

static const wchar_t lowercase_ae = 0x00E4;
static const wchar_t lowercase_oe = 0x00F6;
static const wchar_t lowercase_ue = 0x00FC;
static const wchar_t uppercase_ae = 0x00C4;
static const wchar_t uppercase_oe = 0x00D6;
static const wchar_t uppercase_ue = 0x00DC;
static const wchar_t scharfes_ss = 0x00DF;


int count_double_characters(wchar_t* word, unsigned long length) {
	int result = 0;
	for (int i = 0; i < length; i++) {
		switch (word[i]) {
			case scharfes_ss:
				result++;
				break;
			default:
				break;
		}
	}
	return result;
}

int match(wchar_t* source, unsigned index, wchar_t* pattern, unsigned pattern_length) {
	for (int i = 0; i < pattern_length; i++) {
		if (tolower(source[i + index]) != pattern[i]) return 0;
	}
	return 1;
}

int match_suffix(wchar_t* source, unsigned long length, wchar_t* pattern, unsigned pattern_length) {
	for (int i = 0; i < pattern_length; i++) {
		if (source[length - pattern_length + i] != pattern[i]) return 0;
	}
	return 1;
}

#define APPEND(x) destination[offset] = x; offset++;

unsigned long copy_and_normalize(wchar_t* source, unsigned long length, wchar_t* destination, int modify) {
	// Offset in the destination
	unsigned long offset = 0;
	
	// Iterate over source characters
	for (int i = 0; i < length; i++) {
		if (source[i] == 0) break;
		
		// If the word after it at least 4 characters, skip ahead of a "ge"
		if (i == 0 && modify) {
			unsigned long remaining_length_with_expansion = length + count_double_characters(source,length);
			if (remaining_length_with_expansion >= 4 + 2 && match(source,0,L"ge",2)) {
				i += 1;
				continue;
			}
		}
		
		unsigned long remaining_chars = length - i;
		
		if (source[i] == scharfes_ss && modify) {
			APPEND('s');
			APPEND('*');
		} else if (source[i] == lowercase_ae && modify) {
			APPEND('a');
		} else if (source[i] == lowercase_oe && modify) {
			APPEND('o');
		} else if (source[i] == lowercase_ue && modify) {
			APPEND('u');
		} else if (source[i] == uppercase_ae && modify) {
			APPEND('a');
		} else if (source[i] == uppercase_oe && modify) {
			APPEND('o');
		} else if (source[i] == uppercase_ue && modify) {
			APPEND('u');
		} else if (remaining_chars >= 3 && match(source,i,L"sch",3)) {
			APPEND('$');
			i += 2;
		} else if (remaining_chars >= 2 && match(source,i,L"ei",2)) {
			APPEND('%');
			i += 1;
		} else if (remaining_chars >= 2 && match(source,i,L"ie",2)) {
			APPEND('&');
			i += 1;
		} else {
			APPEND(towlower(source[i]));
		}
	}
	APPEND(0);
	
	// Replace "xx" by "x*"
	
	unsigned destination_length = offset - 1;
	
	for (int i = 0; i < destination_length; i++) {
		int remaining_chars = destination_length - i;
		
		if (remaining_chars >= 2 && destination[i] == destination[i+1]) {
			destination[i+1] = '*';
			i++;
		}
	}
	
	return offset - 1;
}

void copy_and_denormalize(wchar_t* source, unsigned long length, wchar_t* destination) {
	// Replace "x*" by "xx"
	
	for (int i = 0; i < length; i++) {
		unsigned long remaining_chars = length - i;
		
		if (remaining_chars >= 2 && source[i+1] == '*') {
			source[i+1] = source[i];
			i++;
		}
	}
	
	// Offset in the destination
	int offset = 0;
	
	// Iterate over source characters
	for (int i = 0; i < length; i++) {
		if (source[i] == 0) break;
		
		if (source[i] == '$') {
			APPEND('s');
			APPEND('c');
			APPEND('h');
		} else if (source[i] == '%') {
			APPEND('e');
			APPEND('i');
		} else if (source[i] == '&') {
			APPEND('i');
			APPEND('e');
		} else {
			APPEND(source[i]);
		}
	}
	
	destination[offset] = 0;
}

unsigned long strip_suffixes(wchar_t* word, unsigned long length, int uppercase, int case_sensitive) {
	unsigned long string_length = wcslen(word);
	
	while (string_length > 3) {
		if (string_length > 5 &&
			(match_suffix(word, string_length, L"em", 2) ||
			 match_suffix(word, string_length, L"er", 2) ||
			 match_suffix(word, string_length, L"nd", 2))) {
			string_length -= 2;
		} else if (((!uppercase) || case_sensitive) && match_suffix(word, string_length, L"t", 1)) {
			string_length -= 1;
		} else if ((match_suffix(word, string_length, L"e", 1) ||
					match_suffix(word, string_length, L"s", 1) ||
					match_suffix(word, string_length, L"n", 1))) {
			string_length -= 1;
		} else {
			break;
		}
	}
	
	return string_length;
}
	
wchar_t* stem_internal(wchar_t* word, int case_insensitive) {
	int uppercase = !iswlower(word[0]);
	
	unsigned long length = wcslen(word);
	unsigned long intermediate_length = length + count_double_characters(word, length) + 16; // count ß, ae etc. to allocate properly and add some to be safe
	wchar_t* intermediate = malloc(intermediate_length * sizeof(wchar_t));
	wchar_t* result = malloc(intermediate_length * sizeof(wchar_t));
	
	copy_and_normalize(word, length, intermediate, 1);
	
	unsigned long stem_length = strip_suffixes(intermediate, intermediate_length, uppercase, case_insensitive);
	
	intermediate[stem_length] = 0;
	
	copy_and_denormalize(intermediate, stem_length, result);
	
	free(intermediate);
	return result;
}

wchar_t* stem(wchar_t* word) {
	return stem_internal(word, 0);	
}

wchar_t* stem_case_insensitive(wchar_t* word) {
	return stem_internal(word, 1);	
}

wchar_t** segment_internal(wchar_t* word, int case_insensitive) {
	wchar_t** result = malloc(2 * sizeof(wchar_t*));
	int uppercase = !iswlower(word[0]);
	
	unsigned long length = wcslen(word);
	unsigned long intermediate_length = length + count_double_characters(word, length) + 16; // count ß, ae etc. to allocate properly and add some to be safe
	wchar_t* intermediate = malloc(intermediate_length * sizeof(wchar_t));
	wchar_t* stem_result = malloc(intermediate_length * sizeof(wchar_t));
	wchar_t* suffix_result = malloc(intermediate_length * sizeof(wchar_t));
	
	unsigned long normalized_length = copy_and_normalize(word, length, intermediate, 0);
	
	unsigned long stem_length = strip_suffixes(intermediate, intermediate_length, uppercase, case_insensitive);
	
	wcsncpy(suffix_result, intermediate + stem_length, normalized_length - stem_length);
	suffix_result[normalized_length - stem_length] = 0;
	
	intermediate[stem_length] = 0;
	
	copy_and_denormalize(intermediate, stem_length, stem_result);
	
	free(intermediate);
	
	result[0] = stem_result;
	result[1] = suffix_result;
	
	return result;
}

wchar_t** segment(wchar_t* word) {
	return segment_internal(word, 0);	
}

wchar_t** segment_case_insensitive(wchar_t* word) {
	return segment_internal(word, 1);	
}

//#include <locale.h>
//
//int main() {
//	wchar_t word[64];
//    setlocale(LC_ALL, "");
//    
//    wprintf(L"Insert special characters: ");
//    fgetws(word, 64, stdin);
//	word[wcslen(word) - 1] = 0; //remove newline...
//    wprintf(L"%S\n",stem(word));
//    return 0;
//}

