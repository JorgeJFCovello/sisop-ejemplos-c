# Problema del barbero dormilón :sleeping: :barber:

![img](https://i.gifer.com/AsuW.gif)

Un barbero atiende clientes a medida que van llegando, pero mientras no hay nadie, se duerme.

- Cuando un cliente llega, le avisa al barbero que llegó. En ese momento, el barbero se despierta y le corta el pelo, cuando no hay más clientes, el barbero vuelve a dormir.

- Cuando el barbero atiende a un nuevo cliente, el asiento que ocupaba ese cliente se libera.

- Cuando llega un cliente, en caso de haber asientos, se queda esperando, en caso contrario, se va.

#

_Compilar_
```
gcc barberia.c -lpthread
```
_Nota: Es posible definir cantidad de asientos de la barbería enviándolo como parámetro al ejecutar_
