# NLTK官网book笔记

http://www.nltk.org/book/

[NLP工具列表](https://en.wikipedia.org/wiki/Outline_of_natural_language_processing#Natural_language_processing_toolkits)

其它参考书：
https://web.stanford.edu/~jurafsky/slp3/
https://book.douban.com/subject/2403834/
https://book.douban.com/subject/1776634/

## 术语

[word type](http://www.nltk.org/book/ch01.html)，有翻译为`词形`，应该是指合并动词形态、名词复数等之后的词。简称types。

[collocation](http://www.nltk.org/book/ch01.html)，词组、词语搭配、固定搭配。

[Word Sense Disambiguation](http://www.nltk.org/book/ch01.html)，词义消岐。根据上下文确定多义词在句子中的含义。

[Pronoun Resolution](http://www.nltk.org/book/ch01.html)，代词消解。
[anaphora resolution](http://www.nltk.org/book/ch01.html)，指代消解。
[semantic role labeling](http://www.nltk.org/book/ch01.html)，语义角色标注。

[Textual Entailment](http://www.nltk.org/book/ch01.html)，文字蕴含。

[summarization](http://www.nltk.org/book/ch01.html)，摘要。

[Stemming](https://en.wikipedia.org/wiki/Stemming)，词干提取，`having -> hav`。
[Lemmatisation](https://en.wikipedia.org/wiki/Lemmatisation)，词形还原，`having -> have`，
参考：
https://stackoverflow.com/questions/771918/how-do-i-do-word-stemming-or-lemmatization
https://stackoverflow.com/questions/17317418/stemmers-vs-lemmatizers


已经读完
http://www.nltk.org/book/ch01.html









词频统计`fdist1 = FreqDist(text1)`。`FreqDist`可以统计任何事件，比如`fdist = FreqDist(len(w) for w in text1)`。




