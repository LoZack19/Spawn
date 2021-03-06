# Spawn
Utilizzo di spawn per programmi grafici:  
```bash
$ spawn [PROGRAMMA] [ARGOMENTI] 2>/dev/null
```
In questo modo l'output su terminale verrà ignorato. Se invece volete lanciare un'app che deve avere un output sul terminale potete usare questo stesso comando per non visualizzare i messaggi di errore. Altrimenti scrivere:
```bash
$ spawn [PROGRAMMA] [ARGOMENTI]
```

Una volta spawnato il processo potrete anche chiudere il terminale, e il processo resterà aperto.

# Installazione
Per installare spawn bisogna compilarlo con `gcc main.c -o spawn` e installarlo con `sudo install spawn /usr/bin`.  
Alternativamente utilizzare lo script `./install`