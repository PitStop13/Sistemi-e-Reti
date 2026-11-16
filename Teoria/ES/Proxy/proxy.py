import socket

INDIRIZZO = '127.0.0.1'
PORTA = 8888 #QUanti 8?
BUFFER = 4096

def scaricaDaGoogle(percorso):
    # creo il socket per parlare con il server di google sulla porta 80
    s_google = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s_google.connect(('www.google.com', 80))

    # preparo l'header HTTP forzando i dati non compressi (identity)
    richiesta = (
        f"GET {percorso} HTTP/1.0\r\n" #1.1 lascia abilitati i chunked transfer encoding (compaiono in esadecimale le dimensioni del rispettivo blocco), 1.0 no
        "Host: www.google.com\r\n"
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
        "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
        "Accept-Encoding: identity\r\n"
        "Connection: close\r\n\r\n"
    )
    s_google.sendall(richiesta.encode('utf-8'))

    # leggo la risposta a blocchi finche google non chiude la connessione, serve per lettura completa della risposta senza perdere pezzi
    risposta_grezza = b""
    while True:
        blocco = s_google.recv(BUFFER)
        if not blocco:
            break
        risposta_grezza += blocco

    s_google.close()
    return risposta_grezza


def modificaRisposta(risposta_grezza):
    # controllo se c'e la separazione standard tra header e body
    if b"\r\n\r\n" not in risposta_grezza:
        return risposta_grezza

    parti = risposta_grezza.split(b"\r\n\r\n", 1)
    header_vecchi = parti[0]
    corpo = parti[1]

    # modifico i link solo se sto scaricando una pagina html e non un'immagine,altrimenti faccio un pastis che si sformatta tutto
    if b"text/html" in header_vecchi:
        corpo = corpo.replace(b"https://www.google.com", b"")
        corpo = corpo.replace(b"https://ssl.gstatic.com", b"http://ssl.gstatic.com")
        corpo = corpo.replace(b"https://www.gstatic.com", b"http://www.gstatic.com")

        # rifaccio l'header aggiornando la lunghezza del nuovo body
        header_nuovi = (
            "HTTP/1.0 200 OK\r\n"
            "Content-Type: text/html; charset=ISO-8859-1\r\n"
            f"Content-Length: {len(corpo)}\r\n"
            "Connection: close\r\n\r\n"
        ).encode('utf-8')

        return header_nuovi + corpo

    return risposta_grezza


def gestisciClient(s_client):
    try:
        dati_client = s_client.recv(BUFFER)
        if not dati_client:
            s_client.close()
            return

        # converto in stringa per tirare fuori il percorso dalla prima riga (es. GET / HTTP/1.1)
        testo = dati_client.decode('utf-8', errors='ignore')
        righe = testo.split('\r\n')
        prima_riga = righe[0]

        if prima_riga != "":
            elementi = prima_riga.split(' ')
            if len(elementi) > 1:
                percorso = elementi[1]
            else:
                percorso = '/'

            print("Richiesta ricevuta per:", percorso)

            risposta_google = scaricaDaGoogle(percorso)
            risposta_finale = modificaRisposta(risposta_google)

            s_client.sendall(risposta_finale)

    except Exception as errore:
        print("Errore durante la gestione della richiesta:", errore)

    finally:
        s_client.close()


def avviaServer():
    s_proxy = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # evita l'errore "address already in use" se riavvio subito il programma
    s_proxy.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    s_proxy.bind((INDIRIZZO, PORTA))
    s_proxy.listen(5)

    print(f"Server proxy in ascolto su http://{INDIRIZZO}:{PORTA}")

    while True:
        s_client, addr = s_proxy.accept()
        gestisciClient(s_client)


if __name__ == "__main__":
    avviaServer()