import sys
sys.path.append("..")
from django.shortcuts import render
from django.core import serializers
from django.http import HttpResponse
from django.http import JsonResponse
import json
from scratch import theater
from scratch import Ambassador
from scratch import Nantai
from scratch import Cmmovies

#南紡
data_mall = theater.Mall()
send_mall = json.dumps(data_mall, ensure_ascii=False)
#遠百
data_fe = theater.FE()
send_fe = json.dumps(data_fe, ensure_ascii=False) 
#國賓
data_ambassador = Ambassador.ambassador()
send_ambassador = json.dumps(data_ambassador, ensure_ascii=False) 
#南台
data_nantai = Nantai.nantai()
send_nantai = json.dumps(data_nantai, ensure_ascii=False)
#今日全美
today, beauty = Cmmovies.cm()
send_today = json.dumps(today, ensure_ascii=False)
send_beauty = json.dumps(beauty, ensure_ascii=False)



def mall(request):
    return JsonResponse(send_mall, safe=False, json_dumps_params={'ensure_ascii':False})

def fe(request):
    return JsonResponse(send_fe, safe=False, json_dumps_params={'ensure_ascii':False})

def am(request):
    return JsonResponse(send_ambassador, safe=False, json_dumps_params={'ensure_ascii':False})

def nantai(request):
    return JsonResponse(send_nantai, safe=False, json_dumps_params={'ensure_ascii':False})

def cm_today(request):
    return JsonResponse(send_today, safe=False, json_dumps_params={'ensure_ascii':False})

def cm_beauty(request):
    return JsonResponse(send_beauty, safe=False, json_dumps_params={'ensure_ascii':False})

def search(request):
    dest = dict()
    if request.method=='POST':
        print('post')
        received_json_data=json.loads(request.body)
        print(received_json_data)
        target = str(received_json_data['movie'])
        mall_dict = dict()
        #南紡搜尋
        for day, movie in data_mall.items():
            tmp_list = list()
            if target in movie:
                # print(movie)
                tmp_list = movie[movie.index(target):]
                # print(tmp_list.index('###'))
                if '###' in tmp_list:
                    tmp_list = tmp_list[: tmp_list.index('###')]
                # print(tmp_list)
                mall_dict[day] = tmp_list
        # print(tmp_dict)
        if bool(mall_dict) != False:
            dest['Mall'] = mall_dict
        #遠百搜尋
        fe_dict = dict()
        for day, movie in data_fe.items():
            tmp_list = list()
            if target in movie:
                tmp_list = movie[movie.index(target):]
                if '###' in tmp_list:
                    tmp_list = tmp_list[: tmp_list.index('###')]

                fe_dict[day] = tmp_list
        if bool(fe_dict) != False:
            dest['FE'] = fe_dict
        #國賓搜尋
        ambassador_dict = dict()
        for day, movie in data_ambassador.items():
            tmp_list = list()
            if target in movie:
                tmp_list = movie[movie.index(target):]
                if '###' in tmp_list:
                    tmp_list = tmp_list[: tmp_list.index('###')]
                ambassador_dict[day] = tmp_list
        if bool(ambassador_dict) != False:
            dest['Ambassador'] = ambassador_dict
        #南台搜尋
        nantai_dict = dict()
        for day, movie in data_nantai.items():
            tmp_list = list()
            if target in movie:
                tmp_list = movie[movie.index(target):]
                if '###' in tmp_list:
                    tmp_list = tmp_list[: tmp_list.index('###')]
                nantai_dict[day] = tmp_list
        if bool(nantai_dict) != False:
            dest['Nantai'] = nantai_dict
        #今日搜尋
        today_dict = dict()
        for day, movie in today.items():
            tmp_list = list()
            if target in movie:
                tmp_list = movie[movie.index(target):]
                if '###' in tmp_list:
                    tmp_list = tmp_list[: tmp_list.index('###')]
                today_dict[day] = tmp_list
        if bool(today_dict) != False:
            dest['Today'] = today_dict
        #全美搜尋
        beauty_dict = dict()
        for day, movie in beauty.items():
            tmp_list = list()
            if target in movie:
                tmp_list = movie[movie.index(target):]
                if '###' in tmp_list:
                    tmp_list = tmp_list[: tmp_list.index('###')]
                beauty_dict[day] = tmp_list
        if bool(beauty_dict) != False:
            dest['beauty'] = beauty_dict

        print(dest)
        


    return JsonResponse(dest, safe=False, json_dumps_params={'ensure_ascii':False})


