import asyncio
import aiohttp
from bs4 import BeautifulSoup
import pandas as pd
import time
import sys

sys.stdout.reconfigure(encoding='utf-8')

header = {
    'referer': 'https://ssr1.scrape.center/',
    'user-agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/128.0.0.0 Safari/537.36 Edg/128.0.0.0'
}

async def fetch(session, url):
    async with session.get(url, headers=header) as response:
        return await response.text()

async def crawl_page(session, page):
    url = f'https://ssr1.scrape.center/page/{page}'
    html = await fetch(session, url)
    soup = BeautifulSoup(html, 'html.parser')

    result = soup.find_all(name='div', class_="p-h el-col el-col-24 el-col-xs-9 el-col-sm-13 el-col-md-16")
    score = soup.find_all(name='p', class_='score m-t-md m-b-n-sm')

    page_data = {'名稱': [], '類別': [], '國家': [], '時長': [], '上映時間': [], '分數': []}

    for i in range(len(result)):
        page_data['名稱'].append(result[i].h2.string)

        buttons = result[i].find_all(name='button', class_="el-button category el-button--primary el-button--mini")
        move_type = ','.join(btn.span.string for btn in buttons)
        page_data['類別'].append(move_type)

        info_list = result[i].find_all(name='div', class_='m-v-sm info')
        span_list1 = info_list[0].find_all(name='span')
        page_data['國家'].append(span_list1[0].string)
        page_data['時長'].append(span_list1[2].string)

        span_list2 = info_list[1].find_all(name='span')
        page_data['上映時間'].append(span_list2[0].string if len(span_list2) > 0 else '')

        page_data['分數'].append(score[i].string.strip())

    return page_data

async def main():
    start_time = time.time()
    async with aiohttp.ClientSession() as session:
        tasks = [crawl_page(session, page) for page in range(1, 11)]
        results = await asyncio.gather(*tasks)

    # 合併所有頁面的結果
    final_data = {'名稱': [], '類別': [], '國家': [], '時長': [], '上映時間': [], '分數': []}
    for data in results:
        for key in final_data:
            final_data[key].extend(data[key])

    df = pd.DataFrame(final_data)
    print(df)
    print(f"\n非同步執行時間：{time.time() - start_time:.2f} 秒")

# 執行主程序
if __name__ == "__main__":
    try:
        # 嘗試取得已存在的事件迴圈
        loop = asyncio.get_event_loop()
        loop.run_until_complete(main())
    except RuntimeError as e:
        # 若遇到 event loop 已關閉，建立一個新的 loop 再執行
        if str(e).startswith("Event loop is closed"):
            new_loop = asyncio.new_event_loop()
            asyncio.set_event_loop(new_loop)
            new_loop.run_until_complete(main())

