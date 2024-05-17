import serial
import csv

ser = serial.Serial('COM3', 9600) # remplacer 'COM3' par le port série virtuel utilisé dans la simulation Proteus
filename = 'data.csv'

with open(filename, mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['TANK LEVEL', 'MOIST CONTENT', 'W-PUMP STATUS', 'T-PUMP STATUS']) # en-tête du fichier CSV
    while True:
        try:
            data = ser.readline().decode().strip().split(',') # lire les données série et les diviser en utilisant la virgule comme séparateur
            writer.writerow(data) # écrire les données dans le fichier CSV
        except KeyboardInterrupt:
            break
        except:
            pass

ser.close()