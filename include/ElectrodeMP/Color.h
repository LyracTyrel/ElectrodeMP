
/// @file Color.h
///	@brief Este archivo de cabecera incluye la clase Color asi como su iterador propio.
///
/// La clase color provee de un mecanismo para acceder a las tres componentes de color que nos interesa recuperar (R , G , B). Está hecha a partir
/// de los iteradores especificos con los cuales está parametrizada y estos pueden ser cualquier tipo de iterador interno o referencia (pointer) a
/// un tipo de dato especifico.
/// @author Menightmar
/// @date Viernes 23/03/2018

#ifndef ELECTRODEMP_COLOR_H
#define ELECTRODEMP_COLOR_H

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

// -----------------------------  Bases  ---------------------------------------

// Incluiremos para empezar nuestras cabeceras de C.

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// Incluiremos el tratamiento de tipos.

#include <type_traits>

// Tenemos las utilerias de c++ 11.

#include <utility>

// -------------------------  Configuracion  -----------------------------------

// Incluiremos el archivo de configuración para todo el proyecto ElectrodeMP.

#include <ElectrodeMP/Configuracion.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// Incluiremos todo el codigo dentro del siguiente espacio de nombres.

namespace ElectrodeMP {

// Empezaremos con la declaración formal de la clase Color , dicha clase permitira realizar asignaciones y algunas modificaciones para las imagenes que ElectrodeMP
// pueda gestionar.

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// --------------------------------  Color  ------------------------------------

/// @brief El espacio de Color en la Imagen de Caratula contenida está gestionada por los canales R , G ,  B.  Tenemos una estructura auxiliar que conjunta los
/// tres elementos más importantes en la imagen , el valor RGB de cada pixel para poderlos utilizar en un solo tipo de dato.
///
/// La clase tiene como parametro de plantilla el Tipo de elemento de Pixel el cual se está analizando , esté formara el tipo de componente interno en la clase.
/// Se utilizan los iteradores en lugar de la copia de los valores para hacerlos modificables directamente. Estos iteradores a su vez son creados mediante un
/// tipo parametrizable de plantilla. De está manera puede ser cualquier iterador valido de color.
/// @tparam TypeComponent Tipo de dato por componente de Color (Tipo de Pixel) , por ejemplo : unsigned char (0 - 255) , float (0.0 - 1.0) , etc.
/// @tparam TypeIterator Tipo de iterador de color a utilizar para cada canal de color RGB.

template <typename TypeComponent , typename TypeIterator>
class Color {
	
	// Referencia a elementos publicos.
	
	public :
		
		// -------------------------  Referencias  -----------------------------
		
		/// @brief Definiremos el tipo de componente para trabajar con el en el resto de codigo.
		
		typedef TypeComponent CComponent;
		
		/// @brief Definimos el tipo de iterador a considerar de manera interna.
		
		typedef TypeIterator CIterator;
	
	// Los elementos internos que gestiona nuestra clase Color son los siguientes :

	private :
		
		// -----------------------  Iteradores  --------------------------------
		
		// Tenemos los tres iteradores de colores de la Imagen (R , G , B).
		
		CIterator R;
		CIterator G;
		CIterator B;
		
		// Declaramos la clase del Iterador como amiga de está.
		
		template <typename IteratorTypeComponent , typename IteratorTypeIterator>
		friend class Color_Iterator;
	
	// Ahora tenemos los metodos de acceso publico.
	
	public :
		
		// ----------------  Constructor / Asignador -----------------------
		
		/// @name Constructores.
		/// @brief Tenemos los siguientes constructores para la Clase de Color que tenemos.
		/// @{
		
		/// @brief Constructor por defecto vacio que inicia los colores en 0.
		
		Color () = default;
		
		/// @brief Constructor privado para iniciar con los tres iteradores de color que tenemos.
		/// @param Rojo Iterador de Color R.
		/// @param Verde Iterador de Color G.
		/// @param Azul Iterador de Color B.
		
		Color (const CIterator & Rojo , const CIterator & Verde , const CIterator & Azul) : R (Rojo) , G (Verde) , B (Azul) {}
		
		/// @brief Constructor de conversión.
		/// @tparam NewTypeIterator Tipo de iterador al cual convertir.
		/// @param Convertir Objeto de Color a copiar sus valores internos.
		
		template <typename NewTypeIterator>
		Color (const Color <const CComponent , NewTypeIterator> & Convertir) :
			R (static_cast <CIterator> (Convertir.R)) ,
			G (static_cast <CIterator> (Convertir.G)) ,
			B (static_cast <CIterator> (Convertir.B)) {}
		
		/// @brief Constructor de copia implementado por defecto.
		/// @param Copia Color para copiar sus componentes.
		
		Color (const Color & Copia) = default;
		
		/// @brief Constructor de movimiento implementado por defecto.
		/// @param Mover Color para mover sus elementos.
		
		Color (Color && Mover) = default;
		
		/// @brief Asignador de Copia por defecto.
		/// @brief Copia Color para copiar sus componentes.
		/// @return El objeto de color actual.
		
		Color & operator = (const Color & Copia) = default;
		
		/// @brief Asignador de Copia por defecto.
		/// @brief Mover Color para copiar sus componentes.
		/// @return El objeto de color actual.
		
		Color & operator = (Color && Mover) = default;
		
		/// @}
		
		// ----------------------  Accesores  ------------------------------
		
		/// @name Accesores.
		/// @brief Tenemmos todos los metodos para acceder a la información de color actual.
		/// @{
		
		/// @brief Metodo para acceder a la referencia de Color Rojo actual.
		/// @return Referencia a la Componente de Color Rojo.
		
		ELECTRODEMP_FORCE_INLINE CComponent & Red () const {
			
			// Devolvemos la referencia a la componente de color actual.
			
			return (*R);
			
		}
		
		/// @brief Metodo para acceder a la referencia de Color Verde actual.
		/// @return Referencia a la Componente de Color Verde.
		
		ELECTRODEMP_FORCE_INLINE CComponent & Green () const {
			
			// Devolvemos la referencia a la componente de color actual.
			
			return (*G);
			
		}
		
		/// @brief Metodo para acceder a la referencia de Color Azul actual.
		/// @return Referencia a la Componente de Color Azul.
		
		ELECTRODEMP_FORCE_INLINE CComponent & Blue () const {
			
			// Devolvemos la referencia a la componente de color actual.
			
			return (*B);
			
		}
		
		// -------------------  Conversión Const -------------------------------
		
		/// @brief Conversión explicita para un Iterador diferente con la misma Componente pero constante de color actual.
		///
		/// Está conversión realiza el traspaso de un iterador a un const_iterador , utilizar solamente cuando el tipo base sea
		/// un tipo inqualificado (por ejemplo : int (int *) , float (float *) , etc).
		/// @tparam NewTypeIterator Tipo de iterador destino al cual convertir.
		/// @return Iterador de Color para el Nuevo tipo de Iterador interno a utilizar , la misma componente pero en const.
		
		template <typename NewTypeIterator>
		ELECTRODEMP_FORCE_INLINE operator Color <const typename std::decay <CComponent>::type , NewTypeIterator> () const {
			
			// Devolvemos un nuevo Color con los mismos iteradores actuales.
			
			return Color <const typename std::decay <CComponent>::type , NewTypeIterator> (R , G , B);
			
		}
		
		/// @}
	
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// ------------------------------  Iterador  -----------------------------------

/// @brief Definiremos nuestro propio iterador para recorrer todos los elementos en la Caratula como elementos de la estructura Color que definimos.
///
/// Estos iteradores permiten recorrer toda la imagen Color por Color utilizando los tres componentes presentes en la imagen para poder acceder rapidamente
/// a la información en la imagen. Toma como parametros de plantilla los mismos tipos que el elemento de @ref Color.
/// @tparam TypeComponent Tipo de dato por componente de Color (Tipo de Pixel) , por ejemplo : unsigned char (0 - 255) , float (0.0 - 1.0) , etc.
/// @tparam TypeIterator Tipo de iterador de color a utilizar para cada canal de color RGB.

template <typename TypeComponent , typename TypeIterator>
class Color_Iterator : public std::iterator <std::bidirectional_iterator_tag , Color <TypeComponent , TypeIterator>> {
	
	// Referencia a elementos publicos.
	
	public :
		
		// -------------------------  Referencias  -----------------------------
		
		/// @brief Definiremos el tipo de Color para como dato interno para nuestro iterador.
		
		typedef Color <TypeComponent , TypeIterator> value_type;
		
		/// @brief Definiremos el tipo de componente para trabajar con el en el resto de codigo.
		
		typedef typename value_type::CComponent CComponent;
		
		/// @brief Definimos el tipo de iterador a considerar de manera interna.
		
		typedef typename value_type::CIterator CIterator;
	
	// Tendremos de manera interna lo siguiente
	
	private :
		
		// ----------------------  Elementos  --------------------------
		
		// En general nuestro iterador cuenta con tres sub-iteradores de la clase CImage que comprenden los tres canales (R , G , B) por los cuales nuestro
		// elemento de imagen se puede mover. Tenemos el siguiente Color de base para la iteración.
		
		mutable value_type Iterador_Base;
	
	// Este tipo de iterador cuenta con las siguientes funciones
	
	public :
		
		// --------------  Constructores / Asignadores  ----------------
		
		/// @name Constructores.
		/// @brief Contamos con los siguientes constructores de nuestro iterador para poderlo inicializar de manera simple.
		/// @{
		
		/// @brief El constructor por defecto vacio que inicia los iteradores internos por defecto.
		
		Color_Iterator () = default;
		
		/// @brief El constructor con el inicio del iterador asignado.
		/// @param Inicio_R Iterador de inicio para el Canal R de la Imagen.
		/// @param Inicio_G Iterador de inicio para el Canal G de la Imagen.
		/// @param Inicio_B Iterador de inicio para el Canal B de la Imagen.
		
		explicit Color_Iterator (const CIterator & Inicio_R , const CIterator & Inicio_G , const CIterator & Inicio_B) :
			Iterador_Base (Inicio_R , Inicio_G , Inicio_B) {}
		
		/// @brief Constructor de Conversión desde un Iterador diferente con la misma componente Const.
		/// @tparam NewTypeIterator Otro tipo de iterador para convertir.
		/// @param Convertir Objeto Iterador de Color con otro tipo de iterador interno para iniciar los valores.
		
		template <typename NewTypeIterator>
		explicit Color_Iterator (const Color_Iterator <const typename std::decay <CComponent>::type , NewTypeIterator> & Convertir) :
			Iterador_Base (Convertir.Iterador_Base) {}
		
		/// @brief Constructor de Copia para el Iterador de Color.
		/// @param Copia iterador a copiar para inicializar los componentes.
		
		Color_Iterator (const Color_Iterator & Copia) = default;
		
		/// @brief Constructor de Movimiento para el Iterador de Color.
		/// @param Mover iterador a mover para inicializar los componentes.
		
		Color_Iterator (Color_Iterator && Mover) = default;
		
		/// @brief El asignador de Copia por defecto.
		/// @param Copia Iterador de Color a copiar sus valores.
		/// @return Esté iterador actual.
		
		Color_Iterator & operator = (const Color_Iterator & Copia) = default;
		
		/// @brief El asignador de Movimiento por defecto.
		/// @param Copia Iterador de Color a mover sus valores.
		/// @return Esté iterador actual.
		
		Color_Iterator & operator = (Color_Iterator && Mover) = default;
		
		/// @}
		
		// ------------------------  Comparación  ----------------------
		
		/// @name Comparadores.
		/// @brief Implementación de los metodos y operadores de Comparación de los iteradores Bidireccionales.
		/// @{
		
		/// @brief Es comparable por igualdad.
		/// @param Iterador a Comparar con el actual.
		/// @return True si son @b iguales los iteradores.
		
		ELECTRODEMP_FORCE_INLINE bool operator == (const Color_Iterator & Comparador) const {
			
			// Devuelve true si nuestros iteradores internos son iguales.
			
			return (
				(Iterador_Base.R == Comparador.Iterador_Base.R) &&
				(Iterador_Base.G == Comparador.Iterador_Base.G) &&
				(Iterador_Base.B == Comparador.Iterador_Base.B)
			);
			
		}
		
		/// @brief Es comparable por diferencia.
		/// @param Iterador a Comparar con el actual.
		/// @return True si son @b diferentes los iteradores.
		
		ELECTRODEMP_FORCE_INLINE bool operator != (const Color_Iterator & Comparador) const {
			
			// Devuelve true si todos nuestros iteradores internos son diferentes.
			
			return (
				(Iterador_Base.R != Comparador.Iterador_Base.R) &&
				(Iterador_Base.G != Comparador.Iterador_Base.G) &&
				(Iterador_Base.B != Comparador.Iterador_Base.B)
			);
			
		}
		
		/// @}
		
		// ---------------------  Desreferencia  -----------------------
		
		/// @name Desreferenciadores.
		/// @brief Implementación de los metodos y operadores de Desreferencia de los iteradores Bidireccionales.
		/// @{
		
		/// @brief Es desreferenciable utilizando el operador @b *.
		/// @return Referencia al color actual (R , G , B).
		
		ELECTRODEMP_FORCE_INLINE value_type & operator * () const {
			
			// Se devuelve la estructura Color con los valores de los tres iteradores internos.
			
			return Iterador_Base;
			
		}
		
		/// @brief Es desreferenciable utilizando el operador @b ->.
		/// @return Referencia al color actual (R , G , B).
		
		ELECTRODEMP_FORCE_INLINE value_type * operator -> () const {
			
			// Se devuelve la des-referencia del Color para los iteradores internos.
			
			return &Iterador_Base;
			
		}
		
		/// @}
		
		// ----------------------  Incrementable  ----------------------
		
		/// @name Incrementadores.
		/// @brief Implementación de los metodos y operadores de Incremento (Post y Pre) de los iteradores Bidireccionales.
		/// @{
		
		/// @brief Se puede pre-incrementar nuestro iterador actual.
		/// @return Referencia al iterador actual.
		
		Color_Iterator & operator ++ () {
			
			// Devuelve la referencia a este iterador una vez que se desplaze los iteradores internamente.
			
			++ Iterador_Base.R , ++ Iterador_Base.G , ++ Iterador_Base.B;
			
			// Devolvemos este elemento.
			
			return (*this);
			
		}
		
		/// @brief Se puede pre-incrementar nuestro iterador actual.
		/// @return Referencia al iterador actual constante.
		
		const Color_Iterator & operator ++ () const {
			
			// Devuelve la referencia a este iterador una vez que se desplaze los iteradores internamente.
			
			++ Iterador_Base.R , ++ Iterador_Base.G , ++ Iterador_Base.B;
			
			// Devolvemos este elemento.
			
			return (*this);
			
		}
		
		/// @brief Se puede post-incrementar nuestro iterador actual.
		/// @return Referencia al iterador actual.
		
		Color_Iterator operator ++ (int) {
			
			// Copiamos este iterador.
			
			Color_Iterator Iterador_Actual (*this);
			
			// Movemos todos los iteradores internos.
			
			++ Iterador_Base.R , ++ Iterador_Base.G , ++ Iterador_Base.B;
			
			// Devolvemos el iterador anterior
			
			return Iterador_Actual;
			
		}
		
		/// @brief Se puede post-incrementar nuestro iterador actual.
		/// @return Referencia al iterador actual constante.
		
		const Color_Iterator operator ++ (int) const {
			
			// Copiamos este iterador.
			
			Color_Iterator Iterador_Actual (*this);
			
			// Movemos todos los iteradores internos.
			
			++ Iterador_Base.R , ++ Iterador_Base.G , ++ Iterador_Base.B;
			
			// Devolvemos el iterador anterior
			
			return Iterador_Actual;
			
		}
		
		/// @}
		
		// ----------------------  Decrementable  ----------------------
		
		/// @name Decrementadores.
		/// @brief Implementación de los metodos y operadores de Decremento (Post y Pre) de los iteradores Bidireccionales.
		/// @{
		
		/// @brief Se puede pre-decrementar nuestro iterador actual.
		/// @return Referencia al iterador actual.
		
		Color_Iterator & operator -- () {
			
			// Devuelve la referencia a este iterador una vez que se desplaze los iteradores internamente.
			
			-- Iterador_Base.R , -- Iterador_Base.G , -- Iterador_Base.B;
			
			// Devolvemos este elemento.
			
			return (*this);
			
		}
		
		/// @brief Se puede pre-decrementar nuestro iterador actual.
		/// @return Referencia al iterador actual constante.
		
		const Color_Iterator & operator -- () const {
			
			// Devuelve la referencia a este iterador una vez que se desplaze los iteradores internamente.
			
			-- Iterador_Base.R , -- Iterador_Base.G , -- Iterador_Base.B;
			
			// Devolvemos este elemento.
			
			return (*this);
			
		}
		
		/// @brief Se puede post-decrementar nuestro iterador actual.
		/// @return Referencia al iterador actual.
		
		Color_Iterator operator -- (int) {
			
			// Copiamos este iterador.
			
			Color_Iterator Iterador_Actual (*this);
			
			// Movemos todos los iteradores internos.
			
			-- Iterador_Base.R , -- Iterador_Base.G , -- Iterador_Base.B;
			
			// Devolvemos el iterador anterior
			
			return Iterador_Actual;
			
		}
		
		/// @brief Se puede post-decrementar nuestro iterador actual.
		/// @return Referencia al iterador actual constante.
		
		const Color_Iterator operator -- (int) const {
			
			// Copiamos este iterador.
			
			Color_Iterator Iterador_Actual (*this);
			
			// Movemos todos los iteradores internos.
			
			-- Iterador_Base.R , -- Iterador_Base.G , -- Iterador_Base.B;
			
			// Devolvemos el iterador anterior
			
			return Iterador_Actual;
			
		}
		
		// ----------------------  Conversión  -------------------------
		
		/// @brief Conversión explicita para un Iterador diferente con la misma Componente pero constante de color actual.
		///
		/// Está conversión realiza el traspaso de un iterador a un const_iterador , utilizar solamente cuando el tipo base sea
		/// un tipo inqualificado (por ejemplo : int (int *) , float (float *) , etc).
		/// @tparam NewTypeIterator Tipo de iterador destino al cual convertir.
		/// @return Iterador de Color para el Nuevo tipo de Iterador interno a utilizar , la misma componente pero en const.
		/// @warning TypeIterator debe ser convertible a NewTypeIterator.
		
		template <typename NewTypeIterator>
		ELECTRODEMP_FORCE_INLINE operator Color_Iterator <const typename std::decay <CComponent>::type , NewTypeIterator> () const {
			
			// Devolvemos la conversión al tipo de valor especifico constante.
			
			return Color_Iterator <const typename std::decay <CComponent>::type , NewTypeIterator> (Iterador_Base.R , Iterador_Base.G , Iterador_Base.B);
			
		}
		
		/// @}
	
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

} // namespace ElectrodeMP.

// ----------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // ELECTRODEMP_COLOR_H
