#!/usr/bin/python
# -*- coding:utf-8 -*-

'''
用一元语言模型分词
'''

import language_model_uni

def score(sentence):
    '''
    输入要分词的句子sentence
    返回二元元组：第一个元素是分词的list，第二个元素是分词概率log
    参考Beautiful Data第14章。目前这种方式对于n个字的句子，需要遍历2^(n-1)种模式。
    对于特别长的句子，会报栈溢出错误。
    如果缓存数据，可以加快速度。
    '''
    if not hasattr(score, 'model'):
        score.model = language_model_uni.LanguageModelUni('199801_train.txt')

    if not sentence:
        return [], 0
    if len(sentence) == 1:
        return [sentence], score.model.get(sentence)
    result = []
    for i in range(len(sentence)):
        left_key = sentence[:i+1]
        left = ([left_key], score.model.get(left_key))
        right = score(sentence[i+1:])
        left = (left[0] + right[0], left[1] + right[1])
        result.append(left)
    return max(result, key=(lambda x: x[1]))

def seg_unigram(sentence):
    '''
    sentence是unicode格式，用一元语言模型分词，返回分词list，词也是unicode格式
    '''
    if not sentence:
        return []
    return score(sentence)[0]

if __name__ == '__main__':
    import sys
    #seg_unigram(u'你好')
    s = u'我喜欢五星酒店'
    for word in seg_unigram(s):
        sys.stdout.write(word.encode('utf8'))
        sys.stdout.write('/')
    print('')

