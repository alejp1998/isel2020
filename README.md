# ISEL 2020 - EJERCICIOS

Ejercicios realizados para la asignatura de "Ingeniería de Sistemas Electrónicos (ISEL)" en el curso 2019/2020. Todos los ejercicios tienen un Makefile asociado, por lo que pueden compilarse haciendo "make" en su directorio correspondiente, así como borrar los archivos que genera al compilar haciendo "make clean".

## Ejercicios 4-9

Implementación de un interruptor con botones y temporizadores que enciende una luz, una alarma que activa un buzzer y un led cuando detecta presencia (si esta activada) y un código de tres dígitos introducido con un pulsador como FSM independientes. 

## Ejercicio 12

Implementación mediante un ejecutivo cíclico de las tres FSM (switch, alarm y code) de manera conjunta en un mismo sistema, donde el código de tres dígitos activa o desactiva la alarma si es introducido correctamente. 

## Ejercicio 13

Como el 12 pero implementado con FreeRTOS.

## Ejercicio 14

Como el 12 pero implementado con un Reactor.

## Directorio "spin"

Contiene los modelos en promela y especificaciones correspondientes a las tres máquinas de estados de los ejercicios 12, 13 y 14; verificando también que la interacción entre "alarm" y "code" es correcta:

* **switch.pml :** modelo en Promela del interruptor, para verificar sus propiedades basta con ejecutar el comando "make switch" dentro del directorio "spin".

* **code.pml :** modelo en Promela del código (con una abstracción de la FSM de la alarma que solo desactiva la señal "alarm_code" cuando se activa), para verificar sus propiedades basta con ejecutar el comando "make code" dentro del directorio "spin".

* **alarm.pml :** modelo en Promela de la alarma (con una abstracción de la FSM del código que solo activa la señal "alarm_code" de forma no determinista), para verificar sus propiedades basta con ejecutar el comando "make alarm" dentro del directorio "spin".