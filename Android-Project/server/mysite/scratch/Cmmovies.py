import requests
from bs4 import BeautifulSoup
import re
from urllib.parse import unquote

page = list()
today = dict()
beauty = dict()


def cm():
    r = requests.get("https://www.cm-movie.com.tw/category/time/") #將此頁面的HTML GET下來
    soup = BeautifulSoup(r.text,"html.parser") #將網頁資料以html.parser

    for t in soup.find_all('a', class_='tc-grid-bg-link'):
        page.append(t.get('href'))

    # print(page)

    for p in page:
        r = requests.get(p) #將此頁面的HTML GET下來
        #print(r.text) #印出HTML
        soup = BeautifulSoup(r.text,"html.parser") #將網頁資料以html.parser

        mlist = list()
        # name = soup.find_all('div', id = 'content')

        # for t in name:
        #     print(t.text)
        for tt in soup.find_all(['strong', 'span', 'b', 'h4']):
            # print(str(dict(tt.attrs).get('name', '')))
            if tt.text != '':
                if tt.name == 'span':
                    c = str(dict(tt.attrs).get('style', ''))[:9]
                    if c == 'font-size':
                        mlist.append(tt.text)
                if tt.name == 'b':
                    mlist.append('###')
                    mlist.append(tt.text)
                if tt.name == 'h4':
                    mlist.append(tt.text)
                if tt.name == 'strong':
                    if tt.text == '今日戲院' or tt.text == '全美戲院':
                        mlist.append(tt.text)


        day = unquote(p)
        day = day[33: len(day) - 1]
        # print(day)
        #today
        today[day] = mlist[mlist.index('今日戲院')+1 : mlist.index('全美戲院')]
        # print(mlist[mlist.index('今日戲院')+1 : mlist.index('全美戲院')])

        #beauty
        beauty[day] = mlist[mlist.index('全美戲院')+1 : ]

    return today, beauty



