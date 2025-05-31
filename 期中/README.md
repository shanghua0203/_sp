# 111210505吳尚樺期中作業

[爬取的網站](https://ssr1.scrape.center/)  
- 有使用chatGPT教我該如何使用Thread工具  
- movie_1.py是之前學習爬蟲時寫的程式

我在程式結束的最後段加上了執行時間  
未使用多線程時：
```
                                   名稱            類別  ...           上映時間   分數
0        霸王别姬 - Farewell My Concubine        剧情,爱情,  ...  1993-07-26 上映  9.5
1                      这个杀手不太冷 - Léon     剧情,动作,犯罪,  ...  1994-09-14 上映  9.5
2   肖申克的救赎 - The Shawshank Redemption        剧情,犯罪,  ...  1994-09-10 上映  9.5
3                     泰坦尼克号 - Titanic     剧情,爱情,灾难,  ...  1998-04-03 上映  9.5
4                罗马假日 - Roman Holiday     剧情,喜剧,爱情,  ...  1953-08-20 上映  9.5
..                                ...           ...  ...            ...  ...
95             大闹天宫 - The Monkey King        动画,奇幻,  ...  1965-12-31 上映  9.0
96                    天空之城 - 天空の城ラピュタ     动画,奇幻,冒险,  ...  1992-05-01 上映  9.0
97          音乐之声 - The Sound of Music  剧情,爱情,歌舞,传记,  ...  1965-03-02 上映  9.0
98          辛德勒的名单 - Schindler's List     剧情,历史,战争,  ...  1993-11-30 上映  9.5
99             魂断蓝桥 - Waterloo Bridge     剧情,爱情,战争,  ...  1940-05-17 上映  9.5

[100 rows x 6 columns]

執行時間：2.16 秒
```
使用多線程後
```
                                              名稱  ...   分數
0                                     飞屋环游记 - Up  ...  8.8
1            黑客帝国3：矩阵革命 - The Matrix Revolutions  ...  8.8
2                             速度与激情5 - Fast Five  ...  8.9
3                              勇敢的心 - Braveheart  ...  8.9
4                             三傻大闹宝莱坞 - 3 Idiots  ...  8.9
..                                           ...  ...  ...
95                   一一 - Yi yi: A One and a Two  ...  8.8
96                        美丽人生 - La vita è bella  ...  9.1
97  海上钢琴师 - La leggenda del pianista sull'oceano  ...  9.1
98                               千与千寻 - 千と千尋の神隠し  ...  9.1
99                   迁徙的鸟 - The Travelling Birds  ...  9.1

[100 rows x 6 columns]

執行時間：0.54 秒
```

### 使用多線程之後速度大概是使用多線程的四倍快