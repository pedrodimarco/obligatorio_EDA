// Estructuras de Datos y Algoritmos - Obligatorio 2021
// Tecnologo en Informatica FIng - DGETP - UTEC
//
// editor.c
// Modulo de implementacion de editor.

#include "editor.h"
#include <string.h>
#include <iostream>

using namespace std;

struct nodo_editor{
	/* aquí deben figurar los campos/estructuras que usted considere necesarios
	para implementar el editor. Ej: texto, diccionario, etc*/
	texto t;
};

struct nodo_texto{
	linea primeraLinea; // puntero apuntando a la primera linea
	linea l; 
	int cantLineas;
};

struct nodo_linea{
	palabra primera;
	linea sig;
	int cantPalabras;
};

struct nodo_palabra{
	Cadena word;
	palabra sig;
	palabra ant;
};

struct nodo_arbol{
	Cadena word;
	arbol izq;
	arbol der;
};

editor CrearEditor(){
	// Crea la estructura editor.
	editor e = new (nodo_editor);
	// crear el resto de las estructuras que se incluyan en el editor
	texto tex = new (nodo_texto);

	e->t = tex;
	e->t->cantLineas = 0;
	e->t->primeraLinea = NULL;
	e->t->l = NULL;

	return e;
}

linea CrearLinea(){
	linea nuevaLinea = new (nodo_linea);

	nuevaLinea->primera = NULL;
	nuevaLinea->cantPalabras = 0;
	nuevaLinea->sig = NULL;
	return nuevaLinea;
}

palabra CrearPalabra(){
	palabra nuevaPalabra = new (nodo_palabra);
	nuevaPalabra->word = NULL;
	nuevaPalabra->ant = NULL;
	nuevaPalabra->sig = NULL;
	return nuevaPalabra;
}

TipoRetorno InsertarLinea(editor &e){
	// Inserta una nueva línea vacía al final del texto.
	// Este requerimiento debe ser resuelto en O(1) peor caso.
	// Ver más detalles en la letra del obligatorio.
	editor aux = CrearEditor();
	aux = e;

	linea nuevaLinea = CrearLinea();

	// if(e->t->cantLineas == 0){
	// 	e->t->l = nuevaLinea;
	// 	e->t->cantLineas++;
	// }
	// else {
	// 	while(e->t->l != NULL){
	// 		e->t->l = e->t->l->sig;
	// 	}

	// 	e->t->l->sig = nuevaLinea;
	// 	e->t->cantLineas++;
	// }
	e->t->l = e->t->primeraLinea;

	if (e->t->l != NULL){
		while (e->t->l->sig != NULL){
			e->t->l = e->t->l->sig;
		}
		e->t->l->sig = nuevaLinea;
		e->t->cantLineas++;
	}
	else{
		e->t->primeraLinea = nuevaLinea;
		e->t->l = nuevaLinea;
		e->t->cantLineas++;
	}
	return OK;
}

TipoRetorno BorrarLinea(editor &e, Posicion posicionLinea){
	// Borra la línea en la posición indicada.
	// Ver más detalles en la letra del obligatorio.
	if ((posicionLinea >= 1) && (posicionLinea <= e->t->cantLineas)){
		Posicion cont = 1;
		editor aux = CrearEditor();

		while (cont != posicionLinea - 1) {
			cont++;
			e->t->l = e->t->l->sig;
		}

		aux->t->l = e->t->l->sig;
		// Engancho la linea anterior con la línea siguiente (respecto a la linea a ser borrada)
		e->t->l->sig = aux->t->l->sig;

		delete (aux->t->l);

		e->t->cantLineas--;

		return OK;
	}
	else {
		return ERROR;
	}
}

TipoRetorno BorrarLinea2(editor &e, Posicion posicionLinea){

	linea iter = e->t->l, ant = NULL;
	Posicion contPosicion = 0;

	while (contPosicion != posicionLinea){
		contPosicion++;
		ant = iter;
		iter = iter->sig;
	}

	if (ant == NULL) { // SOY EL PRIMERO
		ant = iter;
		delete iter;
		iter = ant->sig;

		while (iter != NULL){
			ant = iter;
			iter = iter->sig;
			ant->cantPalabras--; // PUEDE ESTAR MAL LA SINTAXIS
		}
	}

	else{ // NO SOY EL PRIMERO
		iter = iter->sig;
		delete ant->sig;
		ant->sig = iter;

		while (iter != NULL){
			ant = iter;
			iter = iter->sig;
			ant->cantPalabras--;
		}
	}
	return NO_IMPLEMENTADA;
}

TipoRetorno BorrarTodo(editor &e){
	// Borra todas las líneas del texto.
	// Ver más detalles en la letra del obligatorio.
	if (e->t->cantLineas == 0)
		return OK;
	else{
		int cont;
		for (cont = 1; cont <= e->t->cantLineas; cont++)
		{
			BorrarLinea(e, cont);
		}
		e->t->cantLineas = 0;

		return OK;
	}
}

TipoRetorno BorrarOcurrenciasPalabraEnTexto(editor &e, Cadena palabraABorrar){
	// Borra todas las ocurrencias de una palabra en el texto.
	// Ver más detalles en la letra del obligatorio.
	linea aux = e->t->l;
	palabra ant = NULL, iter = e->t->l->primera;
	if (aux != NULL){

		while (aux != NULL){ // RECORRO LAS LINEAS

			while (iter->word != NULL){ // RECORRO LAS PALABRAS

				if (iter->word == palabraABorrar){ // HAY Q BORRAR

					if (ant == NULL){ // LA PALABRA A BORRAR ES LA PRIMERA DE LA
					  // PRIMERA  LINEA
						ant = iter->sig;
						delete iter;
						iter = ant->sig;
					}
					else{ // NO SOY EL PRIMERO
						ant->sig = iter->sig;
						delete iter;
						iter = ant->sig;
					}
				}

				ant = iter;
				iter = ant->sig; // ESTO SE PUEDE HACER?
			}

			aux = aux->sig;
		}
		return OK;
	}
	else
		return NO_IMPLEMENTADA;
}

TipoRetorno InsertarLineaEnPosicion(editor &e, Posicion posicionLinea){
	if (e->t->l != NULL){
		linea nuevaLinea, anterior, auxiliar;
		Posicion contPosicion = 0;

		nuevaLinea = CrearLinea();
		anterior = NULL;
		auxiliar = e->t->l;
		while (contPosicion != posicionLinea){
			contPosicion++;
			anterior = auxiliar;
			auxiliar = auxiliar->sig;
		}
		anterior->sig = nuevaLinea;
		nuevaLinea->sig = auxiliar;
	}
	else if (posicionLinea == 0){
		editor ed = e;
		InsertarLinea(ed);
	}
	return OK;
}

TipoRetorno InsertarLineaEnPosicion2(editor &e, Posicion posicionLinea){
	if ((posicionLinea >= 1) && (posicionLinea <= e->t->cantLineas + 1))
		return ERROR;
	else{
		int cont = 1;

		linea nuevaLinea = CrearLinea();

		// Creo un auxiliar
		editor aux = CrearEditor();

		while (cont != posicionLinea){
			cont++;
			e->t->l = e->t->l->sig;
		}
		aux = e;

		// Engancho la nueva linea a la lista de lineas.
		e->t->l->sig = nuevaLinea;

		// Engancho las lineas restantes con la nueva linea.
		e->t->l->sig = aux->t->l;

		e->t->cantLineas++;
	}
	return OK;
}

TipoRetorno InsertarPalabra(editor &e, Posicion posicionLinea, Posicion posicionPalabra, Cadena palabraAIngresar){
	if (e->t->l != NULL){
		linea auxiliar;
		auxiliar = e->t->l;
		Posicion contPosicion = 0, contPosicionPalabra = 0;
		palabra p;
		while (contPosicion != posicionLinea){
			contPosicion++;
			auxiliar = auxiliar->sig;
		}
		p = auxiliar->primera;
		if (p != NULL){
			palabra nuevaPalabra, panterior, pauxiliar;
			nuevaPalabra = CrearPalabra();
			panterior = NULL;
			pauxiliar = p;

			while (contPosicionPalabra != posicionPalabra){
				contPosicionPalabra++;
				panterior = pauxiliar;
				pauxiliar = pauxiliar->sig;
			}
			panterior->sig = nuevaPalabra;
			nuevaPalabra->word = palabraAIngresar;
			nuevaPalabra->ant = panterior;
			nuevaPalabra->sig = pauxiliar;
			pauxiliar->ant = nuevaPalabra;
		}
	}
	return OK;
}

TipoRetorno ImprimirLinea(editor &e, Posicion posicionLinea){
	if (e->t->l != NULL){
		linea auxiliar = e->t->l;
		Posicion contPosicion = 0, linea = 0;
		palabra p;
		while (contPosicion != posicionLinea){
			contPosicion++;
			auxiliar = auxiliar->sig;
		}
		p = auxiliar->primera;
		if (p != NULL){
			while (p->sig != NULL){
				linea++;
				printf("%d:", linea);
				printf(" %s", p->word);
			}
			return OK;
		}
		else if (p == NULL){
			return ERROR;
		}
	}
	else if (e->t->l == NULL){
		return ERROR;
	}
	return OK;
}

TipoRetorno ImprimirTexto(editor &e){
// Imprime el texto por pantalla.
	if (e->t->cantLineas == 0){
		cout << "Texto vacio\n";
		return OK;
	}
	else{
		int cont = 1;
		e->t->l = e->t->primeraLinea;
		while (e->t->l->sig != NULL){
			if (e->t->l->cantPalabras == 0)
				cout << cont << ": \n";
			else{
				while (e->t->l->primera->sig != NULL){
					cout << cont << ": " << e->t->l->primera->word << " ";
					e->t->l->primera = e->t->l->primera->sig;
				}
				cout << "\n";
			}
			e->t->l = e->t->l->sig;
			cont++;
		}
		return OK;
	}
}

TipoRetorno ComprimirTexto(editor &e){
	// Comprime las palabras del texto. Para implementar esta operación no debe generarse un nuevo documento.
	// Ver más detalles en la letra del obligatorio.
	return NO_IMPLEMENTADA;
}
/*
TipoRetorno InsertarPalabra(editor & e, Posicion posicionLinea, Posicion posicionPalabra, Cadena palabraAIngresar){
// Inserta una palabra en una línea.
// Ver más detalles en la letra del obligatorio.
	if(((posicionLinea <= e->t->cantLineas) && (posicionLinea > 0))) {

		int cont = 1;

		while(cont != posicionLinea) { //"entro" a la linea que corresponde.
			e->t->l = e->t->l->sig;
			cont++;
		}

		if((posicionPalabra >= 1) && (posicionPalabra <= e->t->l->cantPalabras + 1) && (posicionPalabra <= MAX_CANT_PALABRAS_X_LINEA)) {

			Cadena word = new(char[256]);
			strcpy(word, palabraAIngresar); //Copio el string en la variable que pedí memoria.



		}
		else {
			return ERROR;
		}

		return OK;
	}
	else {
		return ERROR;
	}
}
*/
TipoRetorno BorrarPalabra(editor &e, Posicion posicionLinea, Posicion posicionPalabra){
	// Borra la palabra en la posición indicada.
	// Ver más detalles en la letra del obligatorio.
	return NO_IMPLEMENTADA;
}

TipoRetorno BorrarOcurrenciasPalabraEnLinea(editor &e, Posicion posicionLinea, Cadena palabraABorrar){
	// Borra todas las ocurrencias de una palabra en la línea indicada.
	// Ver más detalles en la letra del obligatorio.
	return NO_IMPLEMENTADA;
}
/*
TipoRetorno ImprimirLinea(editor & e, Posicion posicionLinea){
// Imprime la línea por pantalla.
// Ver más detalles en la letra del obligatorio.
	return NO_IMPLEMENTADA;
}
*/
TipoRetorno IngresarPalabraDiccionario(editor &e, Cadena palabraAIngresar){
	// Agrega una palabra al diccionario.
	// Esta operación debe realizarse en a lo sumo O(log n) promedio.
	// Ver más detalles en la letra del obligatorio.
	
	return NO_IMPLEMENTADA;
}

TipoRetorno BorrarPalabraDiccionario(editor &e, Cadena palabraABorrar){
	// Borra una palabra del diccionario.
	// Ver más detalles en la letra del obligatorio.
	return NO_IMPLEMENTADA;
}

TipoRetorno ImprimirDiccionario(editor &e){
	// Muestra las palabras del diccionario alfabéticamente.
	// Esta operación debe realizarse en O(n) peor caso.
	// Ver más detalles en la letra del obligatorio.
	return NO_IMPLEMENTADA;
}

TipoRetorno ImprimirTextoIncorrecto(editor &e){
	// Muestra las palabras del texto que no se encuentran en el diccionario.
	// Ver más detalles en la letra del obligatorio.
	return NO_IMPLEMENTADA;
}

TipoRetorno ImprimirUltimasPalabras(editor &e){
	// Imprime las últimas MAX_CANT_ULTIMAS_PALABRAS palabras ingresadas al texto.
	// Ver más detalles en la letra del obligatorio.
	return NO_IMPLEMENTADA;
}

TipoRetorno DestruirEditor(editor &e){
	// Destruye la estructura editor y libera la memoria asociada.
	return NO_IMPLEMENTADA;
}
