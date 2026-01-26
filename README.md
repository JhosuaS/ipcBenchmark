# IPC Benchmark

Este proyecto implementa y compara el rendimiento (latencia) de distintos mecanismos de Comunicación Entre Procesos (IPC) en sistemas UNIX/Linux. El objetivo es medir el tiempo requerido para transferir **100 MB** de datos entre procesos utilizando diferentes estrategias.

## Mecanismos Evaluados

El benchmark cubre los siguientes métodos IPC:
* **Tuberías sin nombre (Unnamed Pipes)**
* **Tuberías con nombre (Named Pipes / FIFO)**
* **Memoria Compartida (Shared Memory - POSIX)**
* **Colas de Mensajes (Message Queues - POSIX)**
* **Sockets (UNIX Domain)**

## Requisitos

* GCC Compiler
* GNU Make
* Entorno Linux/UNIX (necesario para las librerías POSIX como `<mqueue.h>` y `<sys/shm.h>`)

## 🛠️ Compilación

El proyecto cuenta con un `Makefile` automatizado que gestiona la compilación y la estructura de directorios.

Para compilar todo el proyecto:
`make`
Esto creará automáticamente las carpetas bin/ (para los ejecutables) y results/ (para los datos de salida).

Para limpiar los ejecutables y archivos de resultados:
`make clean`
## Ejecución de las pruebas
Todos los ejecutables se generan en la carpeta bin/. Las pruebas están diseñadas para transferir un bloque total de 100 MB.
### Unnamed Pipes
`./bin/unnamed_pipes`
### Mecanismos Cliente - Servidor

| Mecanismo          | Terminal 1 (Lector)   | Terminal 2 (Escritor) |
| ------------------ | --------------------- | --------------------- |
| Memoria compartida | `./bin/shm_reader`    | `./bin/shm_writer`    |
| Colas de Mensajes  | `./bin/mq_reader`     | `./bin/mq_writer`     |
| Named pipes        | `./bin/fifo_reader`   | `./bin/fifo_writer`   |
| Sockets            | `./bin/socket_reader` | `./bin/socket_writer` |

## Resultados
Los resultados de las pruebas se almacenan automáticamente en: results/metrics.csv

>**Nota**: El sistema funciona en modo append (añadir), por lo que cada ejecución agregará una nueva línea al archivo CSV sin borrar los datos anteriores.