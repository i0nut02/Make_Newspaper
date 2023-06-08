## Make_Newspaper
Make-Newspaper è un programma che permette all’utente di trasformare un file di testo in un giornale, il tutto potendo personalizzare a piacimento il formato delle pagine e delle colonne del giornale.

## Utilizzo
Dopo il download dei file del programma, l'utente potrà posizionarsi nella cartella di una delle due versioni del programma (uni-processo e multi-processo), dopo di che potrà creare l'eseguibile "makeNewspaper" digitando "make" all'interno del terminale, infine l'utente potrà eseguire il programma e visualizzare il formato dell'input atteso digitando "./makeNewpaper --help".

esempio:

<img width="596" alt="Schermata 2023-06-08 alle 14 57 25" src="https://github.com/i0nut02/Make_Newspaper/assets/99051485/febfc967-45d7-4829-9b3a-96172442f913">


## Archittetura del programma 
Il programma è diviso in tre processi principali:
```mermaid
graph LR
A[Lettura delle parole dei singoli paragrafi] --> B[Allocazione delle parole in una pagina di giornale dinamica]
B --> D[Scrittura righe nel giornale]
```
###  File :

 - #### main.c
	 - si occupa di leggere l’input dell’utente e inizializzare la struttura che ci permeterrà di gertire il file di output, la pagina di giornale e il suo formato.

- #### read_file.c
	- si occupa di leggere e salvare in una lista le parole fino alla terminazione di un paragrafo.

- #### make_newspaper.c:
	- si occupa di ricevere, allocare e giustificare le parole dei singoli paragrafi all'interno di una riga che rappresenta una riga della singola colonna.

- #### newspaper_manager.c: 
	- si occupa di gestire :
		- gli indici di scrittura della prossima riga e colonna della lista di stringhe che rappresenta la pagina di giornale.
		- la scrittura delle righe delle colonne su una lista di stringhe.
		- la scrittura della spaziatura tra colonne, paragrafi su una lista di stringhe.
		- la scrittura di una riga della lista di stringhe, e di una nuova pagina sul giornale.
		- chiusura del giornale (file di output).

- #### utils.c: 
	- si occupa di fornire funzioni di supporto agli altri file.
