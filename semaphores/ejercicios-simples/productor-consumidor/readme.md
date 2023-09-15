# Problema de tipo productor-consumidor

Un proceso/hilo produce datos que son posteriormente procesados por otro proceso/hilo

- El productor y el consumidor comparten un buffer, donde el productor escribe y el consumidor lee.
- El productor no puede escribir un elemento en el buffer si el mismo está lleno.
- El consumidor no puede leer un elemento del buffer si el mismo está vacío.

# 
_Compilar_
```
gcc productor_consumidor.c -lpthread
```
