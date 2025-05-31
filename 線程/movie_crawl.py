import requests
from bs4 import BeautifulSoup
import pandas as pd
import sys
import time
from concurrent.futures import ThreadPoolExecutor, as_completed

sys.stdout.reconfigure(encoding='utf-8')
start_time = time.time()

header = {
    'referer': 'https://ssr1.scrape.center/',
    'user-agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/128.0.0.0 Safari/537.36 Edg/128.0.0.0'
}

def crawl_page(page):
    url = f'https://ssr1.scrape.center/page/{page}'
    response = requests.get(url, headers=header)
    soup = BeautifulSoup(response.text, 'html.parser')
    result = soup.find_all(name='div', class_="p-h el-col el-col-24 el-col-xs-9 el-col-sm-13 el-col-md-16")
    score = soup.find_all(name='p', class_='score m-t-md m-b-n-sm')

    page_data = {'名稱': [], '類別': [], '國家': [], '時長': [], '上映時間': [], '分數': []}
    
    for i in range(len(result)):
        page_data['名稱'].append(result[i].h2.string)

        button_list = result[i].find_all(name='button', class_="el-button category el-button--primary el-button--mini")
        move_type = ','.join([btn.span.string for btn in button_list])
        page_data['類別'].append(move_type)

        info_list = result[i].find_all(name='div', class_='m-v-sm info')
        span_list = info_list[0].find_all(name='span')
        page_data['國家'].append(span_list[0].string)
        page_data['時長'].append(span_list[2].string)

        span_list = info_list[1].find_all(name='span')
        page_data['上映時間'].append(span_list[0].string if len(span_list) > 0 else '')
        page_data['分數'].append(score[i].string.strip())
    
    return page_data

# 使用 ThreadPoolExecutor 平行爬取
all_data = {'名稱': [], '類別': [], '國家': [], '時長': [], '上映時間': [], '分數': []}
with ThreadPoolExecutor(max_workers=5) as executor:
    futures = [executor.submit(crawl_page, page) for page in range(1, 11)]
    for future in as_completed(futures):
        page_data = future.result()
        for key in all_data:
            all_data[key].extend(page_data[key])

data = pd.DataFrame(all_data)
print(data)

end_time = time.time()
print(f"\n多線程執行時間：{end_time - start_time:.2f} 秒")
