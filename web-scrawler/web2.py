# -*- coding: UTF-8 -*-
#早安健康
import requests
from bs4 import BeautifulSoup
import MySQLdb
conn = MySQLdb.connect(host="localhost",user="root", passwd="",db="zenbo109",charset="utf8")
cursor = conn.cursor()
# fo = open("web.txt","w")
ori = "https://www.everydayhealth.com.tw/article/"
headers = {'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.102 Safari/537.36'}
a = 12700
while(a<21000):	

	url = ori+str(a)
	res = requests.get(url,headers=headers)
	# res.encoding='utf-8'
	soup = BeautifulSoup(res.text,'html.parser')
	# articles = soup.select('h3.title a')
	re = soup.select('#article_page')
	if re == []:
		a=a+1
	else:
		a=a+1
		title = soup.select('h1.title')[0].text.strip()
		print('title is '+title)
		for z in re:
			SQL="INSERT INTO article(title,keyword,words) VALUES(%s, %s ,%s)"			
			try:
				cursor.execute(SQL,(title,'',z.text))
				conn.commit()
			except MySQLdb.Error as error:
				print(error)
			

