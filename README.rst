
**ElectrodeMP - Audio Media Player Alpha**
==========================================

.. image:: /resources/ElectrodeMP_Logo_256.png
   :align: center

**ElectrodeMP** (Electrode Audio Media Player) es una biblioteca escrita en lenguaje C++ 17. Su finalidad es que permita acceder a las funciones b�sicas de cualquier reproductor
multimedia y cualquiera pueda expandir sus funcionalidades para hacerlo un reproductor personalizado.

Los componentes base escenciales para que funcione son : 

	- **PortAudio** : Biblioteca para realizar salida de audio digital.
	- **FFmpeg** : Biblioteca para demuxer y decodificar audio digital.
	- **TagLib** : Biblioteca para leer y modificar metadatos (Tags) de audio.

Se ofrece una variedad de funciones simples para abrir streams de audio desde archivos en diferentes formatos , entre los que tenemos a:

	- Formatos de Audio : MP3 , ACC , AC3 , OGG , FLAC , M4A , WAV
	- Formatos de Videos : MOV , MP4 , 3GP

Ofrece un **selector** de streams de audio para poder establecer el flujo a utilizar cuando el audio o video presente m�s de uno.

Provee de un lector y editor de etiquetas ID3v2, APE entre otras. Dando la posibilidad de anexar informaci�n adicional al archivo de audio y guardarlo.
Actualmente cuenta con la siguiente lista de Campos para metadatos entre otros.

	.. image:: /resources/ID3v2_Logo.png

	- Titulo
	- Artista
	- Compositor
	- �lbum
	- A�o de Publicaci�n
	- N�mero de pista
	- Genero
	- Fecha de Creaci�n
	- Fecha de Modificaci�n
	- Caratula del �lbum

Documentaci�n
-------------

La documentaci�n oficial puede ser encontrada Online en la siguiente direcci�n : 


Reproductor
-----------

El funcionamiento b�sico del reproductor hace que programar un audio player con ElectrodeMP solamente requiera de unas pocas l�neas de c�digo.

Veamos un ejemplo simple para crear un Reproductor, abrir un archivo MP3 e iniciar la reproducci�n del audio inmediatamente utilizando la API y el dispositivo de salida predeterminado.

Comenzaremos incluyendo la cabecera del Reproductor importando el espacio de nombres de ElectrodeMP.

.. code:: c++
	
	// Incluimos la cabecera del reproductor.
	
	#include <ElectrodeMP/Reproductor.h>
	
	// Importamos el espacio de nombres.
	
	using namespace ElectrodeMP;

Ahora vamos a crear un Reproductor para reproducir un audio.

.. code:: c++
	
	int main () {
		
		.......
		
		// Creamos un objeto Reproductor.
		
		Reproductor Media; // Constructor por defecto (inicia decoder y portaudio).
		
		// Abriremos un archivo de audio.
		
		Media.Open ("Audio Ejemplo.mp3");
		
		// Iniciaremos la Reproducci�n del audio.
		
		Media.Play (); // Comienza la reproducci�n.
		
		.......
		
		// Cerramos el archivo de audio para terminar el programa.
		
		Media.Close ();
		
	}

Caratula
--------

Presentaremos ahora un ejemplo simple del uso del etiquetador el cual permite realizar la asignaci�n y lectura de los metadatos en los archivos de audio.

Screenshot
----------

Vista previa del reproductor.

.. image:: /resources/snapshot-10-04-2021.PNG
   :align: center
   :width: 512px
   :height: 300px
   :scale: 50%

.. image:: /resources/snapshot-10-04-2021-2.PNG
   :align: center
   :width: 512px
   :height: 300px
   :scale: 50%
