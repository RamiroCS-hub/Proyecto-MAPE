from firebase import firebase
from sys import path
path.append("C:\\Users\\Usuario\\Desktop\\Proyecto MAPE\\emailSender")
from emailAviso import emailSender
from datetime import datetime

firebase = firebase.FirebaseApplication("https://fir-test-50790-default-rtdb.firebaseio.com/",None)

resultado = firebase.get("\Alarma", None)
print(resultado)
if resultado == None:
    now = datetime.now()	
    time = now.strftime("%B %d %Y %H:%M:%S")
    emailSender(time,"ramirocarnicersouble8@gmail.com")
