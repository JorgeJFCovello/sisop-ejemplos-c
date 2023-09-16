# Biblioteca Medrano
La biblioteca de Medrano busca sincronizar su sistema de reserva, retiro y devolución de algunos
libros frecuentemente pedidos para estudiar. Se sabe que se ofrecen para reserva 5 títulos distintos y
que hay un stock de 20 copias por cada uno.

Los clientes realizan la reserva de un libro siempre que haya copias disponibles, una vez que haya al
menos una copia del mismo lista para retirar se acerca a biblioteca y lo retira, por último estudia y
vuelve a biblioteca para devolverlo.

Reservas se encarga de, si hay reservas pendientes, tomar una copia y dejarla disponible para ser
retirada. Devoluciones se encarga de, si hay devoluciones, tomar una copia devuelta y registrarla
disponible nuevamente, cada 100 copias devueltas realiza un festejo que suele ser bastante largo.

**Variables compartidas:** reservas, retiros[5], devoluciones, contFestejo