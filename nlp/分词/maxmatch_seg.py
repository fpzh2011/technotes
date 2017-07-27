#!/usr/bin/python
# -*- coding:utf-8 -*-

'''
基于词典的最大匹配算法。包括正向匹配、反向匹配、双向匹配。
'''

class Words:
    '''
    词列表
    优化措施：按词长度分为多个列表
    '''
    def __init__(self, filename):
        # 词列表
        self.words = Words.__load_dict(filename) 
        # 最大词长
        self.max_len = max(len(word) for word in self.words)

    def __contains__(self, word):
        return word in self.words

    @staticmethod
    def __load_dict(filename):
        '''
        从文件（utf8）加载词表，一行一个单词，返回词集合（unicode）
        '''
        words = set()
        with open(filename) as f:
            for line in f:
                words.add(line.strip().decode('utf8'))
        return words

def seg_sentence_fmm(sentence, words):
    '''
    对输入的句子分词（正向最大匹配），句子是unicode格式，返回词列表
    '''
    # 最大词长
    word_max_len = words.max_len
    # 句子长度
    sen_len = len(sentence)
    # 词起始位置
    start = 0
    result = []
    while start < sen_len:
        # 词终止位置（不含）
        end = min(sen_len, start + word_max_len)
        while end > start:
            new_word = sentence[start : end]
            if new_word in words:
                result.append(new_word)
                break 
            end -= 1
        # 词典中没有的单字
        if end <= start:
            result.append(sentence[start : start + 1])
            start += 1
        else:
            start = end
    return result

def seg_sentence_bmm(sentence, words):
    '''
    对输入的句子分词（逆向最大匹配），句子是unicode格式，返回词列表
    '''
    # 最大词长
    word_max_len = words.max_len
    # 句子长度
    sen_len = len(sentence)
    # 词终止位置（不含）
    end = sen_len
    result = []
    while end > 0:
        # 词起始位置
        start = max(0, end - word_max_len)
        while end > start:
            new_word = sentence[start : end]
            if new_word in words:
                result.append(new_word)
                break 
            start += 1
        # 词典中没有的单字
        if end <= start:
            result.append(sentence[end - 1 : end])
            end -= 1
        else:
            end = start
    return list(reversed(result))

def score(result, words):
    '''
    对分词结果进行评分，分值越小效果越好
    '''
    return sum((\
        # 词个数
        len(result), \
        # 单字个数。模拟替代“最大词长”
        sum(1 for word in result if len(word) == 1), \
        # 生词个数
        sum(1 for word in result if word not in words) \
        ))

def seg_sentence_dual(sentence, words):
    '''
    对输入的句子分词（双向最大匹配），句子是unicode格式，返回词列表
    '''
    f = seg_sentence_fmm(sentence, words)
    b = seg_sentence_bmm(sentence, words)
    return f if score(f, words) < score(b, words) else b

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(description='基于词表的中文分词')
    parser.add_argument('--dict', default='jieba.dict.txt.small', help='词表文件')
    args = parser.parse_args()
    words = Words(args.dict)

    '''
    我们在野生动物园玩
    研究生命的起源
    结婚的和尚未结婚的
    上海大学城书店
    总统有意见他
    泰国人民很友好
    '''
    import sys
    for line in sys.stdin:
        # 每种分词方法打印一行
        for func in (seg_sentence_fmm, seg_sentence_bmm, seg_sentence_dual):
            print u'/'.join(func(line.strip().decode('utf8'), words))




