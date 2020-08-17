import requests
from bs4 import BeautifulSoup
import re

r = requests.get("http://www.skcinemas.com/MovieList.aspx") #將此頁面的HTML GET下來
#print(r.text) #印出HTML
soup = BeautifulSoup(r.text,"html.parser") #將網頁資料以html.parser

# name = soup.find_all('td', class_='dxdvItem')

for t in soup.find_all('a'):
    link = t.get('href')
    if str(link)[:11] == 'MovieDetail':
        print(link)