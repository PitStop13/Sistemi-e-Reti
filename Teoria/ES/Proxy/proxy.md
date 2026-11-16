# Relazione Tecnica: Server Proxy HTTP in Python

## 1. Parte Teorica

### Cos'è un Proxy Server
Un **Proxy** è un server che fa da intermediario tra un client (in questo caso il nostro browser web) e un server di destinazione (in questo caso `www.google.com`). Invece di collegarsi direttamente a Google, il browser invia la richiesta al proxy locale (`127.0.0.1:8888`); il proxy la inoltra a Google, riceve la risposta, la modifica al volo e la restituisce al browser.

Questo specifico programma si comporta come un **Reverse Proxy con riscrittura dei contenuti**: intercetta il traffico diretto verso un singolo server specifico e modifica il codice HTML ricevuto per impedire al browser di uscire dal proxy tramite link assoluti in HTTPS.

### I Socket e il livello di trasporto (TCP)
Per far comunicare processi diversi attraverso la rete si utilizzano i **Socket**, che rappresentano i punti terminali (endpoint) di una connessione. Nel programma vengono creati socket con due parametri fondamentali:
* `AF_INET`: indica l'utilizzo del protocollo di rete **IPv4**.
* `SOCK_STREAM`: indica l'utilizzo del protocollo di trasporto **TCP** (orientato alla connessione, affidabile, basato su flusso di byte).

Il programma gestisce contemporaneamente **due socket TCP distinti** per ogni richiesta:
1. **Socket Lato Client (`s_client`)**: mette in comunicazione il browser dell'utente con il programma Python sulla macchina locale (`localhost`).
2. **Socket Lato Server (`s_google`)**: mette in comunicazione il programma Python (che qui agisce da client) con il web server reale di Google sulla porta `80`.

### Struttura dei messaggi HTTP
Il protocollo HTTP (HyperText Transfer Protocol) è un protocollo testuale di livello Applicazione. Sia le richieste che le risposte hanno una struttura divisa in due parti, separate da una **riga vuota** identificata dalla sequenza di caratteri `\r\n\r\n` (Carriage Return + Line Feed ripetuti due volte):
1. **Header (Intestazione)**: contiene i metadati della comunicazione. Nella richiesta inizia con la *Request Line* (es. `GET /index.html HTTP/1.1`), seguita da campi come `Host`, `User-Agent` e `Accept-Encoding`. Nella risposta inizia con la *Status Line* (es. `HTTP/1.1 200 OK`), seguita da informazioni come `Content-Type` e `Content-Length`.
2. **Body (Corpo)**: contiene il carico utile (payload) vero e proprio, come il codice sorgente HTML della pagina o i byte di un'immagine.

---

## 2. Analisi del Codice Funzione per Funzione

### Costanti Globali
All'inizio dello script vengono definiti tre parametri di configurazione:
* `INDIRIZZO = '127.0.0.1'`: indirizzo IP di loopback (*localhost*), rende il server accessibile solo dal computer locale.
* `PORTA = 8888`: porta TCP su cui il processo si mette in ascolto.
* `BUFFER = 4096`: dimensione massima (in byte) di ogni singolo blocco letto tramite la funzione `recv()`.

---

### Funzione `scarica_da_google(percorso)`

* **Cosa fa**: Si occupa di aprire una connessione TCP verso il server di Google sulla porta 80 (HTTP in chiaro), inviare la richiesta per la risorsa desiderata e raccogliere tutti i pacchetti della risposta fino al termine della trasmissione.
* **Cosa le arriva (Input)**:
  * `percorso` (*stringa*): l'URI richiesto dal browser (ad esempio `'/'` per la homepage oppure `'/imghp'` per la sezione immagini).
* **Cosa succede passo passo**:
  1. Crea il socket `s_google` e stabilisce il *three-way handshake* TCP con `www.google.com` sulla porta `80` tramite `connect()`.
  2. Costruisce l'header HTTP di richiesta usando il metodo `GET`. Due righe sono fondamentali per il funzionamento del proxy:
     * `Accept-Encoding: identity`: obbliga Google a inviare il testo in chiaro e **non compresso** (senza `gzip`). Se Google inviasse dati compressi, non potremmo leggere e modificare i link nell'HTML.
     * `Connection: close`: chiede a Google di chiudere la connessione TCP appena finito l'invio della pagina, permettendo al nostro ciclo `while` di capire quando i dati sono terminati.
  3. Converte la stringa in byte (`encode('utf-8')`) e la trasmette con `sendall()`.
  4. Entra in un ciclo `while True` dove accumula nella variabile `risposta_grezza` i blocchi di byte letti con `recv(BUFFER)`. Quando `recv()` restituisce zero byte (`b""`), significa che Google ha chiuso la connessione e il ciclo termina.
  5. Chiude il socket `s_google`.
* **Cosa manda (Output / Return)**:
  * Restituisce `risposta_grezza` (*bytes*): l'intera risposta HTTP originale di Google (Header + Body).

---

### Funzione `modifica_risposta(risposta_grezza)`

* **Cosa fa**: Analizza la risposta ricevuta da Google, separa gli header dal body e, se il contenuto è una pagina web HTML, riscrive gli URL interni e ricalcola l'intestazione HTTP.
* **Cosa le arriva (Input)**:
  * `risposta_grezza` (*bytes*): i byte grezzi restituiti dalla funzione `scarica_da_google`.
* **Cosa succede passo passo**:
  1. Verifica la presenza del delimitatore `b"\r\n\r\n"`. Se non è presente, restituisce i dati così come sono.
  2. Tramite `split(b"\r\n\r\n", 1)` divide i byte in due parti al primo delimitatore trovato: `header_vecchi` e `corpo`. Il parametro `1` è fondamentale per evitare di spezzare anche il codice HTML nel caso contenesse righe vuote al suo interno.
  3. Controlla se negli header originali compare la dicitura `b"text/html"`:
     * **Se è una pagina HTML**: usa il metodo `.replace()` sui byte del `corpo` per rimuovere `https://www.google.com` (trasformando i link assoluti in link relativi che continueranno a passare per `127.0.0.1:8888`) e per declassare da `https://` a `http://` i domini dei file statici (`gstatic.com`). Successivamente, siccome la sostituzione del testo ha cambiato il numero di byte totali della pagina, costruisce `header_nuovi` calcolando il nuovo `Content-Length: {len(corpo)}`.
     * **Se NON è HTML** (es. un'immagine PNG, un logo o uno script): salta le modifiche per non corrompere i file binari.
* **Cosa manda (Output / Return)**:
  * Restituisce i *bytes* finali (gli header rigenerati + il body modificato se HTML, oppure la risposta originale intatta per gli altri tipi di file).

---

### Funzione `gestisci_client(s_client)`

* **Cosa fa**: Gestisce l'intero ciclo di vita della comunicazione con il browser per una singola richiesta, coordinando la lettura della domanda, la chiamata alle funzioni ausiliarie e l'invio della risposta finale.
* **Cosa le arriva (Input)**:
  * `s_client` (*oggetto socket*): il socket dedicato alla connessione attiva con il browser dell'utente.
* **Cosa succede passo passo**:
  1. Legge i byte inviati dal browser con `s_client.recv(BUFFER)`. Se la richiesta è vuota, chiude il socket ed esce.
  2. Converte i byte ricevuti in testo (`decode`) e isola la prima riga dell'header HTTP (es. `GET /search?q=test HTTP/1.1`).
  3. Divide la prima riga sugli spazi (`split(' ')`) ed estrae il secondo elemento (`elementi[1]`), che corrisponde al percorso richiesto dal browser.
  4. Passa il percorso a `scarica_da_google(percorso)` ottenendo i dati da remoto.
  5. Passa i dati ricevuti a `modifica_risposta()` per adattarli al proxy.
  6. Invia il pacchetto pronto al browser con `s_client.sendall(risposta_finale)`.
  7. Nel blocco `finally` (che viene eseguito sempre, anche in caso di errori di rete intercettati da `except`), chiude la connessione con il browser tramite `s_client.close()`.
* **Cosa manda (Output / Return)**:
  * Non ha un valore di `return` Python, ma **invia direttamente sulla rete** (attraverso il socket `s_client`) il flusso di byte della risposta HTTP verso il browser.

---

### Funzione `avvia_server()`

* **Cosa fa**: Inizializza il socket principale del server proxy, lo lega alla porta locale e mantiene attivo il ciclo infinito di ascolto delle connessioni in entrata.
* **Cosa le arriva (Input)**:
  * Nessun parametro in ingresso (utilizza le costanti globali `INDIRIZZO` e `PORTA`).
* **Cosa succede passo passo**:
  1. Crea il socket principale `s_proxy`.
  2. Imposta l'opzione `SO_REUSEADDR` a `1`: permette al sistema operativo di riutilizzare immediatamente la porta `8888` se il programma viene chiuso e riavviato subito, evitando il blocco temporaneo (`TIME_WAIT`) della porta TCP.
  3. Esegue il `bind((INDIRIZZO, PORTA))` per associare il processo all'IP `127.0.0.1` e alla porta `8888`.
  4. Esegue `listen(5)` per mettere il socket in stato di ascolto passivo, impostando una coda massima di 5 connessioni pendenti.
  5. Entra nel ciclo infinito `while True`: la chiamata bloccante `s_proxy.accept()` ferma l'esecuzione finché il browser non si collega. Quando arriva una richiesta, `accept()` restituisce il nuovo socket dedicato `s_client` e l'indirizzo del client `addr`, passando subito `s_client` alla funzione `gestisci_client()`.
* **Cosa manda (Output / Return)**:
  * Non restituisce valori (esegue un ciclo continuo fino all'interruzione manuale del programma).

---

## 3. Schema riassuntivo del flusso dei dati

```text
[Browser Utente] 
       |
       | 1. GET / HTTP/1.1 (su 127.0.0.1:8888)
       v
[gestisci_client()] ---> estrae il percorso "/"
       |
       v
[scarica_da_google("/")] 
       |
       | 2. GET / HTTP/1.1 (su [www.google.com:80](https://www.google.com:80) con Accept-Encoding: identity)
       v
[Server di Google] ---> restituisce Header + HTML originale
       |
       v
[modifica_risposta()] ---> sostituisce i link "https://" e ricalcola il Content-Length
       |
       | 3. Invia nuovi Header + HTML modificato tramite s_client.sendall()
       v
[Browser Utente] ---> visualizza la pagina passando sempre dal proxy
```