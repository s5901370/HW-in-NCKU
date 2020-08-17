# -*- coding: UTF-8 -*-

import requests
from bs4 import BeautifulSoup
import MySQLdb
conn = MySQLdb.connect(host="localhost",user="root", passwd="",db="zenbo109",charset="utf8")
cursor = conn.cursor()
# fo = open("web.txt","w")

origin = 'https://www.everydayhealth.com.tw'
headers = {'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/70.0.3538.102 Safari/537.36'}
# res.encoding='utf-8'
# print(res.text)
# articles = soup.select('h3.title a')
l = ['earnosethroat','eye','tooth']
for aaa in l:
	SQL = "SELECT `symptom`FROM {} WHERE 1".format(aaa)
	cursor.execute(SQL)
	category = cursor.fetchall()
	for www in category:
		sympton = www[0]
		a=1
		while True:
			url = "https://www.everydayhealth.com.tw/tag/"+sympton+"/"+str(a)
			res = requests.get(url,headers=headers)
			soup = BeautifulSoup(res.text,'html.parser')

			re_test = soup.select('.search-container.no_result')
			if not re_test:
				re = soup.select('.articles-list-container')

				for x in re:
					for y in x.select('.list'):
						href = y.find_all('a',href=True)[0]['href']
						res2 = requests.get((origin+href),headers)
						soup2 = BeautifulSoup(res2.text,'html.parser')
						re2 = soup2.select('#article_page')
						title = y.select('.title')[0].text.strip()
						print('title is '+title)
						for z in re2:
							SQL="INSERT INTO article(title,keyword,words) VALUES(%s,%s,%s)"
							try:
								cursor.execute(SQL,(title,sympton,z.text))
							except MySQLdb.Error as error:
								print(error)

							# print(z.text)
						# print('END\nEND\nEND\n')
						# print('ref URL is '+href)
				a=a+1
				print(a)
				conn.commit()
			else:
				break




# for x in re:
# 	print('ref URL is '+x.find_all('a',href=True)[0]['href'])
# 	print('title is '+x.select('.title')[0].text)
	# print(x)
# print(re.select('.font18.no-overflow')[0])
# 	print(x['href'])
# fo.write(soup.text.encode("utf8").decode("cp950", "ignore")) #prevent error message
# fo.close()


