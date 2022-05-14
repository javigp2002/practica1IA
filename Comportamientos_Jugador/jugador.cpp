#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;



void ComportamientoJugador::actualizarVistaMapa(Sensores sensores,  vector< vector< unsigned char> > & mapa, int ffil, int ccol){
	int new_fil = ffil, new_col = ccol;
	int contador = 1, contador_ant = 0;

	mapa[new_fil][new_col] = sensores.terreno[0];

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
					mapa[new_fil][new_col+j] = sensores.terreno[contador];
				else // este oeste --> filas
					mapa[new_fil+j][new_col] = sensores.terreno[contador];
				
				if (brujula == 0 || brujula == 1) //norte este
					contador++;
				else  // sur oeste
					contador--;	
			}
		}
}

void ComportamientoJugador::actualizarVista (Sensores sensores){
	int contador = 8;
	for (int i=0; i < 4; i++){
		if (i== 1) contador=4;
		else if (i==2) contador = 1;
		else if (i==3) contador = 0;
		for (int j=i; j < 8-(i*2);j++ ){
			if (sensores.terreno[contador] == 'M')
				mapaBusqueda[i][j] = sensores.terreno[contador];
			else if (sensores.terreno[contador] == 'K' || sensores.terreno[contador] == 'D')
				mapaBusqueda[i][j] = '1';
			
			contador++;
		}
	}
}


void ComportamientoJugador::actualizarVistaMapaAuxiliar(Sensores sensores){
	actualizarVistaMapa(sensores, mapaAuxiliar, fil_aux, col_aux);
}
void ComportamientoJugador::actualizarVistaMapaResultado(Sensores sensores){
	actualizarVistaMapa(sensores, mapaResultado, fil, col);
}
// void ComportamientoJugador::actualizarVistaMapaRecorrido(Sensores sensores){
// 	actualizarVistaMapa(sensores, mapaRecorrido, fil_aux, col_aux);
// }

void ComportamientoJugador::ResetMapa(vector< vector< unsigned char> > & mapa){
	vector<unsigned char> aux(MAX_MAPA_AUX, '?');
      for (unsigned int i = 0; i < MAX_MAPA_AUX; i++){
         mapaAuxiliar.push_back(aux);
      }
}
/*
	PRE: SOLO VALE CUANDO NO SABEMOS DONDE EMPEZAMOS Y TODO EL MAPA EMPIEZA A '?' 
*/
void ComportamientoJugador::copiarAuxiliarEnResultado(vector< vector< unsigned char> > & mapaAux, vector< vector< unsigned char> > & mapaRes){
	int fil_res=0, col_res=0;

	
	int tamResultado = mapaRes.size();

	for (fil_res=0; fil_res < tamResultado ; fil_res++){
		
		for (int col_res = 0; col_res < tamResultado; col_res++){
			
			if (mapaRes[fil_res][col_res] == '?')
				mapaRes[fil_res][col_res]= mapaAux[fil_res+minFilAux][col_res+minColAux];
			
		}

	}
}
void SacaMapa(vector< vector< unsigned char> > mapaAux, int fil, int col, int f_ini = 0, int col_ini=0){
	
	for (size_t i = f_ini; i < fil; i++)
	{
		cout << endl;
		for (size_t c = col_ini; c < col; c++)
		{
			cout << mapaAux[i][c]<< " ";
		}
		cout << endl;
	}	
	
}

bool ComportamientoJugador::pasoPosible (Sensores sensores, int sen, bool estoy_buscando){
	
	bool puedo_pasar = sensores.terreno[sen] != 'M' && sensores.terreno[sen] != 'P' ;
	
	if (estoy_buscando){
		if (puedo_pasar){ // dara true si no se sale de la vista
									// NORTE					NO IR A LA DERECHA DEL BORDE		NO IR A LA IZQUIERDA DEL BORDE			PERMITIR IR HACIA ATRAS
			
			
			if (fil_bus == 0) return !(brujula_interna == 0 || (brujula_interna == 1 && col == 6) || (brujula_interna == 3 && col == 0) || (brujula_interna == 2 && (col== 0 || col == 6)));
			else if (fil_bus==1) return !((brujula_interna == 1 && col == 5) || (brujula_interna == 3 && col == 1)|| (brujula_interna == 2 && (col== 1 || col  == 5)));
			else if (fil_bus==2) return !((brujula_interna == 1 && col == 4) || (brujula_interna == 3 && col == 2));
			else if (fil_bus==3) return brujula_interna == 0;
			else return false;
		}
	}
	

	

	if ((bikini && zapatillas) )
		return puedo_pasar; 
	else if (zapatillas)
		return puedo_pasar &&sensores.terreno[sen] != 'A' ;
	else if (bikini)
		return puedo_pasar && sensores.terreno[sen] != 'B';
	else
		return puedo_pasar &&sensores.terreno[sen] != 'A'&& sensores.terreno[sen] != 'B';
}

int ComportamientoJugador::encuentraMenor(){
	int norte,sur,este,oeste,salida, minimo, max_fil, max_col, val_max,f,c;
	vector< vector< int> > mapa;

	norte = sur = este = oeste = 2000;

	bool radial  = (hay_bikini && !bikini) || (hay_zapatillas && !zapatillas) || (hay_posicionamiento && !bien_situado) || hay_bateria;
	if (radial){
		norte = sur = este = oeste = 2000;
		val_max = 500;
		mapa = mapaRadial;
		max_fil = FIL_MATRIZ_RADIAL;
		max_col = COL_MATRIZ_RADIAL;
		f= fil_bus; c = col_bus;
	}  else {
		norte = sur = este = oeste = VAL_MAX_SALIDA;
		val_max = VAL_MAX_SALIDA-1;
		mapa = mapaPotencial;
		max_fil = FIL_MATRIZ_POTENCIAL;
		max_col = COL_MATRIZ_POTENCIAL;
		f=fil; c= col;
	}
	
	
	if (f > 0) norte = mapa[f-1][c];
	if (f < max_fil-1) sur = mapa[f+1][c];
	if (c > 0) oeste = mapa[f][c-1];
	if (c < max_col -1) este = mapa[f][c+1];
 
	minimo = min( min(norte,sur), min (este,oeste));

	if (minimo >= val_max)  return 5;

	if (minimo == norte) salida = 0;
	else if (minimo == sur) salida = 2;
	else if (minimo == este ) salida = 1;
	else salida = 3;

	return salida;

}

Action ComportamientoJugador::buscaObjetoPotencial(Sensores sensores){
	int min= encuentraMenor();
	
	bool radial  = (hay_bikini && !bikini) || (hay_zapatillas && !zapatillas) || (hay_posicionamiento && !bien_situado) || hay_bateria;
	if (min >=4){
		if (radial){
			hay_bikini = false;
			hay_zapatillas= false;
			hay_posicionamiento = false;
			hay_bateria = false;
		} else {
			resetMapaPotencial();
			pintaMapaPotencial('?');
		}
		return accionPorDefecto(sensores);
	}

	int b;
	
	if (radial){
		b = brujula_interna;
	} else
		b= brujula;

	Action accion = accionMapaPotencial(min,b);
	
	if (radial){
		if (accion == actFORWARD) {
			mapaRadial[fil_bus][col_bus] = 1000;
			switch (brujula_interna){
				case 0: fil_bus--; break;
				case 1: col_bus++; break;
				case 2: fil_bus++; break;
				case 3: col_bus--; break;
			}
			
			
		} else if (accion == actTURN_L) {
			brujula_interna=(brujula_interna+3)%4;
		} else {
			brujula_interna = (brujula_interna+1)%4;
		
		}
	}

	return accion;
}


bool ComportamientoJugador::esInaccesible (Sensores sensores, int num_sensor){
	return 0;
}

Action ComportamientoJugador::accionSinNada(Sensores sensores){
	Action accion = actIDLE;
	
	if (sensores.terreno[0] == 'X') 
		if (sensores.bateria < 2*sensores.vida && sensores.bateria < VAL_MAX_BATERIA) 
			return accion;

	if (sensores.superficie[2] != '_'){
		return accion;
	}


	int f=fil_aux,c=col_aux, fil_sens3=fil_aux, col_sens3=col_aux;
	int fil_aux_mas_tres = f, col_aux_mas_tres = c;
	
	int sensor;
	//meter for en metodo
	bool busca_bateria = false;
	if (sensores.bateria < VAL_MIN_BATERIA){
		busca_bateria = true;
	}

	encuentraCasillasImportantes(sensores, busca_bateria);

	// conocer que vamos a hacer en la siguiente accion
	switch (brujula){
		case 0: f--; fil_aux_mas_tres = f-3; col_aux_mas_tres = c+3; fil_sens3--; col_sens3++; break;
		case 1: c++; col_aux_mas_tres = c+3; fil_aux_mas_tres = f+3; col_sens3++; fil_sens3++; break;
		case 2: f++; fil_aux_mas_tres = f+3; col_aux_mas_tres = c-3; fil_sens3++; col_sens3--; break;
		case 3: c--; col_aux_mas_tres = c-3; fil_aux_mas_tres = f-3; col_sens3--; fil_sens3--; break;
	}

	if ((hay_bikini && !bikini) || (hay_zapatillas && !zapatillas) || (hay_posicionamiento && !bien_situado) || hay_bateria){
		accion = accionBusquedaCasilla(sensores);
	} else {

		if (buscaSalida){
			if (bien_situado){
				accion = accionBuscaSalidaBienSituado(sensores);
			} else 
				accion = accionBuscaSalidaMalSituado(sensores);

		
		}else {
			accion = accionPorDefecto(sensores);
			
		}
		fil_bus=5; col_bus=4;
		brujula_interna = 0;
		segundaVez = true;		
		
		hay_zapatillas = false;
		hay_bikini=false;
		hay_bateria = false;
	} 
		
	return accion;
}

 Action ComportamientoJugador::accionBusquedaCasilla(Sensores sensores){
	Action accion = actIDLE;
	busca_salida = salida=false;
	if (segundaVez){
		resetMapaRadial();
		if (hay_posicionamiento && !bien_situado){
			pintaMapaRadial('G');
		} else if (hay_bikini && !bikini){
			pintaMapaRadial('K');
		} else if (hay_zapatillas && !zapatillas)
			pintaMapaRadial('D');
		else
			pintaMapaRadial('X');

		segundaVez=false;
	}
	
	accion = buscaObjetoPotencial(sensores);
	

	num_iteracion =0;

	return accion;
 }


Action ComportamientoJugador::accionBuscaSalidaBienSituado(Sensores sensores){
	Action accion = actIDLE;
	if (!salida){
		resetMapaPotencial();
		pintaMapaPotencial('?');

		salida = true;
		accion = buscaObjetoPotencial(sensores);

		if (accion == actFORWARD){
			pilaRecorrido.push(actTURN_L);pilaRecorrido.push(actTURN_L);pilaRecorrido.push(actFORWARD);
		} else if (accion == actTURN_L){
			pilaRecorrido.push(actTURN_R);
		} else
			pilaRecorrido.push(actTURN_L);	
	} else {
		accion = buscaObjetoPotencial(sensores);

		pilaRecorrido.push(accion);

		if (mapaPotencial[fil][col]== 0 ||(accion == actFORWARD && (sensores.terreno[2] == 'P' || sensores.terreno[2] == 'M'))){
			buscaSalida = salida = false;
			salida_encontrada = true;
			busca_salida=0;
		} else if (accion == actFORWARD)
				mapaPotencial[fil][col] = VAL_MAX_SALIDA;
	}

	return accion;
}

Action ComportamientoJugador::accionBuscaSalidaMalSituado(Sensores sensores){

	Action accion = actIDLE;
	if(pilaRecorrido.empty()){
		unsigned char actual = sensores.terreno[0], siguiente = sensores.terreno[2], el_otro;
		if (actual == 'A') el_otro = 'B';
		if (actual == 'B') el_otro = 'A';

		if ((actual != 'M') && actual != 'P'){
			accion = actFORWARD;
				// quizas tengo que quitar el movimiento de antes para que se compenetre
		}else
			accion = actTURN_R;

		
		bool estoy_cubierto = (((actual == 'A' && !bikini) || (actual == 'B' && !zapatillas)) && (actual == siguiente || actual ==el_otro) );
		if (!estoy_cubierto)
			buscaSalida = false;

	} else {
		if (sensores.terreno[3] == 'M'){
			encerradoCamino = true;
		}
		if (encerradoCamino && sensores.terreno[3] != 'M'){
			accion = actFORWARD;
			buscaSalida = false;
		}
		Action anterior = pilaRecorrido.top();
		if (anterior == actFORWARD) {
			accion = actFORWARD;
		} else if (anterior == actTURN_L) {
			accion = actTURN_R;
		} else {
			accion = actTURN_L;
			
		}
		pilaRecorrido.pop();
		
		if (pilaRecorrido.empty()){
			buscaSalida = false;
		}
	}

	return accion;
}

Action ComportamientoJugador::accionMapaPotencial(int min, int b){
	Action accion;
	if (min == 0){
		if (b == 0) accion= actFORWARD; 
		else if (b == 1) accion = actTURN_L;
		else if (b == 2) accion = actTURN_R;
		else accion = actTURN_R; 
	} else if (min == 1) {
		if (b == 0) accion = actTURN_R;
		else if (b == 1) accion = actFORWARD;
		else if (b == 2) accion = actTURN_L;
		else accion = actTURN_L; 
	} else if (min == 2){
		if (b == 0) accion = actTURN_R;
		else if (b == 1) accion = actTURN_R;
		else if (b == 2) accion = actFORWARD;
		else accion = actTURN_L; 
	} else{
		if (b == 0) accion = actTURN_L;
		else if (b == 1) accion = actTURN_R;
		else if (b == 2) accion = actTURN_R;
		else accion = actFORWARD; 
	}

	return accion;
}
bool ComportamientoJugador::pasoMapaNoVisto(Sensores sensores){
	int f=fil_aux,c=col_aux;
	int fil_aux_mas_tres = f, col_aux_mas_tres = c;
	int fil_aux_delante = f, col_aux_delante = c;

	switch (brujula){
		case 0: f--; fil_aux_mas_tres = f-3; fil_aux_delante = fil_aux_mas_tres; col_aux_mas_tres = c+3;  break;
		case 1: c++; col_aux_mas_tres = c+3; col_aux_delante = col_aux_mas_tres; fil_aux_mas_tres = f+3; break;
		case 2: f++; fil_aux_mas_tres = f+3; fil_aux_delante = fil_aux_mas_tres; col_aux_mas_tres = c-3;  break;
		case 3: c--; col_aux_mas_tres = c-3; col_aux_delante = col_aux_mas_tres; fil_aux_mas_tres = f-3;  break;
	}
	bool gira= false;
	bool noSeSaleDeMapa1 = fil_aux_mas_tres >=0 && fil_aux_mas_tres < MAX_MAPA_AUX && col_aux_mas_tres >= 0 && col_aux_mas_tres < MAX_MAPA_AUX;
	bool noSeSaleDeMapa2 = fil_aux_delante >=0 && fil_aux_delante < MAX_MAPA_AUX && col_aux_delante >= 0 && col_aux_delante < MAX_MAPA_AUX;

	
	bool rodeadoPrecipicios = false;

	if ((noSeSaleDeMapa1 && mapaAuxiliar[fil_aux_mas_tres][col_aux_mas_tres] != '?') && (noSeSaleDeMapa2 && mapaAuxiliar[fil_aux_delante][col_aux_delante] != '?') && rodeadoPrecipicios )
		gira = true;

	return gira;
}
Action ComportamientoJugador::accionPorDefecto(Sensores sensores){
	Action accion;
	
	// cout << "por defecto" << endl;
	int f=fil_aux,c=col_aux;
	int fil_aux_mas_tres = f, col_aux_mas_tres = c;
	int fil_aux_delante = f, col_aux_delante = c;

	switch (brujula){
		case 0: f--; fil_aux_mas_tres = f-3; fil_aux_delante = fil_aux_mas_tres; col_aux_mas_tres = c+3;  break;
		case 1: c++; col_aux_mas_tres = c+3; col_aux_delante = col_aux_mas_tres; fil_aux_mas_tres = f+3; break;
		case 2: f++; fil_aux_mas_tres = f+3; fil_aux_delante = fil_aux_mas_tres; col_aux_mas_tres = c-3;  break;
		case 3: c--; col_aux_mas_tres = c-3; col_aux_delante = col_aux_mas_tres; fil_aux_mas_tres = f-3;  break;
	}
	bool gira= false;
	bool noSeSaleDeMapa1 = fil_aux_mas_tres >=0 && fil_aux_mas_tres < MAX_MAPA_AUX && col_aux_mas_tres >= 0 && col_aux_mas_tres < MAX_MAPA_AUX;
	bool noSeSaleDeMapa2 = fil_aux_delante >=0 && fil_aux_delante < MAX_MAPA_AUX && col_aux_delante >= 0 && col_aux_delante < MAX_MAPA_AUX;

	if ((noSeSaleDeMapa1 && mapaAuxiliar[fil_aux_mas_tres][col_aux_mas_tres] != '?') && (noSeSaleDeMapa2 && mapaAuxiliar[fil_aux_delante][col_aux_delante] != '?') )
		gira = true;

	
	if (pasoPosible(sensores,2) && mapaRecorrido[f][c] == -1 && !gira){
		accion = actFORWARD;
		num_iteracion=0;
		
	} else {
		accion = actTURN_R;
		num_iteracion++;
	} 

	if (num_iteracion >= 4){ //ha dado una vuelta completa{
		buscaSalida=true;
		salida=false;

		pilaRecorrido.pop();pilaRecorrido.pop();pilaRecorrido.pop();

		accion=actTURN_L;
		turnL =true;
		
	}

	return accion;
}

//////////////////////////////////////////////////////////////////////////////////////

void ComportamientoJugador::switchAccion (){
	switch (ultimaAccion)
		{
		case actFORWARD:
			switch (brujula){
				case 0: fil--; fil_aux--;break;
				case 1: col++; col_aux++;break;
				case 2: fil++; fil_aux++;break;
				case 3: col--; col_aux--;break;
			}
			
			mapaRecorrido[fil_aux][col_aux]++; 
	
			break;
		case actTURN_L:
			brujula=(brujula+3)%4;
			break;
		
		case actTURN_R:
			brujula = (brujula+1)%4;
			break;
	}

}

void ComportamientoJugador::comprobaciones (Sensores sensores){
	

	if (sensores.terreno[0] == 'G' and !bien_situado){
		fil = sensores.posF;
		col = sensores.posC;
		minFilAux = fil_aux - fil;
		minColAux = col_aux - col;
		
		if (he_reseteado)
			he_reseteado=false;	

		copiarAuxiliarEnResultado(mapaAuxiliar, mapaResultado);
		
		hay_posicionamiento = false;
		bien_situado =true;
		
	}
	if (sensores.reset && bien_situado){
		fil = sensores.posF;
		col = sensores.posC;
		minFilAux = fil_aux - fil;
		minColAux = col_aux - col;

		hay_posicionamiento = false;
	}
	
	if (sensores.nivel == 1){
		brujula=sensores.sentido;
	}

	if (sensores.terreno[0] == 'D'){ zapatillas= true; segundaVez = true;}
	if (sensores.terreno[0] == 'K'){ bikini= true;segundaVez = true;}
	if (sensores.terreno[0] == 'X') {hay_bateria = false;segundaVez = true;}
	
	if (bien_situado){
		fil = sensores.posF;
		col=sensores.posC;

		brujula=sensores.sentido;
		actualizarVistaMapaResultado(sensores);
	} 
	actualizarVistaMapaAuxiliar(sensores);
}

void ComportamientoJugador::encuentraCasillasImportantes(Sensores sensores, bool busca_bateria){
	for (int i=1; i<16 && !hay_zapatillas && !hay_bikini && !hay_posicionamiento && !hay_bateria; i++) {
		if ('G'== sensores.terreno[i] && !bien_situado)
			hay_posicionamiento = true;
		if ('K'== sensores.terreno[i] && !bikini)
			hay_bikini = true;
		if ('D'== sensores.terreno[i] && !zapatillas)
			hay_zapatillas = true;
		
		if(busca_bateria){
			if ('X'== sensores.terreno[i]){
				hay_bateria=true;	
			}
		}
	}
}

void ComportamientoJugador::meterEnPila(Action accion){
	if (num_iteracion >= 4){
		num_iteracion = 0;
	} else if (!buscaSalida && !salida_encontrada) {
		pilaRecorrido.push(accion);
	} else
		salida_encontrada = false;

	if (turnL){
		turnL =false;
	}

}

void ComportamientoJugador:: pintaPrecipicios(){
	for (size_t i = 0; i < mapaResultado.size(); i++)
	{
		for (size_t j = 0; j < mapaResultado.size(); j++)
		{
			if (i>2 && i < (mapaResultado.size()-3)){
				if (j>2 && j <mapaResultado.size()-3 ) j= (mapaResultado.size()-3);
			} 
			mapaResultado[i][j] = 'P';
		}	
	}
}

void ComportamientoJugador::resetMapaRadial(){
	vector<int> aux_radial(COL_MATRIZ_RADIAL, 1000);
	for (size_t i = 0; i < FIL_MATRIZ_RADIAL; i++)
	{
		mapaRadial.push_back(aux_radial);
	}
}

void ComportamientoJugador::pintaMapaRadial(unsigned char c){
	int i_auxiliar, j_auxiliar, fil_obj = 1, col_obj=7; 

	stack<pair<int,int>> valores; 
	
	int valor_defecto_j = j_auxiliar, valor_defecto_i = i_auxiliar;

	unsigned char c_actual;
	for (size_t i = 0; i < FIL_MATRIZ_RADIAL; i++){
		if (brujula == 0 || brujula == 3){
			i_auxiliar = i-5;
		} else
			i_auxiliar = (5-i);
		
		//cout << endl;
		for (size_t j = 0; j < COL_MATRIZ_RADIAL; j++){
			if (brujula == 0 || brujula == 1){
				j_auxiliar = j-4;
			} else
				j_auxiliar = 4-j;

			if (brujula==0 || brujula == 2) // norte,sur -->columnas
				c_actual = mapaAuxiliar[fil_aux+i_auxiliar][col_aux+j_auxiliar];
			else // este oeste --> filas
				c_actual= mapaAuxiliar[fil_aux+j_auxiliar][col_aux+i_auxiliar];
			

			if (c_actual == c){
				fil_obj = i;
				col_obj = j;
				
				mapaRadial[i][j] = 0;
				
			} else if (c_actual != 'M' && c_actual != 'P' && c_actual != '?')
				mapaRadial[i][j] = 0;
			else	
				mapaRadial[i][j] = 1000;
		}		
	}

	
	actualizaMapaRadial(fil_obj,col_obj);
	
}


void ComportamientoJugador::actualizaMapaRadial(int fil_obj, int col_obj){
	for (size_t i = 0; i < FIL_MATRIZ_RADIAL; i++)
	{
		for (size_t j = 0; j < COL_MATRIZ_RADIAL; j++)
		{
			if (!(i == fil_obj && j == col_obj)){
				int horizontal = i-fil_obj;
				int vertical  =j-col_obj;
				mapaRadial[i][j] += abs(horizontal) + abs(vertical);
			}

		}
	}
	
}


///////////////////////////////////////////////////////////////////////////

void ComportamientoJugador::resetMapaPotencial(){
	vector<int> aux_Potencial(COL_MATRIZ_POTENCIAL, VAL_MAX_SALIDA);
	for (size_t i = 0; i < FIL_MATRIZ_POTENCIAL; i++)
	{
		mapaPotencial.push_back(aux_Potencial);
	}
}

void ComportamientoJugador::pintaMapaPotencial(unsigned char c){
	int i_auxiliar, j_auxiliar, fil_obj = 1, col_obj=7; 

	stack<pair<int,int>> valores; 

	
	for (size_t i = 0; i < FIL_MATRIZ_POTENCIAL; i++)
	{		
		// cout << endl;
		for (size_t j = 0; j < COL_MATRIZ_POTENCIAL; j++)
		{
			unsigned char c_actual = mapaResultado[i][j];
			int valor;
			
			if (c_actual == c){
				valores.push({i,j});
				valor=0;
			}else if (c_actual == 'A' && !bikini) {
				valor = 20000;
			} else if (c_actual == 'B' && !zapatillas){
				valor = 10000;
			}
			else if(c_actual != 'M' && c_actual != 'P' ){
				valor=0;
			} else
				valor = VAL_MAX_SALIDA;

			mapaPotencial[i][j] = valor;
		}
	}

	int siz=valores.size();
	for (size_t i = 0; i < siz; i++)
	{
		actualizaMapaPotencial(valores.top().first,valores.top().second);
		valores.pop();
	}

}

void ComportamientoJugador::comprobacionesIniciales(Sensores sensores){
	

	mismo_camino=false;
	if (sensores.reset){

		fil = col = fil_aux = col_aux =  fil_ini_aux = col_ini_aux = fil_ini = col_ini= fila_anterior = columna_anterior= 99;

		fil_bus=5; col_bus=4;
		minFilAux = minColAux=200;
		
		brujula = brujula_interna = 0;
		ultimaAccion = actIDLE;
		girar_derecha = true;
		bien_situado = true;
		
		bikini = zapatillas = hay_bikini = hay_zapatillas = hay_bateria = turnL =false;
		
		mismo_camino = buscaSalida= salida = salida_encontrada = hay_posicionamiento= false;
		primeraVez= segundaVez= true;
		num_iteracion= nSalida = 0;

		vector<unsigned char> aux(MAX_MAPA_AUX, '?');
		vector<unsigned char> aux_busqueda(6, '?');
		vector<int> rec(MAX_MAPA_AUX, -1);
		
		for (unsigned int i = 0; i < MAX_MAPA_AUX; i++){
			for (unsigned int j = 0; j < MAX_MAPA_AUX; j++)
			{
				mapaAuxiliar[i][j] = '?';
				mapaRecorrido[i][j] = -1;
			}
		}
		for (unsigned int i=0; i < 4; i++)
			for (size_t j = 0; j < 6; j++)
			{
				mapaBusqueda[i][j] = '?';
			}
			

		resetMapaRadial();
		pilaRecorrido = stack<Action>();
		he_reseteado= true;
	}
		

	if (sensores.nivel > 0 && primeraVez){
		bien_situado=false;
		primeraVez = false;
	} 
	

}
void ComportamientoJugador::actualizaMapaPotencial(int fil_obj, int col_obj){
	for (size_t i = 0; i < FIL_MATRIZ_POTENCIAL; i++)
	{
		for (size_t j = 0; j < COL_MATRIZ_POTENCIAL; j++)
		{
			if (!(i == fil_obj && j == col_obj) && mapaResultado[i][j] != '?'){
				int horizontal = i-fil_obj;
				int vertical  =j-col_obj;
				mapaPotencial[i][j] += abs(horizontal) + abs(vertical);
			}
			
		}
		
	}
	
}
Action ComportamientoJugador::think(Sensores sensores){

	
	comprobacionesIniciales(sensores);
	
	Action accion = actIDLE;

	switchAccion();
	
	comprobaciones(sensores);
	
	// DECIDIR NUEVA ACCION

	accion = accionSinNada(sensores);
	meterEnPila(accion);
	
	ultimaAccion =accion;
	
	return accion;
	
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
