Copyright 2021 Diaconu Tudor-Gabriel

Tema 1 IOCLA 2021

Am ales sa citesc fiecare linie cu un fgets, salvand intregul string si
parsandu-l cu ajutorul lui sscanf in variabile care reprezinta comanda, 
numele fisierului/directorului si noul nume pentru comanda mv, apeland
functiile conform parametrilor ceruti. Am implementat acel while, constient
fiind de faptul ca aplicatia se opreste la intalnirea comenzii stop.

In continuare, voi explica sumar implementarea functiilor principale ale
aplicatiei.

TOUCH:
Functia touch initializeaza un pointer la noul fisier. De asemenea, printr-un
alt pointer, parcurge listele de directori si fisiere pentru a vedea daca nu
exista deja un fisier/director cu numele dorit. In cazul in care nu exista,
se adauga noul fisier la finalul listei de fisiere sau la inceput(in cazul in
care lista era goala).

MKDIR:
Functia mkdir functioneaza intr-un mod similar functiei de touch, insa
diferenta o face faptul ca vom adauga noul director la finalul listei de
directoare.

LS:
Functia ls parcurge ambele liste ale folderului parinte(cea de fisiere si
cea de directoare) si printeaza numele fiecarui element gasit.

RM:
Functia rm parcurge lista de fisiere din directorul parinte si in momentul
in care gaseste fisierul dorit(dupa nume), ii nulifica legaturile si elibereaza
memoria alocata pentru numele fisierului si pentru fisier.

RMDIR:
Functia rmdir parcurge lista de directoare din directorul parinte si in
momentul in care gaseste directorul dorit(dupa nume). In momentul in care
gasim directorul pe care dorim sa il stergem, golim directoarele copil ale
directorului pe care il stergem, prin functia recursiva care este apelata
pentru toate directoarele din lista de directoare. Aceasta functioneaza
asemanator unui DFS pe un graf, adica, cat timp lista de directoare a
directorului pe care il stergem inca mai are directoare, vom patrunde in
lista de directoare a acestuia pana cand ajungem la un director gol. In
momentul in care dam de un director descendent gol, ii stergem fisierele si
il stergem si pe el, urmandu-se aceasta operatie pe cale ascendenta a
structurii arborescente formata de sistemul de directoare prin functia
recursiva.

CD:
Functia cd intampina 2 cazuri. Primul caz este cel in care ii este oferit ca
parametru string-ul "..", lucru in care target-ul se muta pe parinte. Cel de-al
doilea caz este cel in care se da numele folderului pe care vrem sa ne mutam,
moment in care, folderul respectiv este cautat in lista de directoare, iar in
cazul in care este gasit, target-ul se muta pe el.

PWD:
Pentru functia de pwd, m-am folosit de un vector de string-uri, initial gol.
Apoi, am parcurs calea ascendenta, de la directorul pe care ne aflam pana la
home, adaugand in vectorul de string-uri, pe rand, numele fiecarui folder prin
care ne-am deplasat si tinand minte si numarul de foldere pe care le-am parcurs.
Asadar, vectorul de string-uri rezultat va avea, in ordine inversa, calea de la
directorul curent pana la home. Apoi, am parcurs invers vectorul, folosindu-ma
de faptul ca stiam numarul de string-uri din vector, concatenand la string-ul
pe care il returnam ca rezultat numele folder-ului si slash-urile aferente,
pentru a se pastra forma dorita.

STOP:
Functia stop elibereaza recursiv folder-ul dat ca parametru, in modul descris
la explicatia functiei RMDIR.

TREE:
Functia tree afiseaza structura arborescenta a unui folder. Aceasta parcurge
listele de directoare, respectiv fisiere si le afiseaza numele, initial afisand
tabul corespunzator respectarii formatului cerut. In cazul directoarelor,
operatia de tree este una recursiva, intrucat vrem sa afisam in aceasta
structura si ceea ce se afla in interiorul lor.

MV:
Functia mv afla printr-o functie daca fisierul dorit a fi modificat este
director sau fisier. Apoi, ii schimba numele directorului/fisierului,
stergandu-l din interiorul listei si readaugandu-l la finalul listei.

Feedback tema:
O tema foarte ok care m-a ajutat sa imi amintesc anumite concepte legate de
pointeri si de liste. Kudos!