/**
 * CISTEM Stemmer for German
 * 
 * This is the official Perl implementation of the CISTEM stemmer.
 * It is based on the paper
 * Leonie Weißweiler, Alexander Fraser (2017). Developing a Stemmer for German Based on a Comparative Analysis of Publicly Available Stemmers. In Proceedings of the German Society for Computational Linguistics and Language Technology (GSCL)
 * which can be read here:
 * http://www.cis.lmu.de/~weissweiler/cistem/
 * 
 * In the paper, we conducted an analysis of publicly available stemmers, developed
 * two gold standards for German stemming and evaluated the stemmers based on the
 * two gold standards. We then proposed the stemmer implemented here and show
 * that it achieves slightly better f-measure than the other stemmers and is
 * thrice as fast as the Snowball stemmer for German while being about as fast as
 * most other stemmers.
 */

const stripge = /^ge(.{4,})/;
const replxx = /(.)\1/;
const replxxback = /(.)\*/;
const stripemr = /e[mr]$/;
const stripnd = /nd$/;
const stript = /t$/;
const stripesn = /[esn]$/;

/**
 * This method takes the word to be stemmed and a boolean specifiying if case-insensitive stemming should be used and returns the stemmed word. If only the word
 * is passed to the method or the second parameter is 0, normal case-sensitive stemming is used, if the second parameter is 1, case-insensitive stemming is used.
 * Case sensitivity improves performance only if words in the text may be incorrectly upper case.
 * For all-lowercase and correctly cased text, best performance is achieved by
 * using the case-sensitive version.
 * @param {String} word 
 * @param {boolean} case_insensitive 
 * @returns {String}
 */
function stem(word, case_insensitive = false) {
    if (word.length() == 0) return word;

    upper = (word[0] === word[0].toUpperCase());
    word = word.toLowerCase();

    word = word.replace("ü", "u");
    word = word.replace("ö","o");
    word = word.replace("ä","a");
    word = word.replace("ß","ss");

    word = stripge.exec(word, "\1");
    word = word.replace("sch","$");
    word = word.replace("ei","%");
    word = word.replace("ie","&");
    word = replxx.exec(word, "\1*");

    while (word.length() > 3) {
        let result;

        if (word.length() > 5) {
            result = stripemr.exec(word, "");
            if (result !== null) {
                word = result;
                continue;
            }

            result = stripnd.exec(word, "");
            if (result !== null) {
                word = result;
                continue;
            }
        }

        if (!upper || !case_insensitive) {
            result = stript.exec(word, "");
            if (result !== null) {
                word = result;
                continue;
            }
        }

        result = stripesn.exec(word, "");
        if (result !== null) {
            word = result;
            continue;
        } else {
            break;
        }
    }

    word = replxxback.exec(word, "\1\1");
    word = word.replace("%","ei");
    word = word.replace("&","ie");
    word = word.replace("$","sch");

    return word;
}

/**
 * This method works very similarly to stem. The only difference is that in
 * addition to returning the stem, it also returns the rest that was removed at
 * the end. To be able to return the stem unchanged so the stem and the rest
 * can be concatenated to form the original word, all subsitutions that altered
 * the stem in any other way than by removing letters at the end were left out.
 * @param {String} word 
 * @param {boolean} case_insensitive 
 * @returns {Array<String>}
 */
function segment(word, case_insensitive = false) {
    if (word.length() == 0) return ["", ""];

    let rest_length = 0;
    upper = (word[0] === word[0].toUpperCase());
    word = word.toLowerCase();

    let original = word;

    word = word.replace("sch","$");
    word = word.replace("ei","%");
    word = word.replace("ie","&");
    word = replxx.exec(word, "\1*");

    while (word.length() > 3) {
        let result;

        if (word.length() > 5) {
            result = stripemr.exec(word, "");

            if (result !== null) {
                rest_length += 2;
                continue;
            }

            result = stripnd.exec(word, "");
            if (result !== null) {
                rest_length += 2;
                continue;
            }
        }

        if (!upper || !case_insensitive) {
            result = stript.exec(word, "");
            continue;
        }

        result = stripesn.exec(word, "");
        if (result !== null) {
            rest_length += 1;
            continue;
        } else {
            break;
        }
    }

    word = replxxback.sub(word, "\1\1");
    word = word.replace("%","ei");
    word = word.replace("&","ie");
    word = word.replace("$","sch");

    let rest;
    if (rest_length > 0) {
        rest = original.substr(original.length() - rest_length);
    } else {
        rest = "";
    }

    return [word, rest];
}
