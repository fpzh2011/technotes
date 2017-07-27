#!/usr/bin/python
# -*- coding:utf-8 -*-

'''
一元语言模型
因为数据少，用dict保存词频，没有使用Trie树。
'''

from math import log

class LanguageModelUni:
    def __init__(self, filename):
        '''
        根据训练文件filename（utf8格式）初始化词频。数据没做平滑，仅对生词作简单处理。
        处理后的词是unicode，词频用log处理。
        '''
        word_count_total = 0
        word_count = {}
        with open(filename) as f:
            for line in f:
                line = line.strip().decode('utf8')
                for word in line.split(u'/'):
                    word_count_total += 1
                    if word in word_count:
                        word_count[word] += 1
                    else:
                        word_count[word] = 1
        word_count_log = {}
        max_len = 0
        for word, count in word_count.iteritems():
            word_count_log[word] = log(float(count)/word_count_total)
            if len(word) > max_len:
                max_len = len(word)
        self.word_freq = word_count_log
        self.max_len = max_len
        # 生词词频
        self.word_count_total = word_count_total

    def get(self, word):
        '''
        返回word的词频（log，加法）。word是unicode。
        '''
        return (len(word) * (log(0.1) - log(self.word_count_total))) if word not in self.word_freq else self.word_freq[word]

if __name__ == '__main__':
    model = LanguageModelUni('199801_train.txt')
    print model.get(u'xyz')
    print model.get(u'中国')
