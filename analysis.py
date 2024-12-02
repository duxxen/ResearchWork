import re
import os
import pandas as pd
import matplotlib.pyplot as plt

respath: str = '.\\x64\\Debug\\result.txt'
exepath: str = '.\\x64\\Debug\\ResearchWork.exe'

class data:
    def __init__(self, type: str, size: int, time: float):
        self.type = type
        self.size = size
        self.time = time

if __name__ == '__main__':
    file = open(respath, 'r')
    content = file.read()
    
    re.findall()

    file.close()