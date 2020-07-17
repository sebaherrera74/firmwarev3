# TP RTOS2 - CESE 11va Cohorte
  - Sebastian Herrera
  - Felipe Sarche
  - Sofía Bertinat
 
## Elección de la arquitectura y del esquema de memoria dinámica utilizada

### Arquitectura 
La arquitectura a utilizar consta de: 
+  CAPA 1 : En esta capa ya desarrollada, se utilizaran las funciones de uart, a través de la capa de abstracción SAPI.
  
+  CAPA 2: Capa a desarrollar, aquí llegarán los datos recibidos de la uart la cual realizará  la separación de los frames y validará el CRC .
  
+  CAPA 3: Capa a desarrollar recibirá los paquetes, validará y realizará el proceso de minusculizar o mayusculizar de acuerdo a el dato que vendrá en el    campo propuesto del tp.

https://docs.google.com/drawings/d/1BW3x_W7sk-AwTYrHzdIXdBaN9S6iLWg1zm-4mk-oeP0/edit?usp=sharing


### Esquema de Memoria a utilizar 
Se elijió implementar heap4 proveeida por freeRTOS debido a que :

  Minimiza la fragmentación de la memoria. 
  
  Es adecuada para aplicaciones que asignan y liberan memorias constantemente. 
  
  Las tareas del sistema implementado no necesitan estar corriendo todas al mismo tiempo.
  
  El pvPortMalloc implementa el algoritmo de "first fit" para ubicar en el heap bloques libres del tamaño solicitado

Tambien se uitlizara memory pools  de la firma QM debido a que :

  Permite hacer alocaciones de tamaño fijo.
  
  Garantiza la correcta administración de la memoria dinámica reservada para los mensajes entrantes y salientes de la Uart.
  
  Posee comportamiento determinista en sistemas de tiempo real que evitan errores de falta de memoria 

 
