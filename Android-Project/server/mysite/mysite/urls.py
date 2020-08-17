"""mysite URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/2.2/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
import sys
sys.path.append("..")
from django.contrib import admin
from django.urls import path
from trips.views import *

urlpatterns = [
    path('admin/', admin.site.urls),
    path('T.S.Mall/', mall),
    path('FE21/', fe),
    path('Ambassador/', am),
    path('Nantai/', nantai),
    path('Cmmovies_today/', cm_today),
    path('Cmmovies_beauty/', cm_beauty),
    path('search_movie/', search),
]
