# hierarchical-heavy-hitters

> comando para ler arquivo pcap:

`tcpdump -q -n -t -r arquivo.pcap > data.txt`

- -q: quite mode
- -r: read mode
- -n: do not use names for address
- -t: do not print timestamp

## comandos
- `./tratador data.txt > input.txt`
- `./Offline_Overlap input.txt <phi> > output.txt`
- phi: numero real entre 0 e 1.
