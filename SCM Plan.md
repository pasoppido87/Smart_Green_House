#SCM Plan

- Introduzione:

	Si vuole automatizzare il controllo di un impianto di serra per la coltivazione di basilico a scopo didattico.
Il sistema verrà realizzato mediante l'ausilio di Arduino; in virtù di tale scelta progettuale, l'ambiente di sviluppo SW sarà lo stesso IDE di Arduino.
Si vogliono controllare altezza della pianta, temperatura dell'aria, umidità del terreno e luminosità esterna.

- Management
Il team di sviluppo è organizzato come segue:
  * Configuration Manager : Russo Andrea;
  * Change control board member: Oppido Pasquale e Marzucco Angelo;
  * Developer: Russo, Oppido, Marzucco;
  * Auditor: Russo, Oppido, Marzucco.

	Le responsabilità sono distribuite in maniera uniforme in quanto essendo ridotto il numero di compomenti del team risulta inutile una suddivisione netta degli sviluppatori.
Ognuno degli stessi contribuirà equamente allo sviluppo del sistema SW, sul quale verranno effettuate successivmente le normali attività di testing.


- Regole di naming: 
 	* funzioni: il nome delle funzioni deve essere con la lettera minuscola e nel caso di nomi composti separati da underscore.
	* variabili: il nome delle variabili è assegnato usando un approccio CamelCase. Le variabili devono avere un minimo di quattro lettere. 

 - Schedule:

	La prima versione funzionante del software deve essere rilasciata entro il 30/06/2016. In particolare entro questa data devono essere implementate le seguenti funzioni:
	* light_control() : si occupa di regolare l'illuminazione della serra dalla lettura di un sensore crepuscolare.
	* umidity_control() : si occupa di regolare l'umidità del terreno dalla lettura di un sensore di umidità e con il controllo di una pompa di irrigazione.
	* ready_to_collect() : si occupa tenere sotto controllo l'altezza della piantina tramite l'uso di un sensore ad ultrasuoni, al fine di segnalare la pronta raccolta al raggiungimento di una certa altezza della piantina.

- Tool Utilizzati:

	per il sistema di versioning verrà utilizzato GitHub ed il repository del progetto si trova al link: https://github.com/pasoppido87/Smart_Green_House .
Sulle singole macchine è stato istallato l'IDE di Arduino per lo sviluppo SW e SourceTree, linkato all'account GitHub di ogni singolo sviluppatore; quest'ultimo gestisce i commit locali ed i push fatti in remoto riferiti al progetto.
