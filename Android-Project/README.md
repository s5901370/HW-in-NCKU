# Android Project

## Frontend:
Platform: Android Studio

## Backend:
Platform: Python3 Virtual environment  
Tool:Django  
Install use model: 
```
pip install -r requirements.txt
```
Before running server, please check server and phone on the same domain.  
And change server_ip on Network class based on your ip address.  
Run server: 
```
cd server\mysite
python manage.py runserver 0.0.0.0:8000
```

## Contributors:
F74054114 陳暉元：
Connect app to the server , parse the json from the server.

F74052277 廖禹竣：
Build django server, crawl theater information and package info to json. Then send it to frontend.

F74056182 蔡博揚：
The frontend feature design in Android Studio.
