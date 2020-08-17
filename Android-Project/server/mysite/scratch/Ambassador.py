import requests
from bs4 import BeautifulSoup
import re
import datetime


def ambassador():
    today = datetime.date.today()
    # print(today)

    tlist = str(today).split('-')
    # print(tlist)

    every = dict()

    page = 0
    for page in range(8):
        query = 'https://www.ambassador.com.tw/home/Showtime?ID=ace1fe19-3d7d-4b7c-8fbe-04897cbed08c&DT=' + tlist[0] + '/' + tlist[1] + '/' + str(int(tlist[2])+page)

        r = requests.get(query) #將此頁面的HTML GET下來
        #print(r.text) #印出HTML
        soup = BeautifulSoup(r.text,"html.parser") #將網頁資料以html.parser

        time = None
        day = soup.find_all('li', class_='has-submenu')
        for t in day:
            for tt in t.find_all('a'):
                if '當日' in tt.text:
                    # print(tt.text[4:])
                    time = tt.text[4:]


        mlist = list()
        dlist = list()

        name = soup.find_all('div', 'theater-box')
        # print(name)

        for t in name:
            for tt in t.find_all(['h3','p', 'h6']):
                if tt.name == 'h3':
                    mlist.append('###')
                    mlist.append(tt.text.strip())
                # print(tt.text)
                if tt.name == 'p':
                    c = str(dict(tt.attrs).get('class', ''))
                    # print(c[2:len(c)-2])
                    c = c[2:len(c)-2]
                    if c == 'tag-seat':
                        mlist.append(tt.text.strip())
                if tt.name =='h6':
                    mlist.append(tt.text.strip())
                # print('===================')

        w = (datetime.datetime.now().weekday()+page) % 7
        # print(tlist[0] + '/' + tlist[1] + '/' + str(int(tlist[2])+page))
        index = tlist[1] + '/' + str(int(tlist[2])+page)
        if w == 0:
            index = index + ' (一)'
        if w == 1:
            index = index + ' (二)'
        if w == 2:
            index = index + ' (三)'
        if w == 3:
            index = index + ' (四)'
        if w == 4:
            index = index + ' (五)'
        if w == 5:
            index = index + ' (六)'
        if w == 6:
            index = index + ' (日)'
        # print(index)
        # print(mlist)

        every[index] = mlist

    return every
    # print(every)
