#!/usr/bin/python
# -*- coding:utf-8 -*-

import sys

import maxmatch_seg
import unigram_seg

'''
评估各种分词方法的效果。
从标准输入读取待测试的数据。
'''

def segmentation_set(seg_list):
    '''
    输入词列表，返回集合。集合元素包含3项：词，起始位置，终止位置（不含）
    '''
    start = 0
    result = set()
    for seg in seg_list:
        size = len(seg)
        result.add((seg, start, start + size,))
        start += size
    return result

def score_sentence(src, seg):
    '''
    统计分词结果：标注正确、应标注个数、实际标注个数
    src是原始标注，seg是程序标注
    '''
    src_set = segmentation_set(src)
    seg_set = segmentation_set(seg)
    correct = len(src_set.intersection(seg_set))
    return (correct, len(src), len(seg),)

def print_seg_score(method, total_score):
    '''
    打印分词评分：recall, precision, f1
    method是分词方法描述字符串
    total_score是三元元组，各个元素分别是：标注正确、应标注个数、实际标注个数
    '''
    total_score = func[2]
    correct = total_score[0] + 0.0
    recall = correct/total_score[1]
    precision = correct/total_score[2]
    print method, recall, precision, 2 * precision * recall / (precision + recall)

if __name__ == '__main__':
    '''
    python evaluate.py < 199801_test.txt
    '''
    # 最大匹配的评估，不需要划分训练集、测试集
    words = maxmatch_seg.Words('199801_word_train.txt')
    func_result = []
    func_result.append(['seg_sentence_dual', maxmatch_seg.seg_sentence_dual, (0, 0, 0,)])
    func_result.append(['seg_sentence_fmm', maxmatch_seg.seg_sentence_fmm, (0, 0, 0,)])
    func_result.append(['seg_sentence_bmm', maxmatch_seg.seg_sentence_bmm, (0, 0, 0,)])
    import operator
    for line in sys.stdin:
        line = line.strip().decode('utf8')
        raw_line = line.replace(u'/', '')
        raw_seg = line.split(u'/')
        for func in func_result:
            # 分词
            seg = func[1](raw_line, words)
            # 估分
            score = score_sentence(raw_seg, seg)
            # 估分累加
            func[2] = map(operator.add, score, func[2])
    for func in func_result:
        print_seg_score(func[0], func[2])


'''
seg_sentence_dual 0.935859956846 0.884143384378 0.909266890678
seg_sentence_fmm 0.932449363124 0.880588307793 0.905777105404
seg_sentence_bmm 0.935129115334 0.883060971301 0.908349496512
'''
