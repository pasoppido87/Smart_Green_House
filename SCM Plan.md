# SCM Plan

- **Introduzione**:

	Si vuole automatizzare il controllo di un impianto di serra per la coltivazione di basilico a scopo didattico.
Il sistema verrà realizzato mediante l'ausilio di Arduino ed Android Studio(al fine di sviluppare un applicativo per smartphone); in virtù di tale scelta progettuale, gli ambienti di sviluppo SW saranno gli IDE di Arduino e di Android Studio.
Si vogliono controllare altezza della pianta, temperatura ed umidità dell'aria, umidità del terreno e luminosità esterna. L'impianto prevede la possibilità di controllare l'attivazione dei processi di attuazione mediante un controllo automatico(gestito mediante Arduino) o manuale(pilotando gli stessi tramite app Android connessa al sistema via bluetooth). E' possibile, ad ogni modo, visualizzare lo stato dell'impianto tramite l'applicazione Android dal proprio smartphone.

- **Management**
    
   Il team di sviluppo è organizzato come segue:
  * Configuration Manager : Russo Andrea;
  * Change control board member: Oppido Pasquale e Marzucco Angelo;
  * Developer: Russo, Oppido, Marzucco;
  * Auditor: Russo, Oppido, Marzucco.

	Le responsabilità sono distribuite in maniera uniforme in quanto essendo ridotto il numero di compomenti del team risulta inutile una suddivisione netta degli sviluppatori.
Ognuno degli stessi contribuirà equamente allo sviluppo del sistema SW, sul quale verranno effettuate successivmente le normali attività di testing.


- **Regole di naming**: 
 	* funzioni: il nome delle funzioni deve essere con la lettera minuscola e nel caso di nomi composti separati da underscore.
	* variabili: il nome delle variabili è assegnato usando un approccio CamelCase. Le variabili devono avere un minimo di quattro lettere. 

- **Schedule**:

	La prima versione funzionante del software (in grado di soddisfare tutti i requisiti) verrà rilasciata entro il 16/07/2016.
        I costi del sistema stimati sono inferiori a 100 euro (escludendo impianti di attuazione quali pompe di irrigazione, lampade UV, sistemi di riscaldamento e di ventilazione).
        In particolare devono essere implementate le seguenti funzioni in Arduino:
       
 * manual_control(): si occupa di abilitare i segnali i controllo per l'attuazione manualmente, tramite le informazioni fornite dall'applicazione Android e catturate dal sistema Arduino attraverso un modulo Bluetooth 2.0
 * auto_control(): si occupa di controllare la serricoltura autonomanente mediante controllo a ciclo chiuso tramite sensori. In particolare in tale modalità viene ciclato in cotrollo tramite le seguenti funzionalità:
      1.  light_control() : si occupa di regolare l'illuminazione della serra dalla lettura di un sensore crepuscolare.
	    2.  umidity_gnd_control() : si occupa di regolare l'umidità del terreno dalla lettura di un sensore di umidità e con il controllo di una pompa di irrigazione.
	    3.  temp_humid_control() : si occupa di controllare la temperatura e l'umidità dell'aria attivando/disattivando i segnali di controllo per i sistemi di riscaldamento e/o di umidificazione dell'aria.
        4.  ready_to_collect() :  si occupa tenere sotto controllo l'altezza della piantina tramite l'uso di un sensore ad ultrasuoni, al fine di segnalare la pronta raccolta al raggiungimento di una certa altezza della piantina.
        
      Per quanto riguarda l'applicazione Android, la medesima deve fornire le seguenti funzionalità: 
      * connessione bluetooth ad Arduino per interfacciarsi correttamente alla serricoltura;
      * possibilità di leggere lo stato dell'impianto e segnalare eventuale superamento delle soglie di ottimale lavoro nella serricoltura;
      * possibilità di abilitare il controllo manuale dell'impianto, con annesso pilotaggio dei segnali di controllo verso gli attuatori all'interno della serra; in tale caso deve essere prevista anche la possibilità di ritornare in modalità di controllo automatica della serricoltura.

- **Tool Utilizzati**:

	Per il sistema di versioning verrà utilizzato GitHub ed il repository del progetto si trova al link [GitHub](https://github.com/pasoppido87/Smart_Green_House) .
Sulle singole macchine sono stati istallati l'IDE di Arduino e l'IDE di Android Studio per lo sviluppo SW e SourceTree, linkato all'account GitHub di ogni singolo sviluppatore; quest'ultimo gestisce i commit locali ed i push fatti in remoto riferiti al progetto.
