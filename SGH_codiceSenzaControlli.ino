/*
Copyright 2016 Marzucco Angelo
Copyright 2016 Russo Andrea
Copyright 2016 Oppido Pasquale

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

//Progetto Smart Green House(Arduino Code)
//Il sistema colleziona i dati raccolti all'interno della serricoltura del basilico quali:
//1)ALTEZZA DELLA PIANTA da terra(istallazione del trasduttore di distanza a tempo di volo a 30 cm dal punto dove sono stati impiantati i semi);
//2)TEMPERATURA ED UMIDITà DELL'ARIA(mediante un unico modulo, DHT, il quale comunica digitalmente con Arduino);
//3)UMIDITà DEL TERRENO(tramite un trasduttore connesso ad una particolare sonda in grado di rilevare il tasso di umidità);
//4)LUMINOSITà(resistore LDR, il quale cambia valore assumendo resistenze dell'ordine dei MOhm al buoio o centinaia di Ohm in condizioni di piena luminosità).

//Il sistema invia mediante protocollo seriale i dati raccolti dall'impianto ad un dispositivo Bluetooth vers. 2.0 ed i medesimi possono essere visualizzati su uno smartphone tramite apposita applicazione

//N.B. Ogni singola riga del codice è stata commentata ai fini di aumentare la leggibilità del codice e di documentare lo stesso
#include <DHT.h>                     //libreria utile per controllare il sensore DHT per la lettura dell'umidità e la temperatura dell'ambiente
#include <SoftwareSerial.h>          //libreria atta a creare una porta di comunicazione seriale virtuale differente da quella costituita su Arduino UNO dai pin 0 ed 1(seriale via USB)

//Definizione dei pin analogico/digitali affidati alla lettura delle uscite prodotte dai sensori
#define dhtPin A0                 //collego il sensore DHT al pin A0
#define luxPin A1                 //collego il sensore di luminosità ambientale al pin A1
#define humGPin A2                //collego il sensore per monitorare l'umidità del terreno al pin A2
#define echo 3                    //seleziono i pin interessati allo studio dell'altezza della pianta tramite  HC-SR04
#define trigger 2                 //traduttore di tistanza a tempo di volo

//Definizione dei pin digitali affidati alla comunicazione seriale tramite Bluetooth(modulo HC-05)
#define rx_pin 10                //ingresso a cui collego il pin di trasmissione TX del modulo bluetooth
#define tx_pin 11                //uscita a cui collego il pin di ricezione dei dati RX del modulo bluetooth

//Definizione delle variabili globali del sistema
int luce = 0;               //variabile in cui vado a scrivere il valore della luminosità ambientale
int luce_analog = 0;        //variabile d'appoggio per la lettura analogica del valore della luminosità
int temp_aria = 0;          //"temp_aria" e "umidi_aria" mantengono i valori di temperatura ed umidità ambientale
int umidi_aria = 0;         //tirati fuori dal modulino DHT (valori estrapolati sfruttando la libreria realizzata ad hoc)
int umidi_terra = 0;        //variabile in cui vado a scrivere il valore dell'umidità percentuale del terreno
int umi_ter_analog = 0;     //variabile di appoggio per acquisire il segnale analogico riferito all'umidità del terreno da poi adattare ad un valore percentuale(%)
int durata = 0;             //variabile utile a calcolare la durata dell'impulso di eco nel trasduttore a tempo di volo
int distanza = 0;           //variabile utilizzata al fine di convertire la durata dell'eco in una distanza
int altezza = 0;            //variabile che immagazzina il valore dell'altezza della pianta.

DHT dht(dhtPin,DHT11);                                          //creazione istanza di DHT al fine di leggere temperatura ed umidità dell'aria(passo il pin ed il tipo di DHT## utilizzato)
SoftwareSerial bluetooth = SoftwareSerial(rx_pin,tx_pin);       //creo un istanza in grado di definire una porta seriale virtuale utile alla comunicazione bluetooth dei dati

void setup() {
Serial.begin(9600);             //abilito la porta seriale (COM#) per la lettura dei dati prodotti dal sistema(posso anche eliminare se non stampo i dati sul monitor seriale)
dht.begin();                    //abilito la lettura dati mediante il modulo DHT
bluetooth.begin(9600);          //abilito la comunicazione seriale virtuale, con baud 9600 bps, per interconnettermi al modulo bluetooth HC-05
pinMode(echo,INPUT);            //abilito I/O del trasduttore a tempo di volo (echo = IN, trigger = OUT)
pinMode(trigger,OUTPUT); 
}


void loop() {
temp_humid_control();
light_control();
humidity_control();
ready_to_collect();
invio_BLUETOOTH();           //abilito prima il trasferimento dei dati via bluetooth, poi li stampo sulla seriale(non necessario il SECONDO passaggio, OK ai fini del testing di unità)
//serial_print();
delay(20000);                //controllo lo stato del sistema ogni 20 secondi(evito così possibili errori di trasmissione quando mi connetto tramite smartphone al sistema)
}

//Funzione in grado di acquisire l'umidità e la temperatura dell'aria andando a scrivere i valori nelle variabili globali
void temp_humid_control(){
  temp_aria = dht.readTemperature();        //acquisisco i valori di temperarura ed umidità dell'aria
  umidi_aria = dht.readHumidity();          //scrivendoli nelle variabili globali
  delay(100);
}

//Funzione in grado di leggere il valore della luminosità ambientale andando a modificare il valore della vaiabile globale "luce"
void light_control(){
  luce_analog = analogRead(luxPin);              //leggo dal pin analogico il valore restituito dal trasduttore di luminosità
  luce = map(luce_analog, 0, 1023, 0, 100);      //rappresento tale valore su di una scala percentuale(da 0 a 100%)
  delay(100);
}

//Funzione in grado di leggere il valore percentuale dell'umidità del terreno
void humidity_control(){
  umi_ter_analog = analogRead(humGPin);                   //acquisisco il valore analogico riferito all'umidità del terreno
  umidi_terra = map(umi_ter_analog, 0, 1023, 100, 0);     //trasformo il valore analogico, in base alla caratteristica del trasduttore, e lo porto in un range 0-100%
  delay(100);
}

//Funzione in grado di calcolare l'altezza della pianta(distanza di istallazione dal suolo = 30 cm)
void ready_to_collect(){
  digitalWrite( trigger, LOW );              //invio un segnale alto per 10 ms  e verifico quale sia il suo eco sulla porta di ritorno del medesimo
 digitalWrite( trigger, HIGH );
 delayMicroseconds( 10 );
 digitalWrite( trigger, LOW );
 durata = pulseIn( echo, HIGH );            //calcolo mediante la funzione "pulseIn" la durata dell'impulso
 distanza = 0.034 * durata / 2;             //converto tale durata temporale in una distanza 
 altezza = 30 - distanza;                   //supponendo che il sensore si trovi al di sopra della pianta ed è istallato ad una altezza di 30 cm, l'altezza della pianta stessa viene calcolata
                                            //in maniera differenziale(30 cm sono una pura prova, potrebbe essere un'altezza maggiore o anche minore)
delay(100);
}


//Funzione per sintetizzare le varie primitive di stampa utilizzate all'interno del codice(stampo i miei dati sul monitor seriale in questo modo)
void serial_print(){
  Serial.print("Air Temperature[*C]: "); 
  Serial.println(temp_aria);
  delay(200);
  Serial.print("Air Humidity[%]: "); 
  delay(200);
  Serial.print("Brightness[%]: "); 
  Serial.println(luce);
  delay(200);
  Serial.print("Ground Humidity[%]: "); 
  Serial.println(umidi_terra);
  delay(200);
  Serial.print("Plant Height[cm]: "); 
  Serial.println(altezza);
  delay(200);
}


//Funzione in grado di trasmettere, tramite una comunicazione seriale, i dati del sistema via bluetooth ad un dispositvo esterno, quale ad esempio uno smartphone connesso al modulo HC-05
void invio_BLUETOOTH(){
  bluetooth.println(temp_aria);  //trametto il valore della temperatura dell aria
  delay(100);
  bluetooth.println(umidi_aria);  //trasmetto il valore dell'umidità dell'aria
  delay(100);
  bluetooth.println(luce);      //trasmetto il valore della luminosità ambientale
  delay(100);
  bluetooth.println(umidi_terra);  //trasmetto il valore dell'umidità del terreno
  delay(100);
  bluetooth.println(altezza);    //trasmetto il valore dell'altezza della pianta
  delay(100);
}


