#!/usr/bin/python
# -*- coding:utf-8 -*-

import logging
import sys

'''
处理语料原始数据：人民日报199801
python convert_corpus.py | sed 's/\/$//' > 199801_new.txt
tr '/' '\n' < 199801_new.txt | sort | uniq > 199801_word.txt
shuf 199801_new.txt > 199801_shuf.txt
head -2000 199801_shuf.txt > 199801_test.txt
tail -n +2001 199801_shuf.txt > 199801_train.txt
tr '/' '\n' < 199801_train.txt | sort | uniq > 199801_word_train.txt
'''

def convert_corpus_file(filename):
    '''
    处理语料文件
    原始数据一行是一个句子，用两个空个分隔各项。第一项是句子标识予以忽略。后续各项包括词及其词性标注，以斜杠分隔。
    个别复合词用[]包围，需要单独处理。
    数据输出到标准输出。
    '''
    with open(filename) as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            items = line.split('  ')
            items = items[1:]
            for item in items:
                if not item or item[0] == '/':
                    logging.warning('empty word: ' + line)
                    continue
                sep = item.find('/')
                if sep == -1:
                    logging.warning('invalid format: ' + line)
                    continue
                sys.stdout.write(item[0:sep] if item[0] != '[' else item[1:sep])
                sys.stdout.write('/')
            sys.stdout.write('\n')

if __name__ == '__main__':
    convert_corpus_file('199801.txt')
