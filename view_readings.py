
import pyrebase #librería para manejar Firebase con python

config = { #estos identificadores corresponden a los publicados en la wiki 
"apiKey" : "AIzaSyC6y5rPcMl6RU7kMSaSRhd4m6ccrF0siNs",
"authDomain" : "pipin-ca62a.firebaseapp.com",
"databaseURL": "https://pipin-ca62a-default-rtdb.firebaseio.com/",
"projectId" : "pipin-ca62a",
"storageBucket" : "pipin-ca62a.appspot.com",
"messagingSenderId" : "272936523214",
"appId" : "1:272936523214:web:87a26d7bdd965f2bd3479f",
"measurementId" : "G-9YW4HNKKNK"
}

firebase = pyrebase.initialize_app(config) #se mandan los identificadores 
db = firebase.database() #se accesa a la base de datos de fyrebase
info = db.child("readings").get() #entrar a la ruta específica donde están los elementos
print("Inicializando lectura de base de datos")
print()
for item in info.each(): #imprime para cada elemento en readings/
    if(item.key()=="numero"): #caso especifico para el dato _numero_
        if(item.val()>9):
            print(" Numero mayor que 9")
        else:
            print(f' Numero = {item.val()}')    
    elif("temperaturaC" == item.key()): #caso para temperatura
        print(f' Elemento {item.key()} = {item.val()}°C')
    elif("temperaturaF" == item.key()): #caso para temperatura 
        print(f' Elemento {item.key()} = {item.val()}°F')
    elif("humedad" == item.key()): #caso para humedad 
        print(f' Elemento {item.key()} = {item.val()}%')
    elif("distancia" == item.key()): #caso para distancia
        print(f' Elemento {item.key()} = {item.val()} cm.')
    elif("isFlame" == item.key()): #caso para el booleano que indica si hay flama
        if(item.val()==False):
            print(" No hay presencia de flama")
        else:
            print(" Hay presencia de flama")
    elif("movimiento"==item.key()): #caso para booleano de movimiento
        if(item.val()==False):
            print(" No hay presencia de movimiento")
        else:
            print(" Hay presencia de movimiento")
print()
print("Lectura finalizada. Gracias!")
