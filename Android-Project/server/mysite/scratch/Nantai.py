import requests
from bs4 import BeautifulSoup
import re
import datetime

def nantai():
    every = dict()
    today = datetime.date.today()
    # print(today)

    tlist = str(today).split('-')
    # print(tlist)

    r = requests.get("http://www.nt-movie.com.tw/showtime.php") #將此頁面的HTML GET下來
    r.encoding='utf-8'
    #print(r.text) #印出HTML
    soup = BeautifulSoup(r.text,"html.parser") #將網頁資料以html.parser

    mlist = list()

    name = soup.find_all('div', class_='info')

    for t in name:
        for tt in t.find_all(['div', 'ul']):
            c = str(dict(tt.attrs).get('class', ''))
            # print(c[2:len(c)-2])
            c = c[2:len(c)-2]
            if c == 'movieTitle':
                # print(tt.text)
                mlist.append("###")
                mlist.append(tt.text)
            if c == 'times\', \'gray33':
                # print(tt.text)
                mlist.append(tt.text.strip())

    w = (datetime.datetime.now().weekday()) % 7
    index = tlist[1] + '/' + str(int(tlist[2]))
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

    # print(mlist)
    every[index] = mlist
    # print(every)
    return every

        
    