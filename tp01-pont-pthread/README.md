# TP1 — Le pont en POSIX

Un pont supporte au maximum 15 tonnes. Les camions pèsent 15 t, les voitures 5 t.
L'accès est géré par un moniteur (mutex + variables de condition).

- `question1.c` : moniteur simple (acquisition / libération)
- `question2.c` : priorité aux camions

## Compilation et exécution

```bash
gcc question1.c -o question1 -pthread && ./question1
gcc question2.c -o question2 -pthread && ./question2
```