import requests
from bs4 import BeautifulSoup
import re


def Mall():
    r = requests.get("http://www.vscinemas.com.tw/vsweb/theater/detail.aspx?id=16") #將此頁面的HTML GET下來
    #print(r.text) #印出HTML
    soup = BeautifulSoup(r.text,"html.parser") #將網頁資料以html.parser
    name = soup.find_all('h1')
    # ename = soup.find_all('h2')
    # time = soup.find_all('ul', class_='bookList')
    day = soup.find_all('div', class_='slidesArea')
    mlist = list()
    tlist = list()
    dlist = list()


    # tmp = soup.find_all('div', class_='theaterTime')
    # print(tmp)



    # day = soup.select('div.dataBanner > div.slidesArea > a[href^="/#movieTime"]')
    # print(day)
    for t in day:
        b = t.find_all('a')
        # print(b)
        for tt in b:
            # print(tt.get('href'))
            dlist.append(tt.get('href'))
            tlist.append(tt.text)

    # print(dlist)
    # print(tlist)

    length = len(dlist)

    for d in range(length):
        t = dlist[d][1:]
        mlist.append('@@@')
        mlist.append(tlist[d])
        # print(t)
        # tmp = soup.find_all('article', id=t)
        for tt in soup.find_all('article', id=t):
            # print(tt.find_all('h1'))
            for name in tt.find_all(['h1', 'h4', 'ul']):
                # print(name.name)
                if name.name == 'h1':
                    mlist.append("###")
                    # print("=========================================")
                mlist.append(name.get_text().strip())



            

    # print(mlist)
    j = arrangement(mlist)
    
    return j

def FE():
    r = requests.get("https://www.vscinemas.com.tw/vsweb/theater/detail.aspx?id=15") #將此頁面的HTML GET下來
    #print(r.text) #印出HTML
    soup = BeautifulSoup(r.text,"html.parser") #將網頁資料以html.parser
    name = soup.find_all('h1')
    # ename = soup.find_all('h2')
    # time = soup.find_all('ul', class_='bookList')
    day = soup.find_all('div', class_='slidesArea')
    mlist = list()
    tlist = list()
    dlist = list()


    # tmp = soup.find_all('div', class_='theaterTime')
    # print(tmp)



    # day = soup.select('div.dataBanner > div.slidesArea > a[href^="/#movieTime"]')
    # print(day)
    for t in day:
        b = t.find_all('a')
        # print(b)
        for tt in b:
            # print(tt.get('href'))
            dlist.append(tt.get('href'))
            tlist.append(tt.text)

    # print(dlist)
    # print(tlist)

    length = len(dlist)

    for d in range(length):
        t = dlist[d][1:]
        mlist.append('@@@')
        mlist.append(tlist[d])
        # print(t)
        # tmp = soup.find_all('article', id=t)
        for tt in soup.find_all('article', id=t):
            # print(tt.find_all('h1'))
            for name in tt.find_all(['h1', 'h4', 'ul']):
                # print(name.name)
                if name.name == 'h1':
                    mlist.append("###")
                    # print("=========================================")
                mlist.append(name.get_text().strip())



            

    # print(mlist)
    j = arrangement(mlist)
    return j

def arrangement(movie):
    index = list()
    every = list()
    j = dict()
    i = 0
    for tmp in movie:
        if tmp == '@@@':
            index.append(i)
        i= i + 1

    # print(index)
    i = 0
    length = len(index)
    for i in range(length - 1):
        # print(movie[(index[i]+1) : index[i+1]])
        every.append(movie[(index[i]+1) : index[i+1]])
    # print(movie[(index[len(index) - 1]+1):])
    every.append(movie[(index[len(index) - 1]+1):])

    # print(every)
    for tmp in every:
        j[tmp[0]] = tmp[1:]

    # print(j)
    return j
            

if __name__ == "__main__":
    Mall()
