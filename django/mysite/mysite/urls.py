from django.conf.urls import *
from mysite.views import *

urlpatterns = patterns('',
    ('^hello/$', hello),
)
