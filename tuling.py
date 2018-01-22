#encoding=utf-8 

import json, urllib
import re,sys
import os
from aip import AipSpeech

#""" 你的 APPID AK SK """
APP_ID = '10728169'
API_KEY = 'HPbFPjx7VCZw7qP7iGst3Pb6'
SECRET_KEY = 'b4042fd51f84d232d72e61ea86d8692f'
client = AipSpeech(APP_ID, API_KEY, SECRET_KEY)
client.setConnectionTimeoutInMillis(30000)

def yuyinhecheng(tuling_result):
    result  = client.synthesis(tuling_result, 'zh', 1, {'vol': 5,})
    # 识别正确返回语音二进制 错误则返回dict 参照下面错误码
    if not isinstance(result, dict):
        with open('auido.mp3', 'wb') as f:
            f.write(result)
            os.system("mpg123 auido.mp3")

def tuling(duihua):
    key = '486d3637a0174d23b786bf1c68c685b8' #图灵机器人key，暂时使用我的
    api = 'http://www.tuling123.com/openapi/api?key=' + key + '&info='
    request = api + duihua
    page = urllib.urlopen(request)
    response = page.read()
    dic_json = json.loads(response)
    tuling_result = dic_json['text']
    print(tuling_result)  #获取图灵机器人的回复
    yuyinhecheng(tuling_result)

if __name__ == "__main__":
    while True:
        words = sys.stdin.readline().strip()
        if len(words)==0:
            continue
        tuling(words)
