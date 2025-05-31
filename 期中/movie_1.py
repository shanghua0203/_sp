import requests
from bs4 import BeautifulSoup
import pandas as pd
import sys
import time

sys.stdout.reconfigure(encoding='utf-8')
start_time = time.time()

header = {
    'referer':'https://ssr1.scrape.center/',
    'user-agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/128.0.0.0 Safari/537.36 Edg/128.0.0.0'
}
movie_info = {'名稱':[],'類別':[],'國家':[],'時長':[],'上映時間':[],'分數':[]}
for page in range(1, 11):
    respons = requests.get('https://ssr1.scrape.center/page/%d' %page, headers=header)

    soup = BeautifulSoup(respons.text, 'html.parser')
    result = soup.find_all(name='div', class_="p-h el-col el-col-24 el-col-xs-9 el-col-sm-13 el-col-md-16")
    for i in range(len(result)):
        movie_info['名稱'].append(result[i].h2.string)
        button_list = result[i].find_all(name='button', class_="el-button category el-button--primary el-button--mini")
        move_type = ''
        for btn in button_list:
            move_type += btn.span.string + ','
        movie_info['類別'].append(move_type)
        info_list = result[i].find_all(name='div', class_='m-v-sm info')
        span_list = info_list[0].find_all(name='span')
        movie_info['國家'].append(span_list[0].string)
        movie_info['時長'].append(span_list[2].string)
        span_list = info_list[1].find_all(name='span')
        if len(span_list) > 0:
            movie_info['上映時間'].append(span_list[0].string)
        else :
            movie_info['上映時間'].append('')
        score = soup.find_all(name = 'p', class_='score m-t-md m-b-n-sm')
        movie_info['分數'].append(score[i].string.strip())

end_time = time.time()  # 記錄結束時間
data = pd.DataFrame(movie_info)
print(data)
print(f"\n執行時間：{end_time - start_time:.2f} 秒")

