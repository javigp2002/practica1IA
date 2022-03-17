#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;



void ComportamientoJugador::actualizarVistaMapa(Sensores sensores){
	int new_fil = fil, new_col = col;
	int contador = 1, contador_ant = 0;

	mapaResultado[new_fil][new_col] = sensores.terreno[0];

	for (int i = 0; i < 3; i++)
		{
			switch (brujula){
				case 0: new_col--; new_fil--; break;
				case 1: new_col++; new_fil--; break;
				case 2: new_col--; new_fil++; break;
				case 3: new_col--; new_fil--; break;
			}
			
			// se cuenta diferente en sur y oeste (de 3-1, 8-4, 16-9)
			if (brujula == 2 || brujula == 3){
				contador = contador_ant + 3+2*i;
				contador_ant = contador;
			}

			for (int j = 0; j < 3+2*i; j++) {
				if (brujula==0 || brujula == 2) // norte,sur -->columnas
					mapaResultado[new_fil][new_col+j] = sensores.terreno[contador];
				else // este oeste --> filas
					mapaResultado[new_fil+j][new_col] = sensores.terreno[contador];
				
				
				if (brujula == 0 || brujula == 1) //norte este
					contador++;
				else  // sur oeste
					contador--;	
			}
		}
}

Action ComportamientoJugador::think(Sensores sensores){

	if (sensores.nivel > 0){
		bien_situado=false;
	}
	Action accion = actIDLE;
	switch (ultimaAccion)
	{
		case actFORWARD:
			switch (brujula){
				case 0: fil--;break;
				case 1: col++;break;
				case 2: fil++; break;
				case 3: col--;break;
			}
			break;
		case actTURN_L:
			brujula=(brujula+3)%4;
			girar_derecha = (rand()%2==0);
			break;
		
		case actTURN_R:
			brujula = (brujula+1)%4;
			girar_derecha = (rand()%2==0);
			break;
	}

	if (sensores.terreno[0] == 'G' and !bien_situado){
		fil = sensores.posF;
		col = sensores.posC;
		bien_situado =true;
	}

	if (bien_situado){
		
		fil = sensores.posF;
		col=sensores.posC;
		brujula=sensores.sentido;
		actualizarVistaMapa(sensores);
		
	}
	// dECIDIR LA NUEVA ACIION
	if ((sensores.terreno[2] == 'T' or sensores.terreno[2] == 'S' 
		or sensores.terreno[2] == 'G') and sensores.superficie[2] == '_') {
		accion = actFORWARD;
	} else if (!girar_derecha){
		accion = actTURN_L;
	}
	else {
		accion = actTURN_R;
	}

	ultimaAccion =accion;

	return accion;
	
	/*
	cout << "Posicion: fila " << sensores.posF << " columna " << sensores.posC << " ";
	switch(sensores.sentido){
		case 0: cout << "Norte" << endl; break;
		case 1: cout << "Este" << endl; break;
		case 2: cout << "Sur " << endl; break;
		case 3: cout << "Oeste" << endl; break;
	}
	cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];
	cout << endl;

	cout << "Superficie: ";
	for (int i=0; i<sensores.superficie.size(); i++)
		cout << sensores.superficie[i];
	cout << endl;

	cout << "Colisión: " << sensores.colision << endl;
	cout << "Reset: " << sensores.reset << endl;
	cout << "Vida: " << sensores.vida << endl;
	cout << endl;

	*/
	// Determinar el efecto de la ultima accion enviada
	
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}




	
	// if (brujula == 0){ // NORTE
	// 	for (int i = 0; i < 3; i++)
	// 	{
	// 		new_col--;
	// 		new_fil--;
			
		
	// 		for (int j = 0; j < 3+2*i; j++) {
	// 			mapaResultado[new_fil][new_col+j] = sensores.terreno[contador];
	// 			contador++;	
	// 		}
	// 	}
	// } else if (brujula == 1){ // este
	// 	for (int i = 0; i < 3; i++)
	// 	{
	// 		new_col++;
	// 		new_fil--;
			
		
	// 		for (int j = 0; j < 3+2*i; j++) {
	// 			mapaResultado[new_fil+j][new_col] = sensores.terreno[contador];
	// 			contador++;	
	// 		}
	// 	}
	// } else if (brujula == 2){ // SUR
		
	// 	for (int i = 0; i < 3; i++)
	// 	{
	// 		new_col--;
	// 		new_fil ++;
			
	// 		contador = contador_ant + 3+2*i;
	// 		contador_ant = contador;
	// 		for (int j = 0; j < 3+2*i; j++) {
	// 			mapaResultado[new_fil][new_col+j] = sensores.terreno[contador];
	// 			contador--;	
	// 		}
	// 	}
	// } else { // oeste
	// 	for (int i = 0; i < 3; i++)
	// 	{
	// 		new_col--;
	// 		new_fil--;
			
	// 		contador = contador_ant + 3+2*i;
	// 		contador_ant = contador;

	// 		for (int j = 0; j < 3+2*i; j++) {
	// 			mapaResultado[new_fil+j][new_col] = sensores.terreno[contador];
	// 			contador--;	
	// 		}
	// 	}
	// }
	

