
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

//Prelevati i medesimi, IN MODALITà AUTOMATICA, lo stesso è in grado di abilitare:
//1)IRRIGAZIONE TERRENO, nelle ore fresche(luminosità bassa) e se il terreno scende sotto una determinata soglia di umidità(35% nel caso);
//2)ATTIVAZIONE LAMPADE UV, quando la luminosità decresce mediante un controllo ad isteresi su tale parametro;
//3)ATTIVAZIONE SISTEMA DI RISCALDAMENTO, se la temperatura scende sotto la soglia dei 18 gradi(abilitazione pilotata da un controllo ad isteresi).
//4)ATTIVAZIONE SITEMA DI UMIDIFICAZIONE ARIA(tramite ventilazione) se l'umidità dell'aria scende sotto una determinata soglia

//In MODALITà MANUALE il sistema è in grado di pilotare, tramite applicazione Android connessa al bluetooth dell'impianto, 1),2),3),4) abilitando e disabilitando degli switch
//presenti all'interno dell'applicazione (sezione CONTROLLO MANUALE).

//Il sistema invia mediante protocollo seriale i dati raccolti dall'impianto ad un dispositivo Bluetooth vers. 2.0 ed i medesimi possono essere visualizzati 
//sullo smartphone tramite l'apposita applicazione(sezione dell'applicazione Smart Green House -> LETTURA DATI)

//N.B. Ogni singola riga del codice è stata commentata ai fini di aumentare la leggibilità del codice e di documentare lo stesso
#include <DHT.h>                      //libreria utile per controllare il sensore DHT per la lettura dell'umidità e la temperatura dell'ambiente (DHT library -> Adafruit vers. 1.2.3)
#include <SoftwareSerial.h>          //libreria atta a creare una porta di comunicazione seriale virtuale differente da quella costituita su Arduino UNO dai pin 0 ed 1(seriale via USB)

//Definizione dei pin analogico/digitali affidati alla lettura delle uscite prodotte dai sensori
#define dhtPin A0            //collego il sensore DHT al pin A0
#define luxPin A1           //collego il sensore di luminosità ambientale al pin A1
#define humGPin A2          //collego il sensore per monitorare l'umidità del terreno al pin A2
#define echo 3              //seleziono i pin interessati allo studio dell'altezza della pianta tramite  HC-SR04
#define trigger 2           //traduttore di tistanza a tempo di volo

//Definizione dei pin digitali a cui sono affidati i controlli via arduino dell'impianto
#define luce_UV 8           //pin ON/OFF luce ultravioletta notturna all'interno della serra
#define riscalda_ven 5      //attivazione riscaldamento al di sotto di una determinata temperatura
#define raccolta 4          //segnalamento possibilità di cimatura/raccolta della pianta di basilico
#define irriga 6            //abilito un segnale che ci permette di irrigare il terreno in determinate condizioni climatiche(in funzione di umidità del terreno e della luminosità)
#define umidifica 7         //abilito un segnale che ci permette di attivare/disattivare un sistema di umidificazione ambietale provvisto di una ventola
#define led_manual 12       //definisco un LED di stato che indica il tipo di controllo effettuato sull'impianto(ON -> MANUALE, OFF -> AUTOMATICO)

//Definizione dei pin digitali affidati alla comunicazione seriale tramite Bluetooth(modulo HC-05)
#define rx_pin 10          //ingresso a cui collego il pin di trasmissione TX del modulo bluetooth
#define tx_pin 11          //uscita a cui collego il pin di ricezione dei dati RX del modulo bluetooth

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

char message = 0;     //carattere utile per catturare i dati inviati tramite Bluetooth dall app android e controllare i parametri del sistema(modalità MANUALE/AUTOMATICA, in MANUALE attivazione di 1),2),3),4))
int manual;           //variabile intera utile a controllare la modalità in cui funziona l'intero impianto (utilizzata nel loop() per ovviare a tale operazione)

DHT dht(dhtPin,DHT11);                                          //creazione istanza di DHT al fine di leggere temperatura ed umidità dell'aria(passo il pin ed il tipo di DHT## utilizzato)
SoftwareSerial bluetooth = SoftwareSerial(rx_pin,tx_pin);       //creo un istanza in grado di definire una porta seriale virtuale utile alla comunicazione bluetooth dei dati

void setup() {
//Serial.begin(9600);             //abilito la porta seriale (COM#) per la lettura dei dati prodotti dal sistema(posso anche eliminare se non stampo i dati sul monitor seriale)
dht.begin();                    //abilito la lettura dati mediante il modulo DHT
bluetooth.begin(9600);          //abilito la comunicazione seriale virtuale, con baud 9600 bps, per interconnettermi al modulo bluetooth HC-05
pinMode(echo,INPUT);            //abilito I/O del trasduttore a tempo di volo (echo = IN, trigger = OUT)
pinMode(trigger,OUTPUT); 
manual = 0;                    //inizializzo manual=0 per far partire l'impianto da una condizione di controllo AUTOMATICO mediante apposite funzionalità presenti nel codice

//Imposto come output i pin digitali
pinMode(luce_UV,OUTPUT);           //uscita per abilitare l'illuminazione UV nella serra
pinMode(riscalda_ven, OUTPUT);     //uscita per abilitare la ventola con annesso sistema di riscaldamento nella serra
pinMode(raccolta,OUTPUT);          //uscita per lanciare un allert a favore della cimatura/raccolta della pianta
pinMode(irriga,OUTPUT);            //uscita volta ad attivare il ssitema di irrigazione del terreno
pinMode(umidifica,OUTPUT);         //uscita utile ad abilitare il sistema di umidificazione dell'aria
digitalWrite(umidifica,LOW);      
digitalWrite(luce_UV,LOW);
digitalWrite(riscalda_ven,LOW);
digitalWrite(raccolta,LOW);
digitalWrite(irriga,LOW);

pinMode(led_manual,OUTPUT);   // LED di stato per indicare il tipo di controllo effettuato sul sistema
digitalWrite(led_manual,LOW);
}

void loop() {
    stato_impianto();                //aggiornamento variabili contenenti lo stato(parametri d'interesse) dell'impianto
    selection_auto_manual();         //verifico il tipo di controllo da effettuare sull'impianto
    if(manual) manual_control();     //se manual = 1, abilito il controllo manuale tramite l'applicazione android(piloto opportunamente gli switch)
    else auto_control();             //se manual = 0, Arduino controllo autonomamente l'impianto
    invio_bluetooth();               //trasferisco, tramite bluetooth, i valori acquisiti riferiti allo stato della serra e della pianta
   }

//Funzione in gradi di aggiornare i parametri riferiti allo stato dell'impianto

void stato_impianto(){
  temp_aria = dht.readTemperature();        //acquisisco i valori di temperarura ed umidità dell'aria
  umidi_aria = dht.readHumidity();          //scrivendoli nelle variabili globali
  luce_analog = analogRead(luxPin);              //leggo dal pin analogico il valore restituito dal trasduttore di luminosità
  luce = map(luce_analog, 0, 1023, 0, 100);     //rappresento tale valore su di una scala percentuale(da 0 a 100%)
  umi_ter_analog = analogRead(humGPin);                   //acquisisco il valore analogico riferito all'umidità del terreno
  umidi_terra = map(umi_ter_analog, 0, 1023, 100, 0);     //trasformo il valore analogico, in base alla caratteristica del trasduttore, e lo porto in un range 0-100%

 //controllo altezza mediante le soglie digitali inviate da parte del sistema(echo e trigger)
 digitalWrite( trigger, LOW );             //invio un segnale alto per 10 ms  e verifico quale sia il suo eco sulla porta di ritorno del medesimo
 digitalWrite( trigger, HIGH );
 delayMicroseconds( 10 );
 digitalWrite( trigger, LOW );
 durata = pulseIn( echo, HIGH );           //calcolo mediante la funzione "pulseIn" la durata dell'impulso
 distanza = 0.034 * durata / 2;            //converto tale durata temporale in una distanza 
 altezza = 30 - distanza;     //supponendo che il sensore si trovi al di sopra della pianta ed è istallato ad una altezza di 30 cm, l'altezza della pianta stessa viene calcolata
                              //in maniera differenziale(30 cm sono una pura prova, potrebbe essere un'altezza maggiore o anche minore)
}


//Funzione in grado di controllare la temperatura e l'umidità dell'aria

void temp_humid_control(){
  //Controllo temperatura della serra
  if(temp_aria < 18){
    digitalWrite(riscalda_ven,HIGH);       //attivazione sistema areazione-riscaldamento
  }
  else if(temp_aria >= 22){
    digitalWrite(riscalda_ven,LOW);        //disattivazione sistema areazione-riscaldamento
  }
    //Controllo umidità aria della serra
  if(umidi_aria < 45 ){              
    digitalWrite(umidifica,HIGH);           //attivazione sistema umidificazione ambiente
  }
  else if(umidi_aria > 75){
    digitalWrite(umidifica,LOW);         //diattivazione sistema umidificazione ambiente
  }
  
}

//Funzione in grado di pilotare l'accensione/spegnimento dell'impianto di illuminazione ultravioletto in base alle diverse condizioni di luce

void light_control(){
  //Controllo sull'accensione dell'illuminazione UV all'interno della serra (controllo ad ISTERESI: 1) attivo con luminosità < 40% 2) disattivo con luminosità >=42% (evito sfarfallii UV)
  if(luce < 40){
    digitalWrite(luce_UV,HIGH);        //attivo luce UV
  }
  else if(luce >= 42){
    digitalWrite(luce_UV,LOW);        //disattivo luce UV
  }
 }

//Funzione in grado di pilotare il sistema automatico di irrigazione della serra
void humidity_gnd_control(){
  
  //Controllo irrigazione del terreno: la medesima viene attivata anche in funzione della luminosità ambientale(irrigazione nelle ore fresce della giornata)
  if((umidi_terra < 35)&&(luce < 45)){
    digitalWrite(irriga,HIGH);   //attiva l'irrigazione della serra
  }
  else if(umidi_terra > 60){
    digitalWrite(irriga,LOW);    //disattiva l'irrigazione della serra
  }
}

//Funzione in gradi di abilitare un LED di stato (nel controllo autonomo da parte di Arduino della serra) in relazione all'altezza della piantina di basilico
void ready_to_collect(){
 
 //Controllo raccolta/cimatura del basilico
 if(altezza >= 20){                         //se l'altezza è maggiore di 20 cm, è possibile raccogliere/cimare la pianta
  digitalWrite(raccolta,HIGH);             //abilito un LED di allerta per segnalare la possibilità di cimare/raccogliere la pianta
 }
 else{
  digitalWrite(raccolta,LOW);              //altezza non sufficiente per la cimatura/raccolta del basilico
 }
 }                             


//Funzione per visulaizzare sul monitor seriale i valori attribuiti alle variabili di stato della serra
void serial_print(){
  Serial.print("Air Temperature[*C]: "); 
  Serial.println(temp_aria);
  delay(500);
  Serial.print("Air Humidity[%]: ");
  Serial.println(umidi_aria);
  delay(500);
  Serial.print("Brightness[%]: "); 
  Serial.println(luce);
  delay(500);
  Serial.print("Ground Humidity[%]: "); 
  Serial.println(umidi_terra);
  delay(500);
  Serial.print("Plant Height[cm]: "); 
  Serial.println(altezza);
  delay(500);
}

//Funzione in grado di trasmettere, tramite una comunicazione seriale, i dati del sistema via bluetooth all'applicazione Android connessa al modulo HC-05
void invio_bluetooth(){  //invio una determinata stringa di dati che inizia per * e termina con # al fine di condificare correttamente tramite l'app l'inizio e la fine trasmissione dati
  bluetooth.print("*");
  bluetooth.print(temp_aria);  //trametto il valore della temperatura dell aria
  bluetooth.print(" ");
  bluetooth.print(umidi_aria);  //trasmetto il valore dell'umidità dell'aria
  bluetooth.print(" ");
  bluetooth.print(luce);      //trasmetto il valore della luminosità ambientale
  bluetooth.print(" ");
  bluetooth.print(altezza);  //trasmetto il valore dell'umidità del terreno
  bluetooth.print(" ");
  bluetooth.print(umidi_terra);    //trasmetto il valore dell'altezza della pianta
  bluetooth.print(" ");
  bluetooth.println("#");
  delay(1000);            //inserisco un ritardo di 1 secondi al fine di evitare l'intasamento del buffer seriale su cui trasmetto i dati
                         //N.B. pur aggiornando l app Android, la medesima rileva i vecchi pacchetti inviati. In questo modo vado ad ottenere sempre il valore attuale dello stato sull APP
                         //svantaggio: ritardo di 1 secondi l'abilitazione dei vari switch in modalità di CONTROLLO MANUALE(tramite app) sul sistema
}

//Funzione in grado di abilitare i controlli sull'impianto tramite il pilotaggio di opportuni switch presenti nell'applicazione Android nella finestra CONTROLLO MANUALE
void manual_control(){
  
  while(bluetooth.available()){
    message = bluetooth.read();   //quando invio dati tramite bluetooth accumulo i caratteri trasmetti nella stringa "message"
  }
  
  if(!bluetooth.available()){
    if(message == '3'){               //attivazione LUCE UV via bluetooth tramite l'app
     digitalWrite(luce_UV,HIGH);             
    }
    else if(message == '2'){          //spegnimento LUCE UV via bluetooth tramite l'app
    digitalWrite(luce_UV,LOW);             
    }
    else if (message == '5'){         //attivazione UMIDIFICAZIONE ARIA via bluetooth tramite l'app
    digitalWrite(umidifica,HIGH);           
    }
    else if(message == '4'){          //spegnimento UMIDIFICAZIONE ARIA via bluetooth tramite l'app
     digitalWrite(umidifica,LOW);           
    }
     else if (message == '7'){        //attivazione IRRIGAZIONE TERRENO via bluetooth tramite l'app
     digitalWrite(irriga,HIGH);      
    }
    else if (message == '6'){        //spegnimento IRRIGAZIONE TERRENO via bluetooth tramite l'app
     digitalWrite(irriga,LOW);         
     
    }
    else if (message == '9'){             //attivazione RISCALDAMENTO ARIA via bluetooth tramite l'app
     digitalWrite(riscalda_ven,HIGH);       
    }
    else if (message == '8'){             //disattivazione RISCALDAMENTO ARIA via bluetooth tramite l'app
     digitalWrite(riscalda_ven,LOW);        
    }
       }
  }

//Funzione utile ad istanziare il controllo automatico mediante Arduino della serricoltura del basilico
void auto_control(){
      temp_humid_control();      //abilito le funzionalità ed i relativi controllo come visto nelle descrizione delle funzioni vista nei passi precedenti
      light_control();
      humidity_gnd_control();
      ready_to_collect();
     }

//Funzione, dipendente dai comandi inviati tramite l'applicazione Android, in grado di selezionare il tipo di controllo applicato alla serra
void selection_auto_manual(){
  while(bluetooth.available()){
  message = bluetooth.read();   //quando invio dati tramite bluetooth accumulo i caratteri trasmetti nella stringa "message"
  }
  if(message == '0') {      //il sistema permane nella condizione di controllo automatico dell impianto
  manual = 0;
  digitalWrite(12,LOW);
  }
  else if(message == '1'){   //il sistema viene controllato manualmente tramite l'applicazione Android
  manual = 1;
  digitalWrite(12,HIGH);
  digitalWrite(umidifica,LOW);      //resetto lo stato dei controlli applicati nella condizione di controllo automatico
  digitalWrite(luce_UV,LOW);
  digitalWrite(riscalda_ven,LOW);
  digitalWrite(irriga,LOW);
  
}
}

void testOk(){  //invio una determinata stringa di dati che inizia per * e termina con # al fine di condificare correttamente tramite l'app l'inizio e la fine trasmissione dati
  //stringa per testare la "Lettura Dati" entro i range di corretto lavoro della serricoltura
  bluetooth.print("*");
  bluetooth.print(20);
  bluetooth.print(" ");
  bluetooth.print(50);
  bluetooth.print(" ");
  bluetooth.print(50);
  bluetooth.print(" ");
  bluetooth.print(15);
  bluetooth.print(" ");
  bluetooth.print(50);
  bluetooth.print(" ");
  bluetooth.println("#");
  delay(1000);  
}

void testTemp15(){  //invio una determinata stringa di dati che inizia per * e termina con # al fine di condificare correttamente tramite l'app l'inizio e la fine trasmissione dati
  //stringa per testare la "Lettura Dati"; Temperatura sotto il range di normale funzionamento
  bluetooth.print("*");
  bluetooth.print(15);      //temperatura out of bound
  bluetooth.print(" ");
  bluetooth.print(50);
  bluetooth.print(" ");
  bluetooth.print(50);
  bluetooth.print(" ");
  bluetooth.print(15);
  bluetooth.print(" ");
  bluetooth.print(50);
  bluetooth.print(" ");
  bluetooth.println("#");
  delay(1000);  
}

void testUmidita80(){  //invio una determinata stringa di dati che inizia per * e termina con # al fine di condificare correttamente tramite l'app l'inizio e la fine trasmissione dati
  //stringa per testare la "Lettura Dati"; Umidità Aria sopra il range di normale funzionamento
  bluetooth.print("*");
  bluetooth.print(20);
  bluetooth.print(" ");
  bluetooth.print(80);        //umidità out of bound
  bluetooth.print(" ");
  bluetooth.print(50);
  bluetooth.print(" ");
  bluetooth.print(15);
  bluetooth.print(" ");
  bluetooth.print(50);
  bluetooth.print(" ");
  bluetooth.println("#");
  delay(1000);  
}

void testUmidita40(){  //invio una determinata stringa di dati che inizia per * e termina con # al fine di condificare correttamente tramite l'app l'inizio e la fine trasmissione dati
  //stringa per testare la "Lettura Dati"; Umidità Aria sotto il range di normale funzionamento
  bluetooth.print("*");
  bluetooth.print(20);
  bluetooth.print(" ");
  bluetooth.print(40);       //umidità out of bound
  bluetooth.print(" ");
  bluetooth.print(50);
  bluetooth.print(" ");
  bluetooth.print(15);
  bluetooth.print(" ");
  bluetooth.print(50);
  bluetooth.print(" ");
  bluetooth.println("#");
  delay(1000);  
}

void testLuce35(){  //invio una determinata stringa di dati che inizia per * e termina con # al fine di condificare correttamente tramite l'app l'inizio e la fine trasmissione dati
  //stringa per testare la "Lettura Dati"; Luminosità ambientale sotto il range di normale funzionamento
  bluetooth.print("*");
  bluetooth.print(20);
  bluetooth.print(" ");
  bluetooth.print(50);
  bluetooth.print(" ");
  bluetooth.print(35);   //Luce out of bound
  bluetooth.print(" ");
  bluetooth.print(15);
  bluetooth.print(" ");
  bluetooth.print(50);
  bluetooth.print(" ");
  bluetooth.println("#");
  delay(1000);  
}

void testAltezza22(){  //invio una determinata stringa di dati che inizia per * e termina con # al fine di condificare correttamente tramite l'app l'inizio e la fine trasmissione dati
  //stringa per testare la "Lettura Dati"; Altezza pianta sopra il range di raccolta/cimatura
  bluetooth.print("*");
  bluetooth.print(20);
  bluetooth.print(" ");
  bluetooth.print(50);
  bluetooth.print(" ");
  bluetooth.print(50);
  bluetooth.print(" ");
  bluetooth.print(22);    //altezza out of bound
  bluetooth.print(" ");
  bluetooth.print(50);
  bluetooth.print(" ");
  bluetooth.println("#");
  delay(1000);  
}

void testAllAlert(){  //invio una determinata stringa di dati che inizia per * e termina con # al fine di condificare correttamente tramite l'app l'inizio e la fine trasmissione dati
  //stringa per testare la "Lettura Dati"; Tutti i parametri sotto il range di normale funzionamento
  bluetooth.print("*");
  bluetooth.print(15); 
  bluetooth.print(" ");
  bluetooth.print(30);
  bluetooth.print(" ");
  bluetooth.print(30);
  bluetooth.print(" ");
  bluetooth.print(25);
  bluetooth.print(" ");
  bluetooth.print(20);
  bluetooth.print(" ");
  bluetooth.println("#");
  delay(1000);  
}
