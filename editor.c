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
	texto t;
	arbol d;
};

struct nodo_texto{
	linea primeraLinea; // Puntero apuntando a la primera linea
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
	editor e = new(nodo_editor);
	texto tex = new(nodo_texto);

	e->t = tex;
	e->d = NULL; //Diccionario apunta a NULL en un principio.
	e->t->cantLineas = 0;
	e->t->primeraLinea = NULL; //Puntero a primera linea.
	e->t->l = NULL; //Puntero a linea.

	return e;
}

linea CrearLinea(){
	linea nuevaLinea = new(nodo_linea);

	nuevaLinea->primera = NULL;
	nuevaLinea->sig = NULL;
	nuevaLinea->cantPalabras = 0;
	
	return nuevaLinea;
}

palabra CrearPalabra(){
	palabra nuevaPalabra = new(nodo_palabra);

	nuevaPalabra->word = NULL;
	nuevaPalabra->ant = NULL;
	nuevaPalabra->sig = NULL;

	return nuevaPalabra;
}

TipoRetorno InsertarLinea(editor &e){
	// Inserta una nueva línea vacía al final del texto.
	linea nuevaLinea = CrearLinea();

	e->t->l = e->t->primeraLinea; //Me aseguro que siempre apunto a la primera linea

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
	if ((posicionLinea >= 1) && (posicionLinea <= e->t->cantLineas)){
		Posicion cont = 1;
		editor aux = CrearEditor();
		// linea auxiliar = CrearLinea();

		while (cont != posicionLinea - 1) {
			cont++;
			e->t->l = e->t->l->sig;
		}

		// auxiliar = e->t->l->sig;
		aux->t->l = e->t->l->sig;
		// Engancho la linea anterior con la línea siguiente (respecto a la linea a ser borrada)
		// e->t->l->sig = auxiliar->sig;
		e->t->l->sig = aux->t->l->sig;

		delete (aux->t->l);

		e->t->cantLineas--;
		return OK;
	}
	else {
		return ERROR;
	}
}

TipoRetorno BorrarTodo(editor &e){
	// Borra todas las líneas del texto.
	if (e->t->cantLineas == 0)
		return OK;
	else{
		int cont;
		for (cont = 1; cont <= e->t->cantLineas; cont++){
			BorrarLinea(e, cont);
		}
		e->t->cantLineas = 0;
		e->t->l = NULL;
		return OK;
	}
}

TipoRetorno BorrarOcurrenciasPalabraEnTexto(editor &e, Cadena palabraABorrar){
	// Borra todas las ocurrencias de una palabra en el texto.
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
				iter = ant->sig;
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

TipoRetorno ImprimirLinea(editor &e, Posicion posicionLinea){
	if(((posicionLinea <= e->t->cantLineas) && (posicionLinea > 0))){
		
		if(e->t->l->cantPalabras == 0)
			return OK;
		else { 
			Posicion cont = 1;

			e->t->l = e->t->primeraLinea;

			while(cont != posicionLinea){ //Encuentro la linea correspondiente.
				cont++;
				e->t->l = e->t->l->sig;
			}
			while(e->t->l->primera->sig != NULL){
				cout << cont << ": " << e->t->l->primera->word << " ";
				e->t->l->primera = e->t->l->primera->sig;
			}
			return OK;
		}
	}
	else
		return ERROR;
}

TipoRetorno ImprimirTexto(editor &e){
// Imprime el texto por pantalla.
	if (e->t->cantLineas == 0){
		cout << "Texto vacio\n";
		return OK;
	}
	else{
		int cont = 1;
		e->t->l = e->t->primeraLinea; //Me aseguro de apuntar a la 1er linea.

		while (e->t->l->sig != NULL){
			if (e->t->l->cantPalabras == 0){ 
				cout << cont << ": \n";
			}
			else{
				while (e->t->l->primera->sig != NULL){
					cout << cont << ": " << e->t->l->primera->word << " ";
					e->t->l->primera = e->t->l->primera->sig;
				}
				if(e->t->l->primera->sig == NULL){
					cout << cont << ": " << e->t->l->primera->word << " ";
				}
				cout << "\n";
			}
			e->t->l = e->t->l->sig;
			cont++;
		}
		if (e->t->l->sig == NULL){
			cout << cont << ": \n";
		}
		return OK;
	}
}

TipoRetorno ComprimirTexto(editor &e){
	// Comprime las palabras del texto. Para implementar esta operación no debe generarse un nuevo documento.
	return NO_IMPLEMENTADA;
}

TipoRetorno InsertarPalabra(editor & e, Posicion posicionLinea, Posicion posicionPalabra, Cadena palabraAIngresar){
// Inserta una palabra en una línea.
	if(((posicionLinea <= e->t->cantLineas) && (posicionLinea > 0))) { //Chequeo de poisicionLinea valido.

		Posicion cont = 1;
		e->t->l = e->t->primeraLinea; //Me aseguro de apuntar a la 1era linea.
		
		while(cont != posicionLinea) { //"entro" a la linea que corresponde.
			e->t->l = e->t->l->sig;
			cont++;
		}

		if((posicionPalabra >= 1) && (posicionPalabra <= e->t->l->cantPalabras + 1) 
			&& (posicionPalabra <= MAX_CANT_PALABRAS_X_LINEA)) {
			Posicion cont2 = 1;

			while(cont2 != posicionPalabra) { //"entro" al nodo palabra correspondiente.
				e->t->l->primera = e->t->l->primera->sig;
				cont2++;
			}

			Cadena word = new(char[256]);
			strcpy(word, palabraAIngresar); //Copio el string en la variable que pedí memoria.

			palabra nuevaPalabra = new(nodo_palabra);
			nuevaPalabra->word = word;
			nuevaPalabra->ant = NULL;
			nuevaPalabra->sig = NULL;
			
			if(posicionPalabra > e->t->l->cantPalabras) {
				//Si la posición está "vacia" ingreso el nuevo nodo palabra sin necesidad de
				// correr otras palabras una posición hacía adelante.
				e->t->l->primera = nuevaPalabra;
				e->t->l->cantPalabras++;
			}
			else {
				//La posición ya contiene una palabra, por lo que hay que enganchar la nuevaPalabra
				// en esta posición, y correr las que ya estaban una posición hacia adelante. 
				cout << "algo\n";
			}
			
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

TipoRetorno BorrarPalabra(editor & e, Posicion posicionLinea, Posicion posicionPalabra){
	if(e->t->l != NULL){
		linea auxiliar;
		auxiliar = e->t->l;
		Posicion contPosicion = 0, contPosicionPalabra = 0;
		palabra p = new(nodo_palabra);

		while(contPosicion != posicionLinea){
			contPosicion++;
			auxiliar = auxiliar->sig;
		}
		p = auxiliar->primera;
		if(p != NULL){
			palabra panterior,pauxiliar;
			panterior = NULL;
			pauxiliar = p;  // apunta a la primera palabra 

			while(contPosicionPalabra != posicionPalabra){
				contPosicionPalabra++;
				panterior = pauxiliar;
				pauxiliar = pauxiliar->sig;
			}
			if(contPosicionPalabra == 0){  	// LA PALABRA A BORRAR ES LA PRIMERA
				e->t->l->primera = p->sig;
				delete(p);
				p = NULL;								// DELA LINEA
			}
		else if (pauxiliar->sig == NULL){		// ESTOY EN LA ULTIMA PALABRA
				delete(p);
				p = NULL;
			}
			else{									//PALABRA EN EL MEDIO
				panterior->sig = pauxiliar->sig;
				delete(pauxiliar);
				pauxiliar = NULL;
			}
			return OK;
		}
		return ERROR;
	
	return ERROR;
	
}
}

TipoRetorno BorrarOcurrenciasPalabraEnLinea(editor &e, Posicion posicionLinea, Cadena palabraABorrar){
	// Borra todas las ocurrencias de una palabra en la línea indicada.
	return NO_IMPLEMENTADA;
}

TipoRetorno agregar(arbol &d, Cadena palabraAIngresar) {

	arbol nodoD = new(nodo_arbol);
	Cadena w = new(char[256]);
	strcpy(w, palabraAIngresar); //Copio el string en la variable que pedí memoria.
	nodoD->word = w;
	nodoD->izq = NULL;
	nodoD->der = NULL;

	if(strcmp(palabraAIngresar, d->word) == 0)
		return ERROR;
	else if(strcmp(palabraAIngresar, d->word) > 0) {
        if (d->der == NULL){
            d->der = nodoD;
        }
        else{
            agregar(d->der, palabraAIngresar);
        }
    }
    else{
        if (d->izq == NULL){
            d->izq = nodoD;
        }
        else{
            agregar(d->izq, palabraAIngresar);
        }
    }
	return OK;
}

TipoRetorno IngresarPalabraDiccionario(editor &e, Cadena palabraAIngresar){
	// Agrega una palabra al diccionario.
	if(e->d == NULL){
		arbol nodoD = new(nodo_arbol);
		Cadena w = new(char[256]);
		strcpy(w, palabraAIngresar); //Copio el string en la variable que pedí memoria.
		nodoD->word = w;
		nodoD->izq = NULL;
		nodoD->der = NULL;

		e->d = nodoD;
		return OK;
	}
	else {
		agregar(e->d, palabraAIngresar);
	}
}

TipoRetorno BorrarPalabraDiccionario(editor &e, Cadena palabraABorrar){
	// Borra una palabra del diccionario.
	return NO_IMPLEMENTADA;
}

TipoRetorno ImprimirDiccionario(editor & e){
	mostrarArbol(e->d);
	return OK;
}

TipoRetorno mostrarArbol(arbol d){
	if(d == NULL){
		return OK;
	}
	else{
		mostrarArbol(d->izq);
		cout << d->word << endl;
		mostrarArbol(d->der);
	}
}

TipoRetorno ImprimirTextoIncorrecto(editor &e){
	// Muestra las palabras del texto que no se encuentran en el diccionario.
	return NO_IMPLEMENTADA;
}

TipoRetorno ImprimirUltimasPalabras(editor &e){
	// Imprime las últimas MAX_CANT_ULTIMAS_PALABRAS palabras ingresadas al texto.
	return NO_IMPLEMENTADA;
}

TipoRetorno DestruirEditor(editor &e){
	// Destruye la estructura editor y libera la memoria asociada.
	return NO_IMPLEMENTADA;
}
