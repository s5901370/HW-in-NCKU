# 引入 requests 模組
import requests
import json


# r = requests.get('http://127.0.0.1:8000/T.S.Mall/')
# t = json.loads(r.text)
# print(t)

# r = requests.get('http://127.0.0.1:8000/FE21/')
# t = json.loads(r.text)
# print(t)

r = requests.get('http://127.0.0.1:8000/Ambassador/')
t = json.loads(r.text)
print(t)

# r = requests.get('http://127.0.0.1:8000/Nantai/')
# t = json.loads(r.text)
# print(t)

#r = requests.get('http://127.0.0.1:8000/Cmmovies_today/')
#t = json.loads(r.text)
#print(t)

#r = requests.get('http://127.0.0.1:8000/Cmmovies_beauty/')
#t = json.loads(r.text)
#print(t)

# url = ('http://192.168.0.3:8000/search_movie/')
# data = ({'movie':'MIB星際戰警：跨國行動'}) #movie是要找的電影，多個空白都不能的嚴格
# headers = {'content-type': 'application/json'}
# r = requests.post(url, data=json.dumps(data), headers=headers)
#print(r.text)
