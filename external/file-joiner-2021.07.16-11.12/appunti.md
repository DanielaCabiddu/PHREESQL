# tabella con info input
- posso avere più di una solution contemporaneamente?
- ho i seguenti dati:
    - database
    - title
- una tabella chiave/valore (ph - 8.3 ---- Ca - 300) ordinate, in modo da ricorstruire il file anche con lo stesso ordine.

--> creo due tabelle:
- tabella METADATA, che ha due colonne, chiave/valore (che sono due stringhe)
    -   in questa tabella, vado ad infilare 
        - DATABASE=C:\Program Files (x86)\USGS\Phreeqc Interactive 3.6.1-15000\database\wateq4f.dat
        - TITLE=123437382 PdG
        - NUMBER_OF_SOLUTIONS=1
- tabella SOLUTION_INPUT, che ha come colonne:
    - id autoincement int
    - numero soluzione
    - unit
    - ppm







# simulazioni
le simulazioni sembrerebbero essere tutte uguali.
avrà la tabella simulazioni, in cui mi tengo:
- input
- output
- database
- altri metadati?

e poi, per ogni sezione di interesse, avrò una tabella associata.
Ad esempio, la tabella "Distribution of species" (associata ovviamente ad una certa simulazione).

DOMANDA: ma è importante solo la Distribution of species o sono importanti anche altre sezioni?

DOMANDA: la questione della posizione geografica? potrebbe anche essere un'aggiunta ai metadati della singola simulazione.