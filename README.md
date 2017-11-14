# CISTEM
[![license](https://img.shields.io/github/license/mashape/apistatus.svg)]()

CISTEM is a stemming algorithm for the German language, developed by Leonie Weißweiler and Alexander Fraser. This repository contains official implementations in a variety of programming languages. At the moment, the following languages are available:
- C
- C++
- Haskell
- Java
- Perl
- Python
- Swift

## Performance
We performed a comparative analysis of six publicly available German stemmers, where CISTEM achieved the best results for f-measure and state-of-the-art results for runtime.

<img src="http://cis.lmu.de/~weissweiler/cistem/Plots.svg">

## Gold standards
The *gold_standards* folder contains the two gold standards we used for evaluation. Each file is utf-8 text file with each line containing all the stems of one cluster separated by a single space. Note that we do not supply a reference *stem* for each cluster, as we measure stemming performance as the ability to group words with the same meaning, which is more relevant for information retrieval purposes than the absolute stem. If you use these gold standards in your own research, please cite our paper: [Bibtex](http://cis.lmu.de/~weissweiler/cistem/bibtex/bibtex.php)

More information on how we evaluated runtimes and stemming quality can be found in our paper:

Leonie Weißweiler, Alexander Fraser (2017). [Developing a Stemmer for German Based on a Comparative Analysis of Publicly Available Stemmers.](https://cis.lmu.de/~weissweiler/cistem/) In Proceedings of the German Society for Computational Linguistics and Language Technology (GSCL), to appear.
