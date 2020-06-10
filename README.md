# ISEL 2020 - EJERCICIOS

Ejercicios realizados para la asignatura de "Ingeniería de Sistemas Electrónicos (ISEL)" en el curso 2019/2020. Todos los ejercicios tienen un Makefile asociado, por lo que pueden compilarse haciendo "make" en su directorio correspondiente, así como borrar los archivos que genera al compilar haciendo "make clean".

## Ejercicios 4-9

Implementación de un interruptor con botones y temporizadores que enciende una luz, una alarma que activa un buzzer y un led cuando detecta presencia (si esta activada) y un código de tres dígitos introducido con un pulsador como FSM independientes. 

## Ejercicio 12

Implementación mediante un ejecutivo cíclico de las tres FSM (switch, alarm y code) de manera conjunta en un mismo sistema, donde el código de tres dígitos activa o desactiva la alarma si es introducido correctamente. 

## Ejercicio 13

Como el 12 pero implementado con threads.

## Ejercicio 14

Como el 12 pero implementado con un Reactor.


## PLANIFICACIÓN

### Tiempos de ejecución de las FSMs:

Obtenidos a partir del código en el directorio **characterization**:

![alt text](https://github.com/alejp1998/isel2020/blob/master/exec_times.png "EXECUTION TIMES")

Los tiempos de ejecución en el caso peor (C) que vamos a usar se corresponden con el máximo tiempo de ejecución de cada una de las FSMs tras dispararlas 1000 veces. Así:

* <img src="https://latex.codecogs.com/svg.latex?\Large&space;C_{KBD}=384,013{\mu}s" title="ckbd" />

* <img src="https://latex.codecogs.com/svg.latex?\Large&space;C_{CODE}=30,201{\mu}s" title="ccode" />

* <img src="https://latex.codecogs.com/svg.latex?\Large&space;C_{ALARM}=37,632{\mu}s" title="calarm" />

* <img src="https://latex.codecogs.com/svg.latex?\Large&space;C_{SWITCH}=34,867{\mu}s" title="cswitch" />

Como podemos ver son todos bastante bajos (no llegan a 1ms). Los que más tardan se corresponden con las FSMs de mayor complejidad, como era esperado.

### Asignación de plazos y periódos:
Se ha realizado arbitrariamente, dándoles el mismo valor a los plazos y los periódos. 

Las FSMs con menor plazo son KBD y CODE, ya que se encargan de leer el teclado y activar o desactivar la alarma a partir del código introducido. Por ello les hemos asignado los siguientes plazos (D) y periódos (T):

* <img src="https://latex.codecogs.com/svg.latex?\Large&space;D_{KBD}=T_{KBD}=50ms" title="dkbd" />

* <img src="https://latex.codecogs.com/svg.latex?\Large&space;D_{CODE}=T_{CODE}=50ms" title="dcode" />

Por otro lado, el disparo de la alarma al detectar presencia es importante que tenga lugar rápido, mientras que el switch de la alarma no es muy importante que se ejecute con tanta frecuencia, por eso les hemos asignado los siguientes valores:

* <img src="https://latex.codecogs.com/svg.latex?\Large&space;D_{ALARM}=T_{ALARM}=250ms" title="dalarm" />

* <img src="https://latex.codecogs.com/svg.latex?\Large&space;D_{SWITCH}=T_{SWITCH}=500ms" title="dswitch" />









### Tiempos de ejecución de las FSMs:


## MODELADO Y VERIFICACIÓN

**Directorio "spin":** contiene los modelos en promela y especificaciones correspondientes a las tres máquinas de estados de los ejercicios 12, 13 y 14; verificando también que la interacción entre "alarm" y "code" es correcta:

* **switch.pml :** modelo en Promela del interruptor, para verificar sus propiedades basta con ejecutar el comando "make switch" dentro del directorio "spin".
    
    ![alt text](https://github.com/alejp1998/isel2020/blob/master/switch_fsm.png "SWITCH FSM MODEL")

    **ESPECIFICACION LTL**: 

    * La luz se enciende despúes de pulsar un botón: 
    
    ```python
    ltl button_is_followed_by_light {
        []((button1 || button2) -> <>light);
    }
    ```

    * La luz está encendida hasta que eventualmente salta el timer y se apaga: 

    ```python
    ltl light_on_until_timer {
        [](light -> <>(T -> <> !light);
    }
    ```

* **code.pml :** modelo en Promela del código (con una abstracción de la FSM de la alarma que solo desactiva la señal "alarm_code" cuando se activa), para verificar sus propiedades basta con ejecutar el comando "make code" dentro del directorio "spin".

    ![alt text](https://github.com/alejp1998/isel2020/blob/master/code_fsm.png "CODE FSM MODEL")

    **ESPECIFICACION LTL**: 

    * Se almacena en el buffer un dígito después de salte el temporizador o haya un timeout: 
    
    ```python
    ltl digit_processed_after_count_limit_or_timeout {
        []( ((T || (count >= COUNT_LIMIT)) && (code_st == COUNT)) -> <>(code_st == BUFFER_DIGIT));
    }
    ```

    * Cuando introducimos tres dígitos del código volvemos al estado IDLE: 

    ```python
    ltl three_digits_code_entered_leads_to_idle {
        [](((index+1) > CODE_LENGTH) -> <>(code_st == IDLE));
    }
    ```

    * Si introducimos el código correctamente activamos la señal "alarm_code": 

    ```python
    ltl correct_code_activates_alarm_code_signal {
        [](((current_code[0]==correct_code[0]) && (current_code[1]==correct_code[1]) && (current_code[2]==correct_code[2])) -> <>alarm_code);
    }
    ```

* **alarm.pml :** modelo en Promela de la alarma (con una abstracción de la FSM del código que solo activa la señal "alarm_code" de forma no determinista), para verificar sus propiedades basta con ejecutar el comando "make alarm" dentro del directorio "spin".

    ![alt text](https://github.com/alejp1998/isel2020/blob/master/alarm_fsm.png "ALARM FSM MODEL")

    **ESPECIFICACION LTL**: 

    * La alarma se desactiva al introducir el código correctamente: 
    
    ```python
    ltl alarm_deactivation_after_code_entered {
        []((((alarm_st==TRIGGERED) || (alarm_st==ACTIVE)) && alarm_code) -> <>((alarm_st==INACTIVE)) && !led && !buzzer);
    }
    ```

    * La alarma se dispara eventualmente al detectar presencia si está activa: 

    ```python
    ltl alarm_triggered_after_pir_if_active {
        [](((alarm_st==ACTIVE) && pir_sensor) -> <>((alarm_st==TRIGGERED) && led && buzzer));
    }
    ```


