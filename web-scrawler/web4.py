# -*- coding: UTF-8 -*-
#康健
import requests
from bs4 import BeautifulSoup
import MySQLdb
conn = MySQLdb.connect(host="localhost",user="root", passwd="",db="zenbo109",charset="utf8")
cursor = conn.cursor()
# fo = open("web.txt","w")
origin='https://www.storm.mg'
ori = "https://www.storm.mg/authors/29701/健康醫療網/"
headers = {'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.102 Safari/537.36'}
a = 1
while(a<=34):	
	url = ori+str(a)
	res = requests.get(url,headers=headers)
	# res.encoding='utf-8'
	soup = BeautifulSoup(res.text,'html.parser')
	# articles = soup.select('h3.title a')
	re = soup.select('div.category_cards_wrapper')
	# for y in re:
	for x in re:
		for y in x.select('.category_card.card_thumbs_left'):
			href = y.find_all('a',href=True)[0]['href']
			res2 = requests.get((origin+href),headers)
			soup2 = BeautifulSoup(res2.text,'html.parser')
			re2 = soup2.select('#article_title')
			title = re2[0].text
			re3 = soup2.select('article')
			print('title is '+title)
			SQL="INSERT INTO article(title,keyword,words) VALUES(%s,%s,%s)"
			try:
				cursor.execute(SQL,(title,'',re3[0].text))
				conn.commit()
			except MySQLdb.Error as error:
				print(error)
	print(a)
	a=a+1

