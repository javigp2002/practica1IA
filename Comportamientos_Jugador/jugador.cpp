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

	while (mapaRes[fil_res][col_res] == '?')
		col_res++;
	
	int tamResultado = col_res;
	col_res = 0;

	for (fil_res=0; fil_res < tamResultado ; fil_res++){
		
		for (int col_res = 0; col_res < tamResultado; col_res++){
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
	if (min >=4){
		return accionPorDefecto(sensores);
	}

	int b;
	bool radial  = (hay_bikini && !bikini) || (hay_zapatillas && !zapatillas) || (hay_posicionamiento && !bien_situado) || hay_bateria;
	if (radial){
		b = brujula_interna;
	} else
		b= brujula;
	
	
	// cout<< endl << min << " " << b<< endl;

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

	if (!buscaSalida){
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
	} else{
		if (accion == actFORWARD)
			mapaPotencial[fil][col] = VAL_MAX_SALIDA;
	}

	return accion;




}


Action ComportamientoJugador::buscaObjeto(Sensores sensores, unsigned char c){
	
	if (sensores.terreno[2] == c) {
		if (c == 'K') {
			bikini = true;
			//// cout << "encontrado" << endl;
		} else if (c == 'D') {
			zapatillas = true;
		}
		return actFORWARD;
	}

	if (fil_bus != 0) { // ultima fila que se ve 
		if (ultAccionBus!=actFORWARD && pasoPosible(sensores,2,true)){
			ultAccionBus = actFORWARD;
			if (brujula_interna == 0){
				fil_bus--;
			} else if (brujula_interna == 2){
				fil_bus++;
			}

			// cout << "0.1" << endl;
		} else if (ultAccionBus != actTURN_R && girar_derecha){
			ultAccionBus = actTURN_R;
			brujula_interna = (brujula_interna+1)%4;
			girar_derecha=false;
			// cout << "0.2" << endl;
		} else{
			ultAccionBus = actTURN_L;
			brujula_interna=(brujula_interna+3)%4;
		
			girar_derecha = true;
			// cout << "0.3" << endl;
		}
	} else {
		if (ultAccionBus != actFORWARD && pasoPosible(sensores,2,true)) {
			ultAccionBus = actFORWARD;
			// cout << "1" << endl;

		} else if (segundaVez){
			ultAccionBus = actTURN_R;
			segundaVez = false;
			// cout << "2" << endl;
		}else{
			ultAccionBus = actTURN_L;
			segundaVez = true;
			// cout << "3" << endl;
		}
	}

	// pilaRecorrido.push(ultAccionBus);
	
	return ultAccionBus;
}
bool ComportamientoJugador::esInaccesible (Sensores sensores, int num_sensor){
	return 0;
}

Action ComportamientoJugador::accionSinNada(Sensores sensores){
	Action accion = actIDLE;
	
	int f=fil_aux,c=col_aux, fil_sens3=fil_aux, col_sens3=col_aux;
	int fil_aux_mas_tres = f, col_aux_mas_tres = c;
	
	int sensor;
	//meter for en metodo
	bool busca_bateria = false;
	if (sensores.bateria < VAL_MIN_BATERIA){
		busca_bateria = true;
	}

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

	// conocer que vamos a hacer en la siguiente accion
	switch (brujula){
		case 0: f--; fil_aux_mas_tres = f-3; col_aux_mas_tres = c+3; fil_sens3--; col_sens3++; break;
		case 1: c++; col_aux_mas_tres = c+3; fil_aux_mas_tres = f+3; col_sens3++; fil_sens3++; break;
		case 2: f++; fil_aux_mas_tres = f+3; col_aux_mas_tres = c-3; fil_sens3++; col_sens3--; break;
		case 3: c--; col_aux_mas_tres = c-3; fil_aux_mas_tres = f-3; col_sens3--; fil_sens3--; break;
	}

	if ((hay_bikini && !bikini) || (hay_zapatillas && !zapatillas) || (hay_posicionamiento && !bien_situado) || hay_bateria){
		busca_salida = salida=false;
		 cout << "mapaRadial" << endl;
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
	} else {
		

		if (buscaSalida){
			cout << "buscarSalida" << endl;

			if (bien_situado){
				if (!salida){
					resetMapaPotencial();
					pintaMapaPotencial('?');
				//	cout << endl;
					salida = true;
					accion = buscaObjetoPotencial(sensores);
					pilaRecorrido.pop();
					if (accion == actFORWARD){
						pilaRecorrido.push(actTURN_L);pilaRecorrido.push(actTURN_L);
					} else if (accion == actTURN_L){
						pilaRecorrido.push(actTURN_R);
					} else
						pilaRecorrido.push(actTURN_L);	
				} else {
					accion = buscaObjetoPotencial(sensores);

					pilaRecorrido.push(accion);
					if (busca_salida>= 19 ){
						buscaSalida = salida = false;
						salida_encontrada = true;
						busca_salida=0;
					} else
						busca_salida++;
				}
			} else {
				if(pilaRecorrido.empty()){
					if (pasoPosible(sensores,2)){
						accion = actFORWARD;
						pilaRecorrido.push(accion); // quizas tengo que quitar el movimiento de antes para que se compenetre
					}else
						accion = actTURN_R;
				} else {
					Action anterior = pilaRecorrido.top();
					if (anterior == actFORWARD) {
						accion = actFORWARD;
					} else if (anterior == actTURN_L) {
						accion = actTURN_R;
					} else {
						accion = actTURN_L;
						
					}
				}


					
			}

		
		}else {
			cout << "defecto"<<endl;
			accion = accionPorDefecto(sensores);
			fil_bus=5; col_bus=4;
			brujula_interna = 0;
		}
		
		segundaVez = true;		
		
		hay_zapatillas = false;
		hay_bikini=false;
		hay_bateria = false;
	} 
	
	// cout << " --> "<< pilaRecorrido.size();
	
	return accion;
}


Action ComportamientoJugador::accionPorDefecto(Sensores sensores){
	Action accion;
	
	//cout << "por defecto" << endl;
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

		//if (noSeSaleDeMapa2) cout << mapaAuxiliar[fil_aux_delante][col_aux_delante] << endl;
		if (pasoPosible(sensores,2) &&  mapaRecorrido[f][c] == -1 && !gira){
			accion = actFORWARD;
			num_iteracion=0;
			
		} else {
			accion = actTURN_R;
			num_iteracion++;
			// cout << num_iteracion << endl;
		} 

		if (num_iteracion >= 4){ //ha dado una vuelta completa{
			buscaSalida=true;
			salida=false;
			// elimino las 4 veces q ha dado la vuelta
			pilaRecorrido.pop();pilaRecorrido.pop();pilaRecorrido.pop();

			// la nueva accion sera dar la vuelta para que vuelva por donde vino
			accion=actTURN_L;
			
		}

		return accion;
}

//////////////////////////////////////////////////////////////////////////////////////

void ComportamientoJugador::switchBrujula (){
	switch (ultimaAccion)
		{
			case actFORWARD:
				switch (brujula){
					case 0: fil--; fil_aux--;break;
					case 1: col++; col_aux++;break;
					case 2: fil++; fil_aux++;break;
					case 3: col--; col_aux--;break;
				}
				

		
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
		copiarAuxiliarEnResultado(mapaAuxiliar, mapaResultado);
		hay_posicionamiento = false;
		
		bien_situado =true;
		
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
		// cout << "actualizando" << endl;

	} 
	actualizarVistaMapaAuxiliar(sensores);

	



}

void ComportamientoJugador::meterEnPila(Action accion){
	if (num_iteracion >= 4){
		num_iteracion = 0;
	} else if (!buscaSalida && !salida_encontrada) {
		pilaRecorrido.push(accion);
	} else
		salida_encontrada = false;

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
	//SacaMapa(mapaAuxiliar,110,130,60,90);
	unsigned char c_actual;
	for (size_t i = 0; i < FIL_MATRIZ_RADIAL; i++){
		if (brujula == 0 || brujula == 3){
			i_auxiliar = i-5;
		} else
			i_auxiliar = (5-i);
		
		//cout << endl;
		for (size_t j = 0; j < COL_MATRIZ_RADIAL; j++){
			//cout << fil_aux <<"|"<< col_aux << " --> " <<i_auxiliar << " "<< j_auxiliar << endl;
			if (brujula == 0 || brujula == 1){
				j_auxiliar = j-4;
			} else
				j_auxiliar = 4-j;

			if (brujula==0 || brujula == 2) // norte,sur -->columnas
				c_actual = mapaAuxiliar[fil_aux+i_auxiliar][col_aux+j_auxiliar];
			else // este oeste --> filas
				c_actual= mapaAuxiliar[fil_aux+j_auxiliar][col_aux+i_auxiliar];
			
			// cout << c_actual << " ";

			if (c_actual == c){
				valores.push({i,j});
				
				mapaRadial[i][j] = 0;
				
			} else if (c_actual != 'M' && c_actual != 'P' && c_actual != '?')
				mapaRadial[i][j] = 0;
			else	
				mapaRadial[i][j] = 1000;
			//cout<< mapaRadial[i][j] << " ";
		}

			//cout << endl;
		
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
			}
			else if(c_actual != 'M' && c_actual != 'P' ){
				valor=0;
			} else
				valor = VAL_MAX_SALIDA;

			mapaPotencial[i][j] = valor;
		}
		
	}
	
		for (size_t i = 0; i < FIL_MATRIZ_POTENCIAL; i++)		
				{
					cout << endl;
					for (size_t j = 0; j < COL_MATRIZ_POTENCIAL; j++)
					{
						cout << setw (6)<< mapaPotencial[i][j] << " ";
					}
					
				}


	cout << endl;

	int siz=valores.size();
	for (size_t i = 0; i < siz; i++)
	{
		actualizaMapaPotencial(valores.top().first,valores.top().second);
		valores.pop();
	}
	
	
		for (size_t i = 0; i < FIL_MATRIZ_POTENCIAL; i++)		
				{
					cout << endl;
					for (size_t j = 0; j < COL_MATRIZ_POTENCIAL; j++)
					{
						cout << setw (6)<< mapaPotencial[i][j] << " ";
					}
					
				}
				cout << endl;
}


void ComportamientoJugador::actualizaMapaPotencial(int fil_obj, int col_obj){
	for (size_t i = 0; i < FIL_MATRIZ_POTENCIAL; i++)
	{
		for (size_t j = 0; j < COL_MATRIZ_POTENCIAL; j++)
		{
			if (!(i == fil_obj && j == col_obj)){
				int horizontal = i-fil_obj;
				int vertical  =j-col_obj;
				mapaPotencial[i][j] += abs(horizontal) + abs(vertical);
			}
		}
		
	}
	
}
Action ComportamientoJugador::think(Sensores sensores){

	if (sensores.nivel > 0 && primeraVez){
        bien_situado=false;
		primeraVez = false;
	} 

	mismo_camino=false;
	Action accion = actIDLE;

	switchBrujula();
	mapaRecorrido[fil_aux][col_aux] = brujula; //actualizamos la vista de la brujula
	comprobaciones(sensores);
	if (sensores.terreno[0] == 'X') {
		if (sensores.bateria < sensores.vida){
			accion = actIDLE;
			ultimaAccion = accion;
			return accion;
		}
	}
	
	
	// DECIDIR NUEVA ACCION
	
	accion = accionSinNada(sensores);
	meterEnPila(accion);
	
	ultimaAccion =accion;

	
	
	
	
	return accion;
	
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
