
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
flag=False; #este bool se usa para detectar el cambio en usuario e imprimir un aviso del mismo
for item in info.each(): #imprime para cada elemento en readings/
    if(item.key()=="usuario"):
        print(f' Elemento {item.key()} = {item.val()}')
    if(item.key()=="numero"): #caso especifico para el dato _numero_
        if(item.val()>9):
            db.child("readings").update({"usuario":"elemento mayor a 9"}) #hace el cambio en campo usuario
            flag=True;
            print(" Numero mayor que 9... ")
        else:
            print(f' Numero = {item.val()}')    
    elif("temperaturaC" == item.key()): #caso para temperatura
        print(f' Elemento {item.key()} = {item.val()}°C')
    elif("temperaturaF" == item.key()): #caso para temperatura 
        print(f' Elemento {item.key()} = {item.val()}°F')
    elif("humedad" == item.key()): #caso para humedad 
        print(f' Elemento {item.key()} = {item.val()}%')
    elif("distance" == item.key()): #caso para distancia
        print(f' Elemento {item.key()} = {item.val()} cm.')
    elif("isFlame" == item.key()): #caso para el booleano que indica si hay flama
        if(item.val()==False):
            print(" No hay presencia de flama")
        else:
            print(" Hay presencia de flama")
    elif("movement"==item.key()): #caso para booleano de movimiento
        if(item.val()==False):
            print(" No hay presencia de movimiento")
        else:
            print(" Hay presencia de movimiento")
print()
if(flag==True):
    print("Campo de usuario se actualizará a 'elemento mayor a 9'") #en proxima impresión el cambio se reflejará, en esta no porque toda la información se descargó antes de la modificación
print("Lectura finalizada. Gracias!")
